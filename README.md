# BigNum
Library for basic mathematical operations with preselected bit length.

	#include <stdio.h>
	#include <stdint.h>
	#include "bignum.c"
	#include <time.h>
	#include <sys/resource.h>
	
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
		char * s1 = " ", *s2 = "*";
		copy(&n,num);
		sqr(&stop, num);
		set_full(&prime, 2);
		while ( test_compare(&stop, &prime) >= 0 ) 
		{
			copy(&temp, &n);
			if ( div ( &temp, &prime ) ) 
			{
				copy(&n, &temp );
				sqr(&stop, &n);
				print_number (stdout, s1, &prime, NULL);
				s1 = s2;
			} else 
			{
				inc(&prime);
				if ( i ) inc(&prime);
				i = 1;
			}
		}
		print_number (stdout, "*", &n, "\n");
	}
	
	int main(int argc, char * argv[]) 
	{
		if ( argc < 2 )
		{
			fprintf(stderr, "Prilis malo argumentu!\n");
			return 1;
		}
		my_number max;
		set_from_string(&max,argv[1]);	
		double start = getcputime();
		project_Euler_03(&max);
		printf("time: %f\n", getcputime() - start);
		return 0;
	}


    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ clang -O3 -std=c99 -Wall euler03.c -o euler03 
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ ./euler03 12345678901234567890123456
    Project Euler problem 3
	    12345678901234567890123456 = 2*2*2*2*2*2*3*17*71*218107*244251294564157
    time: 3.932000

gcc -O3 fail!

    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ gcc -O2 -std=c99 -Wall test_bignum.c -o euler03
    dworkin@dworkin-Aspire-E1-572G:~/Programovani$ ./euler03 12345678901234567890123456
    Project Euler problem 3
	    12345678901234567890123456 = 2*2*2*2*2*2*3*17*71*218107*244251294564157
    time: 11.140000
