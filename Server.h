#pragma once

#include <string>
#include <memory>
#include <future>

struct CImpl;

class RtspServer
{
public:
  static constexpr const char* default_audio = "audiotestsrc wave=ticks";
  static constexpr const char* default_video = "videotestsrc";

  /** Prepare a RTSP server
   *
   * The server will publish a stream at rtsp://127.0.0.1:{port}/{path}
   *
   * It can be read using a RTSP client, e.g., with gstreamer:
   * rtspsrc location=rtsp://127.0.0.1:{port}/{path} ! decodebin ! xvimagesink
   *
   * @param path The path on which to make a stream available.
   * @param port The port on which the server will listen (dynamically-allocated if 0).
   * @param audio A gstreamer pipeline string producing raw audio.
   * @param video A gstreamer pipeline string producing raw video.
   */
  explicit RtspServer(
    const std::string& path,
    uint16_t port = 0,
    const std::string& audio = default_audio,
    const std::string& video = default_video);
  RtspServer(const RtspServer&) = delete;
  RtspServer& operator=(const RtspServer&) = delete;
  RtspServer(RtspServer&&) = default;
  RtspServer& operator=(RtspServer&&) = default;
  ~RtspServer();

  /** @returns the port on which the server listens */
  uint16_t port() const;

private:
  std::shared_ptr<CImpl> m_impl;
  std::shared_future<void> m_loop_thread;
};
