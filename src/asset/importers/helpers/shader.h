#pragma once

#include <shader/compiler.h>
#include <utils/uuid.h>

namespace element {
    namespace importers {
        namespace helpers {
            void shader_compile(const uuid& id, shader::shader_stage type);
        } // namespace helpers
    } // namespace importers
} // namespace element
