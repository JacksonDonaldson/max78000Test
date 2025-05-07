A test program for the MAX78000FTHR intended to evaluate development of a QEMU implementation for that board

Based on https://github.com/mitre-cyber-academy/2025-ectf-insecure-example

Build:
./build.sh
    Simple dockerized build; output stored in 'build' directory. Use this to produce a binary for qemu (max78000.bin)

Flashing to a real board:
    Requires installation of msdk tools. https://analogdevicesinc.github.io/msdk/USERGUIDE/
    cd program
    make release
    make flash.openocd

Run test:
    QEMU:
        qemu-system-arm -machine max78000fthr -kernel max78000.bin -device loader,file=max78000.bin,,addr=0x10000000 -serial tcp::4444,server

        Then to get output:
            nc localhost 4444
        and send any character to test UART

        note: The above command loads the binary into the kernel, to set the reset vector and at address 0x10000000 as that's where code is stored.
        This is probably not a best practice, seems like it should be a single load somehow; I'm just not sure how you'd do that.

    Hardware:
        flash to the board
        connect to board via serial
        send any character to test UART

Sample output logs from QEMU and hardware are stored in output_QEMU and output_hardware respectively.

Known differences between QEMU and hardware output:

    Operations that may take some time in hardware occur instantly.

    ICC:
        bit 7 of icc_ctrl is a 1 in hardware after disabling the cache but not in qemu. This field is marked reserved in the user guide.

    UART:
        Hardware initializes UART to all zeros, while qemu initializes UART to the claimed Power-on-reset values.

        bits 0,1, 4, 5, 6, and 7 of uart_status differ between hardware and QEMU. They note the fullness of the rx and tx queue, so this is ok.
    
        bits 0 of PNR occasionally differs. It stores the status of UART CTS, which isn't modelled.

        txpeek, fifo, and intfl occasionally differ when QEMU has data in the queue, while hardware might not, or the other way around

    GCR:
        clkctrl bit 15 is a 0 in qemu but a 1 in hardware; this is a reserved field
        clkctrl bits 25-29 occasionally differ; for simplicity, qemu reports all clocks as ready all the time
        pclkdis1 bit 11 is a 1 in qemu but a 0 in hardware; this is a reserved field

    TRNG:
        qemu TRNG is always active, while hardware requires initialization.
        