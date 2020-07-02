#include <stdio.h>
#include <stdint.h>

int main (void){
	static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
uint8_t BIT = 0b1000;
BIT|=7;
	while (BIT) {
    	if (BIT & 1)
        	printf("1");
    		else
        	printf("0");

    		BIT >>= 1;
	}
	printf("\n");

    if ((BIT & mask[0]) != 0) {
	    printf("O bit eh um");
	    else
		    printf("O bit eh zero");
	}

}
