#pragma once

#include <QProcess>

namespace element {
    namespace editor {
        
        void launch_preview(QProcess* process, bool current_scene);
        void cancel_preview_deferred_launch();
        void setup_preview_launcher(char* argv0);

    } // namespace editor    
} // namespace element