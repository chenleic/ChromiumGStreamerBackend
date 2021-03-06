// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "sync/internal_api/public/attachments/attachment_util.h"

#include <stdint.h>

#include "base/memory/ref_counted_memory.h"
#include "third_party/leveldatabase/src/util/crc32c.h"

namespace syncer {

uint32_t ComputeCrc32c(const scoped_refptr<base::RefCountedMemory>& data) {
  return leveldb::crc32c::Value(data->front_as<char>(), data->size());
}

}  // namespace syncer
