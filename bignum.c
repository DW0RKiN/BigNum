#include <stdio.h>
#include <stdint.h>
#include "bignum.h"



// ( num == 0 )
int test_zero (my_number * num)
{
	int i;

	for (i = 0; i < SUM_FULL; i++)
		if (num->full[i])
			return 0;
	return 1;
}

// a >  b return  1
// a == b return  0
// a <  b return -1
int test_compare (my_number * a, my_number * b)
{
	int i;

	for (i = SUM_FULL-1; i >= 0; i--)
		if (a->full[i] > b->full[i])
			return 1;
		else if (a->full[i] < b->full[i]) 
			return -1;
	return 0;
}


// res = 0
void set_zero (my_number * res)
{
	int i;

	for (i = 0; i < SUM_FULL; i++)
		res->full[i] = 0;
}


// res = num
void copy (my_number * res, my_number * num)
{
	int i;

	for (i = 0; i < SUM_FULL; i++)
		res->full[i] = num->full[i];
}


// res = full
void set_full (my_number * res, my_full full)
{
	int i;
	res->full[0] = full;
	for (i = 1; i < SUM_FULL; i++)
		res->full[i] = 0;
}


// res = "012345"
void set_from_string (my_number * res, char * text)
{
	unsigned char c;
	
	set_zero(res);
	
	while ( *text ) {
		c = *text - '0';
		if ( c < 10 ) {			// ignoruje jine znaky nez 0-9
			mul_half( res, 10 );
			add_half( res, c, -1 );
		}
		text++;
	}
}


// res = res << kolik
// return carry
// 0 <= kolik < 8*sizeof(my_half)
my_half left (my_number * res, int kolik, my_half carry)
{
	int i;
	my_full x = 0;
	my_half *px = (my_half *) & x;

	for (i = 0; i < SUM_HALF; i++) {
		x = res->half[i];
		x <<= kolik;
		res->half[i] = px[0] + carry;
		carry = px[1];
	}

	return carry;
}


// res = res >> kolik
// return carry
// 0 <= kolik < 8*sizeof(my_half)
my_half right (my_number * res, int kolik, my_half carry)
{
	int i;
	my_half n;

	for (i = SUM_HALF - 1; i >= 0; i--) {
		n = res->half[i];
		n >>= kolik;
		n |= carry;
		carry = res->half[i];
		carry <<= 8 * sizeof (my_half) - kolik;
		res->half[i] = n;
	}

	return carry;
}


// res--
// return carry
my_half dec (my_number * res)
{
	int i;

	for (i = 0; i < SUM_FULL; i++) {
		if (res->full[i]--)
			return 0;
	}
	return 1;
}


// res++
// return carry
my_half inc (my_number * res)
{
	int i;

	for (i = 0; i < SUM_FULL; i++) {
		if (++res->full[i])
			return 0;
	}
	return 1;
}


// res = a - b
// return ( a < b )
// &res == &a == &b je povoleno
my_half sub (my_number * res, my_number * a, my_number * b)
{
	int i;
	my_full x = 0;
	my_half *px = (my_half *) & x;
	my_half carry = 0;

	for (i = 0; i < SUM_HALF; i++) {
		px[1] = 1;
		px[0] = a->half[i];
		x -= b->half[i];
		x -= carry;
		res->half[i] = px[0];
		carry = 1 - px[1];
	}

	return carry;
}


// res = res / half
// return res % half
my_half div_half (my_number * res, my_half half)
{
	int i;
	my_full x = 0;
	my_half *px = (my_half *) & x;

	for (i = SUM_HALF - 1; i >= 0; i--) {
		px[1] = px[0];
		px[0] = res->half[i];
		res->half[i] = x / half;
		x = x % half;
	}

	return px[0];
}


// res = res / num
// return ( (res % num) == 0 )
// &res == &b NENI povoleno
int div (my_number * res, my_number * num)
{
	int i;
	my_number m, n;
	my_number *pm = &m, *pn = &n;
	my_half carry = 0;

	set_zero (pm);

//	m << res << !(m - num)

	for (i = 0; i < 8 * sizeof (my_number); i++) {	// rychlostni optimalizace
		carry = left (res, 1, carry);
		if ( carry ) break;
	}
	
	for (; i < 8 * sizeof (my_number); i++) {
		left (pm, 1, carry);
		carry = 1 - sub (pn, pm, num);
		if (carry) {
			my_number *temp;
			temp = pm;
			pm = pn;
			pn = temp;
		}
		carry = left (res, 1, carry);
	}

	return test_zero (pm);		// pozor! diky prohazovani to muze ukazovat na n
}


