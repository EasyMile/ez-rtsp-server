#pragma once

#include <string>
#include <map>

struct Source
{
  Source(const std::string& value) : value(value) {}
  std::string value;
  using Presets = const std::map<std::string, std::string>;
};

struct AudioSource : Source
{
  using Source::Source;
  static Presets presets;
};

struct VideoSource : Source
{
  using Source::Source;
  static Presets presets;
};

template <class T>
void printPresets(std::ostream& out, const std::string& name);
