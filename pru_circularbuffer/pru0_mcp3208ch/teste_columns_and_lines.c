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

   int n = 1000;
   /* a pointer to an int */
   uint8_t p[n];
   srand( (unsigned)time( 0 ) );

	for (int i = 0; i <= n; i++ ) {
     		p[i] = rand();
      //printf( "*(p + %d) : %d\n", i, *(p + i));
   }
	uint8_t bit;
	char ch;
	printf("Digite a quantidade de canais\n");
	scanf("%s \n", &ch);
  switch (ch) {

	case "01": // statement sequence
		  bit = 0b0001;
	     break;
	case "02": // statement sequence
		  bit = 0b1001;
	     break;
	case "03": // statement sequence
		  bit = 0b0101;
	     break;
	case "04": // statement sequence
		  bit = 0b1101;
	     break;
	case "05": // statement sequence
		  bit = 0b0011;
	     break;
	case "06": // statement sequence
		  bit = 0b1011;
	     break;
	case "07": // statement sequence
		  bit = 0b0111;
	     break;
	case "08": // statement sequence
		  bit = 0b1111;
	     break;
	default:    // default statement sequence
		  bit = 0b0001;
	}
	//Teste do bit
	while (bit) {
		if (bit & 1){
			printf("1\n");}
			else{
			printf("0\n");}
			bit >>=1;
			}
	printf("\n");
   buffer1( p, ch, n );

   return 0;
}
