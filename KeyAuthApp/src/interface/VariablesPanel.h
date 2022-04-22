#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::Variable
{
	struct variable_struct { char varname[50], vardata[250]; bool authed; }var_info;

	inline void __stdcall RenderVariables(void)
	{
        Center_Text("Current Variables");
        ImGui::Separator();

        if (ImGui::Button("Create Variable")) ImGui::OpenPopup("c_var"); if (ImGui::BeginPopup("c_var"))
        {
            Center_Text("create variable");
            ImGui::Separator();

            ImGui::Text("variable name");
            ImGui::InputText("##varname", var_info.varname, array_size(var_info.varname));
            ImGui::Text("variable data");
            ImGui::InputText("##expiry", var_info.vardata, array_size(var_info.vardata));

            ImGui::Checkbox("authed", &var_info.authed);

            if (ImGui::Button("create")) Variables::CreateVariable(var_info.varname, var_info.vardata, var_info.authed);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete All Variables")) Variables::DeleteAllVariables();

        if (ImGui::BeginChild("##allvariables", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##var_table", 3, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                ImGui::TableSetupColumn("info", 0, 0.2); ImGui::TableSetupColumn("name"); ImGui::TableSetupColumn("vardata");
                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Variables::v_vars.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.3 + CurrentRow);

                    if (ImGui::Button("[+]")) ImGui::OpenPopup("editvar"); if (ImGui::BeginPopup("editvar")) 
                    {
                        Center_Text("edit variable");
                        ImGui::Separator();

                        ImGui::Text("edit vardata");
                        ImGui::InputText("##editvar", var_info.vardata, array_size(var_info.vardata));

                        if (ImGui::Button("update")) Variables::EditVariable(Variables::v_vars[CurrentRow].varid, var_info.vardata);

                        ImGui::Separator();

                        if (ImGui::Button("delete variable")) Variables::DeleteVariable(Variables::v_vars[CurrentRow].varid);
                        ImGui::SameLine();
                        if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Variables::v_vars[CurrentRow].varid.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(Variables::v_vars[CurrentRow].msg.c_str());

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
