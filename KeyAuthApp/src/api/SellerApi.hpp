#include <regex>
#include <chrono>
#include <iostream>
#include <api/Requests.hpp>

std::string tm_to_readable_time(tm ctx) { char buffer[80]; strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx); return std::string(buffer); }
static std::time_t string_to_timet(std::string timestamp) { auto cv = strtol(timestamp.c_str(), NULL, 10); return (time_t)cv; }
static std::tm timet_to_tm(time_t timestamp) { std::tm context; localtime_s(&context, &timestamp); return context; }

void CopytoClipboard(HWND hwnd, const std::string& s) {
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

namespace Settings
{
    nlohmann::json requests_decoder;

    bool enabled, hwidlock;
    char version[255], download[255], webhook[255], resellerstore[255],
        disabledmsg[255], usernametaken[255], licenseinvalid[255], keytakenmsg[255], nosubmsg[255], userinvalidmsg[255], passinvalidmsg[255],
        hwidmismatchmsg[255], noactivesubmsg[255], blackedmsg[255], pausedmsg[255], expiredmsg[255],

        sellixsecret[255], dayresellerproductid[255], weekresellerproductid[255], monthresellerproductid[255], liferesellerproductid[255];

    void GetAllSettings(void)
    {
        nlohmann::basic_json<> json = requests_decoder.parse(Requests::GetRequest({ "&type=getsettings" }, false));

        if (json["success"] == true)
        {
            enabled = json["enabled"];
            hwidlock = json["hwid-lock"];

            //best way to convert string to char array without loosing allocated size
            auto pushback = [&](char* addr, std::string object)
            {
                int i = 0;
                //can't properly get sizeof a char pointer so i have to initialize all inputs with array len of 255 so it works properly
                memset(addr, NULL, (int)255);
                for (char& c : std::string(json[object])) addr[i++] += c;
            };

            pushback(version, "version");
            pushback(download, "download");
            pushback(webhook, "webhook");
            pushback(resellerstore, "resellerstore");
            pushback(disabledmsg, "disabledmsg");
            pushback(usernametaken, "usernametakenmsg");
            pushback(licenseinvalid, "licenseinvalidmsg");
            pushback(keytakenmsg, "keytakenmsg");
            pushback(nosubmsg, "nosubmsg");
            pushback(userinvalidmsg, "userinvalidmsg");
            pushback(passinvalidmsg, "passinvalidmsg");
            pushback(hwidmismatchmsg, "hwidmismatchmsg");
            pushback(noactivesubmsg, "noactivesubmsg");
            pushback(blackedmsg, "blackedmsg");
            pushback(expiredmsg, "expiredmsg");
            pushback(dayresellerproductid, "dayresellerproductid");
            pushback(weekresellerproductid, "weekresellerproductid");
            pushback(monthresellerproductid, "monthresellerproductid");
            pushback(liferesellerproductid, "liferesellerproductid");
        }
    }

    void UpdateSettings(void)
    {
        std::string is_enabled = enabled ? "true" : "false";
        std::string is_hwidlock = hwidlock ? "true" : "false";

        //take every input and replace every space with "%20" to properly use in a uri
        auto parse = [&](char* addr)
        {
            return std::regex_replace(addr, std::regex(" "), "%20");
        };

        Requests::GetRequest({
            "&type="      "updatesettings"
            "&enabled=" + is_enabled +
            "&hwidcheck=" + is_hwidlock +

            "&ver=" + parse(version) +
            "&download=" + parse(download) +
            "&webhook=" + parse(webhook) +
            "&resellerstore=" + parse(resellerstore) +
            "&appdisabled=" + parse(disabledmsg) +
            "&usernametaken=" + parse(usernametaken) +
            "&keynotfound=" + parse(licenseinvalid) +
            "&keyused=" + parse(keytakenmsg) +
            "&nosublevel=" + parse(nosubmsg) +
            "&usernamenotfound=" + parse(userinvalidmsg) +
            "&passmismatch=" + parse(passinvalidmsg) +
            "&hwidmismatch=" + parse(hwidmismatchmsg) +
            "&noactivesubs=" + parse(noactivesubmsg) +
            "&hwidblacked=" + parse(blackedmsg) +
            "&keyexpired=" + parse(expiredmsg) +
            "&sellixsecret=" + parse(sellixsecret) +
            "&dayproduct=" + parse(dayresellerproductid) +
            "&weekrproduct=" + parse(weekresellerproductid) +
            "&monthproduct=" + parse(monthresellerproductid) +
            "&lifeproduct=" + parse(liferesellerproductid)
            }, true);

        GetAllSettings();
    }

    void ResetHash(void)
    {
        Requests::GetRequest({ "&type=""resethash" }, true);
    }

    void AddHash(std::string hash)
    {
        Requests::GetRequest({ "&type=""addhash""&hash=" + std::regex_replace(hash, std::regex(" "), "%20") }, true);
    }

    void PauseApp(void)
    {
        Requests::GetRequest({ "&type=""pauseapp" }, true);
    }

    void UnPauseApp(void)
    {
        Requests::GetRequest({ "&type=""unpauseapp" }, true);
    }
}

namespace Blacklists
{
    nlohmann::json requests_decoder;

    std::vector<std::string> ips;
    std::vector<std::string> hwid;

    void GetAllBlacklists(void)
    {
        hwid.clear(); ips.clear();

        auto json = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallblacks" }, false));

        if (json["success"])
        {
            for (nlohmann::basic_json<> black : json["blacklists"])
            {
                if (!black["hwid"].is_null()) hwid.push_back(black["hwid"]);
                else if (!black["ip"].is_null())   ips.push_back(black["ip"]);
            }
        }
    }

    void DeleteAllBlack(void)
    {
        Requests::GetRequest({ "&type=""delblacks" }, true);
        GetAllBlacklists();
    }

    void AddBlack(std::string ip, std::string hwid)
    {
        Requests::GetRequest({
            "&type=" "black"
            "&ip=" + std::regex_replace(ip, std::regex(" "), "%20") +
            "&hwid=" + std::regex_replace(hwid, std::regex(" "), "%20")
            }, true);
        GetAllBlacklists();
    }

    void DeleteBlackList(std::string data, bool ishwid)
    {
        Requests::GetRequest({
            "&type=" "delblack"
            "&data=" + std::regex_replace(data, std::regex(" "), "%20") +
            "&blacktype=" + std::regex_replace(ishwid ? "hwid" : "ip", std::regex(" "), "%20")
            }, true);
        GetAllBlacklists();
    }
}

namespace Variables
{
    nlohmann::json requests_decoder;

    struct var_struct
    {
        std::string varid;
        std::string msg;
    };

    std::vector<var_struct> v_vars;

    void GetAllVars(void)
    {
        v_vars.clear();
        auto json = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallvars" }, false));

        if (json["success"])
        {
            for (nlohmann::basic_json<> variable : json["vars"])
                v_vars.push_back(var_struct{ variable["varid"], variable["msg"] });
        }
    }

    void CreateVariable(std::string varname, std::string vardata, bool authed)
    {
        Requests::GetRequest({
            "&type=" "addvar"
            "&name=" + std::regex_replace(varname, std::regex(" "), "%20") +
            "&data=" + std::regex_replace(vardata, std::regex(" "), "%20") +
            "&authed=" + std::to_string(authed)
            }, true);
        GetAllVars();
    }

    void EditVariable(std::string varid, std::string data)
    {
        Requests::GetRequest({
            "&type=" "editvar"
            "&varid=" + std::regex_replace(varid, std::regex(" "), "%20") +
            "&data=" + std::regex_replace(data, std::regex(" "), "%20")
            }, true);
        GetAllVars();
    }

    void DeleteVariable(std::string varid)
    {
        Requests::GetRequest({
            "&type=" "delvar"
            "&name=" + std::regex_replace(varid, std::regex(" "), "%20")
            }, true);
        GetAllVars();
    }

    void DeleteAllVariables(void)
    {
        Requests::GetRequest({ "&type=""delallvars" }, true);
        GetAllVars();
    }
}

namespace Webhooks
{
    void CreateWebhook(std::string url, std::string useragent, bool authed)
    {
        Requests::GetRequest({
            "&type=" "addwebhook"
            "&baseurl=" + std::regex_replace(url, std::regex(" "), "%20") +
            "&ua=" + std::regex_replace(useragent, std::regex(" "), "%20") +
            "&authed=" + std::to_string(authed)
            }, true);
    }
}

namespace Files
{
    nlohmann::json requests_decoder;

    struct file_struct
    {
        std::string id;
        std::string url;
    };

    std::vector<file_struct> v_files;

    void GetAllFiles(void)
    {
        v_files.clear();
        auto json = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallfiles" }, false));

        if (json["success"])
        {
            for (nlohmann::basic_json<> file : json["files"])
            {
                v_files.push_back(file_struct{ file["id"], file["url"] });
            }
        }
    }

    void UploadFile(std::string url)
    {
        Requests::GetRequest({
            "&type=" "upload"
            "&url=" + std::regex_replace(url, std::regex(" "), "%20")
            }, true);
        GetAllFiles();
    }

    void DeleteFiles(std::string fileid)
    {
        Requests::GetRequest({
            "&type="      "delfile"
            "&fileid=" + std::regex_replace(fileid, std::regex(" "), "%20")
            }, true);
        GetAllFiles();
    }

    void DeleteAllFiles()
    {
        if (MessageBoxA(0, "are you sure you want to delete all files?", "delete-files", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({ "&type=""delallfiles" }, true);
            GetAllFiles();
        }
    }
}

namespace Chat
{
    nlohmann::json requests_decoder;

    struct channel_struct
    {
        std::string name;
        std::string delay;
    };

    struct muted_struct
    {
        std::string user;
        std::tm time;
    };

    std::vector<muted_struct> v_muted;
    std::vector<channel_struct> v_channel;

    void GetAllChannels(void)
    {
        v_channel.clear();
        auto json = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallchats" }, false));

        if (json["success"] == true)
        {
            for (nlohmann::basic_json<> channel : json["chats"])
            {
                v_channel.push_back(channel_struct{ channel["name"], channel["delay"] });
            }
        }
    }

    void GetAllMutedUsers(void)
    {
        v_muted.clear();
        auto json = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallmutes" }, false));

        if (json["success"] == true)
        {
            for (nlohmann::basic_json<> channel : json["mutes"])
            {
                v_muted.push_back(muted_struct{ channel["user"], timet_to_tm(string_to_timet(channel["time"])) });
            }
        }
    }

    void MuteUser(std::string username, std::string length)
    {
        Requests::GetRequest({
            "&type="      "muteuser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&time=" + length
            }, true);
        GetAllMutedUsers();
    }

    void UnmuteUser(std::string username)
    {
        Requests::GetRequest({
            "&type=" "unmuteuser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20")
            }, true);
        GetAllMutedUsers();
    }

    void EditChannel(std::string channelname, std::string delay)
    {
        Requests::GetRequest({
            "&type=" "editchan"
            "&name=" + std::regex_replace(channelname, std::regex(" "), "%20") +
            "&delay=" + std::to_string(std::stof(delay) / 60)
            }, true);
        GetAllChannels();
    }

    void CreateChannel(std::string channelname, std::string delay)
    {
        Requests::GetRequest({
            "&type=" "addchannel"
            "&name=" + std::regex_replace(channelname, std::regex(" "), "%20") +
            "&delay=" + std::regex_replace(delay, std::regex(" "), "%20")
            }, true);

        GetAllChannels();
    }

    void DeleteChannel(std::string channelname)
    {
        Requests::GetRequest({
            "&type=" "delchannel"
            "&name=" + std::regex_replace(channelname, std::regex(" "), "%20")
            }, true);
        GetAllChannels();
    }

    void ClearChannel(std::string channelname)
    {
        Requests::GetRequest({
            "&type=" "clearchannel"
            "&name=" + std::regex_replace(channelname, std::regex(" "), "%20")
            }, true);
        GetAllChannels();
    }
}

namespace Licenses
{
    char License[256]{};

    //create grlobal variable of current json object to later refrence without creating new request
    nlohmann::basic_json<> currentobject{};
    nlohmann::json requests_decoder;

    struct FurtherInformation { std::string id, key, note, status, level, genby, usedby, app, banned; tm expires{}, gendate{}, usedon{}; }; FurtherInformation info;

    struct userinfo { std::string user, license; };            std::vector<userinfo> UserInfo;
    struct baninfo { std::string license, user, banreason; }; std::vector<baninfo > bannedinfo;

    std::vector<std::string> BannedKeys{};
    std::vector<std::string> Usernames{};

    int amountbanned = 0;
    int ammountused = 0;

    std::vector<bool> Used{};
    std::vector<bool> Banned{};

    extern void GetAllLicenses()
    {
        Licenses::Used.clear(); Licenses::Banned.clear(); UserInfo.clear();
        currentobject = requests_decoder.parse(Requests::GetRequest({ "&type=""fetchallkeys" }, false));

        if (currentobject["success"])
        {
            amountbanned = 0; ammountused = 0;
            bannedinfo.clear();

            for (nlohmann::basic_json<> key : currentobject["keys"])
            {
                if (!key["banned"].is_null())
                {
                    if (!key["usedby"].is_null()) bannedinfo.push_back(baninfo{ key["key"], key["usedby"], key["banned"] });
                    else                          bannedinfo.push_back(baninfo{ key["key"], key["usedby"].dump(), key["banned"] });

                    Licenses::Banned.push_back(true);
                    amountbanned += 1;
                }
                else Licenses::Banned.push_back(false);

                std::string license = key["key"];
                if (key["status"] == "Used")
                {
                    ammountused += 1;
                    Licenses::Used.push_back(true);
                }
                else Licenses::Used.push_back(false);

                if (!key["usedby"].is_null()) UserInfo.push_back(userinfo{ key["usedby"], key["key"] });
                else                          UserInfo.push_back(userinfo{ key["usedby"].dump(), key["key"] });
            }
        }
    }

    void GetLicenseInformation(std::string license)
    {
        if (currentobject["success"])
        {
            for (nlohmann::basic_json<> key : currentobject["keys"])
            {
                if (key["key"] == license)
                {
                    info.id = key["id"]; info.key = key["key"]; info.note = key["note"].dump();

                    info.expires = timet_to_tm(string_to_timet(key["expires"]));
                    info.gendate = timet_to_tm(string_to_timet(key["gendate"]));

                    if (!key["usedon"].is_null())
                        info.usedon = timet_to_tm(string_to_timet(key["usedon"]));
                    else
                        info.usedon = timet_to_tm(-1);

                    info.status = key["status"].dump();
                    info.level = key["level"];
                    info.genby = key["genby"].dump();

                    info.usedby = key["usedby"].dump();
                    info.app = key["app"].dump();
                    info.banned = key["banned"].dump();

                    //replace allocated c str with string and keep original char size [256]
                    (void*)memset(License, NULL, sizeof License);

                    int i = (int)-1;
                    for (char& Character : Licenses::info.key) License[i += 1] = Character;
                }
            }
        }
    }

    void BanLicense(std::string license, std::string banreason)
    {
        Requests::GetRequest({
            "&type="      "ban"
            "&key=" + std::string(license) +
            "&reason=" + std::regex_replace(banreason, std::regex(" "), "%20")
            }, true);

        GetAllLicenses();
        GetLicenseInformation(License);
    }

    void UnbanLicense(std::string license)
    {
        Requests::GetRequest({
            "&type="      "unban"
            "&key=" + std::regex_replace(license, std::regex(" "), "%20")
            }, true);

        GetAllLicenses();
        GetLicenseInformation(License);
    }

    void DeleteAllLicenses()
    {
        if (MessageBoxA(0, "are you sure you want to delete all licenses?", "delete-licenses", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({
            "&type="      "delalllicenses"
                }, true);

            GetAllLicenses();
        }
    }

    void DeleteAllUnusedLicenses()
    {
        if (MessageBoxA(0, "are you sure you want to delete all unused licenses?", "delete-licenses", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({
            "&type="      "delunused"
                }, true);

            GetAllLicenses();
        }
    }

    void DeleteAllUsedLicenses()
    {
        if (MessageBoxA(0, "are you sure you want to delete all used licenses?", "delete-licenses", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({
            "&type="      "delused"
                }, true);

            GetAllLicenses();
        }
    }

    void DeleteLicense(std::string license)
    {
        if (MessageBoxA(0, "are you sure you want to delete this license?", "delete-licenses", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({
            "&type="      "del"
            "&key=" + std::string(license)
                }, true);

            GetAllLicenses();
        }
    }


    void AddTimeToUnusedLicenses(std::string time)
    {
        Requests::GetRequest({ "&type=""addtime""&time=" + time }, true);
    }

    void GetLicenseFromUsername(std::string license)
    {
        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "getkey"
            "&user=" + std::string(license)
            }, false));

        if (json["success"])
        {
            MessageBoxA(0, std::string("Key: " + json["key"].dump() + " was copied to Clipboard").c_str(), "", 0);
            CopytoClipboard(0, json["key"]);
        }
        else
            MessageBoxA(0, "user doesn't exist or doesn't have a key attached to them", "error", 0);
    }

    void CreateLicense(std::string mask, std::string expiry, std::string level, std::string amount)
    {
        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "add"
            "&expiry=" + std::regex_replace(expiry, std::regex(" "), "%20") +
            "&mask=" + std::regex_replace(mask, std::regex(" "), "%20") +
            "&level=" + std::regex_replace(level, std::regex(" "), "%20") +
            "&amount=" + std::regex_replace(amount, std::regex(" "), "%20")
            }, true));

        if (json["success"])
        {
            MessageBoxA(0, std::string("Key Copied to Clipboard!").c_str(), "Success", 0);

            if (!json["key"].is_null())
                CopytoClipboard(FALSE, std::string(json["key"]));
            else
                CopytoClipboard(FALSE, std::string(json["keys"].dump()));

            GetAllLicenses();
        }
        else
            MessageBoxA(0, std::string(json["message"]).c_str(), "", 0);
    }

    void CreateUserFromLicense(std::string license, std::string username, std::string password)
    {
        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "activate"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&key=" + std::regex_replace(license, std::regex(" "), "%20") +
            "&pass=" + std::regex_replace(password, std::regex(" "), "%20")
            }, true));

        GetAllLicenses();
        GetLicenseInformation(License);
    }

    char NoteInput[256];
    void ChangeNote(std::string note)
    {
        Requests::GetRequest({
            "&type="      "setnote"
            "&key=" + static_cast<std::string>(License) +
            "&note=" + std::regex_replace(note, std::regex(" "), "%20")
            }, true);

        GetAllLicenses();
        GetLicenseInformation(License);
    }
}

