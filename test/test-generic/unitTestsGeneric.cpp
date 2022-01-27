#define UnitTesting
#include <string.h>
#include <unity.h>
#include "logging.h"

bool outputFunction(const char* contents) {
    TEST_ASSERT_EQUAL_STRING("lorem ipse", contents)    ;
    return true;
}

bool loggingTime(char* contents, uint32_t length) {
    strcpy(contents, "123");
    return true;
}

void test_logoutput_initialization() {
    logOutput anOutput;        // create an instance
    TEST_ASSERT_FALSE(anOutput.isActive());
    TEST_ASSERT_FALSE(anOutput.isColored());
    TEST_ASSERT_FALSE(anOutput.hasTimestamp());
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::None, anOutput.getLoggingLevel(subSystem::general));
}

void test_logoutput_settings() {
    logOutput anOutput;        // create an instance
    anOutput.setOutputDestination(outputFunction);
    TEST_ASSERT_TRUE(anOutput.isActive());
    anOutput.setOutputDestination(nullptr);
    TEST_ASSERT_FALSE(anOutput.isActive());
    anOutput.setColoredOutput(false);
    TEST_ASSERT_FALSE(anOutput.isColored());
    anOutput.setColoredOutput(true);
    TEST_ASSERT_TRUE(anOutput.isColored());
    anOutput.setIncludeTimestamp(false);
    TEST_ASSERT_FALSE(anOutput.hasTimestamp());
    anOutput.setIncludeTimestamp(true);
    TEST_ASSERT_TRUE(anOutput.hasTimestamp());
    anOutput.setLoggingLevel(loggingLevel::Warning);
    anOutput.setLoggingLevel(subSystem::machine, loggingLevel::Critical);
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Warning, anOutput.getLoggingLevel(subSystem::general));
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Critical, anOutput.getLoggingLevel(subSystem::machine));
}

void test_logoutput_write() {
    logOutput anOutput;        // create an instance
    anOutput.setOutputDestination(outputFunction);
    bool result = anOutput.write("lorem ipse");
    TEST_ASSERT_TRUE(result);
}

// void test_initialization() {
//     uLog theLog;        // create an instance of the logging object, default parameters
//     TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, theLog.bufferLevel, "Buffer should be empty after init");
//     TEST_ASSERT_EQUAL_CHAR_MESSAGE(0, theLog.logBuffer[0], "Buffer should be properly 0-terminated after init");
// }

// void test_boundaries() {
//     uLog theLog;
//     char tmpStr[theLog.maxItemLength + 4];        // putting some data in the testString
//     for (int x = 0; x < theLog.maxItemLength + 4; x++) {
//         tmpStr[x] = '*';
//     }
//     tmpStr[theLog.maxItemLength + 3] = 0x00;
//     theLog.output(subSystems::general, loggingLevel::Critical, tmpStr);                          // add a msg longer than maximum length of an item
//     TEST_ASSERT_EQUAL_UINT32_MESSAGE(theLog.maxItemLength, theLog.bufferLevel, "");              // check buffer contains not more than maximum length
//     TEST_ASSERT_EQUAL_CHAR_MESSAGE(theLog.logBuffer[theLog.maxItemLength - 1], 0x00, "");        // check buffer is properly terminated
// }

// void test_level_filtering() {
// }

// void test_outputs_available() {
//      uLog theLog(loggingLevel::Warning);
//             theLog.output(loggingLevel::Critical, "123");                                                             // output a msg when output is not yet avaialble
//             theLog.output(loggingLevel::Critical, "456");                                                             // add a second msg
//             Assert::IsTrue(2 * (theLog.timestampLength + strlen("-Critical-") + 3 + 1) == theLog.bufferLevel);        // do all msgs get concatenated correcly ?
//             theLog.setOutputIsAvailable(true);                                                                        // now make output available
//             Assert::IsTrue(theLog.outputIsAvailable);                                                                 // check if setting was modified
//             Logger::WriteMessage(theLog.logBuffer);
//             theLog.output(loggingLevel::Critical, "789");        // add a third msg, everything should go to output
//             Assert::IsTrue(0 == theLog.bufferLevel);             // remaining buffer should be empty
//             Logger::WriteMessage(theLog.logBuffer);
// }

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_logoutput_initialization);
    RUN_TEST(test_logoutput_settings);
    RUN_TEST(test_logoutput_write);
    UNITY_END();
}

