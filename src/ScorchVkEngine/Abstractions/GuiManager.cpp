#include "GuiManager.h"

#include <iterator>
#include <stdexcept>

GuiManager* GuiManager::instance = nullptr;

void GuiManager::setupImGui(VkInstance instance, GLFWwindow* window, VkQueue graphicsQueue, VkRenderPass renderPass)
{
    const VkDescriptorPoolSize pool_sizes[] =
    {
    { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    vkCreateDescriptorPool(presentMan->device, &pool_info, nullptr, &imguiPool);

    ImGuiContext* imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Navigation Flags
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Navigation Flags
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Use Docking Branch

    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance;
    initInfo.PhysicalDevice = presentMan->physicalDevice;
    initInfo.Device = presentMan->device;
    initInfo.Queue = graphicsQueue;
    initInfo.DescriptorPool = imguiPool;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;

    ImGui_ImplVulkan_Init(&initInfo, renderPass);

    io.Fonts->AddFontFromFileTTF("..\\res\\fonts\\SpaceMono-Regular.ttf", 20.0f);

    unsigned char* fontData;
    int texWidth, texHeight;
    io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

    if (vkCreateImage(presentMan->device, &imageInfo, nullptr, &fontImage) != VK_SUCCESS)
        throw std::runtime_error("Failed to create ImGui font image!");

    bufferMan->createImguiFontBuffer(fontImage, graphicsQueue);

    io.Fonts->TexID = reinterpret_cast<ImTextureID>(fontImage);

    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.11f, 0.13f, 0.59f);
    style.Colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.59f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.19f, 0.22f, 0.78f);
    style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.32f, 0.31f, 0.32f, 0.59f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 0.59f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(1.00f, 0.39f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.12f, 0.11f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.72f, 0.29f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(1.00f, 0.47f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(1.00f, 0.39f, 0.00f, 0.78f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(1.00f, 0.39f, 0.00f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(1.00f, 0.39f, 0.00f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(1.00f, 0.39f, 0.00f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 0.39f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 0.39f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 0.39f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Tab]                    = ImVec4(1.00f, 0.39f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_TabHovered]             = ImVec4(0.32f, 0.31f, 0.32f, 0.78f);
    style.Colors[ImGuiCol_TabActive]              = ImVec4(0.12f, 0.11f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]           = ImVec4(0.59f, 0.20f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(1.00f, 0.39f, 0.00f, 0.59f);
    style.Colors[ImGuiCol_DockingPreview]         = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style.Colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.00f, 0.39f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style.WindowRounding = 8;
    style.ChildRounding = 4;
    style.PopupRounding = 4;
    style.GrabRounding = 4;
    style.FrameRounding = 4;
    style.ScrollbarRounding = 4;
    style.TabRounding = 4;

    style.FrameBorderSize = 1;
    style.SeparatorTextBorderSize = 1;
}

void GuiManager::destroyImGui()
{
    bufferMan->destroyImguiFontBuffer(fontImage);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    vkDestroyDescriptorPool(presentMan->device, imguiPool, nullptr);
    ImGui::DestroyContext();
}

void GuiManager::newFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiManager::renderGui()
{
    ImGui::Render();
}
