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

class Order {
	database db;
public:
	long userID;
	long productID;
	long orderID;

	// all functions
	Order()
	{
		try
		{
			db = database("shopx.db");
			db<<
			  " create table if not exists user ("
			  " userid biginteger not null,"
			  " productid biginteger not null,"
			  " orderid biginteger not null"
			  ");";
			  
		}
		
	}
	// returns true if successful
	bool placeOrder(long userID, long productID) 
	{
      db<<"insert into user (userID,produtID) values(?,?);"
        <<userID
		<<productID;
		return true;      
      
	}
	Order getOrder(long orderID) {

	}
	// get all orders placed by a particular user
	// returns : array of orders
	Order* getAllOrders(long userID) {

	}
	// get all orders of a product
	// returns array of orders
	Order* getProductOrders(long productID) {

	}
}

int main(int argc, char **argv)
{
	try {
		Cgicc cgi;

		database db("shopx.db");

		// Write here
		
	}
	catch(exception& e) {
		// handle any errors - omitted for brevity
	}
}
