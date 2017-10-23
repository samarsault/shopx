/*
* Manipulate Items
* Method: POST JSON Data
* Parameters:
*		action: add, remove, update, search
*
* Response Params:
*	success: (bool)
*	error: (error message if any)
*	item: requested item
*	items: requested item(s) if many
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
		ItemsDB items;
		Cgicc cgi;
		CgiEnvironment env = cgi.getEnvironment();
		json j = json::parse(env.getPostData());
		if (j.find("method") != j.end()) 
		{
			string method = j["method"];
			json it = j["item"];		
			if (method == "add") {
				string name = it["name"], desc = it["description"], imL = it["imageLink"], cat=it["category"];
				int price = it["price"], disc = it["discount"], inv = it["inventory"];
				long s_id = it["seller_id"];
				Item i;
				i.name = name;
				i.seller_id = s_id;
				i.description = desc;
				i.imageLink = imL;
				i.category = cat;
				i.price = price;
				i.discount = disc;
				i.inventory = inv;
				
				if(items.addItem(i)) {
					long id;
					resp["success"] = true;
					db << "select _id from items where seller_id = ? and imageLink = ? " << s_id << imL >> id;
					resp["_id"] = id;
				}
				
			} else if (method == "remove") {
				long _id = it["_id"];
				if(items.deleteItem(_id))
					resp["success"] = true;
			} else if (method == "get") {
				if (it.find("_id") != it.end()) {
					long _id = it["_id"];
					Item i = items.getItem(_id);
					resp["item"]["_id"] = _id;
					resp["item"]["name"] = i.name;
					resp["item"]["seller_id"] = i.seller_id;
					resp["item"]["description"] = i.description;
					resp["item"]["imageLink"] = i.imageLink;
					resp["item"]["category"] = i.category;
					resp["item"]["price"] = i.price;
					resp["item"]["discount"] = i.discount;
					resp["item"]["inventory"] = i.inventory;
					resp["success"] = true;
				}
				if (it.find("seller_id") != it.end()) {
					long seller_id = it["seller_id"];
					vector<Item> myItems = items.userItems(seller_id);
					json itemArr;
					for (int j = 0, l = myItems.size();j < l;j++)
					{
						Item i = myItems[j];
						itemArr.push_back({
							{ "name", i.name },
							{ "_id", i._id },
							{ "seller_id", i.seller_id },
							{ "description", i.description },
							{ "imageLink", i.imageLink },
							{ "category", i.category },
							{ "price", i.price },
							{ "discount", i.discount },
							{ "inventory", i.inventory }
						});
					}
					resp["items"] = itemArr;
					resp["success"] = true;
				}
			} else if (method == "search") {
				string query = it["name"];
				vector<Item> itVect = items.search(query);
				json itemArr;
				for (int j = 0, l = itVect.size();j < l;j++)
				{
					Item i = itVect[j];
					itemArr.push_back({
						{ "_id", i._id },
						{ "name", i.name },
						{ "seller_id", i.seller_id },
						{ "description", i.description },
						{ "imageLink", i.imageLink },
						{ "category", i.category },
						{ "price", i.price },
						{ "discount", i.discount },
						{ "inventory", i.inventory }
					});
				}
				resp["items"] = itemArr;
				resp["success"] = true;
			} else if (method == "all") {
				vector<Item> itVect = items.All();
				json itemArr;
				for (int j = 0, l = itVect.size();j < l;j++)
				{
					Item i = itVect[j];
					itemArr.push_back({
						{ "_id", i._id },
						{ "name", i.name },
						{ "seller_id", i.seller_id },
						{ "description", i.description },
						{ "imageLink", i.imageLink },
						{ "category", i.category },
						{ "price", i.price },
						{ "discount", i.discount },
						{ "inventory", i.inventory }
					});
				}
				resp["items"] = itemArr;
				resp["success"] = true;}
			}
			
		}
		catch(exception& e) {
			resp["error"] = e.what();
		}
		cout << resp.dump();
	}