// res = res + ( num << (i+1)*8*sizeof(my_half))
// Pozor! i bude hned zvyseno
void add_half (my_number * res, my_half num, int i)
{
	my_full m;
	my_half *pm = (my_half *) & m;
	pm[1] = num;

	while (pm[1]) {
		i++;
		if (i >= SUM_HALF) {
			fprintf (stderr, "Preteceni! Zvyste SUM_FULL.\n");
			break;
		}
		pm[0] = pm[1];
		pm[1] = 0;

		m += res->half[i];
		res->half[i] = pm[0];
	}
}


// res = a + b
// return carry
// &res == &a == &b je povoleno
my_half add (my_number * res, my_number * a, my_number * b)
{
	int i;
	my_full m = 0;
	my_half *pm = (my_half *) & m;

	for (i = 0; i < SUM_HALF; i++) {
		m += a->half[i];
		m += b->half[i];
		res->half[i] = pm[0];

		pm[0] = pm[1];		// carry
		pm[1] = 0;
	}

	return pm[0];
}


// res = res*half
void mul_half (my_number * res, my_half half)
{
	int i;
	my_full m;
	my_half *pm = (my_half *) & m;

	for (i = SUM_HALF - 1; i >= 0; i--) {
		m = half;
		m *= res->half[i];
		res->half[i] = pm[0];
		add_half (res, pm[1], i);
	}
}


// res = a*b
// &res == &a je povoleno
void mul (my_number * res, my_number * a, my_number * b)
{
	int i_r, i_a, i_b;
	my_full m;
	my_half *pm = (my_half *) & m;
	my_number r;

	set_zero (&r);

	for (i_a = SUM_HALF - 1; i_a >= 0; i_a--) {

		if (a->half[i_a] == 0)
			continue;

		for (i_b = 0; i_b < SUM_HALF; i_b++) {

			if (b->half[i_b] == 0) continue;
			i_r = i_a + i_b;
			if (i_r >= SUM_HALF) {
				fprintf (stderr, "preteceni o %i half!\n", 1 + i_r - SUM_HALF);
				continue;
			}

			m = a->half[i_a];
			m *= b->half[i_b];
			m += r.half[i_r];
			r.half[i_r] = pm[0];
			add_half (&r, pm[1], i_r);
		}
	}
	copy (res, &r);		// cena za moznost &res == &a
}


// res = sqrt(num)
// &res == &num je povoleno
void sqr (my_number * res, my_number * num)
{
	my_number bit, temp, n;
	copy(&n, num);
	set_zero (res);
	set_zero (&bit);
	bit.half[SUM_HALF - 1] = 1 << (8 * sizeof (my_half) - 2);
	
	while (test_compare (&bit, &n) == 1 )	// rychlostni optimalizace
		right (&bit, 2, 0);

	while (!test_zero (&bit)) {
		add (&temp, res, &bit);
		right (res, 1, 0);

		if (!sub (&temp, &n, &temp)) {
			copy (&n, &temp);
			add (res, res, &bit);
		}
		right (&bit, 2, 0);
	}
}


// res = 0
void print_digit (my_number * res)
{
	my_half zbytek;

	zbytek = div_half (res, 10);
	if (!test_zero (res))
		print_digit (res);
	printf ("%i", (int) zbytek);

}


void print_number (char * before, my_number * num, char * after)
{
	my_number a;
	
	if (before)
		printf ("%s", before);

	copy (&a, num);
	print_digit (&a);

	if (after)
		printf ("%s", after);
}


void print_hex_number (char * before, my_number * num, char * after)
{
	int i;

	if (before)
		printf ("%s", before);

	for (i = SUM_HALF - 1; i >= 0; i--)
		printf ("%0" HALFSIZE "X ", num->half[i]);

	if (after)
		printf ("%s", after);
}


