#ifndef __DCSBIOS_ENCODERS_H
#define __DCSBIOS_ENCODERS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {
	template <class PinOps = DefaultPinOps>
	class RotaryEncoder : PollingInput {
		private:
			const char* msg_;
			const char* decArg_;
			const char* incArg_;
			char pinA_;
			char pinB_;
			char lastState_;
			signed char delta_;
			char readState() {
				return (PinOps::digitalRead(pinA_) << 1) | PinOps::digitalRead(pinB_);
			}
			void pollInput() {
				char state = readState();
				switch(lastState_) {
					case 0:
						if (state == 2) delta_--;
						if (state == 1) delta_++;
						break;
					case 1:
						if (state == 0) delta_--;
						if (state == 3) delta_++;
						break;
					case 2:
						if (state == 3) delta_--;
						if (state == 0) delta_++;
						break;
					case 3:
						if (state == 1) delta_--;
						if (state == 2) delta_++;
						break;
				}
				lastState_ = state;
				
				if (delta_ >= 4) {
					if (sendDcsBiosMessage(msg_, incArg_))
						delta_ -= 4;
				}
				if (delta_ <= -4) {
					if (sendDcsBiosMessage(msg_, decArg_))
						delta_ += 4;
				}
			}
		public:
			RotaryEncoder(const char* msg, const char* decArg, const char* incArg, char pinA, char pinB) {
				msg_ = msg;
				decArg_ = decArg;
				incArg_ = incArg;
				pinA_ = pinA;
				pinB_ = pinB;
				PinOps::pinMode(pinA_, INPUT_PULLUP);
				PinOps::pinMode(pinB_, INPUT_PULLUP);
				delta_ = 0;
				lastState_ = readState();
			}
	};
}

#endif