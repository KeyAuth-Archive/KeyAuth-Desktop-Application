#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::Setting
{
    char hashtoadd[400];

	inline void __stdcall RenderSettings(void)
	{
        Center_Text("Current App Settings");
        ImGui::Separator();

        if (ImGui::BeginChild("##allsettings", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            if (ImGui::Button("Pause App")) Settings::PauseApp();
            ImGui::SameLine();
            if (ImGui::Button("Unpause App")) Settings::UnPauseApp();
            ImGui::SameLine();
            if (ImGui::Button("Reset Hash")) Settings::ResetHash();
            ImGui::SameLine();
            if (ImGui::Button("Add Hash")) ImGui::OpenPopup("resethash"); if (ImGui::BeginPopup("resethash"))
            {
                ImGui::Text("md5 hash");
                ImGui::InputText("##addhash", hashtoadd, array_size(hashtoadd));
                if (ImGui::Button("add hash")) Settings::AddHash(hashtoadd);
                ImGui::EndPopup();
            }
            ImGui::Separator();

            ImGui::Text("allows users to open app");
            ImGui::Checkbox("app enabled", &Settings::enabled);
            ImGui::Text("lock user to their pc");
            ImGui::Checkbox("hwid-lock", &Settings::hwidlock);
            ImGui::Separator();

            ImGui::Text("version");
            ImGui::InputText("##version", Settings::version, array_size(Settings::version));
            ImGui::Text("download url (doesn't work mak is lazy asf and won't fix)");
            ImGui::InputText("##download", Settings::download, array_size(Settings::download));
            ImGui::Text("webhook");
            ImGui::InputText("##webhook", Settings::webhook, array_size(Settings::webhook));
            ImGui::Text("reseller store");
            ImGui::InputText("##resellerstore", Settings::resellerstore, array_size(Settings::resellerstore));
            ImGui::Text("disabled message");
            ImGui::InputText("##disabledmsg", Settings::disabledmsg, array_size(Settings::disabledmsg));
            ImGui::Text("username taken");
            ImGui::InputText("##usernametaken",  Settings::usernametaken, array_size(Settings::usernametaken));
            ImGui::Text("license invalid");
            ImGui::InputText("##licensetaken",  Settings::licenseinvalid, array_size(Settings::licenseinvalid));
            ImGui::Text("password invalid");
            ImGui::InputText("##passwordinvalid",  Settings::passinvalidmsg, array_size(Settings::passinvalidmsg));
            ImGui::Text("hwid mismatch");
            ImGui::InputText("##hwidmismatch",  Settings::hwidmismatchmsg, array_size(Settings::hwidmismatchmsg));
            ImGui::Text("no active subs msg");
            ImGui::InputText("##noactivesubs",  Settings::noactivesubmsg, array_size(Settings::noactivesubmsg));
            ImGui::Text("blacked message");
            ImGui::InputText("##blackedmsg", Settings::blackedmsg, array_size(Settings::blackedmsg));
            ImGui::Text("key expired msg");
            ImGui::InputText("##expiredmsg",  Settings::expiredmsg, array_size(Settings::expiredmsg));
            ImGui::Separator();
            ImGui::Text("sellix secret");
            ImGui::InputText("##sellixsecret",  Settings::sellixsecret, array_size(Settings::sellixsecret));
            ImGui::Text("day reseller product id");
            ImGui::InputText("##dayresellerproductid",  Settings::dayresellerproductid, array_size(Settings::dayresellerproductid));
            ImGui::Text("week reseller product id");
            ImGui::InputText("##weekresellerproductid",  Settings::weekresellerproductid, array_size(Settings::weekresellerproductid));
            ImGui::Text("month reseller product id");
            ImGui::InputText("##monthresellerproductid",  Settings::monthresellerproductid, array_size(Settings::monthresellerproductid));
            ImGui::Text("life reseller product id");
            ImGui::InputText("##liferesellerproductid",  Settings::liferesellerproductid, array_size(Settings::liferesellerproductid));
            ImGui::Separator();

            if (ImGui::Button("save settings")) Settings::UpdateSettings();
        }
        ImGui::SameLine();
	}
}