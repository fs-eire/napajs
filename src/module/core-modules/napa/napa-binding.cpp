// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#define NAPA_LOG_DISABLED
#include "napa-binding.h"

#include <zone/worker-context.h>

#include <napa/zone.h>

using namespace napa;
using namespace napa::module;

static void CreateZone(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    CHECK_ARG(isolate, args[0]->IsString(), "first argument to createZone must be a string");
    v8::String::Utf8Value zoneId(args[0]->ToString());

    std::stringstream ss;
    if ((args.Length() > 1) && (!args[1]->IsUndefined())) {
        CHECK_ARG(isolate, args[1]->IsObject(), "second argument to createZone must be an object");
        auto settingsObj = args[1]->ToObject(context).ToLocalChecked();

        auto settingsMap = napa::v8_helpers::V8ObjectToMap<std::string>(isolate, settingsObj);

        for (const auto& kv : settingsMap) {
            ss << " --" << kv.first << " " << kv.second;
        }
    }

    try {
        auto zoneProxy = std::make_unique<napa::Zone>(*zoneId, ss.str());
        args.GetReturnValue().Set(0);
    } catch (const std::exception& ex) {
        JS_FAIL(isolate, ex.what());
    }
}

void binding::Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
    NAPA_SET_METHOD(exports, "createZone", CreateZone);
}
