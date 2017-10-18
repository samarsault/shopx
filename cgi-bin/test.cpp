#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

int main(void)
{
    json j = json::parse("{\"email\":\"sadfdf\",\"pass\":\"asfd\"}");
    auto j2 = R"(
        {
          "happy": true,
          "name": "samar",
          "pi": 3.141
        }
      )"_json;
      std::string s = j2["name"];
    std::cout << s << std::endl;
}