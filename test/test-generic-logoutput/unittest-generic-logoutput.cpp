#define unitTest
#include <string.h>
#include <unity.h>
#include "logging.h"

bool outputFunction0(const char* contents) {
    TEST_ASSERT_EQUAL_STRING("lorem ipse", contents);
    return true;
}

bool outputFunction1(const char* contents) {
    TEST_MESSAGE(contents);
    uint32_t contentsLength = strlen(contents);
    TEST_ASSERT_LESS_OR_EQUAL(logItem::maxItemLength, contentsLength);
    return true;
}

bool loggingTime(char* contents, uint32_t length) {
    strcpy(contents, "2022-01-29T19:46:51Z");
    return true;
}

/*
2022-01-29T19:46:51+00:00
2022-01-29T19:46:51Z
20220129T194651Z
*/

void test_logOutput_initialization() {
    logOutput anOutput;                                                                               // create an instance
    TEST_ASSERT_FALSE(anOutput.isActive());                                                           // output should be inactive after construction
    TEST_ASSERT_FALSE(anOutput.isColoredOutput());                                                    // colored output should be OFF
    TEST_ASSERT_FALSE(anOutput.hasTimestampIncluded());                                               // timestamps should be OFF
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::None, anOutput.getLoggingLevel(subSystem::general));        // all subsystems should have loggingLevel intialized to None
}

void test_logOutput_settings() {
    logOutput anOutput;                                                                                   // create an instance
    anOutput.setOutputDestination(outputFunction0);                                                       // attach an output function callback
    TEST_ASSERT_TRUE(anOutput.isActive());                                                                // now output shoud be active
    anOutput.setOutputDestination(nullptr);                                                               // remove output callback
    TEST_ASSERT_FALSE(anOutput.isActive());                                                               // should be back to inactive
    anOutput.setColoredOutput(false);                                                                     // test setting this option
    TEST_ASSERT_FALSE(anOutput.isColoredOutput());                                                        //
    anOutput.setColoredOutput(true);                                                                      //
    TEST_ASSERT_TRUE(anOutput.isColoredOutput());                                                         //
    anOutput.setIncludeTimestamp(false);                                                                  // test setting this option
    TEST_ASSERT_FALSE(anOutput.hasTimestampIncluded());                                                   //
    anOutput.setIncludeTimestamp(true);                                                                   //
    TEST_ASSERT_TRUE(anOutput.hasTimestampIncluded());                                                    //
    anOutput.setLoggingLevel(loggingLevel::Warning);                                                      // set all loggingLevels for this output, for all subSystems
    anOutput.setLoggingLevel(subSystem::machine, loggingLevel::Critical);                                 // overrule one particular subSystem to another level
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Warning, anOutput.getLoggingLevel(subSystem::general));         // verify setting
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Critical, anOutput.getLoggingLevel(subSystem::machine));        //
}

void test_logOutput_write() {
    logOutput anOutput;                                    // create an instance
    anOutput.setOutputDestination(outputFunction0);        // attach an output callback
    bool result = anOutput.write("lorem ipse");            // write something,
    TEST_ASSERT_TRUE(result);                              // check it was succesfully written
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_logOutput_initialization);
    RUN_TEST(test_logOutput_settings);
    RUN_TEST(test_logOutput_write);
    UNITY_END();
}