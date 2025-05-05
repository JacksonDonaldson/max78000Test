#include <stdio.h>
#include "mxc_device.h"
#include "icc.h"
#include "uart.h"
#include "mxc_delay.h"
#include "led.h"
typedef unsigned int uint;

#define reg(base, offset) (*(uint*) (base + offset))
#define UART_BAUD 115200

void print_icc_state(char* base_addr){
    printf("INFO: %08x\n", reg(base_addr, 0));
    printf("SZ:   %08x\n", reg(base_addr, 4));
    printf("CTRL: %08x\n", reg(base_addr, 0x100));
    printf("IVAL: %08x\n", reg(base_addr, 0x700));
    printf("\n");
}

void print_serial_state(char* base_addr){
    printf("CTRL: %08x\n", reg(base_addr, 0));
    printf("STAT: %08x\n", reg(base_addr, 4));
    printf("INTE: %08x\n", reg(base_addr, 8));
    printf("INTF: %08x\n", reg(base_addr, 0xc));
    printf("CLKD: %08x\n", reg(base_addr, 0x10));
    printf("OSR:  %08x\n", reg(base_addr, 0x14));
    printf("TXPE: %08x\n", reg(base_addr, 0x18));
    printf("PNR:  %08x\n", reg(base_addr, 0x1c));
    printf("FIFO: %08x\n", reg(base_addr, 0x20));
    printf("DMA:  %08x\n", reg(base_addr, 0x30));
    printf("WKEN: %08x\n", reg(base_addr, 0x34));
    printf("WKFL: %08x\n", reg(base_addr, 0x38));
    printf("\n");
}

void test_icc(){
    printf("Post-boot ICC0:\n");
    print_icc_state((char*)(0x4002a000));

    MXC_ICC_Enable(MXC_ICC0);

    printf("Enabled ICC0:\n");
    print_icc_state((char*)(0x4002a000));

    MXC_ICC_Disable(MXC_ICC0);

    printf("Disabled ICC0:\n");
    print_icc_state((char*)(0x4002a000));
}

void test_uart_num(int num, char* addr){
    printf("Post-boot UART%d: \n", num);
    print_serial_state(addr);


    printf("Initializing UART%d\n", num);

    mxc_uart_regs_t * target = MXC_UART0;
    if(num == 1){
        target = MXC_UART1;
    }
    else if(num == 2){
        target = MXC_UART2;
    }

    int error = MXC_UART_Init(target, UART_BAUD, MXC_UART_APB_CLK);
    if(error != E_NO_ERROR){
        printf("Error initializing UART: %d\n", error);
        return;
    }
    printf("after initializing UART%d\n", num);
    print_serial_state(addr);

}
void test_uart(){
    char* uart0 = (char*)0x40042000;
    test_uart_num(0, uart0);
    
    int first_char = MXC_UART_ReadCharacter(MXC_UART0);
    printf("read char %c\n", (char)first_char);

    
    

    test_uart_num(1, (char*)0x40043000);
    test_uart_num(2, (char*)0x40044000);

    int freq = MXC_UART_GetFrequency(MXC_UART0);
    printf("default UART0 freq: %08x\n", freq);
    MXC_UART_SetFrequency(MXC_UART0, 9600, MXC_UART_APB_CLK);
    freq = MXC_UART_GetFrequency(MXC_UART0);
    MXC_UART_SetFrequency(MXC_UART0, 115200, MXC_UART_APB_CLK);

    printf("set UART0 freq: %08x\n", freq);

    

    //MXC_UART_Shutdown(MXC_UART0);
    //printf("post-shutdown:\n");
    print_serial_state(uart0);

}

int main(){
    test_uart();
    test_icc();
    

    printf("main finished\n");

    while(1){
        LED_Off(LED1);
        MXC_Delay(500000);
        LED_On(LED1);
        MXC_Delay(500000);}
        
}