#include <window/Framework.hpp>

struct OverlayWindow 
{
    WNDCLASSEX WindowClass;
    HWND Hwnd = NULL;
    LPCSTR Name = NULL;
}Overlay;

HWND WindowClass::GetOverlayHWND(){return Overlay.Hwnd;}

struct DirectX9Interface 
{
    LPDIRECT3D9 IDirect3D9 = NULL;
    LPDIRECT3DDEVICE9 pDevice = NULL;
    D3DPRESENT_PARAMETERS pParameters;
    MSG Message;
}DirectX9;

LPDIRECT3DDEVICE9 GetPDevice(){return DirectX9.pDevice;}
LPDIRECT3D9 GetIDirect3D9(){return DirectX9.IDirect3D9;}
D3DPRESENT_PARAMETERS GetParameters(){return DirectX9.pParameters;}
MSG GetMsg(){return DirectX9.Message;}

struct AppInfo
{
    std::string Title;
    int width = { 1215 };
    int height = { 850 };
}appinfo;

void WindowClass::WindowTitle(std::string name){appinfo.Title = name;}
void WindowClass::WindowDimensions(int width, int height)
{
    appinfo.width = width;
    appinfo.height = height;
}

bool CreateDeviceD3D(HWND hWnd) {
    if ((DirectX9.IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
        return false;
    }
    ZeroMemory(&DirectX9.pParameters, sizeof(DirectX9.pParameters));
    DirectX9.pParameters.Windowed = TRUE;
    DirectX9.pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    DirectX9.pParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    DirectX9.pParameters.EnableAutoDepthStencil = TRUE;
    DirectX9.pParameters.AutoDepthStencilFormat = D3DFMT_D16;
    DirectX9.pParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (DirectX9.IDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX9.pParameters, &DirectX9.pDevice) < 0) {
        return false;
    }
    return true;
}

void ClearD3D() {
    if (DirectX9.pDevice) {
        DirectX9.pDevice->Release();
        DirectX9.pDevice = NULL;
    }

    if (DirectX9.IDirect3D9) {
        DirectX9.IDirect3D9->Release();
        DirectX9.IDirect3D9 = NULL;
    }
}

void ClearAll() { ClearD3D(); UnregisterClass(Overlay.WindowClass.lpszClassName, Overlay.WindowClass.hInstance); }

void ResetDevice() {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    if (DirectX9.pDevice->Reset(&DirectX9.pParameters) == D3DERR_INVALIDCALL) IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void Center(float avail_width, float element_width, float padding){ImGui::SameLine((avail_width / 2) - (element_width / 2) + padding);}

void Center_Text(const char* format, float spacing, ImColor color) {
    Center(ImGui::GetContentRegionAvail().x, ImGui::CalcTextSize(format).x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);
    ImGui::TextColored(color.Value, format);
}

void MoveWindowToCenter()
{
    RECT rectClient, rectWindow;
    HWND hWnd = Overlay.Hwnd;
    GetClientRect(hWnd, &rectClient);
    GetWindowRect(hWnd, &rectWindow);

    int width = rectWindow.right - rectWindow.left;
    int height = rectWindow.bottom - rectWindow.top;

    MoveWindow(
        hWnd,
        GetSystemMetrics(SM_CXSCREEN) / 2 - (width) / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - (height) / 2,
        rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);
}

ImVec2 GetWindowSize()
{
    RECT rect;
    GetWindowRect(Overlay.Hwnd, &rect);

    return ImVec2((rect.right - rect.left), (rect.bottom - rect.top));
}

void CenterModal(ImVec2 size)
{
    //get current window position from hwnd
    RECT rect;
    GetWindowRect(Overlay.Hwnd, &rect);

    //draws new window relative to the position of the current window
    ImGui::SetNextWindowPos
    (ImVec2((rect.left + (((rect.right - rect.left) / 2) - (size.x / 2))), (rect.top + (((rect.bottom - rect.top) / 2) - (size.y / 2)))));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
}

void SetTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;

    style->ScrollbarSize = 15;
    style->GrabMinSize = 10;
    style->ChildBorderSize = 1;
    style->PopupBorderSize = 1;
    style->TabBorderSize = 1;
    style->ChildRounding = 4;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 9;
    style->GrabRounding = 3;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);

    style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

bool Application::Create(HINSTANCE hInstance)
{
    Overlay.Name = { appinfo.Title.c_str() };
    Overlay.WindowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, Overlay.Name, NULL };
    Overlay.WindowClass.hIcon = { (HICON)(HANDLE)LoadImageW(hInstance, L"icon.ico", IMAGE_ICON, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE) };

    (ATOM)RegisterClassExA(&Overlay.WindowClass);

    Overlay.Hwnd = CreateWindow(Overlay.Name, Overlay.Name, WS_OVERLAPPEDWINDOW, 0, 0, appinfo.width, appinfo.height, NULL, NULL, Overlay.WindowClass.hInstance, NULL);
    if (CreateDeviceD3D(Overlay.Hwnd) == FALSE) { ClearAll(); }

    MoveWindowToCenter(); ShowWindow(Overlay.Hwnd, SW_SHOW); UpdateWindow(Overlay.Hwnd);

    ImGui::CreateContext();

    ImGuiIO* IO = &ImGui::GetIO();
    IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    SetTheme();

    if (ImGui_ImplWin32_Init(Overlay.Hwnd) && ImGui_ImplDX9_Init(DirectX9.pDevice))
        return true;
    else return false;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (DirectX9.pDevice != NULL && wParam != SIZE_MINIMIZED) {
            DirectX9.pParameters.BackBufferWidth = LOWORD(lParam);
            DirectX9.pParameters.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

