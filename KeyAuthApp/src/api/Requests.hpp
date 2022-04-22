#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>  
#include <chrono>
#include <thread>

namespace Requests
{
    std::string GetRequest(std::string url, bool showmsg);

    void SetSellerKey(std::string key);

    class Response
    {
    public:
        //function declarations
        bool isRateLimited() const;
        bool PopupIsOpen() const;
        std::string getReqStatus() const;
        bool RequestSuccess() const;
        void setPopupStatus(bool status);
    };
    extern Response response;
}