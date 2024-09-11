#include "airplay.hpp"
#include "plugin.h"
#include <log/log.hpp>

extern "C" {
#include <obs/obs-module.h>
#include <obs/obs-properties.h>

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("AirPlay", "en-US")

static auto sourceName(void *v) -> const char *
{
  return static_cast<AirPlay *>(v)->name();
}

static auto sourceCreate(obs_data *data, obs_source *obsSource) -> void *
{
  return new AirPlay(data, obsSource);
}

static auto sourceDestroy(void *v) -> void
{
  delete static_cast<AirPlay *>(v);
}

static auto sourceUpdate(void *v, obs_data_t *data) -> void
{
  LOG(__func__, v, data);
}

static auto sourceWidth(void *v) -> uint32_t
{
  return static_cast<AirPlay *>(v)->getWidth();
}

static auto sourceHeight(void *v) -> uint32_t
{
  return static_cast<AirPlay *>(v)->getHeight();
}

static auto sourceProperties(void *v) -> obs_properties_t *
{
  UNUSED_PARAMETER(v);

  obs_properties_t *properties = obs_properties_create();

  obs_properties_add_bool(
    properties,
    USE_SYSTEM_MAC_ADDRESS_PROPERTY,
    "Use the system MAC address"
  );

  obs_properties_add_text(
    properties,
    SERVER_NAME_PROPERTY,
    "Server name",
    OBS_TEXT_DEFAULT
  );

  return properties;
}

static struct obs_source_info source = {.id = "AirPlay",
                                        .type = OBS_SOURCE_TYPE_INPUT,
                                        .output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO,
                                        .get_name = sourceName,
                                        .create = sourceCreate,
                                        .destroy = sourceDestroy,
                                        .get_width = sourceWidth,
                                        .get_height = sourceHeight,
                                        .get_properties = sourceProperties,
                                        .update = sourceUpdate,
                                        .icon_type = OBS_ICON_TYPE_DESKTOP_CAPTURE};

bool obs_module_load(void)
{
  obs_register_source(&source);
  return true;
}
}
