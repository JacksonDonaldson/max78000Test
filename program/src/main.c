#include <stdio.h>
#include "mxc_device.h"
#include "icc.h"
typedef unsigned int uint;

void print_icc_state(char* base_addr){

    
    printf("INFO: %08x\n", *((uint*) (base_addr + 0x0)));
    printf("SZ:   %08x\n", *((uint*) (base_addr + 0x4)));
    printf("CTRL: %08x\n", *((uint*) (base_addr + 0x100)));
    printf("IVAL: %08x\n\n", *((uint*) (base_addr + 0x700)));
    


}

int main(){
    printf("Post-boot ICC0:\n");
    print_icc_state((char*)(0x4002a000));

    MXC_ICC_Enable(MXC_ICC0);

    printf("Enabled ICC0:\n");
    print_icc_state((char*)(0x4002a000));

    MXC_ICC_Disable(MXC_ICC0);

    printf("Disabled ICC0:\n");
    print_icc_state((char*)(0x4002a000));

    printf("main finished\n");
    while(1){}
}