# Copyright 2019 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
load("//build_config:build_config.bzl", "build_config")

cc_library(
    name = "base",
    hdrs = [
        "compiler_specific.h",
        "debug/leak_annotations.h",
        "macros.h",
        "no_destructor.h",
        "optional.h",
        "stl_util.h",
        "template_util.h",
    ],
    copts = build_config.default_copts,
    visibility = ["//visibility:public"],
    deps = [
        "//build:build_config",
        "//polyfills",
    ],
)
