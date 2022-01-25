#include <unity.h>

void test_initialization() {
   TEST_ASSERT_TRUE(false); 
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialization);
    UNITY_END();
}