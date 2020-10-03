# BipTask

The BipTask is a fork of [Yuukari's BipTaskTemplate repository]( https://github.com/Yuukari/BipTaskTemplate) 
except customised to suit my needs. It requires an android app as well which can be found 
[here](https://github.com/Yuukari/BipTask). BipTask is an application for BipOS 
(a modified firmware for the Amazefit Bip Device), further information can be found on 
[MyAmazefit website](https://myamazefit.ru). BipTask allows (with the use of Android **Tasker** application) 
to send commands to your phone from the Amazefit Bip watch.

# Features
- Application send a length 2 array of bytes, the first byte is used as a mode and the second is the data
- Contains 3 screen which can be navigated by swiping up or down
- Application starts in screen 2
- Screen 1 is use to send any mode/data
- Screen 2 and 3 have 4 corner buttons (for data sending number) and a center button to increment the mode number to send

# Building
## Requirements
- Requires GNU ARM toolchain - [gcc-arm-none-eabi-8-2019-q3-update-win32-sha2.exe](https://href.li/?https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- Requires the RePacker application for packing the binary elf into the resource file. Can be downloaded on MyAmazefit site [here](https://myamazfit.ru/threads/bip-razrabotka-prilozhenij-dlja-bipos-sdk.1170/)
- libbip library [here](https://github.com/MNVolkov/libbip)
- BipEmulator is a nice to have - [here](https://github.com/freebip/BipEmulator)

Compile the elf binary using the bat script (might need modification to point to wherever your GNU ARM toolchain is installed). 
Then pack the elf binary into the .res file using the **RePacker**