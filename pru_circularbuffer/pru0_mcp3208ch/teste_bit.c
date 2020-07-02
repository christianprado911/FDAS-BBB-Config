#include <stdio.h>
#include <stdint.h>

int main (void){

uint8_t mask[] = {8, 4, 2, 1};
uint8_t BIT = 0b1000;
BIT|=0;

	while (BIT) {
    	if (BIT & 1)
        	printf("1");
    		else
        	printf("0");

    		BIT >>= 1;
	}
	printf("\n");

    if ((BIT & mask[0]) != 0)
	    printf("O bit eh um\n");
	else
	    printf("O bit eh zero\n");
}