namespace Sessions
{
    nlohmann::json requests_decoder;

    struct session_struct
    {
        std::string sessionid;
        std::string username;
        std::string expiry;
        std::string valid;
    };

    std::vector<session_struct> v_sessions{};

    void GetAllSessions(void)
    {
        v_sessions.clear();
        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "fetchallsessions"
            }, false));

        if (json["success"] == true)
        {
            for (nlohmann::basic_json<> session : json["sessions"])
            {

                v_sessions.push_back(session_struct
                    {
                        session["id"], session["credential"].dump(), tm_to_readable_time(timet_to_tm(string_to_timet(session["expiry"]))), session["validated"]
                    });
            }
        }
    }

    void KillAllSessions()
    {
        if (MessageBoxA(0, "are you sure you want to kill all sessions?", "kill-sessions", MB_YESNO) == IDYES)
        {
            auto json = requests_decoder.parse(Requests::GetRequest({
                "&type="      "killall"
                }, true));

            GetAllSessions();
        }
    }

    void KillSession(std::string sessionid)
    {
        auto json = requests_decoder.parse(Requests::GetRequest({
                "&type="      "kill"
                "&sessid=" + sessionid
            }, true));

        GetAllSessions();
    }
}

namespace Subs
{
    nlohmann::json requests_decoder;

