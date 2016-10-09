#include "DebugVK.h"
#include "utils/Logger.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

bool DebugVK::s_enableValidationLayers = VULKAN_USE_VALIDATION_LAYERS;
bool DebugVK::s_enableDebugMarkers = VULKAN_USE_DEBUG_MARKERS;

std::vector<const c8*> DebugVK::s_validationLayerNames =
{
    // This is a meta layer that enables all of the standard
    // validation layers in the correct order :
    // threading, parameter_validation, device_limits, object_tracker, image, core_validation, swapchain, and unique_objects
    "VK_LAYER_LUNARG_standard_validation"
};

PFN_vkCreateDebugReportCallbackEXT  DebugVK::s_vkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT DebugVK::s_vkDestroyDebugReportCallbackEXT = VK_NULL_HANDLE;

VkDebugReportCallbackEXT            DebugVK::s_msgCallback;


std::string DebugVK::errorString(VkResult errorCode)
{
    switch (errorCode)
    {
#define STR(r) case VK_ ##r: return #r
        STR(NOT_READY);
        STR(TIMEOUT);
        STR(EVENT_SET);
        STR(EVENT_RESET);
        STR(INCOMPLETE);
        STR(ERROR_OUT_OF_HOST_MEMORY);
        STR(ERROR_OUT_OF_DEVICE_MEMORY);
        STR(ERROR_INITIALIZATION_FAILED);
        STR(ERROR_DEVICE_LOST);
        STR(ERROR_MEMORY_MAP_FAILED);
        STR(ERROR_LAYER_NOT_PRESENT);
        STR(ERROR_EXTENSION_NOT_PRESENT);
        STR(ERROR_FEATURE_NOT_PRESENT);
        STR(ERROR_INCOMPATIBLE_DRIVER);
        STR(ERROR_TOO_MANY_OBJECTS);
        STR(ERROR_FORMAT_NOT_SUPPORTED);
        STR(ERROR_SURFACE_LOST_KHR);
        STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        STR(SUBOPTIMAL_KHR);
        STR(ERROR_OUT_OF_DATE_KHR);
        STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        STR(ERROR_VALIDATION_FAILED_EXT);
        STR(ERROR_INVALID_SHADER_NV);
#undef STR
    default:
        return "UNKNOWN_ERROR";
    }
}

VkBool32 DebugVK::messageCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, u64 srcObject, size_t location, s32 msgCode, c8* layerPrefix, c8* msg, void* userData)
{
    // Error that may result in undefined behaviour
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("[s%]Object: %d Code %d: %s", layerPrefix, srcObject, msgCode, msg);
    };
    // Warnings may hint at unexpected / non-spec API usage
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARNING("[s%]Object: %d Code %d: %s", layerPrefix, srcObject, msgCode, msg);
    };
    // May indicate sub-optimal usage of the API
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_WARNING("[s%]Object: %d Code %d: %s", layerPrefix, srcObject, msgCode, msg);
    };
    // Informal messages that may become handy during debugging
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
        LOG_INFO("[s%]Object: %d Code %d: %s", layerPrefix, srcObject, msgCode, msg);
    }
    // Diagnostic info from the Vulkan loader and layers
    // Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
        LOG_DEBUG("[s%]Object: %d Code %d: %s", layerPrefix, srcObject, msgCode, msg);
    }

    // The return value of this callback controls wether the Vulkan call that caused
    // the validation message will be aborted or not
    // We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message 
    // (and return a VkResult) to abort
    // If you instead want to have calls abort, pass in VK_TRUE and the function will 
    // return VK_ERROR_VALIDATION_FAILED_EXT 
    if (DebugVK::s_enableValidationLayers)
    {
        return VK_TRUE;
    }
    else
    {
        return VK_FALSE;
    }
}

void DebugVK::setupDebugging(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callBack, void* userData)
{
    if (s_vkCreateDebugReportCallbackEXT == VK_NULL_HANDLE)
    {
        s_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
        ASSERT(s_vkCreateDebugReportCallbackEXT, "Can't get address");
    }

    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgCreateInfo.pNext = nullptr;
    dbgCreateInfo.flags = flags;
    dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)DebugVK::messageCallback;
    dbgCreateInfo.pUserData = userData;

    VkDebugReportCallbackEXT& callback = (callBack != VK_NULL_HANDLE) ? callBack : DebugVK::s_msgCallback;

    VkResult result = s_vkCreateDebugReportCallbackEXT(instance, &dbgCreateInfo, nullptr, &callback);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("DebugVK::setupDebugging: vkCreateDebugReportCallbackEXT error %s", DebugVK::errorString(result).c_str());
    }
}

void DebugVK::freeDebugCallback(VkInstance instance)
{
    if (s_vkDestroyDebugReportCallbackEXT == VK_NULL_HANDLE)
    {
        s_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
        ASSERT(s_vkCreateDebugReportCallbackEXT, "Can't get address");
    }

    if (DebugVK::s_msgCallback != VK_NULL_HANDLE)
    {
        s_vkDestroyDebugReportCallbackEXT(instance, DebugVK::s_msgCallback, nullptr);
    }
}

} //namespace vk
} //namespace renderer
} //namespace v3d