/*
* Place and Manage Orders
* Method: POST JSON Data
* Parameters:
*		action: place, cancel, finish
*		item_id:
*		user_id:
*		count:
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

json getUser(long id) {
		UserDB users;
		User u = users.getUser(id);
		json j;
		j = {
			{ "email", u.email },
			{ "name", u.name },
			{ "_id", u._id },
			{ "address", u.address }
		};
		return j;
}
int main(int argc, char **argv)
{
	cout << HTTPContentHeader("application/json"); 
	json resp;
	resp["success"] = false;
	try {
		OrderDB orders;
		Cgicc cgi;
		CgiEnvironment env = cgi.getEnvironment();
		json j = json::parse(env.getPostData());
		if (j.find("method") != j.end())
		{
			string method = j["method"];
			if (method == "place") {
				if(orders.placeOrder(j["user_id"])) {
					resp["success"] = true;
				}
			} else if(method == "finish") {
				int order_id = j["_id"];
				if(orders.finishOrder(order_id)) {
					resp["success"] = true;
				}
				j["success"] = true;
			} else if (method == "cancel") {
				int order_id = j["_id"];
				if(orders.cancelOrder(order_id)) {
					resp["success"] = true;
				}
			} else if (method == "get") {
				long id = j["user_id"];
				vector <Order> O = orders.getAllOrders(id);
				json orderArr;
				for (int j = 0, l = O.size();j < l;j++)
				{
					Order i = O[j];
					orderArr.push_back({
						{ "_id", i._id },
						{ "user_id", i.user_id },
						{ "product_id", i.product_id },
						{ "complete", i.complete },
						{ "count", i.count }
					});
				}
				resp["orders"] = orderArr;
				resp["success"] = true;
			} else if( method == "getprod") {	
				long id = j["product_id"];
				vector <Order> O = orders.getProductOrders(id);
				json orderArr;
				for (int j = 0, l = O.size();j < l;j++)
				{
					Order i = O[j];
					orderArr.push_back({
						{ "_id", i._id },
						{ "user", getUser(i.user_id) },
						{ "product_id", i.product_id },
						{ "complete", i.complete },
						{ "count", i.count }
					});
				}
				resp["orders"] = orderArr;
				resp["success"] = true;
			}
		}
	}
	catch(exception& e) {
		resp["error"] = e.what();
	}
	cout << resp.dump();
}


