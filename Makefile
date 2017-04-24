BOARD_TAG         = mega
BOARD_SUB         = atmega2560

### Default is 115200, if not set
# MONITOR_BAUDRATE  = 115200

### The usb-port your board is connected to - if not set: it will autodetect
# MONITOR_PORT      = /dev/tty.USB47-or-whatever

### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
AVR_TOOLS_DIR     = utils/atmel-avr-tools

### Path to Arduino Hardware Lib
ARDUINO_STDLIB    = deps/StdLib-DuinoHardware

OPTIMIZATION_LEVEL = s # 3 # s
# *TODO* cleanup below

### CFLAGS_STD
# CFLAGS_STD        =

### CXXFLAGS_STD
# CXXFLAGS_STD      =

### C++FLAGS
# CXXFLAGS         = -pedantic -Wall -Wextra

### Include the work horse that does the actual work!
include utils/wirduino-makefile/Arduino.mk
