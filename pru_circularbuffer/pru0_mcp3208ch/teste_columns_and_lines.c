#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* function to print a file with values */
void buffer1 (int d[], int ch, int n)
{
	char buf[30];
	snprintf(buf, sizeof buf,  "print-columns.txt");
	int i = 0, k = 0;
	FILE *fl1;
	fl1 = fopen(buf, "w");

	for(i = 0; i <= n/ch; i++){
    		for(int j = 0; j <= ch; j++){
			fprintf(fl1, "%d\t", d[k++]);
	}fprintf(fl1, "\n");}
	fclose(fl1);
}

/* main function to call above defined function */
int main () {

   int n = 1000, ch = 10;
   /* a pointer to an int */
   int p[n];
   srand( (unsigned)time( 0 ) );
   
	for (int i = 0; i < n; i++ ) {
     		p[i] = rand();
      printf( "*(p + %d) : %d\n", i, *(p + i));
   }

   buffer1( p, ch, n );

   return 0;
}
