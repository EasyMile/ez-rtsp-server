#include <Server.h>
#include "sources.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <cstdint>
#include <thread>
#include <chrono>

namespace po = boost::program_options;

template <class T>
void validate(boost::any& v, const std::vector<std::string>& values, T*, int)
{
  static_assert(
    std::is_same<T, AudioSource>::value or std::is_same<T, VideoSource>::value, "Unsupported type");

  po::validators::check_first_occurrence(v);
  const std::string& s = po::validators::get_single_string(values);

  // custom, arbitrary pipeline
  if (s.find("launch:") == 0) {
    try {
      auto source = T{{}};
      source.value.pipe = s.substr(strlen("launch:"));
      v = boost::any(std::move(source));
    } catch (std::out_of_range&) {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    return;
  }

  // preset
  try {
    v = boost::any(T{T::presets.at(s)});
  } catch (std::out_of_range&) {
    throw po::validation_error(po::validation_error::invalid_option_value);
  }
}

void usage(const po::options_description& desc)
{
  std::cerr << desc << std::endl;
  exit(1);
}

po::variables_map parseArgs(int argc, char** argv)
{
  po::options_description desc(
    "Spawns an RTSP Server publishing a single media; prints the URL in the standard output");
  desc.add_options()("help", "Show this help.");
  desc.add_options()("presets", "List audio and video source presets.");

  desc.add_options()(
    "path", po::value<std::string>()->default_value("/example"), "Path to publish to.");
  desc.add_options()(
    "port",
    po::value<uint16_t>()->default_value(0),
    "Port to listen to. If 0, the port will be allocated dynamically.");
  desc.add_options()(
    "audio",
    po::value<AudioSource>()->default_value(AudioSource::presets.at("dummy"), "dummy"),
    "Audio source preset or pipeline prefixed with 'launch:'.");
  desc.add_options()(
    "video",
    po::value<VideoSource>()->default_value(VideoSource::presets.at("dummy"), "dummy"),
    "Video source preset or pipeline prefixed with 'launch:'.");
  desc.add_options()(
    "post-audio",
    po::value<std::string>()->default_value(""),
    "GStreamer pipeline fragment for audio post-processing.");
  desc.add_options()(
    "post-video",
    po::value<std::string>()->default_value(""),
    "GStreamer pipeline fragment for video post-processing.");
  desc.add_options()("width", po::value<int>()->default_value(640), "Video width.");
  desc.add_options()("height", po::value<int>()->default_value(480), "Video height.");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const boost::program_options::unknown_option& e) {
    std::cerr << e.what() << std::endl;
    usage(desc);
  }
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    exit(1);
  }

  if (vm.count("presets")) {
    printPresets<AudioSource>(std::cout, "Audio");
    printPresets<VideoSource>(std::cout, "Video");
    exit(1);
  }

  return vm;
}

int main(int argc, char** argv)
{
  auto vm = parseArgs(argc, argv);

  auto path = vm["path"].as<std::string>();
  auto port = vm["port"].as<uint16_t>();

  auto audio = vm["audio"].as<AudioSource>().value;
  auto video = vm["video"].as<VideoSource>().value;
  audio.post = vm["post-audio"].as<std::string>();
  video.post = vm["post-video"].as<std::string>();
  video.width = vm["width"].as<int>();
  video.height = vm["height"].as<int>();

  auto server = RtspServer{path, port, audio, video};
  std::cout << "rtsp://127.0.0.1:" << server.port() << path << std::endl;

  while (true) {
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::seconds{100});
  };

  return 0;
}
