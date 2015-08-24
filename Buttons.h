#ifndef __DCSBIOS_BUTTONS_H
#define __DCSBIOS_BUTTONS_H

#include "Arduino.h"

namespace DcsBios {
	template <class PinOps = DefaultPinOps>
	class ActionButton : PollingInput {
		private:
			char* msg_;
			char* arg_;
			char pin_;
			char lastState_;
			void pollInput() {
				char state = PinOps::digitalRead(pin_);
				if (state != lastState_) {
					if (lastState_ == HIGH && state == LOW) {
						if (sendDcsBiosMessage(msg_, arg_))
							lastState_ = state;
					}
				}
			}
		public:
			ActionButton(char* msg, char* arg, char pin) {
				msg_ = msg;
				arg_ = arg;
				pin_ = pin;
				PinOps::pinMode(pin_, INPUT_PULLUP);
				lastState_ = PinOps::digitalRead(pin_);
			}
	};
}

#endif