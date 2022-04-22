#include <d3d9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <string>

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")

static int CurrentPanel = 0;

class WindowClass
{
public:
    HWND GetOverlayHWND();

    void WindowTitle(std::string name);
    void WindowIcon(std::string filename);
    void WindowDimensions(int width, int height);   
};

//related - directx
LPDIRECT3DDEVICE9     GetPDevice();
LPDIRECT3D9           GetIDirect3D9();
D3DPRESENT_PARAMETERS GetParameters();
MSG                   GetMsg();

bool CreateDeviceD3D(HWND hWnd);
void ClearD3D();
void ClearAll();

void ResetDevice();

//GUI helpers
void Center(float avail_width, float element_width, float padding = 15);
void Center_Text(const char* format, float spacing = 15, ImColor color = ImColor(255, 255, 255));
void MoveWindowToCenter();

void CenterModal(ImVec2 size);
ImVec2 GetWindowSize();

//Process handler, ie window destroying, changing size and other commands
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Application
{
    bool Create(HINSTANCE hInstance);
}

