// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_FEATURES_H_
#define URL_URL_FEATURES_H_

#include "polyfills/base/component_export.h"
#include "polyfills/base/feature_list.h"

namespace url {

COMPONENT_EXPORT(URL) BASE_DECLARE_FEATURE(kUseIDNA2008NonTransitional);

// Returns true if Chrome is using IDNA 2008 in Non-Transitional mode.
COMPONENT_EXPORT(URL) bool IsUsingIDNA2008NonTransitional();
}  // namespace url

#endif  // URL_URL_FEATURES_H_
