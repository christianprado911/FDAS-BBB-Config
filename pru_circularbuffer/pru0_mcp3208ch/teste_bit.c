#include <stdio.h>
#include <stdint.h>

int main (void){

int i = 0;
uint8_t BIT = 0b1000;
BIT|=1;

	while (BIT) {
    	if (BIT & 1)
        	printf("1");
    		else
        	printf("0");
    		BIT >>= 1;
		i++;
	}
	printf("\n %d \n", i);
}
