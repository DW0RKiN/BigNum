/* use 

#define MAX_ARRAY_INDEX		1+
#define ARRAY_128_BIT		0 or 1

before #include "bignum.c"
*/

#include <stdio.h>
#include <stdint.h>

#ifndef _BIGNUM
#define _BIGNUM

#ifndef ARRAY_128_BIT
	#define ARRAY_128_BIT		0
#endif

#ifndef MAX_ARRAY_INDEX
	#define MAX_ARRAY_INDEX		2
#endif

#if ARRAY_128_BIT
	#define SUM_FULL	MAX_ARRAY_INDEX
	#define SUM_HALF	(2*MAX_ARRAY_INDEX)
	typedef __uint128_t my_full;
	typedef uint64_t my_half;
	#define HALFSIZE	"16l"	// == sizeof(my_half)
#else
	#define SUM_FULL	MAX_ARRAY_INDEX
	#define SUM_HALF	(2*MAX_ARRAY_INDEX)
	typedef uint64_t my_full;
	typedef uint32_t my_half;
	#define HALFSIZE	"8"	// == sizeof(my_half)
#endif

union mix {
	my_full f;
	my_half h[2];
};

union cislo {
	my_full full[SUM_FULL];
	my_half half[SUM_HALF];
};


typedef union mix my_mix;
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
void print_digit (FILE *stream, my_number * res);

void print_number (FILE *stream, char * before, my_number * num, char * after);

void print_hex_number (FILE *stream, char * before, my_number * num, char * after);


/*
 * Testy hodnoty
 */

// ( num == 0 )
int test_zero (my_number * num);

// a >  b return  1
// a == b return  0
// a <  b return -1
int test_compare (my_number * a, my_number * b);


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
// return carry (.... ..11)
// 0 <= kolik < 8*sizeof(my_half)
my_half shift_left ( my_number * res, int kolik, my_half carry );

// res >>= kolik
// return carry
// 0 <= kolik < 8*sizeof(my_half)
my_half shift_right (my_number * res, int kolik, my_half carry);


/*
 * Nasobeni a deleni
 */

// res = res*half
void mul_half (my_number * res, my_half half);

// res = res / half
// return res % half
my_half div_half (my_number * res, my_half half);

// res = a*b
// &res == &const_a == &const_b je povoleno
void mul (my_number * res, my_number * const_a, my_number * const_b);

// res = res / num
// mod = res % num
// &res != &num, &res != &mod
void div (my_number * res, my_number * num, my_number * mod);


/*
 * Specialni
 */

// res = sqrt(num)
// &res == &num je povoleno
void sqr (my_number * res, my_number * num);

// res = res^pow
void power (my_number * res, unsigned int pow);

// res <<= x
// return x
// hi_byte (res) != 0
int left_to_nonzero (my_number *res);

// res = greatest_common_divisor (a, b)
// &res == &a == &b
void gcd(my_number * res, my_number * a, my_number * b);

#endif
