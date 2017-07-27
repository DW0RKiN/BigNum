# BigNum
Library for basic mathematical operations with preselected bit length.

Support: conversion from/into a string, compare, increase, decrease, addition, subtraction, multiplication, division, square root, exponentiation, bit shifts, ... https://github.com/DW0RKiN/BigNum/blob/master/bignum.h

The size of the number is 64*(1+ARRAY_128_BIT)*MAX_ARRAY_INDEX bits.

	#include <stdio.h>
	#include <stdint.h>
	#include <time.h>
	#include <sys/resource.h>
	
	#define MAX_ARRAY_INDEX		2
	#define ARRAY_128_BIT		1
	#include "bignum.c"
	
	double getcputime(void)        
	{
		struct timeval tim;        
		struct rusage ru;        
		getrusage(RUSAGE_SELF, &ru);        
		tim=ru.ru_utime;        
		double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;        
		tim=ru.ru_stime;        
		t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;        
		return t;
	}
	
	void project_Euler_03(my_number * num)
	{
		print_number ( stdout, "Project Euler problem 3\n\t", num, " =");	
		int i = 0;
		my_number stop, n, temp, prime;
		char * s = " ";
		copy(&n,num);						// n = num;
		sqr(&stop, num);
		set_full(&prime, 2);					// prime = 2;
		while ( test_compare(&stop, &prime) >= 0 ) 
		{
			copy(&temp, &n);				// temp = n;
			if ( div ( &temp, &prime ) ) 			// return !(temp % prime); temp /= prime; 
			{
				copy(&n, &temp );			// n = temp;
				sqr(&stop, &n);				// stop = sqrt(n);
				print_number (stdout, s, &prime, NULL);
				s = "*";

			} else 
			{
				inc(&prime);				// prime++;
				if ( i ) inc(&prime);			// if ( i ) prime++;
				i = 1;
			}
		}
		print_number (stdout, s, &n, "\n");
	}
	
	int main(int argc, char * argv[]) 
	{
		if ( argc < 2 )
		{
			fprintf(stderr, "Prilis malo argumentu!\n");
			return 1;
		}
		my_number max;
		set_from_string(&max,argv[1]);				// max = argv[1]
		double start = getcputime();
		project_Euler_03(&max);
		printf("time: %f\n", getcputime() - start);
		return 0;
	}


compile and run

    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ clang --version
    clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)
    Target: x86_64-pc-linux-gnu
    Thread model: posix
    InstalledDir: /usr/bin
    
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ clang -O3 -std=c99 -Wall euler03.c -o euler03 
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ ./euler03 12345678901234567890123456
    Project Euler problem 3
	    12345678901234567890123456 = 2*2*2*2*2*2*3*17*71*218107*244251294564157
    time: 3.932000

    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ gcc --version
    gcc (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609
    Copyright (C) 2015 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ gcc -O2 -std=c99 -Wall euler03.c -o euler03
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ ./euler03 12345678901234567890123456
    Project Euler problem 3
	    12345678901234567890123456 = 2*2*2*2*2*2*3*17*71*218107*244251294564157
    time: 11.140000
    
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ gcc -O3 -std=c99 -Wall euler03.c -o euler03
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ ./euler03 12345678901234567890123456
    Project Euler problem 3
	    12345678901234567890123456 = 2*2*2*2*2*2*3*17*71*218107*244251294564157
    time: 1619.884000

