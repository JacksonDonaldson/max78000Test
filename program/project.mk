# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analog-devices-msdk.github.io/msdk/USERGUIDE/#build-system

#MXC_OPTIMIZE_CFLAGS = -Og
# ^ For example, you can uncomment this line to 
# optimize the project for debugging

# **********************************************************

# Add your config here!

# This example is only compatible with the FTHR board,
# so we override the BOARD value to hard-set it.
MAXIM_PATH=./msdk-2024_02
override BOARD=FTHR_RevA
MFLOAT_ABI=soft

IPATH+=../deployment
IPATH+=inc/
IPATH+=/secrets
VPATH+=src/

# ****************** eCTF Bootloader *******************
# DO NOT REMOVE
#LINKERFILE=firmware.ld
#STARTUPFILE=startup_firmware.S
#ENTRY=firmware_startup

