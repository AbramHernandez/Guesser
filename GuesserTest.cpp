#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
protected:
    GuesserTest(){}
    virtual ~GuesserTest(){}
    virtual void SetUp(){}
    virtual void TearDown(){}
};

/* distance() tests */

TEST(GuesserTest, distance_identical)
{
    Guesser g("cutie");
    ASSERT_EQ(0, g.distance("cutie"));
}

TEST(GuesserTest, distance_one_difference)
{
    Guesser g("cutie");
    ASSERT_EQ(1, g.distance("cutle"));
}

TEST(GuesserTest, distance_all_different)
{
    Guesser g("cutie");
    ASSERT_EQ(5, g.distance("abcd"));
}

TEST(GuesserTest, distance_shorter_guess)
{
    Guesser g("cutie");
    ASSERT_EQ(2, g.distance("cut"));
}

TEST(GuesserTest, distance_longer_guess)
{
    Guesser g("cutie");
    ASSERT_EQ(2, g.distance("cutieoo"));
}

TEST(GuesserTest, distance_caps_at_secret_length)
{
    Guesser g("cutie");
    ASSERT_EQ(5, g.distance("longlonglonglonglonglonglonglonglongstring"));
}

/* constructor tests */

TEST(GuesserTest, constructor_truncates_long_secret)
{
    std::string long_str(40, 'a');
    Guesser g(long_str);

    ASSERT_EQ(32, g.distance(std::string(40, 'b')));
}

/* match() */

TEST(GuesserTest, match_correct_guess)
{
    Guesser g("winston");
    ASSERT_TRUE(g.match("winston"));
}

TEST(GuesserTest, match_incorrect_small_distance)
{
    Guesser g("winston");
    ASSERT_FALSE(g.match("winstox"));
}

TEST(GuesserTest, match_locks_on_large_distance)
{
    Guesser g("winston");

    ASSERT_FALSE(g.match("zzzzzz"));
    ASSERT_FALSE(g.match("winston"));
}

TEST(GuesserTest, match_three_wrong_guesses_locks)
{
    Guesser g("winston");

    g.match("winstox");
    g.match("winstoy");
    g.match("winstoz");

    ASSERT_FALSE(g.match("winston"));
}

TEST(GuesserTest, match_remaining_decreases)
{
    Guesser g("winston");

    ASSERT_FALSE(g.match("winstox"));
    ASSERT_FALSE(g.match("winstoy"));
    ASSERT_FALSE(g.match("winstoz"));

    ASSERT_FALSE(g.match("winston"));
}

/* regression */

TEST(GuesserTest, lock_does_not_reset_after_lock)
{
    Guesser g("winston");

    g.match("wwwwww"); // locks immediately

    ASSERT_FALSE(g.match("winston"));
}

TEST(GuesserTest, small_distance_does_not_lock)
{
    Guesser g("winston");

    ASSERT_FALSE(g.match("winstox")); // small diff
    ASSERT_TRUE(g.match("winston"));  // should still succeed
}