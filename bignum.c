#include <stdio.h>
#include <stdint.h>
#include "bignum.h"


/***********************
 *  Nastaveni hodnoty  *
 ***********************/


// res = 0
void set_zero (my_number * res)
{
	int i;

	for (i = 0; i < SUM_FULL; i++)
		res->full[i] = 0;
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


// res = num
void copy (my_number * res, my_number * num)
{
	int i;

	for (i = 0; i < SUM_FULL; i++)
		res->full[i] = num->full[i];
}


/*******************
 *  Vypis hodnoty  *
 *******************/


// res = 0
void print_digit (FILE *stream, my_number * res)
{
	my_half zbytek;

	zbytek = div_half (res, 10);
	if (!test_zero (res))
		print_digit (stream, res);
	fprintf (stream, "%i", (int) zbytek);

}


void print_number (FILE *stream, char * before, my_number * num, char * after)
{
	my_number a;
	
	if (before)
		fprintf (stream, "%s", before);

	copy (&a, num);
	print_digit (stream, &a);

	if (after)
		fprintf (stream, "%s", after);
}


void print_hex_number (FILE *stream, char * before, my_number * num, char * after)
{
	int i;

	if (before)
		fprintf (stream, "%s", before);

	for (i = SUM_HALF - 1; i >= 0; i--)
		fprintf (stream, "%0" HALFSIZE "X ", num->half[i]);

	if (after)
		fprintf (stream, "%s", after);
}


/*******************
 *  Testy hodnoty  *
 *******************/


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


/************************
 *  Scitani a odcitani  *
 ************************/


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
	my_full x;
	my_half carry = 0;

#if 0
	x = 0;
	
	for (i = 0; i < SUM_FULL; i++) {

		carry  = __builtin_add_overflow (a->full[i], b->full[i], &res->full[i]);
		carry += __builtin_add_overflow (x, res->full[i], &res->full[i]);
		x = carry;
#else
	for (i = 0; i < SUM_FULL; i++) {

		x = a->full[i];
		res->full[i] = a->full[i] + b->full[i] + carry;
		carry = 0;
		if ( res->full[i] > x ) continue;
		if ( res->full[i] == x && b->full[i] == 0 ) continue;
		carry = 1;
#endif

	}

	return carry;
}


// res = a - b
// return ( a < b )
// &res == &a == &b je povoleno
my_half sub (my_number * res, my_number * a, my_number * b)
{
	int i;
	my_full x = 0;
	my_half carry = 0;

	for (i = 0; i < SUM_FULL; i++) {
		x = a->full[i];
		res->full[i] = a->full[i] - b->full[i] - carry;
		carry = 0;
		if ( x > res->full[i] ) continue;
		if ( res->full[i] == x && b->full[i] == 0 ) continue;
		carry = 1;
	}
	return carry;
}


/************
 *  Posuny  *
 ************/


// res <<= kolik
// return carry (.... ..11)
// 0 <= kolik < 8*sizeof(my_half)
my_half shift_left ( my_number * res, int kolik, my_half carry )
{
	my_mix n, *p = (my_mix *) res;
	my_half new_carry;
	int i = SUM_FULL;

	while (i--) {
		n = *p;
		new_carry = n.h[1] >> (8*sizeof(my_half) - kolik);
		n.f <<= kolik;
		n.h[0] |= carry;
		*p++ = n;
		carry = new_carry;
	}
	
	return carry;
}


// res >>= kolik
// return carry (11.. ....)
// 0 <= kolik < 8*sizeof(my_half)
my_half shift_right ( my_number * res, int kolik, my_half carry )
{
	my_mix n, *p = (my_mix *) res + SUM_FULL;
	my_half new_carry;
	int i = SUM_FULL;
// 	kolik &= 8*sizeof(my_half) - 1;

	while (i--) {
		n = *--p;
		new_carry = n.h[0] << (8*sizeof(my_half) - kolik);
		n.f >>= kolik;
		n.h[1] |= carry;
		*p = n;
		carry = new_carry;
	}
	return carry;
}


/***********************
 *  Nasobeni a deleni  *
 ***********************/


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


// res = a*b
// &res == &const_a == &const_b je povoleno
void mul (my_number * res, my_number * const_a, my_number * const_b)
{
	my_number a,b;
	if ( test_compare( const_a, const_b ) > 0 ) {
		copy( &a, const_b );
		copy( &b, const_a );
	} else {
		copy( &a, const_a );
		copy( &b, const_b );
	}
	set_zero (res);					// musi byt posledni kvuli &res == &const_a == &const_b
	
	while ( ! test_zero( &a) ) {
		if ( shift_right ( &a, 1, 0 )) add(res,res,&b);
		if ( shift_left ( &b, 1, 0 )) fprintf(stderr, "Preteceni pri nasobeni!\n");
	}
}


// res = res / num
// return ( (res % num) == 0 )
// &res == &num NENI povoleno
int div (my_number * res, my_number * num)
{
	int i;
	my_number m, n;
	my_number *pm = &m, *pn = &n;
	my_full carry = 0;

	set_zero (pm);

//	m << res << !(m - num)

	i = left_to_nonzero( res );
	for (; i < 8 * sizeof (my_number); i++) {	// rychlostni optimalizace
		carry = shift_left (res, 1, carry);
		if ( carry ) break;
	}
	
	for (; i < 8 * sizeof (my_number); i++) {
		shift_left (pm, 1, carry);
		carry = 1 - sub (pn, pm, num);
		if (carry) {
			my_number *temp;
			temp = pm;
			pm = pn;
			pn = temp;
		}
		carry = shift_left (res, 1, carry);
	}

	return test_zero (pm);		// pozor! diky prohazovani to muze ukazovat na n
}


/***************
 *  Specialni  *
 ***************/


// res = sqrt(num)
// &res == &num je povoleno
void sqr (my_number * res, my_number * num)
{
	my_number bit, temp, n;
	copy(&n, num);
	set_zero (res);
	set_zero (&bit);
	bit.half[SUM_HALF - 1] = (my_half) 1 << (8 * sizeof (my_half) - 2);
	
	while (test_compare (&bit, &n) == 1 )	// rychlostni optimalizace
		shift_right (&bit, 2, 0);

	while (!test_zero (&bit)) {
		add (&temp, res, &bit);
		shift_right (res, 1, 0);

		if (!sub (&temp, &n, &temp)) {
			copy (&n, &temp);
			add (res, res, &bit);
		}
		shift_right (&bit, 2, 0);
	}
}


// res <<= x
// return x
// hi_byte (res) != 0
int left_to_nonzero (my_number *res) 
{
	int ret,j,k;
	char *str = (char *) res;
	
	// Finding the first non-zero element
	ret = 0;
	j = sizeof (my_number) -1;
	do {
		if ( str[j] ) break;
		ret += 8;
	} while (j--);
	
	if ( ret == 0 || j < 0 ) return ret;

	k = sizeof (my_number);
	do str[--k] = str[j]; while ( j-- );
	while ( k--) str[k] = 0;

	return ret;
}
