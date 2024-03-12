#pragma once

#include <asset/creators/asset_creators_info.h>
#include <QMenu>
#include <QAction>
#include <vector>
#include <string>

namespace element {
    namespace ui {
        
        class menu_new_asset_action : public QAction {
            Q_OBJECT
            private:
                std::string type;
            public:
                menu_new_asset_action(const std::string& type, QObject* parent = nullptr);

                void retranslate();
            private slots:
                void create_asset();
            signals:
                void trigger(const std::string& type);
        };

        class menu_new_asset : public QMenu {
            Q_OBJECT
            private:
                QAction* new_folder_action;
                QAction* new_file_action;
                std::string parent_path;
                std::vector<menu_new_asset_action*> actions;

                std::string get_file_path(bool* ok);
                bool check_exists(const std::string& path);
            public:
                menu_new_asset(QWidget* parent = nullptr);
                menu_new_asset(const std::string& parent_path, QWidget* parent = nullptr);

                void retranslate();
            private slots:
                void action_triggered(const std::string& type);
                void action_new_folder();
                void action_new_file();
        };

    } // namespace ui
} // namespace element