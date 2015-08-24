#ifndef __DCSBIOS_POTS_H
#define __DCSBIOS_POTS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {

	template <class PinOps = DefaultPinOps>
	class Potentiometer : PollingInput {
		private:
			void pollInput() {
				unsigned int state = map(PinOps::analogRead(pin_), 0, 1023, 0, 65535);
				if (state != lastState_) {
					char buf[6];
					utoa(state, buf, 10);
					if (sendDcsBiosMessage(msg_, buf))
						lastState_ = state;
				}
			}
			char* msg_;
			char pin_;
			unsigned int lastState_;
		public:
			Potentiometer(char* msg, char pin) {
				msg_ = msg;
				pin_ = pin;
				pinMode(pin_, INPUT);
				lastState_ = map(PinOps::analogRead(pin_), 0, 1023, 0, 65535);
			}
	};

}

#endif