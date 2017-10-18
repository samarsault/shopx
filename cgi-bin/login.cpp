/*
* Authenticate a User / Create one
*/

#include <iostream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include "json.hpp"
#include "data.hpp"

using namespace std;
using namespace cgicc;
using json = nlohmann::json;

int main(int argc, char **argv)
{
	json resp;
	resp["success"] = false;
	try {
		UserDB users;
		Cgicc cgi;
		cout << HTTPContentHeader("application/json"); 
		CgiEnvironment env = cgi.getEnvironment();
		cout << env.getPostData();
		json j = json::parse(env.getPostData());
		
		// not enough arguments
		auto reg = j.find("register");
		auto emailPtr = j.find("email");
		auto passPtr = j.find("pass");
		
		if (emailPtr== j.end() || passPtr == j.end() || reg == j.end())
			return 1;
		
		string email = *emailPtr, pass =*passPtr;
		bool isRegister = j["register"];
		if (!isRegister) {
			// authenticate user
			if (users.auth(email, pass)) 
				resp["success"] = true;
		} else {
			auto type = j.find("type");
			if (type != j.end()) {
				int t = j["type"];
				if(users.createUser(email, pass, t))
					resp["success"] = true;
			}
		}
	}
	catch(exception& e) {
		// handle any errors - omitted for brevity
		cout << e.what() << endl;
	}
	cout << resp.dump();
}
