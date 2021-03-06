// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_NOTIFICATION_RESOURCES_H_
#define CONTENT_PUBLIC_COMMON_NOTIFICATION_RESOURCES_H_

#include "content/common/content_export.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace content {

// Structure to hold the resources associated with a Web Notification.
// TODO(mvanouwerkerk): Add resources for e.g. action icons - crbug.com/581336.
struct CONTENT_EXPORT NotificationResources {
  // Main icon for the notification. The bitmap may be empty if the developer
  // did not provide an icon, or fetching of the icon failed.
  SkBitmap notification_icon;
};

}  // namespace content

#endif  // CONTENT_PUBLIC_COMMON_NOTIFICATION_RESOURCES_H_
