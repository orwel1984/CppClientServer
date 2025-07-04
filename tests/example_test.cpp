#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Example test class
class ExampleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};

// Simple test
TEST(BasicTest, BasicAssertion) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

// Test with test fixture
TEST_F(ExampleTest, FixtureTest) {
    // This test uses the fixture setup/teardown
    EXPECT_GT(10, 5);
}

// Example of testing a function (you can adapt this for your actual code)
int add(int a, int b) {
    return a + b;
}

TEST(MathTest, Addition) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(0, 0), 0);
}

// Example of using Google Mock (if you need mocking)
class MockCalculator {
public:
    MOCK_METHOD(int, add, (int a, int b), ());
    MOCK_METHOD(int, multiply, (int a, int b), ());
};

TEST(MockTest, MockExample) {
    MockCalculator calc;
    
    // Set up expectations
    EXPECT_CALL(calc, add(2, 3))
        .WillOnce(testing::Return(5));
    
    // Use the mock
    EXPECT_EQ(calc.add(2, 3), 5);
} 