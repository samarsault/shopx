/*
* Authenticate a User or Create one
* Method: POST JSON Data
* Parameters:
*		email: Email ID ( string )
*		pass : password ( string )
*		register: Whether to register (bool)
*       type : (int) if Register = true
*
* Response Params:
*	success: (bool)
*	error: (error message if any)
*	user: (user object)
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
	cout << HTTPContentHeader("application/json") ; 
	json resp;
	resp["success"] = false;
	try {
		UserDB users;
		Cgicc cgi;
		CgiEnvironment env = cgi.getEnvironment();
		json j = json::parse(env.getPostData());
		
		// not enough arguments
		auto reg = j.find("register");
		auto emailPtr = j.find("email");
		auto passPtr = j.find("pass");
		
		if (emailPtr== j.end() || passPtr == j.end() || reg == j.end())
			return 1;
		
		string email = *emailPtr, pass =*passPtr;
		// whether its a registration request
		bool isRegister = j["register"];

		if (!isRegister) {
			// authenticate user
			if (users.auth(email, pass))  {
				resp["success"] = true;
				User u = users.getUser(email);
				resp["user"] = {
					{ "email", u.email },
					{ "type", u.type },
					{ "_id", u._id },
					{ "address", u.address }
				};
			}
		} else {
			auto type = j.find("type");
			if (type != j.end()) {
				int t = j["type"];
				if(users.createUser(email, pass, t)) {
					resp["success"] = true;
					User u = users.getUser(email);
					resp["user"] = {
						{ "email", u.email },
						{ "type", u.type },
						{ "_id", u._id },
						{ "address", u.address }
					};
				}
			}
		}
	}
	catch(exception& e) {
		resp["error"] = e.what();
	}
	cout << resp.dump();
}
