#pragma once

#include <render/context.h>

namespace element {
    class qt_opengl_rendering_context : public rendering_context {
        public:
            ~qt_opengl_rendering_context() final override;

            void swap_buffers() final override;
            void make_current() final override;
    };
} // namespace element
