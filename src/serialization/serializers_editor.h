#pragma once

#include <serialization/serializers.h>

#include <cereal/archives/json.hpp>

namespace element {
    typedef cereal::JSONInputArchive text_deserializer;
    typedef cereal::JSONOutputArchive text_serializer;

    inline text_deserializer create_text_deserializer(std::istream& is) {
        return cereal::JSONInputArchive(is);
    }

    inline text_serializer create_text_serializer(std::ostream& os) {
        return cereal::JSONOutputArchive(os);
    }
} // namespace element
