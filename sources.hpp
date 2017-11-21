#pragma once

#include <Options.h>

#include <string>
#include <map>

template <class T>
struct Source
{
  Source(const T& value) : value(value) {}
  T value;
  using Presets = const std::map<std::string, T>;
  static Presets presets;
};

using AudioSource = Source<AudioOptions>;
using VideoSource = Source<VideoOptions>;

template <class T>
void printPresets(std::ostream& out, const std::string& name);
