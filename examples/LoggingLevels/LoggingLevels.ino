// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <Arduino.h>
#include <logging.h>

uLog theLog(loggingLevel::Error);								// Create a global object to log things. logginglevel is set to Error, which means everything of type Error (or worse) will be logged, and everything lower (like Warning) will NOT be logged

void setup()
    {
	Serial.begin(115200);										//		Configure Serial, which is the output channel for this example
	Serial.flush();												//		Clean up the Serial output
	Serial.println();											//		Start output with a clean line
	delay(250);													//		Wait a little so Serial Monitor on PC is ready to receive our output

	theLog.setOutputIsAvailable(true);

	theLog.output(loggingLevel::Error, "Error 1234 in Setup()");					// Add logging statements in your code. This msg will be logged as the level of the msg is equal or higher than the overall logging level
	theLog.output(loggingLevel::Warning, "Warning 1234 in Setup()");				// Message will not be logged, as Warning is less important than Error
	theLog.output(loggingLevel::Critical, "Critical Error 1234 in Setup()");		// Message will be logged as Critical Error is more important than Error

	theLog.pushLoggingLevel(loggingLevel::Info);									// We can adjust the logging level in the code, so from now on, all items will be logged. See logging.h for available levels
	theLog.output(loggingLevel::Warning, "Warning 5678 in Setup()");				// Message will be logged, as Warning is more important than Info
	}

void loop()
    {
    }


