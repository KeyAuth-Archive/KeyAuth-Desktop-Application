#pragma once

namespace config {

    //all declared as inline to prevent the function being called
    //and as a result filling up more memory. instead we traverse the function instead of calling it because the function will be 
    //used 1000's of times and we want best performance

    class file
    {
    public:
        //allow to write any type of data to the file
        template<typename type>
        inline void write(std::string filename, type data) const
        {
            std::basic_fstream<char, std::char_traits<char>> config;
            
            //open file with trunc to delete its current contents
            config.open(filename, std::ofstream::out | std::ofstream::trunc);
            config << data;
            config.close();
        }

        inline void del(std::string filename, 
            nlohmann::json json, nlohmann::json app) const
        {
            auto iter = json["appinfo"].begin();
            for (; iter != json["appinfo"].end(); ) {

                if (iter.value()["name"] == app["name"] 
                    && iter.value()["key"] == app["key"]) {
                    json["appinfo"].erase(iter);
                }
                else {
                    ++iter;
                }
            }

            write<std::string>("config.json", json.dump());
        }

        inline void add(std::string filename, nlohmann::json json,
            std::string seller, std::string name)
        {
            if (json.is_null())
                json = { { "appinfo", {{{"key", seller},{"name", name}}}} };
            else
                json["appinfo"].push_back({ {"key", seller}, {"name", name} });

            write<std::string>("config.json", json.dump());
        }
    };

    class parser : public file
    {
    private:
        nlohmann::json response_decoder;
    public:
        //read-only 
        inline nlohmann::basic_json<> getjson(std::string filename) const
        {
            std::ifstream in(filename);

            std::string contents((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());

            return response_decoder.parse(contents, nullptr, false);
        }
    };
}