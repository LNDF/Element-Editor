#pragma once

#include <utils/uuid.h>
#include <utils/packed_set.h>
#include <string>

namespace element {
    namespace asset_importer {
        
        void load_dependencies();
        void save_dependencies();
        void create_dependents_data(const uuid& id, const std::string& path);
        void delete_dependency_data(const uuid& id);
        const packed_set<uuid>& get_id_dependencies(const uuid& id);
        const packed_set<std::string>& get_path_dependencies(const uuid& id);
        const packed_set<uuid>& get_dependents(const uuid& id);
        void set_id_dependencies(const uuid& id, const packed_set<uuid>& deps);
        void set_id_dependencies(const uuid& id, packed_set<uuid>&& deps);
        void set_path_dependencies(const uuid& id, const packed_set<std::string>& deps);
        void set_path_dependencies(const uuid& id, packed_set<std::string>&& deps);

    } // namespace asset_importer
} // namespace element
