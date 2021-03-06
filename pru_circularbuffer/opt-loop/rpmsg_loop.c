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
#include <time.h>

#define		BUFFER_PRU		100
#define		MAX_BUFFER_SIZE		685000		// 10 segundos (PRU 1MHz)
#define		CIRCULAR_BUFFER		68500		// 1 Segundo (PRU 1MHz)
#define		DEVICE_NAME		"/dev/rpmsg_pru123"

	int	readBuf[BUFFER_PRU];
	int	circularBuffer[CIRCULAR_BUFFER] = { 0 };	// Empty circular buffer
	int	buffer_10s[MAX_BUFFER_SIZE]	= { 0 };	// Empty 10s buffer
	int	readIndex	=	0;	// Index of the read pointer
	int	writeIndex	=	0;	// Index of the write pointer
	int	bufferLength	=	0;	// Number of values in circular buffer
	int 	count		=	0;  	// Index of buffer_10s / end of loop
	int 	trigger		=	0;  	// Trigger to change from circular buffer to 10s buffer
	char	namefile[20]		;	// Record the date of the signal

void buffer1 (int d[], int n, char namefile[]);
void buffer10 (int d[], int n, char namefile[]);

int main(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	struct pollfd pollfds[1];
	int i;
	int result = 0;

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
	printf("Opened %s, reading buffers\n\n", DEVICE_NAME);

        const char msg[] = 01;
        /* Send 'msg' to the PRU through the RPMsg channel / Test only one channel 
        result = write(pollfds[0].fd, msg, sizeof msg);
        printf("write result = %d\n", result);*/

	/* Inicio Iteração de coleta de dados =================================================*/
	do{
		/* Send 'msg' to the PRU through the RPMsg channel */
		result = write(pollfds[0].fd, msg, sizeof msg);
                if (result > 0) {
			//if (result > 0)
			//	printf("Message %d: Sent to PRU\n", i);

			/* Poll until we receive a message from the PRU */
			result = read(pollfds[0].fd, readBuf, BUFFER_PRU);

			/* Read the buffer and print it if necessary */
			if (result > 0) {
				for (int i=0; i<result/2; i++) {
					int data = ((uint16_t*)readBuf)[i];
					//printf("%d, %d, %d, count %d\n", data, bufferLength, writeIndex, count); // test
					if(trigger == 0 && data > 1500){
						trigger = 1;
						snprintf(namefile, sizeof namefile, "%02d-%02d-%d_%02dh%02dm%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
						printf("Pulso Capturado, trigger %d\n\n", trigger);
					}
				/* Inicio do Buffer Circular (1 segundo) */
				if(trigger == 0){
					circularBuffer[writeIndex] = data;
					writeIndex++;
					if (writeIndex == CIRCULAR_BUFFER) {
						writeIndex = 0;
					}
					if(bufferLength != CIRCULAR_BUFFER){
						bufferLength++;
					}
				}

				/* Inicio da coleta dos 10 segundos real-time */
				if(trigger == 1){
					buffer_10s[count] = data;
					count++;
				}/* Fim Coleta */
				}
			}
		}else
		 printf("read error!!!\n");
        }while(count != MAX_BUFFER_SIZE);
	/* Fim Iteração de coleta de dados. Gravação dos dados em arquivos=================*/
	printf("Impressao dos valores de buffer de 1s\n");
	buffer1(circularBuffer, bufferLength, namefile);
	printf("Fim da impressao buffer 1 s \n Impressao dos valores de buffer 10s\n");
	buffer10(buffer_10s, count, namefile);
	printf("Fim da impressao");
	/* Received all the messages the example is complete */
	//printf("Received %d messages, closing %s\n", NUM_MESSAGES, DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}

//Função que grava os arquivos do buffer circular
void buffer1 (int d[], int n, char namefile[])
{
	char buf[30];
	snprintf(buf, sizeof buf,  "%s-1.txt", namefile);
	int i = 0;
	FILE *fl1;
	fl1 = fopen(buf, "w");

	for(i = 0; i <= n; i++){
		fprintf(fl1, "%d\n", d[i]);
	}
	fclose(fl1);
}
/* Fim da impressão do Buffer circular */

//Função que grava os arquivos do buffer_10s
void buffer10 (int d[], int n, char namefile[])
{
	char buf[30];
	snprintf(buf, sizeof buf,  "%s-2.txt", namefile);
	int i = 0;
	FILE *fl2;
	fl2 = fopen(buf, "w");

	for(i = 0; i <= n; i++){
		fprintf(fl2, "%d\n", d[i]);
	}
	fclose(fl2);
}
