#include <interface/other/helpers.hpp>
#include <window/Framework.hpp>
#include <api/Requests.hpp>
#include <config.hpp>

#define region ImGui::GetContentRegionAvail()

namespace helpers
{
    static bool selectedapp;

    Requests::Response response;
    config::file       file;
    config::parser     parser;

	void message::popup(void)
	{
        CenterModal(ImVec2(375, 225));

        const char* header_text = " keyauth - server response";

        if (response.PopupIsOpen()) ImGui::OpenPopup(header_text);

        if (ImGui::BeginPopupModal(header_text, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginChild("#repsonsechild", ImVec2(region.x, region.y - 27)))
            {
                if (response.isRateLimited()) ImGui::TextColored(ImColor(255, 0, 0), response.getReqStatus().c_str());
                else
                {
                    if (response.RequestSuccess())
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 255, 0, 255));
                    else
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 0, 0, 255));

                    ImGui::TextWrapped(response.getReqStatus().c_str());
                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();
            }
            if (ImGui::Button("close", ImVec2(ImGui::GetContentRegionAvail().x, 23))) { response.setPopupStatus(false); ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
	}

    bool message::status() const
    {
        return selectedapp;
    }

    struct input
    {
        char seller[100], name[100];
    }in;

    void message::startup(void)
    {
        CenterModal(ImVec2(GetWindowSize().x / 2.3, GetWindowSize().y / 2));

        const char* header_text = " select an application";

        if (!selectedapp) ImGui::OpenPopup(header_text);

        if (ImGui::BeginPopupModal(header_text, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            nlohmann::json json = parser.getjson("config.json");

            if (ImGui::Button("Add new application")) ImGui::OpenPopup("newapp");

            if (ImGui::BeginPopup("newapp")) {

                ImGui::InputText("sellerkey", in.seller, IM_ARRAYSIZE(in.seller, ));
                ImGui::InputText("app name", in.name, IM_ARRAYSIZE(in.name));

                if (ImGui::Button("app application"))
                    file.add("config.json", json, in.seller, in.name);

                ImGui::EndPopup();
            }

            if (ImGui::BeginChild("###appspanel", ImVec2(region.x, region.y), true))
            {
                if (json["appinfo"].size() > NULL)
                {
                    for (nlohmann::json app : json["appinfo"])
                    {
                        std::string name = std::string(app["name"]);
                        std::string key = std::string(app["key"]);

                        ImGui::TextWrapped("app name -> %s", name.c_str());
                        ImGui::TextWrapped("seller key -> %s", key.c_str());

                        ImGui::PushID(strlen(name.c_str()) + name.c_str()[0]);

                        if (ImGui::Button("load application", ImVec2(region.x - 45, 23)))
                        {
                            Requests::SetSellerKey(std::string(app["key"]));
                            selectedapp = true;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button("del", ImVec2(region.x, 23))) file.del("config.json", json, app);

                        ImGui::PopID();
                        ImGui::Separator();
                    }
                }
                else Center_Text("no applications yet...");

                ImGui::EndChild();
            }
            ImGui::EndPopup();
        }
    }
}