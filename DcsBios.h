#ifndef __DCSBIOS_H
#define __DCSBIOS_H

#ifndef NULL
#define NULL 0
#endif

#include <stdint.h>

#include "ExportStreamListener.h"
#include "PollingInput.h"
#include "Protocol.h"

#include "DcsBiosNgRS485Master.h"
#include "DcsBiosNgRS485Slave.h"

/*
The following is an ugly hack to work with the Arduino IDE's build system.
The DCS-BIOS Arduino Library is configured with #defines such as DCSBIOS_RS485_MASTER or DCSBIOS_RS485_SLAVE <address>.
To make sure these defines are visible when compiling the code, we can't put it into a separate translation unit.

Normally, those #defines would go in a separate "config.h" or you would use compiler flags. But since Arduino libraries
do not come with their own build system, we are just putting everything into the header file.
*/
#include "DcsBiosNgRS485Master.cpp.inc"
#include "DcsBiosNgRS485Slave.cpp.inc"


#include "PinOps.h"
#include "Buttons.h"
#include "Switches.h"
#include "Encoders.h"
#include "Potentiometers.h"
#include "Leds.h"
#include "Servos.h"


#endif