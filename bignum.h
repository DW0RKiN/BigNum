#include <stdio.h>
#include <stdint.h>

#ifndef _BIGNUM

#define _BIGNUM

#if 1

#define SUM_FULL	2
#define SUM_HALF	(2*SUM_FULL)
typedef __uint128_t my_full;
typedef uint64_t my_half;
#define HALFSIZE	"16l"	// == sizeof(my_half)

#else

#define SUM_FULL	4
#define SUM_HALF	(2*SUM_FULL)
typedef uint64_t my_full;
typedef uint32_t my_half;
#define HALFSIZE	"8"	// == sizeof(my_half)

#endif

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
// return carry
// 0 <= kolik < 8*sizeof(my_full)
my_full left (my_number * res, int kolik, my_full carry);


// res >>= kolik
// return carry
// 0 <= kolik < 8*sizeof(my_full)
my_full right (my_number * res, int kolik, my_full carry);


/*
 * Nasobeni a deleni
 */

// res = res*half
void mul_half (my_number * res, my_half half);

// res = res / half
// return res % half
my_half div_half (my_number * res, my_half half);

// res = a*b
// &res == &a == &b je povoleno
void mul (my_number * res, my_number * const_a, my_number * const_b);

// res = res / num
// return ( (res % num) == 0 )
// &res == &num NENI povoleno
int div (my_number * res, my_number * num);


/*
 * Specialni
 */

// res = sqrt(num)
// &res == &num je povoleno
void sqr (my_number * res, my_number * num);



#endif
