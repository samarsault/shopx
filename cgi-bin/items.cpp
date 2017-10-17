/*
 * Authenticate a User / Create one
 */
  
#include <iostream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <sqlite_modern_cpp.h>

using namespace std;
using namespace sqlite;
using namespace cgicc;


// Name, Price, Description, Discount(FLOAT), IMAGE, SellerID, Inventory
class Item {
	database db;
public:

	// all functions
	Item()
	{
		try
		{  database db("shopx.db");
		   db<<
		    "create table if not exists user ("
			" name text not null,"
			" price integer not null,"
			" description text,"
			" discount float,"
			" imageLink text,"
			" sellerid long,"
			" inventory integer"
			");";
		}
		
	}
	// returns true if successful
	bool addItem(long itemID, string name, int price, string description, string imageLink, long sellerID, int inventory, float discount)
	{
		
	}
	// reduce inventory by 1 or remove from db if inventory=0
	void itemSold(long itemID) {

	}

}
int main(int argc, char **argv)
{
	try {
		Cgicc cgi;

		// Write here
		
	}
	catch(exception& e) {
		// handle any errors - omitted for brevity
	}
}
