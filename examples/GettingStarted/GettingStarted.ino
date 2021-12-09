// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <Arduino.h>
#include <logging.h>											// Step 1. add an #include to include the library into your project

uLog theLog;													// Step 2. Create a global object to log things, before any other code or global objects. 

void setup()
    {
	theLog.output(subSystem::general, loggingLevel::Info, "Entering Setup()\n");	// Step 3. Add logging statements in your code. This msg will be logged, even if Serial is not yet configured

	Serial.begin(115200);										//		Configure Serial, which is the output channel for this example
	Serial.flush();												//		Clean up the Serial output
	Serial.println();											//		Start output with a clean line
	delay(250);													//		Wait 250ms - just to show how the timestamping in the log output works

	theLog.setOutputIsAvailable(true);							// Step 4. Tell theLog that from now on the output is working, so from now on it can send it's logged contents there
	theLog.output(subSystem::general, loggingLevel::Info, "Exiting Setup()\n");		// Step 5. Adds another msg to the log, then outputs everyting
	}

void loop()
    {
    }


