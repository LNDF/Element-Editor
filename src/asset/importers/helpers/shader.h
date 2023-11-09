#pragma once

#include <render/shader_compiler.h>
#include <utils/uuid.h>

namespace element {
    namespace importers {
        namespace helpers {
            void shader_compile(const uuid& id, render::shader_stage type);
        } // namespace helpers
    } // namespace importers
} // namespace element
