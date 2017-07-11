#include <stdio.h>
#include <stdint.h>


#define SUM_FULL	5
#define SUM_HALF	(2*SUM_FULL)


#ifndef _BIGNUM

#define _BIGNUM

typedef uint64_t my_full;
typedef uint32_t my_half;

#define HALFSIZE	"8"	// == sizeof(my_half)


union cislo {
	my_full full[SUM_FULL];
	my_half half[SUM_HALF];
};

typedef union cislo my_number;


/*
 * Nastaveni hodnoty
 */

// res = 0
void set_zero (my_number * res);

// res = full
void set_full (my_number * res, my_full full);

// res = "012345"
void set_from_string (my_number * res, char * text);

// res = num
void copy (my_number * res, my_number * num);


/*
 * Vypis hodnoty 
 */

// res = 0
void print_digit (my_number * res);

void print_number (char * before, my_number * num, char * after);

void print_hex_number (char * before, my_number * num, char * after);


/*
 * Testy hodnoty
 */

// ( num == 0 )
int test_zero (my_number * num);

// ( a > b )
int test_great (my_number * a, my_number * b);


/*
 * Scitani a odcitani
 */

// res--
// return carry
my_half dec (my_number * res);

// res++
// return carry
my_half inc (my_number * res);

// res += num << (i+1)*8*HALFSIZE
// Pozor! i bude hned zvyseno
void add_half (my_number * res, my_half num, int i);

// res = a + b
// return carry
// &res == &a == &b je povoleno
my_half add (my_number * res, my_number * a, my_number * b);

// res = a - b
// return ( a < b )
// &res == &a == &b je povoleno
my_half sub (my_number * res, my_number * a, my_number * b);


/*
 * Posuny
 */

// res <<= kolik
// return carry
// 0 <= kolik < 8*sizeof(my_half)
my_half left (my_number * res, int kolik, my_half carry);

// res >>= kolik
// return carry
// 0 <= kolik < 8*sizeof(my_half)
my_half right (my_number * res, int kolik, my_half carry);


/*
 * Nasobeni a deleni
 */

// res = res*half
void mul_half (my_number * res, my_half half);

// res = res / half
// return res % half
my_half div_half (my_number * res, my_half half);

// res = a*b
// &res == &a je povoleno
void mul (my_number * res, my_number * a, my_number * b);

// res = res / num
// return ( (res % num) == 0 )
// &res == &b NENI povoleno
int div (my_number * res, my_number * num);


/*
 * Specialni
 */

// res = sqrt(num)
void sqr (my_number * res, my_number * num);



#endif
