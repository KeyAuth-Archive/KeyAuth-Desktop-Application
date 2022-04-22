#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::Subscription
{
	struct subscriptions_struct { char Subname[20], Sublevel[4], NewLevel[4]; }c_sub;

	inline void __stdcall RenderSubscriptions(void)
	{
        Center_Text("Current Subscriptions");
        ImGui::Separator();

        if (ImGui::Button("Create Subscription"))
            ImGui::OpenPopup("createsub");

        if (ImGui::BeginPopup("createsub")) {

            Center_Text("create subscription");
            ImGui::Separator();

            ImGui::Text("subscription name");
            ImGui::InputText("##subname", c_sub.Subname, array_size(c_sub.Subname));
            ImGui::Text("subscription level");
            ImGui::InputText("##sublvl", c_sub.Sublevel, array_size(c_sub.Sublevel));

            if (ImGui::Button("create")) Subs::CreateSubscription(c_sub.Subname, c_sub.Sublevel);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();
        ImGui::SameLine();

        if (ImGui::Button("Delete Subscription")) ImGui::OpenPopup("delsub"); if (ImGui::BeginPopup("delsub")) 
        {
            Center_Text("delete subscription");
            ImGui::Separator();

            ImGui::Text("subscription name");
            ImGui::InputText("##subname", c_sub.Subname, array_size(c_sub.Subname));

            if (ImGui::Button("delete")) Subs::DeleteSubscription(c_sub.Subname);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();

        if (ImGui::BeginChild("##subscriptions", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##subscriptions_table", 3, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                ImGui::TableSetupColumn("Edit", (float)0, 0.07f); ImGui::TableSetupColumn("lvl", (float)0, 0.03); ImGui::TableSetupColumn("Subscription Name");
                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Subs::v_subs.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.4 + CurrentRow);

                    if (ImGui::Button("[+]")) ImGui::OpenPopup("edsub"); if (ImGui::BeginPopup("edsub")) 
                    {

                        std::string header{ "edit " + Subs::v_subs[CurrentRow].name + " subscription" };

                        Center_Text(header.c_str());
                        ImGui::Separator();

                        ImGui::Text("new level");
                        ImGui::InputText("##newlvl", c_sub.NewLevel, array_size(c_sub.NewLevel));

                        if (ImGui::Button("confirm")) Subs::EditSubscription(Subs::v_subs[CurrentRow].name, c_sub.NewLevel);
                        ImGui::SameLine();
                        if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Subs::v_subs[CurrentRow].level.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(Subs::v_subs[CurrentRow].name.c_str());
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