
#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::BlackList
{
    struct blacklist_struct { char hwid[100], ip[25]; }; blacklist_struct black;

	inline void __stdcall RenderBlack(void)
	{
        Center_Text("Current Blacklist");
        ImGui::Separator();

        if (ImGui::Button("unBlacklist All"))
        {
            Blacklists::DeleteAllBlack();
        }

        ImGui::SameLine();

        if (ImGui::Button("Add Blacklist")) ImGui::OpenPopup("addblack"); if (ImGui::BeginPopup("addblack"))
        {
            ImGui::Text("blacklist HWID:");
            ImGui::InputText("##blackhwid", black.hwid, array_size(black.hwid));
            ImGui::Text("blacklist ip:");
            ImGui::InputText("##blackip", black.ip, array_size(black.ip));

            if (ImGui::Button("create blacklist"))
            {
                Blacklists::AddBlack(black.ip, black.hwid);
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginChild("##blacklists", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##black_table_hwid", 2, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y)))
            {
                //Initialize Rows in table with name and width respectfully
                ImGui::TableSetupColumn("Load", 0, 0.15f); ImGui::TableSetupColumn("hwid");

                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Blacklists::hwid.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.3 + CurrentRow);

                    if (ImGui::Button("[+]")) ImGui::OpenPopup("hwid"); if (ImGui::BeginPopup("hwid"))
                    {
                        if (ImGui::BeginChild("##furtherhwidinfo", ImVec2(300, 200), false))
                        {
                            Center_Text("Further BlackList Information");

                            std::string username;

                            if (ImGui::BeginChild("##furtherhwidinfo", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 27), true))
                            {
                                for (Users::usernameinfo info : Users::info)
                                {
                                    if (Blacklists::hwid[CurrentRow] == info.hwid)
                                    {
                                        for (Licenses::userinfo user_to_license : Licenses::UserInfo)
                                        {
                                            if (info.username == user_to_license.user)
                                            {
                                                username += " \n - license: " + user_to_license.license;
                                            }
                                        }
                                        username += " \n - user: " + info.username;
                                    }
                                }

                                ImGui::Text("this blacklist affects");
                                ImGui::Separator();

                                ImGui::Text(username.c_str());

                                ImGui::EndChild();
                            }

                            if (ImGui::Button("unblack list"))
                                Blacklists::DeleteBlackList(Blacklists::hwid[CurrentRow], true);

                            ImGui::SameLine();

                            if (ImGui::Button("cancel"))
                                ImGui::CloseCurrentPopup();
                            ImGui::EndChild();
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Blacklists::hwid[CurrentRow].c_str());

                    ImGui::SameLine();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();

            ImGui::SameLine();

            if (ImGui::BeginTable("##black_table_ip", 2, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                ImGui::TableSetupColumn("Load", 0, 0.15f); ImGui::TableSetupColumn("ip");

                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Blacklists::ips.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.3 + CurrentRow);

                    if (ImGui::Button("[+]")) ImGui::OpenPopup("ips"); if (ImGui::BeginPopup("ips"))
                    {
                        if (ImGui::BeginChild("##blacklistedip", ImVec2(300, 200), false))
                        {
                            Center_Text("Further BlackList Information");

                            std::string username;

                            if (ImGui::BeginChild("##furtheripinfo", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 27), true))
                            {
                                for (Users::usernameinfo info : Users::info)
                                {
                                    if (Blacklists::ips[CurrentRow] == info.ip)
                                    {
                                        for (Licenses::userinfo user_to_license : Licenses::UserInfo)
                                        {
                                            if (info.username == user_to_license.user)
                                            {
                                                username += " \n - license: " + user_to_license.license;
                                            }
                                        }
                                        username += " \n - user: " + info.username;
                                    }
                                }

                                ImGui::Text("this blacklist affects");
                                ImGui::Separator();

                                ImGui::Text(username.c_str());

                                ImGui::EndChild();
                            }

                            if (ImGui::Button("unblack list"))
                                Blacklists::DeleteBlackList(Blacklists::ips[CurrentRow], false);

                            ImGui::SameLine();

                            if (ImGui::Button("cancel"))
                                ImGui::CloseCurrentPopup();
                            ImGui::EndChild();
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Blacklists::ips[CurrentRow].c_str());

                    ImGui::SameLine();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();

            ImGui::EndChild();
        }
	}
}