#pragma once

#include <utils/uuid.h>
#include <utils/packed_set.h>

namespace element {
    namespace asset_importer {
        
        void load_dependencies();
        void save_dependencies();
        void delete_dependency_data(const uuid& id);
        const packed_set<uuid>& get_dependencies(const uuid& id);
        const packed_set<uuid>& get_dependents(const uuid& id);
        void set_dependencies(const uuid& id, const packed_set<uuid>& deps);
        void set_dependencies(const uuid& id, packed_set<uuid>&& deps);

    } // namespace asset_importer
} // namespace element
