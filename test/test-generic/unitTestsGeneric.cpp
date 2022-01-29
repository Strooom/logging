#define UnitTesting
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
    strcpy(contents, "123");
    return true;
}

void test_logOutput_initialization() {
    logOutput anOutput;        // create an instance
    TEST_ASSERT_FALSE(anOutput.isActive());
    TEST_ASSERT_FALSE(anOutput.isColoredOutput());
    TEST_ASSERT_FALSE(anOutput.hasTimestampIncluded());
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::None, anOutput.getLoggingLevel(subSystem::general));
}

void test_logOutput_settings() {
    logOutput anOutput;        // create an instance
    anOutput.setOutputDestination(outputFunction0);
    TEST_ASSERT_TRUE(anOutput.isActive());
    anOutput.setOutputDestination(nullptr);
    TEST_ASSERT_FALSE(anOutput.isActive());
    anOutput.setColoredOutput(false);
    TEST_ASSERT_FALSE(anOutput.isColoredOutput());
    anOutput.setColoredOutput(true);
    TEST_ASSERT_TRUE(anOutput.isColoredOutput());
    anOutput.setIncludeTimestamp(false);
    TEST_ASSERT_FALSE(anOutput.hasTimestampIncluded());
    anOutput.setIncludeTimestamp(true);
    TEST_ASSERT_TRUE(anOutput.hasTimestampIncluded());
    anOutput.setLoggingLevel(loggingLevel::Warning);
    anOutput.setLoggingLevel(subSystem::machine, loggingLevel::Critical);
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Warning, anOutput.getLoggingLevel(subSystem::general));
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Critical, anOutput.getLoggingLevel(subSystem::machine));
}

void test_logOutput_write() {
    logOutput anOutput;        // create an instance
    anOutput.setOutputDestination(outputFunction0);
    bool result = anOutput.write("lorem ipse");
    TEST_ASSERT_TRUE(result);
}

void test_uLog_initialization() {
    uLog aLog;
    aLog.setOutput(0, outputFunction0);
    aLog.setTimeSource(loggingTime);
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Warning);
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Warning, aLog.getLoggingLevel(0, subSystem::general));
}

void test_uLog_filtering() {
    uLog aLog;
    // filtering of an individual output
    aLog.setOutput(0, outputFunction0);                                                            // activate a first output
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Warning);                            // give it some level/subsystem settings
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Warning));        // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Info));          // lower level should NOT be logged

    // general filtering taking into account all outputs
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Warning));        // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Info));          // lower level should NOT be logged

    // now add a second output with
    aLog.setOutput(1, outputFunction1);
    aLog.setLoggingLevel(1, subSystem::general, loggingLevel::Info);                            //
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Info));           // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Warning));        // higher level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Debug));         // lower level should NOT be logged
}

void test_uLog_circular_buffer() {
    uLog aLog;
    TEST_ASSERT_EQUAL_UINT32(0, aLog.level);        // empty after creation
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);

    uint32_t newIndex;

    for (int i = 0; i < aLog.length; i++) {            // fill the buffer
        TEST_ASSERT_EQUAL_UINT32(0, aLog.head);        // before pushing
        TEST_ASSERT_EQUAL_UINT32(i, aLog.level);
        newIndex = aLog.pushItem();
        TEST_ASSERT_EQUAL_UINT32(i, newIndex);        // after pushing
        TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((i + 1), aLog.level);
    }
    newIndex = aLog.pushItem();
    TEST_ASSERT_EQUAL_UINT32(0, newIndex);        // test overflow
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
    TEST_ASSERT_EQUAL_UINT32(aLog.length, aLog.level);

    for (int i = 0; i < aLog.length; i++) {        // empty the buffer
        TEST_ASSERT_EQUAL_UINT32(i, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((aLog.length - i), aLog.level);
        aLog.popItem();
        TEST_ASSERT_EQUAL_UINT32((i + 1) % aLog.length, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((aLog.length - (i + 1)), aLog.level);
    }
    aLog.popItem();        // test underflow
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
    TEST_ASSERT_EQUAL_UINT32(0, aLog.level);
}

void test_uLog_output() {
    uLog aLog;
    aLog.setOutput(0, outputFunction1);
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Info);
    //    aLog.output(subSystem::general, loggingLevel::Info, "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
    aLog.output(subSystem::general, loggingLevel::Info, "0123456789ABCDEF                                                                                                0123456789ABCDEF");
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_logOutput_initialization);
    RUN_TEST(test_logOutput_settings);
    RUN_TEST(test_logOutput_write);
    RUN_TEST(test_uLog_initialization);
    RUN_TEST(test_uLog_filtering);
    RUN_TEST(test_uLog_circular_buffer);
    RUN_TEST(test_uLog_output);
    UNITY_END();
}