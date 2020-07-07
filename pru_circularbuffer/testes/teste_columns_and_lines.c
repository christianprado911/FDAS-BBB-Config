#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

//Function to return if ch is equal to 1, 2 3 or 4
int compareD2 (int ch){
	int t[] = {1, 2, 3, 4};
	int i = 0, j = 0;
	
	for(i = 0; i <= 3; i++){
		if(ch != t[i])
			j = 1;
	}
	return j;
	
}

//Function to return if ch is equal to 1, 2 5 or 6
int compareD1 (int ch){
	int t[] = {1, 2, 5, 6};
	int i = 0, j = 0;
	
	for(i = 0; i <= 3; i++){
		if(ch != t[i])
			j = 1;
	}
	return j;
}

/* function to print a file with values 
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
}*/

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
	//uint8_t BIT;
	char ch;
/*	printf("Digite a quantidade de canais\n");
	scanf("%s", &ch);
  switch (ch) {
	case '1': // statement sequence
		BIT = 0b00000001;
	     break;
	case '2': // statement sequence
		BIT = 0b00001001;
	     break;
	case '3': // statement sequence
		BIT = 0b00000101;
	     break;
	case '4': // statement sequence
		BIT = 0b00001101;
	     break;
	case '5': // statement sequence
		BIT = 0b00000011;
	     break;
	case '6': // statement sequence
		BIT = 0b00001011;
	     break;
	case '7': // statement sequence
		BIT = 0b00000111;
	     break;
	case '8': // statement sequence
		BIT = 0b00001111;
	     break;
	default:    // default statement sequence
		BIT = 0b00000001;
	}
	//Teste do bit
	while (BIT) {
		if (BIT & 1)
			printf("1");
			else
			printf("0");
			BIT >>=1;
	}
	printf("\n"); */
	printf("Teste de selecao de canal\n");
	printf("insira o canal, 1 a 8\n");
	scanf("%d", &ch);
	printf("----------------\n");
	printf("D2\t");
	int q = compareD2(ch);
	if(q)
		printf("1");
	else
		printf("0");
	//insert delay cycles
	printf("\nD1\t");
	int o = compareD1(ch);
	if(o)
		printf("1");
	else
		printf("0");
	//insert delay cycles
	printf("\nD0\t");
	if(ch ==1 || ch%2 != 0)
		printf("0");
	else
		printf("1");
	printf("\n");
	//insert delay cycles
		
	
  // buffer1( p, ch, n );

   return 0;
}
