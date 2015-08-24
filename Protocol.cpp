#include "Protocol.h"
#include "PollingInput.h"
#include "ExportStreamListener.h"

namespace DcsBios {
	ExportStreamListener* ExportStreamListener::firstExportStreamListener = NULL;
	PollingInput* PollingInput::firstPollingInput = NULL;

	ProtocolParser::ProtocolParser() {
		state = DCSBIOS_STATE_WAIT_FOR_SYNC;
		sync_byte_count = 0;
	}

	void ProtocolParser::processChar(unsigned char c) {
	  switch(state) {
		case DCSBIOS_STATE_WAIT_FOR_SYNC:
			/* do nothing */
			break;
			
		case DCSBIOS_STATE_ADDRESS_LOW:
			address = (unsigned int)c;
			state = DCSBIOS_STATE_ADDRESS_HIGH;
			break;
			
		case DCSBIOS_STATE_ADDRESS_HIGH:
			address = (c << 8) | address;
			if (address != 0x5555) {
				state = DCSBIOS_STATE_COUNT_LOW;
			} else {
				state = DCSBIOS_STATE_WAIT_FOR_SYNC;
			}
			break;
			
		case DCSBIOS_STATE_COUNT_LOW:
			count = (unsigned int)c;
			state = DCSBIOS_STATE_COUNT_HIGH;
			break;
			
		case DCSBIOS_STATE_COUNT_HIGH:
			count = (c << 8) | count;
			state = DCSBIOS_STATE_DATA_LOW;
			break;
			
		case DCSBIOS_STATE_DATA_LOW:
			data = (unsigned int)c;
			count--;
			state = DCSBIOS_STATE_DATA_HIGH;
			break;
			
		case DCSBIOS_STATE_DATA_HIGH:
			data = (c << 8) | data;
			count--;
			


			//ExportStreamListener::handleDcsBiosWrite(address, data);
			// skip all ESLs that cannot possibly be interested in the current address
			while(startESL && startESL->getLastAddressOfInterest() < address)
				startESL = startESL->nextExportStreamListener;
			if (startESL) {
				ExportStreamListener* el = startESL;
				// assumption: no overlapping ESL ranges
				while(el) {
					if (el->getFirstAddressOfInterest() > address) break;
					if (el->getFirstAddressOfInterest() <= address && el->getLastAddressOfInterest() >= address) {
					
						el->onDcsBiosWrite(address, data);
					}
					el = el->nextExportStreamListener;
				}
			}

			
			address += 2;
			if (count == 0)
				state = DCSBIOS_STATE_ADDRESS_LOW;
			else
				state = DCSBIOS_STATE_DATA_LOW;
			break;
	  }

	  if (c == 0x55)
		sync_byte_count++;
	  else
		sync_byte_count = 0;
	  
	  if (sync_byte_count == 4) {
		state = DCSBIOS_STATE_ADDRESS_LOW;
		sync_byte_count = 0;
		startESL = ExportStreamListener::firstExportStreamListener;
	  }
	  
	}
}