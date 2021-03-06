// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SVGIntegerInterpolationType_h
#define SVGIntegerInterpolationType_h

#include "core/animation/SVGInterpolationType.h"

namespace blink {

class SVGIntegerInterpolationType : public SVGInterpolationType {
public:
    SVGIntegerInterpolationType(const QualifiedName& attribute)
        : SVGInterpolationType(attribute)
    { }

private:
    PassOwnPtr<InterpolationValue> maybeConvertNeutral(const UnderlyingValue&, ConversionCheckers&) const final;
    PassOwnPtr<InterpolationValue> maybeConvertSVGValue(const SVGPropertyBase& svgValue) const final;
    PassRefPtrWillBeRawPtr<SVGPropertyBase> appliedSVGValue(const InterpolableValue&, const NonInterpolableValue*) const final;
};

} // namespace blink

#endif // SVGIntegerInterpolationType_h
