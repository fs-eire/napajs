// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#define NAPA_LOG_DISABLED

#include <napa/capi.h>

#include <providers/providers.h>
#include <settings/settings-parser.h>
#include <utils/debug.h>
#include <v8/v8-common.h>
#include <zone/napa-zone.h>
#include <zone/node-zone.h>
#include <zone/worker-context.h>

#include <napa/log.h>

#include <atomic>
#include <fstream>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>


using namespace napa;

static std::atomic<bool> _initialized(false);
static settings::PlatformSettings _platformSettings;

/// <summary> a simple wrapper around Zone for managing lifetime using shared_ptr. </summary>
struct napa_zone {
    std::string id;
    std::shared_ptr<zone::Zone> zone;
};

napa_zone_handle napa_zone_create(napa_string_ref id) {
    NAPA_ASSERT(_initialized, "Napa wasn't initialized");

    // The actual zone is created upon initialization.
    return new napa_zone { NAPA_STRING_REF_TO_STD_STRING(id), nullptr };
}

napa_result_code napa_zone_init(napa_zone_handle handle, napa_string_ref settings) {
    NAPA_ASSERT(_initialized, "Napa platform wasn't initialized");
    NAPA_ASSERT(handle, "Zone handle is null");

    settings::ZoneSettings zoneSettings;
    if (!napa::settings::ParseFromString(NAPA_STRING_REF_TO_STD_STRING(settings), zoneSettings)) {
        NAPA_DEBUG("Api", "Failed to parse zone settings: %s", settings.data);
        return NAPA_RESULT_SETTINGS_PARSER_ERROR;
    }

    // zoneSettings.id = handle->id;

    // // Create the actual zone.
    // handle->zone = zone::NapaZone::Create(zoneSettings);
    // if (handle->zone == nullptr) {
    //     NAPA_DEBUG("Api", "Failed to create Napa zone '%s' with settings: %s", handle->id.c_str(), settings.data);
    //     return NAPA_RESULT_ZONE_INIT_ERROR;
    // }

    // NAPA_DEBUG("Api", "Napa zone '%s' created with settings: %s", handle->id.c_str(), settings.data);

    return NAPA_RESULT_SUCCESS;
}

napa_result_code napa_zone_release(napa_zone_handle handle) {
    NAPA_ASSERT(_initialized, "Napa platform wasn't initialized");
    NAPA_ASSERT(handle, "Zone handle is null");

    handle->zone = nullptr;
    delete handle;

    return NAPA_RESULT_SUCCESS;
}



#define NAPA_RESULT_CODE_DEF(symbol, string_rep) string_rep

static const char* NAPA_REPONSE_CODE_STRINGS[] = {
#include "napa/result-codes.inc"
};

#undef NAPA_RESULT_CODE_DEF

template<class T, size_t N>
constexpr size_t size(T(&)[N]) { return N; }

const char* napa_result_code_to_string(napa_result_code code) {
    NAPA_ASSERT(code >= 0, "result code out of range (%d)", code);
    NAPA_ASSERT(static_cast<size_t>(code) < size(NAPA_REPONSE_CODE_STRINGS), "result code out of range (%d)", code);

    return NAPA_REPONSE_CODE_STRINGS[code];
}

napa_result_code napa_initialize(napa_string_ref settings) {
    NAPA_ASSERT(!_initialized, "Napa platform was already initialized");

    if (!napa::settings::ParseFromString(NAPA_STRING_REF_TO_STD_STRING(settings), _platformSettings)) {
        return NAPA_RESULT_SETTINGS_PARSER_ERROR;
    }

    //return napa_initialize_common();
    _initialized = true;
    return NAPA_RESULT_SUCCESS;
}

napa_result_code napa_initialize_from_console(int argc, const char* argv[]) {
    NAPA_ASSERT(!_initialized, "Napa platform was already initialized");

    if (!napa::settings::ParseFromConsole(argc, argv, _platformSettings)) {
        return NAPA_RESULT_SETTINGS_PARSER_ERROR;
    }

    //return napa_initialize_common();
    _initialized = true;
    return NAPA_RESULT_SUCCESS;
}
