#pragma once

#include <string>

struct AudioOptions
{
  std::string pipe;
  std::string post;
};

struct VideoOptions
{
  std::string pipe;
  std::string post;
  int width{640};
  int height{480};
};
