#include <gtest/gtest.h>
#include <logging/logging.h>


LogLocalContext log_ctx_("Google test");

TEST(Logging, Basic) {
    Log().debug() << "Hello debug sailor!";
    Log().info() << "Hello info sailor!";
    Log().warning() << "Hello warning sailor!";
    Log().error() << "Hello error sailor!";
    EXPECT_TRUE(true);
}

TEST(Logging, LocalContext) {
    Log(log_ctx_).info() << "Hello contexed sailor!";
    EXPECT_TRUE(true);
}

TEST(Logging, DisabledLog) {
    LogGlobalContext::instance().set_level(Log::Level::Disabled);
    Log().error() << "Hello sailor!";
    EXPECT_TRUE(true);
    LogGlobalContext::instance().set_level(Log::Level::Debug);
}

TEST(Logging, Macros) {
    LOG_DEBUG << "Debug macro output";
    LOG_INFO << "Info macro output";
    LOG_WARNING << "Warning macro output";
    LOG_ERROR << "Error macro output";
    EXPECT_TRUE(true);
}
