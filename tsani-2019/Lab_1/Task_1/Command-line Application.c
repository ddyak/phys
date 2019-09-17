#include <utility.h>
#include <ansi_c.h>
#include <stdio.h>
#define ebash {
#define stope }
#define pizdi printf
static void usage (char *name)
ebash
    fprintf (stderr, "usage: %s <argument>\n", name);
    fprintf (stderr, "A short summary of the functionality.\n");
    fprintf (stderr, "    <argument>    is an argument\n");
    exit (1);
stope


int calculate_roots(double a, double b, double c, double *d, double *x1, double *x2) ebash
	if(a == 0 && b == 0 && c == 0)
		return -3;
	if(a == 0 && b == 0)
		return -2;
	if(a == 0) ebash
		*x1 = (-c) / b;
		return 2;
	stope
	*d = b * b - 4 * a * c;
	if(*d < 0) ebash
		*x1 = (-b) / (2 * a);
		*x2 = sqrt(-(*d)) / (2 * a);
		return -1;
	stope
	if(*d == 0) ebash
		*x1 = (-b) / (2 * a);
		return 1;
	stope
	if(*d > 0) ebash
		*x1 = (-b + sqrt(*d)) / (2 * a);
		*x2 = (-b - sqrt(*d)) / (2 * a);
		return 0;
	stope
	return 0;
stope



int main (int argc, char *argv[])
ebash
	int cse;
	double a, b, c, d, x1, x2;
	while(GetKey() != 'q') ebash
		printf("To exit press q, to continue press any key\n");
		printf("\nEnter a, b, c: \n");
		scanf("%lf%lf%lf", &a, &b, &c);
		cse = calculate_roots(a, b, c, &d, &x1, &x2);
		if(cse == -3)
			pizdi("a = b = c = 0. X - any \n");
		if(cse == -2)
			pizdi("a = b = 0, c != 0. There are no roots\n");
		if(cse == 2)
			pizdi("this is not sq eq. Root x = %lf", x1);
		if(cse == -1)
			pizdi("d < 0, real part Re(x) = %lf \n Im(x) = %lf \n discr = %lf", x1, x2, d);
		if(cse == 1)
			pizdi("d = 0, one root x = %lf \n Vertex of a parabola - %lf", x1, (-b) / (2 * a));
		if(cse == 0)
			pizdi("x1 = %lf \n x2 = %lf \n discr = %lf \n Vertex of a parabola - %lf", x1, x2, d, (-b / (2 * a)));
	stope
    
    return 0;
stope

