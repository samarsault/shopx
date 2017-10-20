/*
* Place and Manage Orders
* Method: POST JSON Data
* Parameters:
*		action: add, remove, update
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
	cout << HTTPContentHeader("application/json"); 
	json resp;
	resp["success"] = false;
	try {
		ItemsDB users;
		Cgicc cgi;
		CgiEnvironment env = cgi.getEnvironment();
		json j = json::parse(env.getPostData());
		
	}
	catch(exception& e) {
		resp["error"] = e.what();
	}
	cout << resp.dump();
}


