#ifndef __DCSBIOS_SWITCHES_H
#define __DCSBIOS_SWITCHES_H

#include "Arduino.h"

namespace DcsBios {

	template <class PinOps = DefaultPinOps>
	class Switch2Pos : PollingInput {
		private:
			char* msg_;
			char pin_;
			char lastState_;
			bool reverse_;
			void init_(char* msg, char pin, bool reverse) {
				msg_ = msg;
				pin_ = pin;
				PinOps::pinMode(pin_, INPUT_PULLUP);
				lastState_ = PinOps::digitalRead(pin_);
				reverse_ = reverse;
			}
			void pollInput() {
				char state = PinOps::digitalRead(pin_);
				if (reverse_) state = !state;
				if (state != lastState_) {
					if (sendDcsBiosMessage(msg_, state == HIGH ? "0" : "1")) {
						lastState_ = state;
					}
				}
			}
		public:
			Switch2Pos(char* msg, char pin, bool reverse) { init_(msg, pin, reverse); }
			Switch2Pos(char* msg, char pin) { init_(msg, pin, false); }
	};

	template <class PinOps = DefaultPinOps>
	class Switch3Pos : PollingInput {
		private:
			char* msg_;
			char pinA_;
			char pinB_;
			char lastState_;
			char readState() {
				if (PinOps::digitalRead(pinA_) == LOW) return 0;
				if (PinOps::digitalRead(pinB_) == LOW) return 2;
				return 1;
			}
			void pollInput() {
				char state = readState();
				if (state != lastState_) {
					if (state == 0)
						if (sendDcsBiosMessage(msg_, "0"))
							lastState_ = state;
					if (state == 1)
						if (sendDcsBiosMessage(msg_, "1"))
							lastState_ = state;
					if (state == 2)
						if(sendDcsBiosMessage(msg_, "2"))
							lastState_ = state;
				}
			}
		public:
			Switch3Pos(char* msg, char pinA, char pinB) {
				msg_ = msg;
				pinA_ = pinA;
				pinB_ = pinB;
				PinOps::pinMode(pinA_, INPUT_PULLUP);
				PinOps::pinMode(pinB_, INPUT_PULLUP);
				lastState_ = readState();
			}
	};

	template <class PinOps = DefaultPinOps>
	class SwitchMultiPos : PollingInput {
		private:
			char* msg_;
			const byte* pins_;
			char numberOfPins_;
			char lastState_;
			char readState() {
				char i;
				for (i=0; i<numberOfPins_; i++) {
					if (PinOps::digitalRead(pins_[i]) == LOW) return i;
				}
				return 0;
			}
			void pollInput() {
				char state = readState();
				if (state != lastState_) {
					char buf[7];
					utoa(state, buf, 10);
					if (sendDcsBiosMessage(msg_, buf))
						lastState_ = state;
				}
			}
		public:
			SwitchMultiPos(char* msg, const byte* pins, char numberOfPins) {
				msg_ = msg;
				pins_ = pins;
				numberOfPins_ = numberOfPins;
				char i;
				for (i=0; i<numberOfPins; i++) {
					PinOps::pinMode(pins[i], INPUT_PULLUP);
				}
				lastState_ = readState();
			}
	};

}

#endif	