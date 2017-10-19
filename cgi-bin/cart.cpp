/*
* Manipulate Cart Items
* Method: POST JSON Data
* Parameters:
*		action: add, remove, update, get
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
	try 
	{
		Cgicc cgi;
		CgiEnvironment env = cgi.getEnvironment();
		json j = json::parse(env.getPostData());
		if (j.find("method") != j.end()) 
		{
			string method = j["method"];
			int user_id = j["user_id"];
			Cart cart(user_id);
			int product_id = j["product_id"];
			
			if (method == "add") {
				int count = j["count"];

				if (cart.add(product_id, count))
					resp["success"] = true;
			}
			else if (method == "remove") {
				if(cart.remove(product_id))
					resp["success"] = true;
			} 
			else if (method == "update") {
				int count = j["count"];
				if (cart.updateCount(product_id, count))
					resp["success"] = true;
			} 
			else if (method == "get") {
				vector<pii> V = cart.getCart();
				if (V.empty()) 
					return 1;

				json itemArr;
				for (int i = 0;i < V.size();i++) {
					itemArr.push_back({
						{ "product_id", V[i].first },
						{ "count", V[i].second }
					});
				}
				resp["items"] = itemArr;
				resp["success"] = true;
			}
		}
	}
	
	catch(exception& e) {
		resp["error"] = e.what();
	}
	cout << resp.dump();
}

