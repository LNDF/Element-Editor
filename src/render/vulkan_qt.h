#pragma once

#include <render/vulkan.h>
#include <QVulkanInstance>

namespace element {
    
    class vulkan_qt {
        private:
            static QVulkanInstance* instance;
        public:
            inline static QVulkanInstance* get_vulkan_instance() {return instance;}
            inline static void set_vulkan_instance(QVulkanInstance* inst) {instance = inst;}
    };

} // namespace element
