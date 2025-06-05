/*
**
** Copyright 2020 OpenHW Group
**
** Licensed under the Solderpad Hardware Licence, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     https://solderpad.org/licenses/
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
*/

#include <stdint.h>
#include <stdio.h>

#define UART_BASE 0x10000000

#define UART_RBR UART_BASE + 0
#define UART_THR UART_BASE + 0
#define UART_INTERRUPT_ENABLE UART_BASE + 4
#define UART_INTERRUPT_IDENT UART_BASE + 8
#define UART_FIFO_CONTROL UART_BASE + 8
#define UART_LINE_CONTROL UART_BASE + 12
#define UART_MODEM_CONTROL UART_BASE + 16
#define UART_LINE_STATUS UART_BASE + 20
#define UART_MODEM_STATUS UART_BASE + 24
#define UART_DLAB_LSB UART_BASE + 0
#define UART_DLAB_MSB UART_BASE + 4

void write_reg_u8(uintptr_t addr, uint8_t value) {
    volatile uint8_t *loc_addr = (volatile uint8_t *)addr;
    *loc_addr = value;
}

uint8_t read_reg_u8(uintptr_t addr) {
    return *(volatile uint8_t *)addr;
}

int is_transmit_empty() {
    return read_reg_u8(UART_LINE_STATUS) & 0x20;
}

void write_serial(char a)
{
    while (is_transmit_empty() == 0) {};

    write_reg_u8(UART_THR, a);
}

void print_uart(const char *str) {

    const char *cur = &str[0];

    while (*cur != '\0') {
        write_serial((uint8_t)*cur);
        ++cur;
    }
}

int main(int argc, char* arg[]) {

    write_reg_u8(UART_LINE_CONTROL, 0x03);     // 8 bits, no parity, one stop bit
	print_uart("Hello World from UART!");

	int a = 0;
	for (int i = 0; i < 5; i++) {
		a += i;
	}

	return 0;
}
