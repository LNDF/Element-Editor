#pragma once

#include <QProcess>

namespace element {
    namespace editor {
        
        void launch_preview(const QProcess& process, bool current_scene);
        void setup_preview_launcher(char* argv0);

    } // namespace editor    
} // namespace element