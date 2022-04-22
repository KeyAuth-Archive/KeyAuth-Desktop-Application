#pragma once

namespace Interface::Session
{
	inline void __stdcall RenderSessions(void)
	{
        Center_Text("Current Sessions");
        ImGui::Separator();

        if (ImGui::Button("Kill All Sessions", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
            Sessions::KillAllSessions();

        if (ImGui::BeginChild("##subscriptions", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##subscriptions_table", 5, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                ImGui::TableSetupColumn("Load", 0, 0.15f); ImGui::TableSetupColumn("id"); ImGui::TableSetupColumn("username"); ImGui::TableSetupColumn("expiry"); ImGui::TableSetupColumn("validated");

                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Sessions::v_sessions.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.3 + CurrentRow);

                    if (ImGui::Button("[+]"))
                        ImGui::OpenPopup("editsub");

                    if (ImGui::BeginPopup("editsub")) {

                        Center_Text("kill session");
                        ImGui::Separator();

                        ImGui::Text("Are you sure you want to kill this session?");

                        if (ImGui::Button("confirm"))
                            Sessions::KillSession(Sessions::v_sessions[CurrentRow].sessionid);
                        ImGui::SameLine();
                        if (ImGui::Button("cancel"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Sessions::v_sessions[CurrentRow].sessionid.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(Sessions::v_sessions[CurrentRow].username == "null" ? "not logged in" : Sessions::v_sessions[CurrentRow].username.c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text(Sessions::v_sessions[CurrentRow].expiry.c_str());
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text(Sessions::v_sessions[CurrentRow].valid == "1" ? "true" : "false");
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