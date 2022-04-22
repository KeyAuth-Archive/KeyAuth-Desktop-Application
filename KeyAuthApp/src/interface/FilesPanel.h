#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::File
{
    struct file_struct { char fileurl[500]; }; file_struct fileinfo;

	inline void __stdcall RenderFiles(void)
	{
        Center_Text("Current Files");
        ImGui::Separator();

        if (ImGui::Button("Upload File"))
            ImGui::OpenPopup("uploadfile");

        if (ImGui::BeginPopup("uploadfile")) {

            Center_Text("upload file");
            ImGui::Separator();

            ImGui::Text("enter file url");
            ImGui::InputText("##subname", fileinfo.fileurl, array_size(fileinfo.fileurl));

            if (ImGui::Button("upload to update"))
                Files::UploadFile(fileinfo.fileurl);
            ImGui::SameLine();
            if (ImGui::Button("cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();
        ImGui::SameLine();

        if (ImGui::Button("Delete All Files"))
            Files::DeleteAllFiles();

        if (ImGui::BeginChild("##files", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGuiTableFlags TableFlags{ ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders };

            if (ImGui::BeginTable("##files_table", 3, TableFlags, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
            {
                ImGui::TableSetupColumn("Edit", 0, 0.085f); ImGui::TableSetupColumn("id", 0, 0.4); ImGui::TableSetupColumn("url");

                ImGui::TableHeadersRow();

                for (int CurrentRow = 0; CurrentRow < Files::v_files.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(CurrentRow * 2.4 + CurrentRow);

                    if (ImGui::Button("[+]"))
                        ImGui::OpenPopup("editfile");

                    if (ImGui::BeginPopup("editfile")) {

                        Center_Text("delete file");
                        ImGui::Separator();

                        ImGui::Text("are you user you want to delete file %s?", Files::v_files[CurrentRow].id);

                        if (ImGui::Button("confirm"))
                            Files::DeleteFiles(Files::v_files[CurrentRow].id);
                        ImGui::SameLine();
                        if (ImGui::Button("cancel"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Files::v_files[CurrentRow].id.c_str());
                    ImGui::TableSetColumnIndex(2);

                    if (ImGui::Button("copy"))
                    {
                        CopytoClipboard(0, Files::v_files[CurrentRow].url);
                    }
                    ImGui::SameLine();
                    ImGui::Text(Files::v_files[CurrentRow].url.c_str());
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
