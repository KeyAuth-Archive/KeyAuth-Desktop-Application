#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::User
{
    static int identifier_mutator{ rand() };

    struct CreateUser { char username[20], password[20], expiry[20], subscription[20]; }c_user;

	inline void __stdcall RenderUsers(void)
	{
        Center_Text("Current Users");

        ImGui::Separator();

        if (ImGui::Button("Create User")) ImGui::OpenPopup("c_user"); if (ImGui::BeginPopup("c_user"))   
        {
            Center_Text("create user");
            ImGui::Separator();

            ImGui::Text("username");
            ImGui::InputText("##create_username", c_user.username, array_size(c_user.username));
            ImGui::Text("password");
            ImGui::InputText("##create_password", c_user.password, array_size(c_user.password));
            ImGui::Text("key subscription (enter name)");
            if (ImGui::BeginChild("##sublevels", ImVec2(ImGui::GetContentRegionAvail().x, 45), false))
            {
                for (Subs::subs_struct subscription : Subs::v_subs)
                {
                    ImGui::Text("%s", subscription.name.c_str());
                }
                ImGui::EndChild();
            }
            ImGui::InputText("##create_subscription", c_user.subscription, array_size(c_user.subscription));
            ImGui::Text("expiry (in days)");
            ImGui::InputText("##create_expiry", c_user.expiry, array_size(c_user.expiry));

            if (ImGui::Button("create")) Users::CreateUser(c_user.subscription, c_user.username, c_user.password, c_user.expiry);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::PopID();
        ImGui::SameLine();

        if (ImGui::Button("Banned Users")) ImGui::OpenPopup("advancedinfo"); if (ImGui::BeginPopup("advancedinfo"))
        {
            Center_Text("more user information");
            ImGui::Separator();

            ImGui::Text("Banned Users");

            if (ImGui::BeginChild("##banneduserinfo", ImVec2(300, 370), true))
            {
                if (Users::userbannedinfo.size() == NULL) Center_Text("no banned users...");
                else
                {
                    for (Users::userbaninfo info : Users::userbannedinfo)
                    {
                        ImGui::TextWrapped("username: %s\nReason:\n%s",
                            info.user == "null" ? "not yet set!" : info.user.c_str(), info.banreason.empty() ? "No Ban Reason" : info.banreason.c_str());

                        ImGui::PushID(identifier_mutator * 4 + identifier_mutator);

                        if (ImGui::Button("unban", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                        {
                            Users::UnBanUser(info.user);
                        }
                        ImGui::Separator();
                    }
                }
                ImGui::EndChild();
            }

            if (ImGui::Button("close"))  ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Extend User(s)")) ImGui::OpenPopup("extend"); if (ImGui::BeginPopup("extend"))
        {
            Center_Text("add time to user");
            ImGui::Separator();

            ImGui::Text("username:");
            ImGui::InputText("##timeusername", Users::SelectedUsername, array_size(Users::SelectedUsername));
            ImGui::Text("time amount (days):");
            ImGui::InputText("##timeamount", Users::ExtendTime, array_size(Users::ExtendTime));

            ImGui::Text("key subscription (enter name)");
            if (ImGui::BeginChild("##sublevels", ImVec2(ImGui::GetContentRegionAvail().x, 45), false))
            {
                for (Subs::subs_struct subscription : Subs::v_subs) ImGui::Text(subscription.name.c_str());
                ImGui::EndChild();
            }
            ImGui::InputText("##create_subscription", Users::ExtensionSubscription, array_size(Users::ExtensionSubscription));

            if (ImGui::Button("add time")) Users::ExtendUser(Users::SelectedUsername, Users::ExtensionSubscription, Users::ExtendTime);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Expired Users")) Users::DeleteExpired();
        ImGui::SameLine();

        if (ImGui::Button("Remove Sub from User")) ImGui::OpenPopup("remsub"); if (ImGui::BeginPopup("remsub"))
        {
            Center_Text("remove a subscription");
            ImGui::Separator();

            ImGui::Text("all current subscriptions");
            if (ImGui::BeginChild("##sublevels", ImVec2(ImGui::GetContentRegionAvail().x, 45), false))
            {
                for (Subs::subs_struct subscription : Subs::v_subs) ImGui::Text(subscription.name.c_str());
                ImGui::EndChild();
            }

            ImGui::Separator();
            ImGui::Text("enter subscription (as string)");
            ImGui::InputText("##editusersub", Users::SubscriptionRemove, array_size(Users::SubscriptionRemove));

            ImGui::Separator();
            ImGui::Text("username");
            ImGui::InputText("##userforsubchange", Users::SelectedUsername, array_size(Users::SelectedUsername));

            if (ImGui::Button("remove")) Users::DeleteSubscription(Users::SelectedUsername, Users::SubscriptionRemove);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Get User Variables")) ImGui::OpenPopup("uservars"); if (ImGui::BeginPopup("uservars"))
        {
            Center_Text("all user variables");
            ImGui::Separator();

            if (ImGui::Button("refresh list", ImVec2(ImGui::GetContentRegionAvail().x, 23))) Users::GetAllUserVariables();

            ImGui::Separator();

            if (ImGui::BeginChild("##allvariables", ImVec2(ImGui::GetContentRegionAvail().x, 350), false))
            {
                for (Users::var user : Users::uservariables)
                {
                    ImGui::Text("username: %s\nvarname: %s\nvardata: %s", user.user.c_str(), user.varname.c_str(), user.vardata.c_str());
                    ImGui::Separator();

                    ImGui::PushID(identifier_mutator * 5 + identifier_mutator);
                    if (ImGui::Button("edit value (down below)", ImVec2(ImGui::GetContentRegionAvail().x, 23)))
                    {
                        Users::EditingVarUsername = user.user;
                        Users::EditingVarName = user.varname;
                    }
                }
                ImGui::EndChild();
            }

            ImGui::Separator();

            ImGui::Text("Now Editing:\n%s\n%s", Users::EditingVarUsername, Users::EditingVarName);
            ImGui::InputText("##editvariable", Users::NewVariableValue, array_size(Users::NewVariableValue));

            if (ImGui::Button("confirm")) Users::SetUsersVariable(Users::EditingVarUsername, Users::EditingVarName, Users::NewVariableValue);
            ImGui::SameLine();
            if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset All HWID's")) Users::ResetAllHWID();

        if (ImGui::BeginChild("##usersection", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true))
        {
            ImGui::Text("banned ="); ImGui::SameLine(); ImGui::TextColored(ImColor(255, 0, 0), "O"); ImGui::SameLine();
            ImGui::Text("not banned ="); ImGui::SameLine(); ImGui::TextColored(ImColor(0, 255, 0), "O");

            ImGuiTableFlags flags2 = ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
            if (ImGui::BeginTable("table_context_menu_2", 4, flags2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 20)))
            {
                ImGui::TableSetupColumn("Load", 0, 0.23f); ImGui::TableSetupColumn(" ", 0, 0.07f); ImGui::TableSetupColumn("Username"); ImGui::TableSetupColumn("IP");

                ImGui::TableHeadersRow();
                for (int CurrentRow = 0; CurrentRow < Users::info.size(); CurrentRow++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushID(identifier_mutator * 4 + identifier_mutator);
                    if (ImGui::Button("[+]")) Users::GetUsernameInformation(Users::info[CurrentRow].username);
                    ImGui::TableSetColumnIndex(1);

                    if (Users::info[CurrentRow].isBanned) ImGui::TextColored(ImColor(255, 0, 0), "O");
                    else ImGui::TextColored(ImColor(0, 255, 0), "O");

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(Users::info[CurrentRow].username.c_str());
                    ImGui::TableSetColumnIndex(3);
                    if (Users::info[CurrentRow].ip != "null")
                    {
                        if (ImGui::Button("copy")) CopytoClipboard(0, Users::info[CurrentRow].ip);
                        ImGui::SameLine();
                        ImGui::Text(Users::info[CurrentRow].ip.c_str());
                    }
                    else ImGui::Text("ip not set"); 

                    ImGui::SameLine();
                }
                ImGui::EndTable();
                ImGui::Text("Amount of Users: %i, Amount Banned %i", Users::info.size(), Users::amount_banned);
            }
            ImGui::TreePop();
            ImGui::EndChild();
        }
        ImGui::SameLine();

        if (ImGui::BeginChild("##UserInformation", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true))
        {
            ImGui::Text("Further User Details");
            ImGui::Separator();

            if (ImGui::Button("Get User Keys"))
            {
                for (Licenses::userinfo user_to_license : Licenses::UserInfo)
                {
                    if (user_to_license.user != "null" && Users::SelectedUsername == user_to_license.user)
                    {
                        CurrentPanel = 0;
                        Licenses::GetLicenseInformation(user_to_license.license);
                    }
                }
            }
            ImGui::SameLine();

            if (ImGui::Button("Reset HWID")) Users::ResetHWID(Users::SelectedUsername);
            ImGui::SameLine();

            if (ImGui::Button("Ban User")) ImGui::OpenPopup("banuser"); if (ImGui::BeginPopup("banuser"))
            {
                Center_Text("ban user");
                ImGui::Separator();

                ImGui::Text("username:");
                ImGui::InputText("##userbanname", Users::SelectedUsername, array_size(Users::SelectedUsername));
                ImGui::Text("ban reason:");
                ImGui::InputText("##userbanreason", Users::BanUserReason, array_size(Users::BanUserReason));

                if (ImGui::Button("ban user")) Users::BanUser(Users::SelectedUsername, Users::BanUserReason);
                ImGui::SameLine();
                if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::PopID();
            ImGui::SameLine();

            if (ImGui::Button("Unban User")) ImGui::OpenPopup("unbanuser"); if (ImGui::BeginPopup("unbanuser"))
            {
                Center_Text("unban user");
                ImGui::Separator();

                ImGui::Text("username:");
                ImGui::InputText("##unban_username", Users::SelectedUsername, array_size(Users::SelectedUsername));

                if (ImGui::Button("unban")) Users::UnBanUser(Users::SelectedUsername);
                ImGui::SameLine();
                if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::SameLine();

            if (ImGui::Button("Delete User")) ImGui::OpenPopup("deleteuser"); if (ImGui::BeginPopup("deleteuser"))
            {
                Center_Text("delete user");
                ImGui::Separator();

                ImGui::Text("username:");
                ImGui::InputText("##unban_username", Users::SelectedUsername, array_size(Users::SelectedUsername));

                if (ImGui::Button("delete")) Users::DeleteUser(Users::SelectedUsername);
                ImGui::SameLine();
                if (ImGui::Button("cancel")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::Text("Username:\n%s", Users::user_info.username.c_str());
            ImGui::Text("Password:\n%s", Users::user_info.password.c_str());

            ImGui::Separator();

            ImGui::Text("change pw (empty = let user change)");

            ImGui::InputText("##changepassword", Users::ChangePwd, array_size(Users::ChangePwd));

            if (ImGui::Button("change")) Users::ChangePassword(Users::SelectedUsername, Users::ChangePwd);

            ImGui::Separator();

            ImGui::Text("HWID:\n%s", Users::user_info.hwid == "null" ? "user never logged on and HWID isn't set!" : Users::user_info.hwid.c_str());
            if (ImGui::Button("copy current hwid"))
            {
                CopytoClipboard(0, Users::user_info.hwid);
            }

            ImGui::Text("add extra hwid");
            ImGui::InputText("##addhwid", Users::AddHWIDText, array_size(Users::AddHWIDText));
            if (ImGui::Button("Add HWID")) Users::AddAditionalHWID(Users::SelectedUsername, Users::AddHWIDText);
            ImGui::Separator();

            std::string user_subscription = "no active subscriptions...";
            std::string username = Users::user_info.username;

            for (Licenses::userinfo user_to_license : Licenses::UserInfo)
            {
                if (Users::SelectedUsername == user_to_license.user)
                {
                    if (Licenses::currentobject["success"])
                    {
                        for (nlohmann::basic_json<> key : Licenses::currentobject["keys"])
                        {
                            if (key["key"] == user_to_license.license)
                            {
                                for (Subs::subs_struct subscription : Subs::v_subs)
                                {
                                    if (subscription.level == key["level"])
                                    {
                                        user_subscription = std::string(key["level"]) + " -> " + subscription.name;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            ImGui::Text("Users Subscription:\n%s", user_subscription.c_str());
            ImGui::Text("Owner:\n%s", Users::user_info.owner.c_str());
            ImGui::Text("Create Date:\n%s", Users::user_info.createdate.c_str());
            ImGui::Text("Last Login:\n%s", Users::user_info.lastlogin.c_str());
            ImGui::Text("Banned:\n%s", Users::user_info.banned == "null" ? "not banned" : Users::user_info.banned.c_str());
            ImGui::Text("IP:\n%s", Users::user_info.ip.c_str());
            ImGui::Text("Cooldown:\n%s", Users::user_info.cooldown.c_str());

            ImGui::EndChild();
        }
	}
}