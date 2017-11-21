#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Server.h>

TEST_CASE("test harness")
{
  RtspServer camera("/");
  CHECK(camera.port() != 0);
}
