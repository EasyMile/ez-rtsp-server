#include "Server.h"

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/gst.h>

#include <chrono>
#include <thread>
#include <sstream>

struct CImpl
{
  GMainLoop* m_loop;       //< Managed by g_main_loop_new/g_main_loop_unref
  GstRTSPServer* m_server; //< Managed by gst_rtsp_server_new/g_object_unref
};

RtspServer::RtspServer(
  const std::string& path, uint16_t port, const std::string& audio, const std::string& video)
  : m_impl{new CImpl}
{
  gst_init(nullptr, nullptr);
  m_impl->m_loop = g_main_loop_new(nullptr, false);
  m_impl->m_server = gst_rtsp_server_new();

  gst_rtsp_server_set_service(m_impl->m_server, std::to_string(port).c_str());

  {
    auto* mounts = gst_rtsp_server_get_mount_points(m_impl->m_server);
    auto* factory = gst_rtsp_media_factory_new();

    std::ostringstream pipeline;
    pipeline << "( ";
    pipeline << video;
    pipeline << " ! videoscale ! video/x-raw,width=640";
    pipeline << " ! vp8enc deadline=1 ! rtpvp8pay name=pay0 pt=96";
    pipeline << " ";
    pipeline << audio;
    pipeline << " ! opusenc ! rtpopuspay name=pay1 pt=97";
    pipeline << " )";
    gst_rtsp_media_factory_set_launch(factory, pipeline.str().c_str());

    // Allow multiple clients
    gst_rtsp_media_factory_set_shared(factory, true);

    // Ownership of factory is transfered
    gst_rtsp_mount_points_add_factory(mounts, path.c_str(), factory);

    g_object_unref(mounts);
  }

  m_loop_thread = std::async(std::launch::async, [this]() { g_main_loop_run(m_impl->m_loop); });

  while (not g_main_loop_is_running(m_impl->m_loop)) {
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }

  // Attach the server to the main loop, which spawns it
  gst_rtsp_server_attach(m_impl->m_server, nullptr);
}

uint16_t RtspServer::port() const
{
  return gst_rtsp_server_get_bound_port(m_impl->m_server);
}

RtspServer::~RtspServer()
{
  g_main_loop_quit(m_impl->m_loop);

  g_object_unref(m_impl->m_server);
  g_main_loop_unref(m_impl->m_loop);
  gst_deinit();
}
