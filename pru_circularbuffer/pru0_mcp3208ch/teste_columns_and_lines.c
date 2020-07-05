#include <stdio.h>

/* function to generate and return random numbers */
int * getRandom( ) {

   static int  r[10];
   int i;

   /* set the seed */
   srand( (unsigned)time( NULL ) );
  
   for ( i = 0; i < 10; ++i) {
      r[i] = rand();
      printf( "r[%d] = %d\n", i, r[i]);
   }

   return r;
}

/* function to print a file with values */
void buffer1 (int d[], int ch, int n)
{
	char buf[30];
	snprintf(buf, sizeof buf,  "print-columns.txt");
	int i = 0, k = 0;
	FILE *fl1;j
	fl1 = fopen(buf, "w");

	for(i = 0; i <= n/ch; i++){
    for(int j = 0; j <= ch; j++){
		fprintf(fl1, "%d\n", d[k++]);
	}}
	fclose(fl1);
}

/* main function to call above defined function */
int main () {

   int n = 1000, ch = 10;
   /* a pointer to an int */
   int *p[];
	
   for (int i = 0; i < n; i++ ) {
     p[i] = getRandom();
      printf( "*(p + %d) : %d\n", i, *(p + i));
   }

   buffer1(p, ch, n )
	
   return 0;
}
