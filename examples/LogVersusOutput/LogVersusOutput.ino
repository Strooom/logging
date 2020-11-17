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

uLog theLog(loggingLevel::Info);								// Create a global object to log things.

void setup()
    {
	Serial.begin(115200);										//		Start Serial
	Serial.flush();												//		Clean up the Serial output
	Serial.println();											//		Start output with a clean line
	delay(250);													//		Wait a little so Serial Monitor on PC is ready to receive our output

	theLog.setOutputIsAvailable(true);

	theLog.log(loggingLevel::Info, "Some message to be logged .................... in Setup()");	// .log() will add the message to the log, but not send the log to the ouput. 
	theLog.log(loggingLevel::Info, "Another message to be logged ................. in Setup()");	// .log() returns faster than .output()

	delay(2000);

	theLog.output(loggingLevel::Info, "third message to be logged ................... in Setup()");	// .output() will add the message to the log AND send the log to the ouput.
	theLog.output(loggingLevel::Info, "fourth message to be logged .................. in Setup()");	// all four lines will appear in the output after the 2 seconds delay, but with the correct timestamp

	delay(2000);
	uint8_t value = 123;
	theLog.snprintf(loggingLevel::Info, "This is how you can do printf kind of things in your logs : value = %d", value);	// .snprintf() always outputs immediately, as the underlying vprintf is slow anyway
	}

void loop()
    {
    }


