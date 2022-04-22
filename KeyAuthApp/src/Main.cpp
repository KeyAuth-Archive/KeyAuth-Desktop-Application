#include <imgui.h>
#include <imgui_internal.h>

#include <Windows.h>

#include <window/Framework.hpp>
#include <api/Requests.hpp>
#include <api/SellerApi.hpp>

#include <Bytes.hpp>

#include <interface/LicensePanel.h>
#include <interface/UsersPanel.h>
#include <interface/SubscriptionsPanel.h>
#include <interface/ChatPanel.h>
#include <interface/SessionsPanel.h>
#include <interface/FilesPanel.h>
#include <interface/WebhooksPanel.h>
#include <interface/VariablesPanel.h>
#include <interface/BlacklistPanel.h>
#include <interface/SettingsPanel.h>
#include <interface/other/helpers.hpp>

#include <fstream>  
#include <imgui_custom.h>
#include <config.hpp>

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "d3d9.lib")

bool InitPos = false;
bool ShowMenu = true;

PDIRECT3DTEXTURE9 TextImage = NULL;
PDIRECT3DTEXTURE9 IconImage = NULL;

nlohmann::json response_decoder;
config::file file;
config::parser parser;
helpers::message msg;

namespace Main
{
    static std::vector<std::string> panels{ { "Licenses" }, { "Users" }, { "Subscriptions" }, { "Chats" }, { "Sessions" }, { "Webhooks" }, { "Files" }, { "Variables" }, { "Blacklists" }, { "Settings" }};

    //initial loading information
    static bool run_once = false;
    static bool GotStartupInformation = false;
    std::string status;

    void GetInfoThread(void)
    {
        //shitty code but best way to manage since rate limit is fucked
        status = "Getting Licenses";        Sleep(500); Licenses::GetAllLicenses();
        status = "Getting Users";           Sleep(500); Users::GetAllUsers();
        status = "Getting Chats";           Sleep(500); Chat::GetAllChannels();
        status = "Getting Subscriptions";   Sleep(500); Subs::FetchAllSubs();
        status = "Getting Files";           Sleep(500); Files::GetAllFiles();
        status = "Getting Sessions";        Sleep(500); Sessions::GetAllSessions();
        status = "Getting Variables";       Sleep(500); Variables::GetAllVars();
        status = "Getting Blacklists";      Sleep(500); Blacklists::GetAllBlacklists();
        status = "Getting Settings";        Sleep(500); Settings::GetAllSettings();

        GotStartupInformation = true;
    }

    inline void GetSellerInfoStartup(void)
    {
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);

        if (ImGui::Begin("##startupinfo", nullptr, flags))
        {
            const int spinner_radius = 30;
            const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);

            ImGui::SetCursorPosX((ImGui::GetMainViewport()->WorkSize.x / 2) - (spinner_radius));
            ImGui::SetCursorPosY((ImGui::GetMainViewport()->WorkSize.y / 2) - (spinner_radius));

            ImGui::Spinner("##spinner", spinner_radius, 10, col);