    struct subs_struct
    {
        std::string name;
        std::string level;
    };

    std::vector<subs_struct> v_subs{};

    void FetchAllSubs()
    {
        v_subs.clear();

        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "fetchallsubs"
            }, false));

        if (json["success"])
        {
            for (nlohmann::basic_json<> subscription : json["subs"])
            {
                v_subs.push_back(subs_struct{ subscription["name"], subscription["level"] });
            }
        }
    }

    void DeleteSubscription(std::string subname)
    {
        Requests::GetRequest({
            "&type="      "delappsub"
            "&name=" + std::regex_replace(subname, std::regex(" "), "%20")
            }, true);

        FetchAllSubs();
    }

    void CreateSubscription(std::string subname, std::string sublvl)
    {
        Requests::GetRequest({
            "&type="      "addsub"
            "&name=" + std::regex_replace(subname, std::regex(" "), "%20") +
            "&level=" + std::regex_replace(sublvl, std::regex(" "), "%20")
            }, true);

        FetchAllSubs();
    }

    void EditSubscription(std::string subname, std::string newvalue)
    {
        Requests::GetRequest({
            "&type="      "editsub"
            "&sub=" + std::regex_replace(subname, std::regex(" "), "%20") +
            "&level=" + std::regex_replace(newvalue, std::regex(" "), "%20")
            }, true);

        FetchAllSubs();
    }
}

