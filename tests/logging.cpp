#include <gtest/gtest.h>
#include <logging/logging.h>


LogLocalContext log_ctx_("Google test");

TEST(Logging, Basic) {
    LogGlobalContext::instance().attach(std::cout);
    Log().debug() << "Hello debug sailor!";
    Log().info() << "Hello info sailor!";
    Log().warning() << "Hello warning sailor!";
    Log().error() << "Hello error sailor!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, LocalContext) {
    LogGlobalContext::instance().attach(std::cout);
    Log(log_ctx_).info() << "Hello contexed sailor!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, DisabledLog) {
    LogGlobalContext::instance()
        .attach(std::cout)
        .set_level(Log::Level::Disabled);
    Log().error() << "Hello sailor!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, Macros) {
    LogGlobalContext::instance().attach(std::cout);
    LOG_DEBUG << "Debug macro output";
    LOG_INFO << "Info macro output";
    LOG_WARNING << "Warning macro output";
    LOG_ERROR << "Error macro output";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, FileOutput) {
    LogGlobalContext::instance().attach("log.txt");
    LOG_INFO << "Wow! So doge!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}