            ImGui::SetCursorPosX((ImGui::GetMainViewport()->WorkSize.x / 2) - (ImGui::CalcTextSize(status.c_str()).x / 2));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);
            ImGui::Text(status.c_str());

            if (!run_once) { CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetInfoThread, 0, 0, 0); run_once = true; }
        }
        ImGui::End();
    }

    void Render(void) {

        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

        ImGui::SetNextWindowPos (ImGui::GetMainViewport()->WorkPos );
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);

        if (ImGui::Begin("##fullscreen_window", nullptr, flags))
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);

            ImGui::BeginChild("##LeftSide", ImVec2(175, ImGui::GetContentRegionAvail().y), true);
            {
                ImGui::SetCursorPosY(15);
                ImGui::SetCursorPosX(30);

                ImGui::Image((void*)IconImage, ImVec2::ImVec2(35, 32));
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
                ImGui::Image((void*)TextImage, ImVec2::ImVec2(75, 35));

                ImGui::SetCursorPosY(100);

                for (unsigned int i = 0; i < panels.size(); i++) {
                    bool selected = (CurrentPanel == i);

                    if (ImGui::Selectable(panels[i].c_str(), &selected)) CurrentPanel = i;

                    //check if item is hovered and is right clicked (1)
                    if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[1]) ImGui::OpenPopup("refresh");
                    if (selected) ImGui::SetItemDefaultFocus();
                }

                if (ImGui::BeginPopup("refresh"))
                {
                    if (ImGui::BeginChild("##refreshsection", ImVec2(100, 23), false))
                    {
                        if (ImGui::Button("refresh", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                        {
                            switch (CurrentPanel)
                            {
                                case 0: Licenses::GetAllLicenses();     break;
                                case 1: Users::GetAllUsers();           break;
                                case 2: Subs::FetchAllSubs();           break;
                                case 3: Chat::GetAllChannels();         break;
                                case 4: Sessions::GetAllSessions();     break;
                                case 6: Files::GetAllFiles();           break;
                                case 7: Variables::GetAllVars();        break;
                                case 8: Blacklists::GetAllBlacklists(); break;
                                case 9: Settings::GetAllSettings();     break;
                            }
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndPopup();
                }
                ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y + 275);
                if (ImGui::Button("reload", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                {
                    GotStartupInformation = false;
                    run_once = false;
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();

            ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
            {
                switch (CurrentPanel)
                {
                    case 0: Interface::License::     RenderLicenses();      break;
                    case 1: Interface::User::        RenderUsers();         break;
                    case 2: Interface::Subscription::RenderSubscriptions(); break;
                    case 3: Interface::Chats::       RenderChats();         break;
                    case 4: Interface::Session::     RenderSessions();      break;
                    case 5: Interface::Webhook::     RenderWebhooks();      break;
                    case 6: Interface::File::        RenderFiles();         break;
                    case 7: Interface::Variable::    RenderVariables();     break;
                    case 8: Interface::BlackList::   RenderBlack();         break;
                    case 9: Interface::Setting::     RenderSettings();      break;
                }
            }
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::End();
        }
        ImGui::End();
    }
}

static WindowClass Window;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

    //AllocConsole();
    //SetConsoleTitleA("debug console");
    //FILE* fp;
    //freopen_s(&fp, "CONOUT$", "w", stdout);

    //setup custom window api
    Window.WindowTitle("KeyAuth");
    Window.WindowDimensions(1215, 850);
    
    //create application window
    if (Application::Create(hInstance))
    {
        //add poppins font from memory (using hxd if you want to change font)
        ImGuiIO* IO = &ImGui::GetIO();
        IO->Fonts->AddFontFromMemoryTTF(&memory::font, sizeof memory::font, 16);

        //create keyauth logo text and logo from memory to display later
        (HRESULT)D3DXCreateTextureFromFileInMemory(GetPDevice(), memory::keyauth_icon, sizeof memory::keyauth_icon, &TextImage);
        (HRESULT)D3DXCreateTextureFromFileInMemory(GetPDevice(), memory::keyauth_logo_text, sizeof memory::keyauth_logo_text, &IconImage);

        MSG currentMSG = GetMsg();
        ZeroMemory(&currentMSG, sizeof(currentMSG));

        //get direct x message and creates application loop while the device isnt exiting
        while (currentMSG.message != WM_QUIT)
        {
            if (PeekMessageA(&currentMSG, NULL, (UINT)0U, (UINT)0U, PM_REMOVE)) 
            { 
                TranslateMessage(&currentMSG); 
                DispatchMessageA(&currentMSG); 
                continue; 
            }

            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();

            //create new application frame
            ImGui::NewFrame();
            {
                if (msg.status() && !Main::GotStartupInformation)
                {
                    Main::GetSellerInfoStartup();
                }
                else
                {
                    msg.startup();       
                    msg.popup();

                    Main::Render();
                }
            }
            ImGui::EndFrame();

            //cleanup, exit device and close window
            GetPDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

            if (GetPDevice()->BeginScene() >= 0) 
            { 
                ImGui::Render(); 
                ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData()); 
                GetPDevice()->EndScene(); 
            }

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
            { 
                ImGui::UpdatePlatformWindows(); ImGui::RenderPlatformWindowsDefault(); 
            }

            HRESULT Result = GetPDevice()->Present(NULL, NULL, NULL, NULL);

            if (Result == D3DERR_DEVICELOST && GetPDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) 
            { 
                ResetDevice(); 
            }

            if (!ShowMenu) { currentMSG.message = WM_QUIT; }
        }
    }

    return 0;
}