namespace Users
{
    char SelectedUsername[256];
    char ChangePwd[256];
    char AddHWIDText[256];

    char ExtendTime[256];
    char ExtensionSubscription[256];

    char BanUserReason[256];

    std::string EditingVarUsername{};
    std::string EditingVarName{};

    char NewVariableValue[256];

    char SubscriptionRemove[256];

    struct var
    {
        std::string user;
        std::string vardata;
        std::string varname;
    };
    std::vector<var> uservariables;

    struct userbaninfo
    {
        std::string user;
        std::string banreason;
    };
    std::vector<userbaninfo> userbannedinfo;

    class FurtherUserInfo
    {
    public:
        std::string username, password, hwid, app, owner, createdate, lastlogin, banned, ip, cooldown;
    };

    FurtherUserInfo user_info;

    nlohmann::basic_json<> currentuserobject;

    struct usernameinfo
    {
        std::string username;
        std::string ip;
        std::string hwid;
        bool isBanned;
    };

    std::vector<bool> isBanned;
    std::vector<usernameinfo> info{};

    static int amount_banned = 0;

    nlohmann::json requests_decoder;

    void GetAllUsers()
    {
        info.clear();
        Licenses::Usernames.clear();

        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "fetchallusers"
            }, false));

        currentuserobject = json;

        userbannedinfo.clear();
        amount_banned = 0;

        if (json["success"])
        {
            for (nlohmann::basic_json<> user : json["users"])
            {
                if (!user["banned"].is_null())
                {
                    userbannedinfo.push_back(userbaninfo{ user["username"], user["banned"] });
                    amount_banned += 1;
                }

                bool isbanned = false;

                if (!user["banned"].is_null()) isbanned = true;

                std::string ip;
                std::string hwid;

                if (!user["ip"].is_null())
                {
                    ip = user["ip"];
                }
                else ip = "null";

                if (!user["hwid"].is_null())
                {
                    hwid = user["hwid"];
                }
                else hwid = "null";

                info.push_back(usernameinfo{ user["username"], ip, hwid, isbanned });

                Licenses::Usernames.push_back(user["username"]);
            }
        }
    }

    void CreateUser(std::string subscription, std::string username, std::string password, std::string expiry)
    {
        auto json = requests_decoder.parse(Requests::GetRequest({
            "&type="      "adduser"
            "&user=" + username +
            "&sub=" + subscription +
            "&expiry=" + expiry +
            "&pass=" + password
            }, true));

        GetAllUsers();
    }

    void GetUsernameInformation(std::string username)
    {
        if (currentuserobject["success"])
        {
            for (nlohmann::basic_json<> users : currentuserobject["users"])
            {
                if (users["username"] == username)
                {
                    if (!users["banned"].is_null())
                        isBanned.push_back(true);
                    else
                        isBanned.push_back(false);

                    user_info.username = users["username"];

                    if (!users["password"].is_null())
                        user_info.password = users["password"];
                    else
                        user_info.password = "password is waiting to be set by user";

                    user_info.hwid = users["hwid"].dump();
                    user_info.app = users["app"];
                    user_info.owner = users["owner"].dump();
                    user_info.createdate = tm_to_readable_time(timet_to_tm(string_to_timet(users["createdate"])));

                    if (users["lastlogin"].dump() != "null")
                    {
                        user_info.lastlogin = tm_to_readable_time(timet_to_tm(string_to_timet(users["lastlogin"])));
                    }
                    else user_info.lastlogin = "not set";

                    user_info.banned = users["banned"].dump();
                    user_info.ip = users["ip"].dump();
                    user_info.cooldown = users["cooldown"].dump();

                    (void)memset(SelectedUsername, NULL, sizeof SelectedUsername);

                    int i = -1;
                    for (char& Character : user_info.username)
                    {
                        SelectedUsername[i += 1] = Character;
                    }
                }
            }
        }
    }

    void DeleteExpired()
    {
        if (MessageBoxA(0, "are you sure you want to delete all expired users?", "delete-users", MB_YESNO) == IDYES)
        {
            Requests::GetRequest({
            "&type=" "delexpusers"
                }, true);

            GetAllUsers();
            GetUsernameInformation(SelectedUsername);
        }
    }

    void DeleteSubscription(std::string username, std::string subscription)
    {
        nlohmann::json json = requests_decoder.parse(Requests::GetRequest({
            "&type=" "delsub"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&sub=" + std::regex_replace(subscription, std::regex(" "), "%20")
            }, true));

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void GetAllUserVariables()
    {
        uservariables.clear();

        nlohmann::json json = requests_decoder.parse(Requests::GetRequest({
           "&type=" "fetchalluservars"
            }, false));

        if (json["success"])
        {
            for (nlohmann::json result : json["vars"])
            {
                uservariables.push_back(var{ result["user"], result["data"], result["name"] });
            }
        }
    }

    void SetUsersVariable(std::string username, std::string varname, std::string vardata)
    {
        nlohmann::json json = requests_decoder.parse(Requests::GetRequest({
            "&type=" "setvar"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&var=" + std::regex_replace(varname, std::regex(" "), "%20") +
            "&data=" + std::regex_replace(vardata, std::regex(" "), "%20")
            }, true));

        GetAllUserVariables();
    }

    void ResetAllHWID()
    {
        if (MessageBoxA(0, "are you sure you want to reset ALL user's hwid lock?", "reset-hwid", MB_YESNO) == IDYES)
        {
            nlohmann::json json = requests_decoder.parse(Requests::GetRequest({
            "&type=" "resetalluser"
                }, true));

            GetAllUsers();
            GetUsernameInformation(SelectedUsername);
        }
    }

    void ExtendUser(std::string username, std::string subscription, std::string expiry)
    {
        Requests::GetRequest({
            "&type=" "extend"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&sub=" + std::regex_replace(subscription, std::regex(" "), "%20") +
            "&expiry=" + std::regex_replace(expiry, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void BanUser(std::string username, std::string reason)
    {
        Requests::GetRequest({
            "&type=" "banuser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&reason=" + std::regex_replace(reason, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void UnBanUser(std::string username)
    {
        Requests::GetRequest({
            "&type=" "unbanuser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void DeleteUser(std::string username)
    {
        Requests::GetRequest({
            "&type=" "deluser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void ResetHWID(std::string username)
    {
        Requests::GetRequest({
            "&type=" "resetuser"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void ChangePassword(std::string username, std::string password)
    {
        Requests::GetRequest({
            "&type=" "resetpw"
            "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
            "&passwd=" + std::regex_replace(password, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

    void AddAditionalHWID(std::string username, std::string hwid)
    {
        Requests::GetRequest({
             "&type=" "addhwiduser"
             "&user=" + std::regex_replace(username, std::regex(" "), "%20") +
             "&hwid=" + std::regex_replace(hwid, std::regex(" "), "%20")
            }, true);

        GetAllUsers();
        GetUsernameInformation(SelectedUsername);
    }

}