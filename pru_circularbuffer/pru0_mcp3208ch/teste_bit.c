#include <stdio.h>
#include <stdint.h>

int main (void){
	
uint8_t BIT = 0b1000;
for(int i =1; i<=8; i++){
	while (BIT) {
    	if (BIT & 1)
        	printf("1");
    		else
        	printf("0");
    		BIT >>= 1;
	}
	printf("\n"); 
	BIT = 0b1000;
	BIT|=i;
}
}
