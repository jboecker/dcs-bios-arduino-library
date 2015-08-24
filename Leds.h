#ifndef __DCSBIOS_LEDS_H
#define __DCSBIOS_LEDS_H

#include "Arduino.h"
#include "ExportStreamListener.h"

namespace DcsBios {

	template <class PinOps = DefaultPinOps>
	class LED : public Int16Buffer {
		private:
			unsigned char pin;
			unsigned int mask;
		public:
			LED(unsigned int address, unsigned int mask, char pin) : Int16Buffer(address), mask(mask), pin(pin) {
				PinOps::pinMode(pin, OUTPUT);
			}
			virtual void loop() {
				if (hasUpdatedData()) {
					if (getData() & mask) {
						PinOps::digitalWrite(pin, 1);
					} else {
						PinOps::digitalWrite(pin, 0);
					}
				}
			}
	};

}

#endif