#include <gtest/gtest.h>
#include <logging/logging.h>


LOG_CONTEXT("Google test");

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
    LOG_INFO << "Hello sailor!";

    {
        LOG_CONTEXT("nested context");
        LOG_INFO << "Nested";
    }

    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, DisabledLog) {
    LogGlobalContext::instance()
        .attach(std::cout)
        .set_level(Log::Level::Disabled);
    LOG_ERROR << "Hello sailor!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, FileOutput) {
    LogGlobalContext::instance().attach("GTest.log");
    LOG_INFO << "Wow! So doge!";
    LogGlobalContext::instance().reset();
    EXPECT_TRUE(true);
}

TEST(Logging, HandlerLevel) {
    LogGlobalContext::instance()
        .attach(std::cout, Log::Level::Debug)
        .attach("GTest.log", Log::Level::Error);

    LOG_DEBUG << "Handler level test: debug log";
    LOG_INFO << "Handler level test: info log";
    LOG_WARNING << "Handler level test: warning log";
    LOG_ERROR << "Handler level test: error log";

    EXPECT_TRUE(true);
}
