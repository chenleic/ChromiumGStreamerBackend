// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_LAYOUT_STORE_H_
#define ASH_DISPLAY_DISPLAY_LAYOUT_STORE_H_

#include <stdint.h>

#include <map>

#include "ash/ash_export.h"
#include "ash/display/display_layout.h"
#include "base/macros.h"

namespace ash {

class ASH_EXPORT DisplayLayoutStore {
 public:
  DisplayLayoutStore();
  ~DisplayLayoutStore();

  const DisplayLayout& default_display_layout() const {
    return default_display_layout_;
  }
  void SetDefaultDisplayLayout(const DisplayLayout& layout);

  // Registeres the display layout info for the specified display(s).
  void RegisterLayoutForDisplayIdPair(int64_t id1,
                                      int64_t id2,
                                      const DisplayLayout& layout);

  // If no layout is registered, it creatas new layout using
  // |default_display_layout_|.
  DisplayLayout GetRegisteredDisplayLayout(const DisplayIdPair& pair);

  // Returns the display layout for the display id pair
  // with display swapping applied.  That is, this returns
  // flipped layout if the displays are swapped.
  DisplayLayout ComputeDisplayLayoutForDisplayIdPair(
      const DisplayIdPair& display_pair);

  // Update the multi display state in the display layout for
  // |display_pair|.  This creates new display layout if no layout is
  // registered for |display_pair|.
  void UpdateMultiDisplayState(const DisplayIdPair& display_pair,
                               bool mirrored,
                               bool default_unified);

  // Update the |primary_id| in the display layout for
  // |display_pair|.  This creates new display layout if no layout is
  // registered for |display_pair|.
  void UpdatePrimaryDisplayId(const DisplayIdPair& display_pair,
                              int64_t display_id);

 private:
  // Creates new layout for display pair from |default_display_layout_|.
  DisplayLayout CreateDisplayLayout(const DisplayIdPair& display_pair);

  // The default display layout.
  DisplayLayout default_display_layout_;

  // Display layout per pair of devices.
  std::map<DisplayIdPair, DisplayLayout> paired_layouts_;

  DISALLOW_COPY_AND_ASSIGN(DisplayLayoutStore);
};

}  // namespace ash

#endif  // ASH_DISPLAY_DISPLAY_LAYOUT_STORE_H_
