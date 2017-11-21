#pragma once

#include <string>

struct AudioOptions
{
  std::string audio;
  std::string post_audio;
};

struct VideoOptions
{
  std::string video;
  std::string post_video;
  int width;
  int height;
};
