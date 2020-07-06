/*
 * Copyright (C) 2016-2018 Texas Instruments Incorporated - http://www.ti.com/
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
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>

#include "resource_table_0.h"
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT			((uint32_t) 1 << 30)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			16
#define FROM_ARM_HOST			17

/*
 * Using the name 'rpmsg-client-sample' will probe the RPMsg sample driver
 * found at linux-x.y.z/samples/rpmsg/rpmsg_client_sample.c
 *
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
//#define CHAN_NAME			"rpmsg-client-sample"
#define CHAN_NAME			"rpmsg-pru"

#define CHAN_DESC			"Channel 123"
#define CHAN_PORT			123

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

uint8_t payload[RPMSG_MESSAGE_SIZE];

#define MISO_REG __R31 //P9_29
#define MOSI_REG __R30 //P9_30
#define SCLK_REG __R30 //P9_31
#define CS_REG __R30 //P9_28

#define MISO_MASK ((uint32_t)(1 << 1))
#define MOSI_MASK ((uint32_t)(1 << 2))
#define SCLK_MASK ((uint32_t)(1 << 0))
#define CS_MASK ((uint32_t)(1 << 3))

inline void mosi_set() {
  MOSI_REG |= MOSI_MASK;
}

inline void mosi_clr() {
  MOSI_REG &= ~MOSI_MASK;
}

inline void sclk_set() {
  SCLK_REG |= SCLK_MASK;
}

inline void sclk_clr() {
  SCLK_REG &= ~SCLK_MASK;
}

inline void cs_set() {
  CS_REG |= CS_MASK;
}

inline void cs_clr() {
  CS_REG &= ~CS_MASK;
}

inline uint8_t miso_rd() {
  if (MISO_REG & MISO_MASK)
    return 1;
  else
    return 0;
}

uint16_t convert() { // ch -> number of channels
	
	uint8_t BIT = 0b1000; // Setup Byte
	//BIT |= ch;
	int i;

		sclk_clr(); // Initialize clock
		cs_clr(); // Set CS to low (active)
		mosi_set(); // Set MOSI to HIGH (start bit)
		__delay_cycles(30); // 30 cycles = 150ns

		sclk_set(); // cycle clock (data transfer)
		__delay_cycles(100); // 100 cycles = 500ns

		// Configuration
		while (BIT) {
		sclk_clr();
		if (BIT & 1)
			mosi_set();
			else
			mosi_clr();
		__delay_cycles(100); // 100 cycles = 500ns
		sclk_set();
		__delay_cycles(100); // 100 cycles = 500ns
			BIT >>=1;
		}
	
        // Wait while device samples channel
        sclk_clr();
        __delay_cycles(100); // 100 cycles = 500ns
        sclk_set();
        __delay_cycles(100); // 100 cycles = 500ns
        sclk_clr(); // Sampling completed

        uint16_t result = 0;

        // Read null bit
        __delay_cycles(100); // 100 cycles = 500ns
        sclk_set();
        int null_bit = miso_rd();
        __delay_cycles(100); // 100 cycles = 500ns
        sclk_clr();
	
	// Read b11, b10,...b0.
	for( i=0; i<=11; i++) {
	__delay_cycles(100); // 100 cycles = 500ns
	sclk_set();
	result |= miso_rd();
	__delay_cycles(100); // 100 cycles = 500ns
	sclk_clr();
	}

        cs_set(); // Release SPI bus
	__delay_cycles(100); // 100 cycles = 500ns - CS Disable Time

        return result;
}

#define BUFFER_SZ 100
uint16_t buffer[BUFFER_SZ];

/*
 * main.c
 */
void main(void)
{
	struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;
	volatile uint8_t *status;

	/* AM335x must enable OCP master port access in order for the PRU to
	 * read external memories.
	 */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	/* Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
	status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

	/* Initialize the RPMsg transport structure */
	pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

        cs_set(); //Set CS to High (idle)
        __delay_cycles(200); // 200 cycles = 1000ns


	/* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
	while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
        while(1) {
		/* Check bit 31 of register R31 to see if the ARM has kicked us */
		if (__R31 & HOST_INT) {
			/* Clear the event status */
			CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
			/* Receive all available messages, multiple messages can be sent per kick */
			if (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
			//int ch = payload;
			int i;
                          for (i=0; i<BUFFER_SZ; i++){
				  //for(j=0; j<ch;j++) {
                            		buffer[i] = convert();
				  }
                          pru_rpmsg_send(&transport, dst, src, buffer, sizeof buffer);
                        }
		}
        }
	__halt();
}
