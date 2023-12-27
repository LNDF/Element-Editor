#pragma once

#include <scenegraph/node_storage.h>
#include <scenegraph/nodes/camera_node.h>
#include <scenegraph/nodes/camera_node_ref.h>

namespace element {
    namespace scenegraph {
        
        class editor_camera_node_storage : public node_storage_base {
            private:
                camera_node editor_cam;
                camera_node_ref editor_cam_ref;
            public:
                editor_camera_node_storage();
                ~editor_camera_node_storage() final override;

                void delete_node(const uuid& id) final override;
                void emplace_node(const uuid& id, const std::string& name, node_ref& parent) final override;
                void emplace_node(const uuid& id, std::string&& name, node_ref& parent) final override;
                void emplace_root(const uuid& id) final override;
                scenegraph::node* get_node_ptr(const uuid& id) final override;
                std::type_index get_type_index() const final override;
                void init_scene(scenegraph::scene* owner_scene) final override;

                camera_node_ref& get_editor_camera();
        };

        camera_node_ref& get_editor_camera();

    } // namespace scenegraph 
} // namespace element 