// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include <string>
#include <unordered_map>

namespace napa {
namespace module {
namespace binding {

    /// <summary> Initialize and export napa related functions and object wraps. </summary>
    void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module);
}
}
}

namespace napa {
namespace v8_helpers {

    inline std::unordered_map<std::string, std::string> V8ObjectToMap(
        v8::Isolate* isolate,
        const v8::Local<v8::Object>& obj) {

        auto context = isolate->GetCurrentContext();
        std::unordered_map<std::string, std::string> res;

        auto maybeProps = obj->GetOwnPropertyNames(context);
        if (!maybeProps.IsEmpty()) {
            auto props = maybeProps.ToLocalChecked();
            res.reserve(props->Length());

            for (uint32_t i = 0; i < props->Length(); i++) {
                auto key = props->Get(context, i).ToLocalChecked();
                auto value = obj->Get(context, key).ToLocalChecked();

                v8::String::Utf8Value keyString(key->ToString(context).ToLocalChecked());
                v8::String::Utf8Value utf8Value(value);
                res.emplace(*keyString, std::string(*utf8Value));
            }
        }
        return res;
    }

}}