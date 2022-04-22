#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::Chats
{
    static int identifier_mutator{ rand() };

    struct users_struct { char mute[20], length[256]; char channel[20], delay[10]; char updatedelay[10]; }; users_struct s_users;

	inline void __stdcall RenderChats(void)
	{
        Center_Text("Current Text Channels");
        ImGui::Separator();

        if (ImGui::Button("Create Channel"))
            ImGui::OpenPopup("createchannel");

        if (ImGui::BeginPopup("createchannel")) {

            Center_Text("create channel");
            ImGui::Separator();

            ImGui::Text("channel name");
            ImGui::InputText("##chnlname", s_users.channel, array_size(s_users.channel));
            ImGui::Text("channel text delay (in seconds)");
            ImGui::InputText("##chnldelay", s_users.delay, array_size(s_users.delay));

            if (ImGui::Button("create"))
                Chat::CreateChannel(s_users.channel, s_users.delay);
            ImGui::SameLine();
            if (ImGui::Button("cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();
        ImGui::SameLine();

        if (ImGui::Button("Muted Users")) ImGui::OpenPopup("mutedusers"); if (ImGui::BeginPopup("mutedusers")) 
        {
            Center_Text("muted users");
            ImGui::Separator();

            if (ImGui::Button("refresh list", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
            {
                Chat::GetAllMutedUsers();
            }
            ImGui::Separator();

            if (ImGui::BeginChild("##bannedlicenseinfo", ImVec2(300, 370), true))
            {
                if (Chat::v_muted.size() == NULL) Center_Text("no muted users...");
                else
                {
                    for (Chat::muted_struct muted : Chat::v_muted)
                    {
                        using namespace std::chrono;
                        using namespace std;

                        if (mktime(&muted.time) > system_clock::to_time_t(system_clock::now()))
                        {
                            time_t ExpiryTime = mktime(&muted.time) - system_clock::to_time_t(system_clock::now());

                            std::string finaltime = {
                                to_string((ExpiryTime) / (24 * 3600)) + " Days, " + to_string((ExpiryTime % (24 * 3600)) / (3600)) + " Hours, " +
                                to_string((ExpiryTime % (3600)) / 60) + " Minutes, " + to_string((ExpiryTime) % 60) + " Seconds"
                            };

                            ImGui::Text("username: %s\nban lifts: %s", muted.user, finaltime.c_str());

                            ImGui::PushID(identifier_mutator * 2 - identifier_mutator);

                            if (ImGui::Button("unmute", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                                Chat::UnmuteUser(muted.user);

                            ImGui::Separator();
                        }
                    }
                }
                ImGui::EndChild();
            }

            if (ImGui::Button("cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();

        ImGui::SameLine();

        if (ImGui::Button("Mute User"))
            ImGui::OpenPopup("muteuser");

        if (ImGui::BeginPopup("muteuser")) {

            Center_Text("mute user");
            ImGui::Separator();

            if (ImGui::BeginChild("##CurrentUsers", ImVec2(ImGui::GetContentRegionAvail().x, 130), false))
            {
                for (std::string user : Licenses::Usernames)
                {
                    ImGui::Text(" - %s", user);
                }
                ImGui::EndChild();
            }

            ImGui::Text("username");
            ImGui::InputText("##usertounmute", s_users.mute, array_size(s_users.mute));

            ImGui::Text("time (in seconds)");
            ImGui::InputText("##usertounmutelength", s_users.length, array_size(s_users.length));

            if (ImGui::Button("mute"))
                Chat::MuteUser(s_users.mute, s_users.length);
            ImGui::SameLine();
            if (ImGui::Button("cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();

        if (ImGui::BeginChild("##allchannels", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##chat_table", 3, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                //Initialize Rows in table with name and width respectfully
                ImGui::TableSetupColumn("Edit", 0, 0.075f); ImGui::TableSetupColumn("channel"); ImGui::TableSetupColumn("cooldown delay");

                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Chat::v_channel.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.2 + CurrentRow);

                    if (ImGui::Button("[+]"))
                        ImGui::OpenPopup("editchat");

                    if (ImGui::BeginPopup("editchat")) {

                        std::string header{ "edit " + Chat::v_channel[CurrentRow].name + " channel" };

                        Center_Text(header.c_str());
                        ImGui::Separator();

                        ImGui::Text("new channel delay (in seconds)");
                        ImGui::InputText("##newlvl", s_users.updatedelay, array_size(s_users.updatedelay));

                        if (ImGui::Button("confirm"))
                            Chat::EditChannel(Chat::v_channel[CurrentRow].name, s_users.updatedelay);
                        ImGui::Separator();
                        if (ImGui::Button("delete channel", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                            Chat::DeleteChannel(Chat::v_channel[CurrentRow].name);
                        ImGui::Separator();
                        if (ImGui::Button("clear messages", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                            Chat::ClearChannel(Chat::v_channel[CurrentRow].name);
                        ImGui::Separator();
                        if (ImGui::Button("cancel"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Chat::v_channel[CurrentRow].name.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(Chat::v_channel[CurrentRow].delay == "1" ?
                        (Chat::v_channel[CurrentRow].delay + " second").c_str() : (Chat::v_channel[CurrentRow].delay + " seconds").c_str());
                    ImGui::SameLine();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
            ImGui::EndChild();
        }
        ImGui::SameLine();
	}
}