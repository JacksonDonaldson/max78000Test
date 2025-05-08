#include <stdio.h>
#include "mxc_device.h"
#include "icc.h"
#include "uart.h"
#include "mxc_delay.h"
#include "led.h"
#include "trng.h"
#include "aes.h"

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

void print_gcr_state(){
    char* base_addr = (char*) 0x40000000;
    printf("SYSCTRL: %08x\n", reg(base_addr, 0));
    printf("RST0:    %08x\n", reg(base_addr, 4));
    printf("CLKCTRL: %08x\n", reg(base_addr, 8));
    printf("PM:      %08x\n", reg(base_addr, 0xc));
    printf("PCLKDIV: %08x\n", reg(base_addr, 0x18));
    printf("PCLKDIS0:%08x\n", reg(base_addr, 0x24));
    printf("MEMCTRL: %08x\n", reg(base_addr, 0x28));
    printf("MEMZ:    %08x\n", reg(base_addr, 0x2c));
    printf("SYSST:   %08x\n", reg(base_addr, 0x40));
    printf("RST1:    %08x\n", reg(base_addr, 0x44));
    printf("PCLKDIS1:%08x\n", reg(base_addr, 0x48));
    printf("EVENTEN: %08x\n", reg(base_addr, 0x4c));
    printf("REVISIO: %08x\n", reg(base_addr, 0x50));
    printf("SYSIE:   %08x\n", reg(base_addr, 0x54));
    printf("ECCERR:  %08x\n", reg(base_addr, 0x64));
    printf("ECCCED:  %08x\n", reg(base_addr, 0x68));
    printf("ECCIE:   %08x\n", reg(base_addr, 0x6c));
    printf("ECCADDR: %08x\n", reg(base_addr, 0x70));

}

void print_trng_state(){
    char* base_addr = (char*) 0x4004d000;
    printf("CTRL:   %08x\n", reg(base_addr, 0));
    printf("STATUS: %08x\n", reg(base_addr, 4));
    printf("DATA:   %08x\n", reg(base_addr, 8));
    printf("\n");
}

void print_aes_state(){
    char* base_addr = (char*) 0x40007400;
    printf("CTRL:   %08x\n", reg(base_addr, 0));
    printf("STATUS: %08x\n", reg(base_addr, 4));
    printf("INTFL:  %08x\n", reg(base_addr, 8));
    printf("INTEN:  %08x\n", reg(base_addr, 0xc));
    printf("FIFO:   %08x\n", reg(base_addr, 0x10));
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
    printf("Starting UART%d: \n", num);
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

    print_serial_state(uart0);

}


void test_gcr(){
    printf("GCR state:\n");
    print_gcr_state();
}

void test_mem_reset(uint* addr, uint reset){
    volatile uint* memz = (uint*)0x4000002c;
    *addr = 0x12345678;
    printf("initial value: %08x\n", *addr);
    *memz |= reset;
    while(*memz & reset){}
    printf("value after memz: %08x\n", *addr);
}

void TRNG_IRQHandler(void)
{
    MXC_TRNG_Handler();
}

volatile int wait;
void trng_callback(void *req, int result)
{
    printf("trng callback: %d\n", result);
    wait = 0;
}

void test_trng(){
    uint data;

    print_trng_state();
    MXC_TRNG_Init();
    printf("after init\n");
    print_trng_state();

    for (int i = 0; i < 5; i++){
        
        MXC_TRNG_Random((unsigned char *)&data, 4);
        printf("random data: %08x\n", data);
    }
    printf("after random\n");
    print_trng_state();

    MXC_TRNG_Shutdown();
    printf("after shutdown\n");
    print_trng_state();

    printf("async rng:\n");
    MXC_TRNG_Init();
    print_trng_state();

    wait = 1;
    NVIC_EnableIRQ(TRNG_IRQn);
    for(int i = 0; i < 5; i++){
        MXC_TRNG_RandomAsync((unsigned char *)&data, 4, &trng_callback);
        while(wait){printf("waiting\n");}
        printf("random data: %08x\n", data);
    }
    MXC_TRNG_Shutdown();
    printf("after shutdown\n");
    print_trng_state();

}

