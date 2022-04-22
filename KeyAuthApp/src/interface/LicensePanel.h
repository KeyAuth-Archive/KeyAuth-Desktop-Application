#pragma once
#include <iostream>

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::License
{
    static int identifier_mutator{ rand() };

    char username[100];
    char password[100];

    char BanReason[256];
    char LicenseFromUsername[256];
    char TimeAmount[256];

    struct LicenseInfo
    {
        char LicenseMask[20], Expiry[20], Level[20], Amount[3]; 
    }l_info;

    inline void __stdcall RenderLicenses(void)
    {
        Center_Text("Current Licenses");
        ImGui::Separator();

        if (ImGui::Button("Create Key")) ImGui::OpenPopup("c_key"); if (ImGui::BeginPopup("c_key"))
        {
            Center_Text("create license");
            ImGui::Separator();

            ImGui::Text("mask e.x. [XXX-XXX-customtag]");
            ImGui::InputText("##mask", l_info.LicenseMask, array_size(l_info.LicenseMask));
            ImGui::Text("key expiry (in days)");
            ImGui::InputText("##expiry", l_info.Expiry, array_size(l_info.Expiry));
            ImGui::Text("key level");

            if (ImGui::BeginChild("##sublevels", ImVec2(ImGui::GetContentRegionAvail().x, 45), false))
            {
                for (Subs::subs_struct subscription : Subs::v_subs)
                {
                    ImGui::Text("subname: %s, sublvl: %s", subscription.name.c_str(), subscription.level.c_str());
                }
                ImGui::EndChild();
            }

            ImGui::InputText("##lvl", l_info.Level, array_size(l_info.Level));
            ImGui::Text("key amount");
            ImGui::InputText("##amount", l_info.Amount, array_size(l_info.Amount));

            if (ImGui::Button("create")) Licenses::CreateLicense(l_info.LicenseMask, l_info.Expiry, l_info.Level, l_info.Amount);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

        ImGui::PopID();    
        ImGui::SameLine();

        if (ImGui::Button("Banned Licenses")) ImGui::OpenPopup("moreinfo"); if (ImGui::BeginPopup("moreinfo"))
        {
            Center_Text("more license information");
            ImGui::Separator();

            ImGui::Text("Banned Licenses");

            if (ImGui::BeginChild("##bannedlicenseinfo", ImVec2(300, 370), true))
            {
                if (Licenses::bannedinfo.size() == NULL)
                    Center_Text("no banned licenses...");
                else
                {
                    for (Licenses::baninfo info : Licenses::bannedinfo)
                    {
                        ImGui::TextWrapped("username: %s\nlicense: %s\nReason:\n%s",
                            info.user == "null" ? "not yet used" : info.user, info.license, info.banreason.empty() ? "No Ban Reason" : info.banreason.c_str());

                        ImGui::PushID(identifier_mutator * 3 + identifier_mutator);

                        if (ImGui::Button("unban", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                        {
                            Licenses::UnbanLicense(info.license);
                        }
                        ImGui::Separator();
                    }
                }
                ImGui::EndChild();
            }

            if (ImGui::Button("close")) ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Add time to unused key")) ImGui::OpenPopup("timetokeys"); if (ImGui::BeginPopup("timetokeys"))    
        {
            Center_Text("add time to (all unused) keys");
            ImGui::Separator();

            ImGui::Text("time amount (days):");
            ImGui::InputText("##addtime", TimeAmount, array_size(TimeAmount));

            if (ImGui::Button("add time")) Licenses::AddTimeToUnusedLicenses(TimeAmount);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete Used")) Licenses::DeleteAllUsedLicenses();
        ImGui::SameLine();
        if (ImGui::Button("Delete Unused")) Licenses::DeleteAllUnusedLicenses();
        ImGui::SameLine();
        if (ImGui::Button("Delete All")) Licenses::DeleteAllLicenses();
        ImGui::SameLine();

        if (ImGui::Button("Get Key from Username")) ImGui::OpenPopup("getKey"); if (ImGui::BeginPopup("getKey"))
        {
            Center_Text("get license from username");
            ImGui::Separator();

            ImGui::Text("Current Users");

            if (ImGui::BeginChild("##CurrentUsers", ImVec2(ImGui::GetContentRegionAvail().x, 130), false))
            {
                for (std::string user : Licenses::Usernames)
                {
                    ImGui::Text(" - %s", user.c_str());
                }
                ImGui::EndChild();
            }

            ImGui::Separator();

            ImGui::Text("username");
            ImGui::InputText("##addtime", LicenseFromUsername, array_size(LicenseFromUsername));

            if (ImGui::Button("get key")) Licenses::GetLicenseFromUsername(LicenseFromUsername);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Create User from Key")) ImGui::OpenPopup("createfromkey"); if (ImGui::BeginPopup("createfromkey"))
        {
            Center_Text("create user from license");
            ImGui::Separator();

            ImGui::Text("license");
            ImGui::InputText("##licenseselection", Licenses::License, array_size(Licenses::License));

            ImGui::Text("username");
            ImGui::InputText("##user", username, array_size(username));

            ImGui::Text("password (empty = use can set)");
            ImGui::InputText("##pass", password, array_size(password));

            ImGui::Separator();
            ImGui::Text("User Information");

            Licenses::GetLicenseInformation(Licenses::License);

            std::string sub{};
            time_t ExpiryTime;

            if (std::mktime(&Licenses::info.usedon) == -1)
            {
                ExpiryTime = mktime(&Licenses::info.expires);

                for (Subs::subs_struct subscription : Subs::v_subs)
                {
                    if (subscription.level == Licenses::info.level)
                    {
                        sub = " -> " + subscription.name;
                    }
                }

                std::string finaltime = {
                        std::to_string((ExpiryTime) / (24 * 3600)) + " Days, " + std::to_string((ExpiryTime % (24 * 3600)) / (3600)) + " Hours, " +
                        std::to_string((ExpiryTime % (3600)) / 60) + " Minutes, " + std::to_string((ExpiryTime) % 60) + " Seconds"
                };

                ImGui::TextWrapped("subscription %s\nexpiry -> %s", sub, ExpiryTime == -1 ? "null" : finaltime.c_str());
            }
            else
            {
                ImGui::TextWrapped("license has already been used and is invalid...");
            }

            ImGui::Separator();

            if (ImGui::Button("create user")) Licenses::CreateUserFromLicense(Licenses::License, username, password);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        if (ImGui::BeginChild("##licensessection", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true))
        {
            ImGui::Text("banned ="); ImGui::SameLine(); ImGui::TextColored(ImColor(0, 0, 255), "O"); ImGui::SameLine();
            ImGui::Text("used ="); ImGui::SameLine(); ImGui::TextColored(ImColor(255, 0, 0), "O"); ImGui::SameLine();
            ImGui::Text("not used ="); ImGui::SameLine(); ImGui::TextColored(ImColor(0, 255, 0), "O");

            ImGuiTableFlags flags2 = ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
            if (ImGui::BeginTable("table_context_menu_2", 4, flags2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 20)))
            {
                ImGui::TableSetupColumn("Load", 0, 0.23f); ImGui::TableSetupColumn(" ", 0, 0.07f); ImGui::TableSetupColumn("License"); ImGui::TableSetupColumn("Username");

                ImGui::TableHeadersRow();
                for (int row = 0; row < Licenses::UserInfo.size(); row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(identifier_mutator * 3 + identifier_mutator);
                    if (ImGui::Button("[+]"))
                    {
                        Licenses::GetLicenseInformation(Licenses::UserInfo[row].license);
                    }
                    ImGui::TableSetColumnIndex(1);

                    if (Licenses::Banned[row]) ImGui::TextColored(ImColor(0, 0, 255), "O");
                    else
                    {
                        if (Licenses::Used[row]) ImGui::TextColored(ImColor(255, 0, 0), "O");
                        else ImGui::TextColored(ImColor(0, 255, 0), "O");
                    }

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", Licenses::UserInfo[row].license.c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", Licenses::UserInfo[row].user == "null" ? "not being used" : Licenses::UserInfo[row].user.c_str());

                    ImGui::SameLine();
                }
                ImGui::EndTable();
                ImGui::Text("Amount of Licenses: %i, Amount Used: %i, Amount Banned %i", Licenses::UserInfo.size(), Licenses::ammountused, Licenses::amountbanned);
            }
            ImGui::TreePop();
            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##LicenseInformation", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGui::Text("Further License Details");
            ImGui::Separator();

            if (ImGui::Button("Delete License")) Licenses::DeleteLicense(Licenses::License);
            ImGui::SameLine();

            if (ImGui::Button("Ban License")) ImGui::OpenPopup("BanLicense"); if (ImGui::BeginPopup("BanLicense"))
            {
                Center_Text("ban license");
                ImGui::Separator();

                ImGui::Text("license key:");
                ImGui::InputText("##banlicense", Licenses::License, array_size(Licenses::License));
                ImGui::Text("ban reason:");
                ImGui::InputText("##banreason", BanReason, array_size(BanReason));

                if (ImGui::Button("ban license"))
                {
                    Licenses::BanLicense(Licenses::License, BanReason);
                    (void)memset(Licenses::License, NULL, sizeof Licenses::License);
                }
                ImGui::SameLine();
                if (ImGui::Button("cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
            ImGui::SameLine();

            if (ImGui::Button("Unban License")) ImGui::OpenPopup("UnbanLicense"); if (ImGui::BeginPopup("UnbanLicense"))
            {
                Center_Text("unban license");
                ImGui::Separator();

                ImGui::Text("license key:");
                ImGui::InputText("##banlicense", Licenses::License, array_size(Licenses::License));

                if (ImGui::Button("unban license"))
                {
                    Licenses::UnbanLicense(Licenses::License);
                }
                ImGui::SameLine();
                if (ImGui::Button("cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            ImGui::Text("License ID: %s", Licenses::info.id.c_str());
            ImGui::Text("License Key:");

            ImGui::InputText("##LicenseKey", Licenses::License, array_size(Licenses::License));
            if (ImGui::Button("Copy License")) CopytoClipboard(FALSE, Licenses::info.key);
            ImGui::Separator();
            ImGui::Text("Note: %s", Licenses::info.note == "null" ? "no note set" : Licenses::info.note.c_str());
            ImGui::InputText("##ChangeNote", Licenses::NoteInput, array_size(Licenses::NoteInput));

            if (ImGui::Button("Save")) Licenses::ChangeNote(Licenses::NoteInput);

            ImGui::Separator();
            {
                std::string finaltime{};

                std::string header{"Expiry\n%s"};

                using namespace std::chrono;
                using namespace std;

                time_t ExpiryTime;
                
                if (std::mktime(&Licenses::info.usedon) == -1)
                {
                    ExpiryTime = mktime(&Licenses::info.expires);
                    header = "Expiry (not expiring)\n%s";
                }
                else ExpiryTime = (mktime(&Licenses::info.usedon) + mktime(&Licenses::info.expires)) - system_clock::to_time_t(system_clock::now());

                finaltime = {
                    to_string((ExpiryTime) / (24 * 3600)) + " Days, " + to_string((ExpiryTime % (24 * 3600)) / (3600)) + " Hours, " +
                    to_string((ExpiryTime % (3600)) / 60) + " Minutes, " + to_string((ExpiryTime) % 60)                   + " Seconds"
                };

                std::string subscription_name{};

                for (Subs::subs_struct subscription : Subs::v_subs)
                {
                    if (subscription.level == Licenses::info.level)
                    {
                        subscription_name = " -> " + subscription.name;
                    }
                }


                bool notused = std::mktime(&Licenses::info.usedon) == -1 ? true : false;
                bool expired = notused ? false : (mktime(&Licenses::info.usedon) + mktime(&Licenses::info.expires)) - system_clock::to_time_t(system_clock::now()) < 0 ? true : false;

                ImGui::Text(header.c_str(), expired ? "license expired" : finaltime.c_str());
                ImGui::Text("Status:\n%s", Licenses::info.status.c_str());

                ImGui::Text("Key Level:\n%s%s", Licenses::info.level.c_str(), subscription_name.c_str());
                ImGui::Text("Generated by:\n%s", Licenses::info.genby.c_str());

                ImGui::Text("Used by:\n%s", Licenses::info.usedby == "null" ? "not used  " : Licenses::info.usedby.c_str());
                ImGui::Text("Banned:\n%s",  Licenses::info.banned == "null" ? "not banned" : Licenses::info.banned.c_str());
            }
            ImGui::EndChild();
        }
    }
}