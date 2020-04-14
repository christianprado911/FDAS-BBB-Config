/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>

/*  CONSTANTS OF CIRCULAR BUFFER */

#define		MAX_BUFFER_SIZE		10000000	// 10 segundos 
#define		CIRCULAR_BUFFER		1000000		// 1 Segundo
//#define		FAILURE			-1
//#define		EXIT_LOOP               1

/* Circular Buffer */

	int	readBuf[CIRCULAR_BUFFER];
	int	circularBuffer[MAX_BUFFER_SIZE] = { 0 };	// Empty circular buffer
	int	readIndex	=	0;	// Index of the read pointer
	int	writeIndex	=	0;	// Index of the write pointer
	int	bufferLength	=	0;	// Number of values in circular buffer
	int 	trRead		=	0;  	// Trigger to readBuf
	int 	trrigger	=	0;  	// Trigger to measure

#define NUM_MESSAGES		10000000 //apenas para teste
#define DEVICE_NAME		"/dev/rpmsg_pru123"

int main(void)
{
	struct pollfd pollfds[1];
	int i;
	int result = 0, count = 0;

	/* Criate Buffer1 and buffer2 file*/
	FILE *fl1;
	fl1 = fopen("buffer1.txt", "w");
	FILE *fl2;
	fl2 = fopen("buffer2.txt", "w");

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

	/*
	 * If the RPMsg channel doesn't exist yet the character device
	 * won't either.
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted.
	 */
	if (pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	printf("Opened %s, reading %d buffers\n\n", DEVICE_NAME, NUM_MESSAGES);

        const char msg[] = "PRU-ARM ADC CH 01 - Buffer Circular";
        /* Send 'msg' to the PRU through the RPMsg channel / Test only one channel 
        result = write(pollfds[0].fd, msg, sizeof msg);
        printf("write result = %d\n", result);*/

	/* Inicio Iteração de coleta de dados =================================================*/
	for (i = 0; i < NUM_MESSAGES; i++) {
		/* Send 'msg' to the PRU through the RPMsg channel */
                result = write(pollfds[0].fd, msg, sizeof msg);

                if (result > 0) {
			/* Send 'msg' to the PRU through the RPMsg channel */
			result = write(pollfds[0].fd, msg, sizeof msg);
			if (result > 0)
				printf("Message %d: Sent to PRU\n", i);

			/* Poll until we receive a message from the PRU */
			result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);

			/* Read the buffer and print it if necessary */
			if (result > 0) {
				for (int i=0; i<result/2; i++) {
					circularBuffer[writeIndex] = ((uint16_t*)readBuf)[i];

				/* Inicio do Buffer Circular (1 segundo) */
				if((circularBuffer[writeIndex] <= 3500) && trigger == 0){
					printf("%d, %d, %d  Write the buffer\n", circularBuffer[writeIndex], bufferLength, writeIndex); // test
					writeIndex++;

					if (writeIndex == CIRCULAR_BUFFER) {
						writeIndex = 0;
					}
					if(bufferLength != CIRCULAR_BUFFER){
						bufferLength++;
					}
				}

				/* Imprime arquivo com 1 segundo de buffer circular */
				if(trRead == 0){
					if(circularBuffer[writeIndex] > 3500 || trigger == 1){
						trigger = 1;
						fprintf(fl1, "%d, %d\n", circularBuffer[readIndex], readIndex);
						bufferLength--;
						readIndex++;
							if (readIndex == CIRCULAR_BUFFER) {
								readIndex = 0;
							}
							if (bufferLength == 0){
								trRead= 1;
							}
					}
				}/* Fim da impressão do Buffer circular */

				/* Inicio da impressao dos 10 segundos de coleta real-time */
				if(trigger == 1 && count <= MAX_BUFFER_SIZE){
					fprintf(fl2, "%d, %d\n", circularBuffer[writeIndex], writeIndex);
					count++;
						if(count == MAX_BUFFER_SIZE){
						count = 0;
						trigger == 0;
						}
				}/* Fim impressao */
				}
			}
		}else
		 printf("read error!!!\n");
        }
	/* Fim Iteração de coleta de dados =================================================*/

	fclose(fl1);
	fclose(fl2);

	/* Received all the messages the example is complete */
	printf("Received %d messages, closing %s\n", NUM_MESSAGES, DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}
