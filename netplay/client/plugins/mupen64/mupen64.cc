#include "mupen64.h"

#include <sstream>

#include "base/netplayServiceProto.grpc.pb.h"
#include "client/button-coder-interface.h"
#include "client/client.h"
#include "client/plugins/mupen64/coder.h"
#include "client/plugins/mupen64/config-handler.h"
#include "client/plugins/mupen64/plugin-impl.h"
#include "client/plugins/mupen64/osal_dynamiclib.h"

#include "glog/logging.h"
#include "grpc++/channel.h"
#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"

#include "m64p_config.h"

// -----------------------------------------------------------------------------
// Global state

static std::unique_ptr<PluginImpl> l_PluginImpl;

// -----------------------------------------------------------------------------
// PluginStartup and helpers

static std::unique_ptr<ConfigHandler> l_ConfigHandler;

EXPORT m64p_error CALL
PluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context,
              void (*DebugCallback)(void *, int, const char *)) {
  VLOG(2) << "Calling PluginStartup";

  if (l_ConfigHandler != nullptr) {
    return M64ERR_ALREADY_INIT;
  }

  l_ConfigHandler = ConfigHandler::MakeConfigHandler(CoreLibHandle, "Netplay");

  return M64ERR_SUCCESS;
}

// -----------------------------------------------------------------------------
// PluginGetVersion

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *plugin_type,
                                        int *plugin_version, int *api_version,
                                        const char **plugin_name, int *caps) {
  VLOG(2) << "Calling PluginGetVersion";

  if (plugin_type != NULL) *plugin_type = M64PLUGIN_NETPLAY;

  if (plugin_version != NULL) *plugin_version = 1;

  if (api_version != NULL) *api_version = NETPLAY_API_VERSION;

  if (plugin_name != NULL) *plugin_name = PluginImpl::kPluginName;

  if (caps != NULL) *caps = 0;

  return M64ERR_SUCCESS;
}

// -----------------------------------------------------------------------------
// InitiateNetplay

EXPORT int CALL InitiateNetplay(NETPLAY_INFO *netplay_info) {
  VLOG(2) << "Calling InitiateNetplay";

  const M64Config& config = M64Config::FromConfigHandler(*l_ConfigHandler);

  if (!config.enabled) {
    return 1;
  }

  // Initialize the client to the server.
  std::stringstream server_addr;
  server_addr << config.server_hostname << ":" << config.server_port;

  LOG(INFO) << "Opening channel to server " << server_addr.str();

  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
      server_addr.str(), grpc::InsecureChannelCredentials());
  std::shared_ptr<NetPlayServerService::StubInterface> stub =
      NetPlayServerService::NewStub(channel);
  std::unique_ptr<PluginImpl::M64Client> client(new NetplayClient<BUTTONS>(
      stub, std::unique_ptr<Mupen64ButtonCoder>(new Mupen64ButtonCoder()),
      config.delay_frames, config.console_id));
  l_PluginImpl.reset(new PluginImpl(config, std::move(client)));

  return l_PluginImpl->InitiateNetplay(netplay_info);
}

// -----------------------------------------------------------------------------
// RomOpen

EXPORT int CALL RomOpen(void) {
  VLOG(2) << "Calling RomOpen";

  return 1;
}

// -----------------------------------------------------------------------------
// RomClosed

EXPORT void CALL RomClosed(void) { VLOG(2) << "Calling RomClosed"; }

// -----------------------------------------------------------------------------
// PutKeys

EXPORT
int CALL PutKeys(const m64p_netplay_frame_update *updates, int nupdates) {
  VLOG(3) << "Calling PutKeys";

  return l_PluginImpl->PutButtons(updates, nupdates);
}

// -----------------------------------------------------------------------------
// GetKeys

EXPORT
int CALL GetKeys(m64p_netplay_frame_update *update) {
  VLOG(3) << "Calling GetKeys";

  return l_PluginImpl->GetButtons(update);
}