// TEST_CLASS(C01_LoggingTests) {
//   public:
//     TEST_METHOD(T03_CheckLogLevelFilter) {
//         {
//             uLog theLog(loggingLevel::Warning);
//             theLog.log(loggingLevel::Info, "some info");        // message 'below' level should not be logged...
//             Assert::IsTrue(0 == theLog.bufferLevel);
//             theLog.log(loggingLevel::Warning, "some warning");        // message with same level should be logged
//             Assert::IsTrue(0 < theLog.bufferLevel);
//         }
//         {
//             uLog theLog(loggingLevel::Warning);
//             theLog.log(loggingLevel::Critical, "some critical error");        // message with higher level should be logged as well
//             Assert::IsTrue(0 < theLog.bufferLevel);
//         }
//     }

//     TEST_METHOD(T04_CheckLoggingItems) {
//         {
//             uLog theLog(loggingLevel::Warning);
//             theLog.log(loggingLevel::Critical, "0123456789");        // add a msg of exactly 10 bytes
//             Assert::IsTrue((theLog.timestampLength + strlen("-Critical-") + 10 + 1) == theLog.bufferLevel);
//         }
//         {
//             uLog theLog(loggingLevel::Warning);
//             theLog.log(loggingLevel::Critical, "123");                                                                // add a first msg
//             theLog.log(loggingLevel::Critical, "456");                                                                // add a second msg
//             Assert::IsTrue(2 * (theLog.timestampLength + strlen("-Critical-") + 3 + 1) == theLog.bufferLevel);        // do all msgs get concatenated correcly ?
//         }
//     }

//     TEST_METHOD(T05_OutputAvailable) {
//         {

//         }
//     }

//     TEST_METHOD(T06_Timestamp) {
//         {
//             uLog theLog(loggingLevel::Warning, false);           // create a logger with timestamp disabled
//             theLog.setIncludeTimestamp(true);                    // now activate timestamp
//             Assert::IsTrue(theLog.includeTimestamp);             // is timestamp properly enabled ?
//             theLog.output(loggingLevel::Critical, "123");        // add a msg, should now have timestamp.. note : unit test under windows will not have a real timestamp - refer to unit test on target for this
//             Logger::WriteMessage(theLog.logBuffer);
//             Assert::IsTrue((theLog.timestampLength + strlen("-Critical-") + 3 + 1) == theLog.bufferLevel);        // check buffer contains timestamp, - and logged text
//         }
//     }

//     TEST_METHOD(T07_BufferOverflowRobustness) {
//     }

//     TEST_METHOD(T08_BufferOverflowRobustness) {
//         uLog theLog(loggingLevel::Warning);        // create a logger
//         char tmpStr[2];
//         tmpStr[0] = '*';
//         tmpStr[1] = 0x00;        // putting some data in the testString
//         for (int x = 0; x < (theLog.bufferLength + 4); x++) {
//             theLog.output(loggingLevel::Critical, tmpStr);        // add msgs until buffer is full
//         }
//         Assert::IsTrue(theLog.bufferLength <= theLog.bufferLevel);                                    // check buffer contains not more than maximum length
//         Assert::IsTrue(strnlen(theLog.logBuffer, theLog.bufferLength) <= theLog.bufferLength);        // check buffer is properly terminated
//     }

//     TEST_METHOD(T09_snprintf) {
//         uLog theLog(loggingLevel::Warning);                                                                   // create a logger
//         theLog.snprintf(loggingLevel::Critical, "test %d", 123);                                              // add a printf style msg
//         Assert::IsTrue((theLog.timestampLength + strlen("-Critical-") + 8 + 1) == theLog.bufferLevel);        // check buffer contains not more than maximum length
//                                                                                                               // TODO : add some tests to check the robustness of vsnprintf for limiting output to the output buffer available
//     }