void AES_encrypt(mxc_aes_keys_t key, uint32_t* data, uint32_t* encrypted, uint len)
{
    mxc_aes_req_t req;
    req.length = len;
    req.inputData = data;
    req.resultData = encrypted;
    req.keySize = key;
    req.encryption = MXC_AES_ENCRYPT_EXT_KEY;

    MXC_AES_Encrypt(&req);
}

void AES_decrypt(mxc_aes_keys_t key, uint32_t* data, uint32_t* decrypted, uint len)
{
    mxc_aes_req_t req;
    req.length = len;
    req.inputData = data;
    req.resultData = decrypted;
    req.keySize = key;
    req.encryption = MXC_AES_DECRYPT_INT_KEY;

    MXC_AES_Decrypt(&req);
}

void single_aes_test(uint keytype){
    uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    MXC_AES_SetExtKey(key, keytype);
    uint8_t data[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    uint32_t encrypted[4] = {0};
    uint32_t decrypted[4] = {1,2,3,4};
    AES_encrypt(keytype, (uint32_t*)data, encrypted, 4);
    printf("encrypted to : %08x %08x %08x %08x\n", encrypted[0], encrypted[1], encrypted[2], encrypted[3]);

    //MXC_AES_SetExtKey(key, keylen);
    //print_aes_state();
    AES_decrypt(keytype, encrypted, decrypted, 4);
    printf("decrypted to : %08x %08x %08x %08x\n", decrypted[0], decrypted[1], decrypted[2], decrypted[3]);

}
void test_aes(){
    print_aes_state();
    MXC_AES_Init();
    printf("after init\n");
    print_aes_state();
    single_aes_test(MXC_AES_128BITS);
    print_aes_state();
    MXC_AES_Init();
    single_aes_test(MXC_AES_192BITS);
    MXC_AES_Init();
    //print_aes_state();
    single_aes_test(MXC_AES_256BITS);
    print_aes_state();
    printf("AES test done\n");
}

int main(){    
    printf("started\n");
    
    while(1){
        int incoming = MXC_UART_ReadCharacter(MXC_UART0);
        switch (incoming)
        {
        case 'i':
            printf("Test ICC:\n");
            test_icc();
            break;
        case 'g':
            printf("Test gcr:\n");
            print_gcr_state();
            break;
        case 'r':
            MXC_SYS_Reset_Periph(31);
            break;
        case 'u':
            incoming = MXC_UART_ReadCharacter(MXC_UART0);
            if(incoming == '1'){
                MXC_SYS_Reset_Periph(12);
                test_uart_num(1, (char*)0x40043000);
            }
            else if(incoming == '2'){
                MXC_SYS_Reset_Periph(28);
                test_uart_num(2, (char*)0x40044000);
            }
            else{
                printf("Test uart\n");
                test_uart();
            }
            break;
        case 'c':
            printf("Initial gcr state:\n");
            print_gcr_state();

            MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);

            MXC_SYS_Clock_Select(MXC_SYS_CLOCK_ISO);

            printf("Final gcr state:\n");
            print_gcr_state();
            break;
        case 'z':
            // Resetting the memory you're working in is a bad plan, hence the lack of 
            // tests for RAM0 and RAM3.

            //printf("ram0");
            //test_mem_reset((uint*)0x20000010, MXC_F_GCR_MEMZ_RAM0);
            printf("ram1");
            test_mem_reset((uint*)0x20008010, MXC_F_GCR_MEMZ_RAM1);
            printf("ram2");
            test_mem_reset((uint*)0x20010010, MXC_F_GCR_MEMZ_RAM2);
            //printf("ram3");
            //test_mem_reset((uint*)0x2001c010, MXC_F_GCR_MEMZ_RAM3);
            break;
        case 't':
            printf("Test TRNG\n");
            test_trng();
        
            break;

        case 'a':
            printf("Test aes\n");
            test_aes();
            break;
        default:
            break;
        }
    }
        
}