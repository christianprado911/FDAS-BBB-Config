#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/* function to print a file with values */
void buffer1 (uint8_t d[], int ch, int n)
{
	char buf[30];
	snprintf(buf, sizeof buf,  "print-columns.txt");
	int i = 0, k = 0;
	FILE *fl1;
	fl1 = fopen(buf, "w");

	for(i = 0; i < n/ch; i++){
    		for(int j = 0; j < ch; j++){
			fprintf(fl1, "%3d\t", d[k++]);
	}fprintf(fl1, "\n");}
	fclose(fl1);
}

/* main function to call above defined function */
int main () {

   int n = 1000, ch = 10;
   /* a pointer to an int */
   uint8_t p[n];
   srand( (unsigned)time( 0 ) );
   uint8_t BIT = 0b1000;
	for (int i = 0; i <= n; i++ ) {
     		p[i] = rand();
      //printf( "*(p + %d) : %d\n", i, *(p + i));
   }
	while (BIT) {
		sclk_clr();
		if (BIT & 1){
			printf("1");}
			else{
			printf(0);}
			BIT >>=1;
			}
   buffer1( p, ch, n );

   return 0;
}
