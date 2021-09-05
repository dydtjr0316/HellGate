#include "stdafx.h"
#include "ImGUIMgr.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include <Engine/Device.h>

CImGUIMgr::CImGUIMgr()
{

}

CImGUIMgr::~CImGUIMgr()
{
    m_pDescHeap->Release();
}


void CImGUIMgr::init(HWND _hwnd)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // ImGUI가 사용할 DescHeap 만들기
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescHeap));

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hwnd);
    ImGui_ImplDX12_Init(DEVICE.Get(), 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, m_pDescHeap,
        m_pDescHeap->GetCPUDescriptorHandleForHeapStart(),
        m_pDescHeap->GetGPUDescriptorHandleForHeapStart());
}