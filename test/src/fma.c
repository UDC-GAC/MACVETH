#include <stdio.h>
#include <stdlib.h>

int
main()
{
	double a[1024];
	double b[1024];
	double c[1024];
	
	for(int i = 0; i < 1024; i++) 
	{
		c[i] += a[i]*b[i];
	}

	printf("c = %f\n", c);
	exit(0);

}
