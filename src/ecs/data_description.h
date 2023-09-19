#pragma once

#include <string>
#include <map>

namespace m2d {

namespace ecs {

class DescriptionBase {
};

class IntDescription : public DescriptionBase {};

class FloatDescription : public DescriptionBase {};

class FixedStringDescription : public DescriptionBase {};

class DixedArrayDescription : public DescriptionBase {};

class StructDescription : public DescriptionBase {

};

}  // namespace ecs

}  // namespace m2d
