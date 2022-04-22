#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>  
#include <chrono>
#include <thread>
#include <api/Requests.hpp>

std::string SellerKey{};

struct Response_Info
{
    std::string current_request_status{};
    bool ratelimited = false;
    bool popup_open = false;
    bool request_success = true;
} r_info;

bool Requests::Response::isRateLimited()        const { return r_info.ratelimited; }
bool Requests::Response::PopupIsOpen()          const { return r_info.popup_open; }
std::string Requests::Response::getReqStatus()  const { return r_info.current_request_status; }
bool Requests::Response::RequestSuccess()       const { return r_info.request_success; }
void Requests::Response::setPopupStatus(bool status) { r_info.popup_open = status; }

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Requests::SetSellerKey(std::string key)
{
    SellerKey = key;
}

std::string Requests::GetRequest(std::string url, bool showmsg)
{
    std::string finalurl = "https://keyauth.win/api/seller/""?sellerkey=" + SellerKey + url;

    nlohmann::json response_decoder;
    std::string response_stream;

    CURL* curl = curl_easy_init();

    bool running = true;

    if (curl) {
        while (running)
        {
            response_stream.clear();
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl, CURLOPT_URL, finalurl.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_stream);
            curl_easy_perform(curl);

            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (http_code == 429)
            {
                r_info.ratelimited = true;
                r_info.popup_open = true;
                r_info.current_request_status = "client was rate limited. waiting to be unblacklisted...";
                r_info.request_success = false;
            }
            else
            {
                r_info.ratelimited = false;
                running = false;
            }
        }
    }
    curl_easy_cleanup(curl);

    nlohmann::basic_json<> json = response_decoder.parse(response_stream);

    if (showmsg)
    {
        r_info.popup_open = true;
        r_info.current_request_status = std::string(json["message"]);
        r_info.request_success = json["success"];
    }
    else if (json["success"] == false)
    {
        r_info.popup_open = true;
        r_info.current_request_status = std::string(json["message"]);
        r_info.request_success = false;
    }

    return response_stream;
}
