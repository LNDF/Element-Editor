#include <core/fs.h>

#include <stdexcept>

namespace element {
    std::istream fs_get_resource(const uuid& id) {
        throw std::runtime_error("Unimplemented");
    }

    std::string fs_get_resource_name(const uuid& id) {
        throw std::runtime_error("Unimplemented");
    }

    uuid fs_get_uuid_from_resource_name(const std::string& name, const std::string& ns) {
        throw std::runtime_error("Unimplemented");
    }
} // namespace element
