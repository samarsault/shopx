/*
* Databae Manager
*/
#ifndef DATA_MANAGER

#define DATA_MANAGER

#include <iostream>
#include <vector>
#include "sqlite_modern_cpp.h"

using namespace std;
using namespace sqlite;

// database object
database db("../data/shopx.db");

enum USER_TYPE {
	BUYER,
	SELLER
};

struct User {
	long _id;
	string email, passwd, address;
	int type;	
};

struct Order {
	long _id;
	long user_id;
	long product_id;
};

struct Item {
	long _id;
	long seller_id;
	string name;
	string description;
	string imageLink;
	string category;
	int price;
	int inventory;
	float discount;
};

class UserDB {
public:
	UserDB() 
	{
		db<<
		"create table if not exists users("
		"_id integer primary key autoincrement not null,"
		" email text unique not null,"
		" password text not null,"
		" address text,"
		" type integer not null"
		");";
	}
	//
	// create a user
	//
	bool createUser(string email, string password, int type, string address="")
	{
		int cnt = 0;
		db << "select count(*) from users where email=?" << email >> cnt;
		if (cnt == 0) {
			db<< "insert into users (email,password,type,address) values(?,?,?,?); "
			<<email
			<<password
			<<type
			<<address; 	
			return true;
		}
		return false;
	}
	bool auth(string email, string pass) 
	{
		User u = getUser(email);
		return u.passwd == pass;
	}
	//
	// get user by id
	//
	User getUser(long ID) {
		// TODO
		User u;
		u._id = ID;
		db<<"select email,password,address,type from users where _id=?"<<ID 
		>>[&](string email, string passwd, string addr, int type) {
			u.email = email;
			u.passwd = passwd;
			u.address = addr;
			u.type = type;
		};
		return u;
	}
	//
	// get user by email
	//
	User getUser(string email) {
		// TODO
		User u;
		u.email = email;
		db<<"select _id,password,address,type from users where email=?"<<email 
		>>[&](long id, string passwd, string addr, int type) {
			u._id = id;
			u.passwd = passwd;
			u.address = addr;
			u.type = type;
		};
		return u;
	}
};

class OrderDB {
public:
	// all functions
	OrderDB()
	{
		db<<
		" create table if not exists orders ("
		" _id integer primary key autoincrement not null"
		" user_id integer not null,"
		" product_id integer not null"
		");";
	}
	// returns true if successful
	bool placeOrder(long userID, long productID) 
	{
		try {
			db<<"insert into orders (user_id,product_id) values(?,?);"
			<<userID
			<<productID;
			return true;      
		} catch (exception &e) {
			return false;
		}
	}
	Order getOrder(long orderID) {
		Order o;
		db << "select user_id, product_id from orders where _id=?" << orderID 
		>> [&](long user_id, long product_id) {
			o.user_id = user_id;
			o.product_id = product_id;
		};
		return o;
	}
	// get all orders placed by a particular user
	// returns : array of orders
	vector<Order> getAllOrders(long userID) {
		vector<Order> O;
		db << "select _id, product_id from orders where user_id=?" << userID 
		>> [&](long _id, long product_id) {
			Order ord = { _id, userID, product_id };
			O.push_back(ord);
		};
		return O;	
	}
	// get all orders of a product
	// returns array of orders
	vector<Order> getProductOrders(long productID) {
		vector<Order> O;
		db << "select _id, user_id from orders where product_id=?" << productID 
		>> [&](long _id, long userID) {
			Order ord = { _id, userID, productID };
			O.push_back(ord);
		};
		return O;
	}
};

class ItemsDB {
public:
	
	ItemsDB()
	{
		db<<
		"create table if not exists items ("
		"_id integer primary key autoincrement not null,"
		" name text not null,"
		" price integer not null,"
		" description text,"
		" category text,"
		" imageLink text,"
		" seller_id integer,"
		" inventory integer"
		" discount float,"
		");";
		
	}
	// returns true if successful
	bool addItem(Item it)
	{
		try {
			db <<"insert into items (name,price,description,category,imageLink,seller_id,inventory,discount) values(?,?,?,?,?,?,?,?);"
			<<it.name
			<<it.price
			<<it.description
			<<it.category
			<<it.imageLink
			<<it.seller_id
			<<it.inventory
			<<it.discount;
			return true;
		} catch(exception &e) {
			return false;
		}
	}
	Item getItem(long _id)
	{
		Item i;
		i._id = _id;
		db << "select * from items where _id = " << _id
		   >> [&](
			long seller_id,
			string name,
			string description,
			string imageLink,
			string category,
			int price,
			int inventory,
			float discount) {
				i.name = name;
				i.description = description;
				i.imageLink = imageLink;
				i.category = category;
				i.seller_id = seller_id;
				i.price = price;
				i.inventory = inventory;
			};
		return i;
	}
	void deleteItem(Item i) {
		db<<"delete from items where _id=" << i._id;
	}
	// reduce inventory by 1 or remove from db if inventory=0
	void itemSold(long itemID) {
		Item i = getItem(itemID);
		i.inventory--;
		if (i.inventory == 0)
			deleteItem(i);
		else {
			db << "update items set inventory = ? where _id = ?" << i.inventory << itemID;
		}
	}
	
};

#endif
