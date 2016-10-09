#pragma once

#include "DeviceContext.h"

#if defined(_VULKAN_RENDER_) 
#   include <vulkan/vulkan.h>
#endif //_VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class DeviceContextVK final : public DeviceContext
    {
    public:

        explicit            DeviceContextVK(const platform::WindowPtr window);
        ~DeviceContextVK();

        void                checkForErrors(const std::string& location = "") override;
        bool                create() override;
        void                destroy() override;
        bool                present() override;
        void                flush();

        bool                setVSync(bool use) override;

    protected:

        TexturePtr          createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) override;
        TexturePtr          createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) override;

        void                fillGrapthicCaps() override;

        void                printExtensionList() const;

#if defined(_PLATFORM_WIN_)
        bool                createWinApiContext();
#endif //_PLATFORM_WIN_

#if defined(_VULKAN_RENDER_) 
        bool                createInstance(bool enableValidation);
        bool                createPhysicalDevice();
        bool                createLogicalDevice(bool useSwapChain, VkQueueFlags requestedQueueTypes);

        u32                 getQueueFamiliyIndex(VkQueueFlagBits queueFlags);

        VkBool32            checkGlobalExtensionPresent(const c8* extensionName);
        VkBool32            checkDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const c8* extensionName);

        VkInstance          m_instance;
        VkPhysicalDevice    m_physicalDevice;

        struct VulkanDevice
        {
            VkDevice                             _device;
            VkPhysicalDeviceProperties           _properties;
            VkPhysicalDeviceFeatures             _features;
            VkPhysicalDeviceMemoryProperties     _memoryProperties;
            std::vector<VkQueueFamilyProperties> _queueFamilyProperties;

            struct
            {
                u32 _graphics;
                u32 _compute;
                u32 _transfer;
            }                                    _queueFamilyIndices;
        };

        VulkanDevice        m_vulkanDevice;

        VkSurfaceKHR        m_surface;
#endif //_VULKAN_RENDER_
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d