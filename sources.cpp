#include "sources.hpp"

#include <vector>
#include <boost/algorithm/string/join.hpp>

template <>
AudioSource::Presets AudioSource::presets = {{"dummy", {"audiotestsrc wave=ticks", ""}},
                                             {"pulse", {"pulsesrc", ""}}};
template <>
VideoSource::Presets VideoSource::presets = {{"dummy", {"videotestsrc", "", 640, 480}},
                                             {"v4l", {"v4l2src", "", 640, 480}}};

template <class T>
void printPresets(std::ostream& out, const std::string& name)
{
  std::vector<std::string> keys;
  for (const auto kv : T::presets) {
    keys.push_back(kv.first);
  }
  out << name << " presets: " << boost::join(keys, ", ") << std::endl;
}

template void printPresets<AudioSource>(std::ostream&, const std::string&);
template void printPresets<VideoSource>(std::ostream&, const std::string&);
