#ifndef __DCSBIOS_PINOPS_H
#define __DCSBIOS_PINOPS_H

namespace DcsBios {
	class DefaultPinOps {
	public:
		static inline void digitalWrite(uint8_t pin, uint8_t value) { digitalWrite(pin, value); }
		static inline uint8_t digitalRead(uint8_t pin) { return digitalRead(pin); }
		static inline void pinMode(uint8_t pin, uint8_t mode) { pinMode(pin, mode); }
		static inline int analogRead(uint8_t pin) { return analogRead(pin); }
	};
	
	#define DECLARE_ADAFRUIT_MCP23017_PINOPS(name, mcp) \
	class name { \
	public: \
		static inline void pinMode(uint8_t pin, uint8_t mode) { \
			switch(mode) { \
				case INPUT: \
					(mcp).pinMode((uint8_t)pin, (uint8_t)INPUT); \
					(mcp).pullUp(pin, LOW); \
				break; \
				 \
				case INPUT_PULLUP: \
					(mcp).pinMode(pin, INPUT); \
					(mcp).pullUp(pin, HIGH); \
				break; \
				 \
				case OUTPUT: \
					(mcp).pinMode(pin, OUTPUT); \
					(mcp).pullUp(pin, LOW); \
				break; \
				 \
			} \
		} \
		 \
		static inline void digitalWrite(uint8_t pin, uint8_t value) { \
			(mcp).digitalWrite(pin, value); \
		} \
		 \
		static inline uint8_t digitalRead(uint8_t pin) { \
			return (mcp).digitalRead(pin); \
		} \
	};
	
}
#endif

