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

typedef pair<int, int> pii;

// database object
database db("./shopx.db");

struct User {
	long _id;
	string email, passwd, address, name;
};

struct Order {
	long _id;
	long user_id;
	long product_id;
	int count;
	int complete;
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
		" address text"
		");";
	}
	//
	// create a user
	//
	bool createUser(string email, string password, string name, string address)
	{
		int cnt = 0;
		db << "select count(*) from users where email=?" << email >> cnt;
		if (cnt == 0) {
			db<< "insert into users (email,password,name,address) values(?,?,?,?); "
			<<email
			<<password
			<< name
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
		db<<"select email,password,address,name from users where _id=?"<<ID 
		>>[&](string email, string passwd, string addr, string name) {
			u.email = email;
			u.passwd = passwd;
			u.address = addr;
			u.name = name;
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
		db<<"select _id,password,address,name from users where email=?"<<email 
		>>[&](long id, string passwd, string addr,string name) {
			u._id = id;
			u.passwd = passwd;
			u.address = addr;
			u.name =name;
		};
		return u;
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
		" inventory integer,"
		" discount float"
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
		db << "select seller_id,name,description,imageLink,category,price,inventory,discount from items where _id = ?"
		   << _id
		   >> [&](
			int seller_id,
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
	bool deleteItem(long _id) 
	{
		try {
			db<<"delete from items where _id=?" << _id;
			return true;
		} catch (exception &e){
			return false;
		}
	}
	bool deleteItem(Item i) {
		return deleteItem(i._id);
	}
	// reduce inventory by 1 or remove from db if inventory=0
	void itemSold(long itemID) {
		Item i = getItem(itemID);
		i.inventory--;
		if (i.inventory == 0) {
			deleteItem(i);
		}
		else {
			db << "update items set inventory = ? where _id = ?" << i.inventory << itemID;
		}
	}

	// Items put up by a particular user
	vector<Item> userItems(long seller_id) 
	{
		vector<Item> V;
		db << "select _id,name,description,imageLink,category,price,inventory,discount from items where seller_id=?"
		 << seller_id
		>> [&](	int _id, 
			string name,
			string description,
			string imageLink,
			string category,
			int price,
			int inventory,
			float discount) {
				Item i;
				i._id = _id;
				i.name = name;
				i.description = description;
				i.imageLink = imageLink;
				i.category = category;
				i.seller_id = seller_id;
				i.price = price;
				i.inventory = inventory;
				V.push_back(i);
			};
		return V;	
	}
	
	vector<Item> search(string query) 
	{
		vector<Item> V;
		db << "SELECT _id,seller_id,name,description,imageLink,category,price,inventory,discount FROM items WHERE name like %?%;" 
		<< query
		>> [&](	long _id, 
			long seller_id,
			string name,
			string description,
			string imageLink,
			string category,
			int price,
			int inventory,
			float discount) {
				Item i;
				i._id = _id;
				i.name = name;
				i.description = description;
				i.imageLink = imageLink;
				i.category = category;
				i.seller_id = seller_id;
				i.price = price;
				i.inventory = inventory;
				i.discount = discount;
				V.push_back(i);
			};
		return V;
	}

	vector<Item> All()
	{
		vector<Item> V;
		db << "select _id,seller_id,name,description,imageLink,category,price,inventory,discount from items" 
		>> [&](	long _id, 
			long seller_id,
			string name,
			string description,
			string imageLink,
			string category,
			int price,
			int inventory,
			float discount) {
				Item i;
				i._id = _id;
				i.name = name;
				i.description = description;
				i.imageLink = imageLink;
				i.category = category;
				i.seller_id = seller_id;
				i.price = price;
				i.inventory = inventory;
				V.push_back(i);
			};
		return V;
	}
	
};

class Cart {
private:
	long user_id;
public:
	Cart(long userid)
	{
		user_id = userid;
		db<<
		"create table if not exists cart ("
		"_id integer primary key autoincrement not null,"
		"user_id integer not null,"
		"product_id integer not null,"
		"inventory integer not null"
		");";
	}
	// returns true if successful
	bool add(long product_id, int count)
	{
		try {
			int cnt = 0;
			db << "select count(*) from cart where user_id = ? and product_id=?" << user_id << product_id >> cnt;
			if (cnt > 0) {
				db << "update cart set inventory = inventory + 1 where product_id = ? and user_id=?"
				 << product_id << user_id;
			} else {
				db <<"insert into cart (user_id,product_id,inventory) values(?,?,?);"
				<< user_id
				<< product_id
				<< count;
			}
			return true;
		} catch(exception &e) {
			return false;
		}
	}

	bool remove(long product_id) 
	{
		try {
			db<<"delete from cart where user_id=? and product_id=?" << user_id << product_id;
			return true;
		}
		catch (exception &e) {
			return false;
		}
	}
	
	bool updateCount(long product_id, int newCount) 
	{
		try {
		db << "update cart set inventory = ? where product_id = ? and user_id=?" 
		   << newCount 
		   << product_id
		   << user_id;
		   return true;
		} catch(exception &e) {
			return false;
		}
	}
	vector<pair<Item, int> > getCart() 
	{
		vector<pair<Item, int> > V;	
		ItemsDB it;
		db << "select product_id, inventory from cart where user_id=?"<<user_id
		>>[&](long product_id, int inventory) {
			V.push_back(make_pair(it.getItem(product_id), inventory));
		};
		return V;
	}
};

class OrderDB {
public:
	// all functions
	OrderDB()
	{
		db<<
		" create table if not exists orders ("
		" _id integer primary key autoincrement not null,"
		" user_id integer not null,"
		" product_id integer not null,"
		" count integer default 1,"
		" complete integer default 0"
		");";
	}
	// returns true if successful
	bool placeOrder(long userID) 
	{
		Cart c(userID);
		vector<pair<Item, int> > V = c.getCart();
		try {
			for (int i = 0;i < V.size();i++)
			{
				Item it = V[i].first;
				int cnt = V[i].second;
				db<<"insert into orders (user_id,product_id,count) values(?,?,?);"
				  << userID
				  << it._id
				  << cnt;
				  c.remove(it._id);
			}
			return true;     
		} catch (exception &e) {
			cout << e.what() << endl;
			return false;
		}
	}
	Order getOrder(long orderID) {
		Order o;
		db << "select user_id, product_id from orders where _id=?" << orderID 
		>> [&](long user_id, long product_id, int cnt, int cmp) {
			o.user_id = user_id;
			o.product_id = product_id;
			o.count = cnt;
			o.complete = cmp;
			o._id = orderID;
		};
		return o;
	}
	// get all orders placed by a particular user
	// returns : array of orders
	vector<Order> getAllOrders(long userID) {
		vector<Order> O;
		db << "select _id, product_id from orders where user_id=?" << userID 
		>> [&](long _id, long product_id, int cnt, int cmp) {
			Order ord = { _id, userID, product_id, cnt, cmp };
			O.push_back(ord);
		};
		return O;	
	}
	// get all orders of a product
	// returns array of orders
	vector<Order> getProductOrders(long productID) {
		vector<Order> O;
		db << "select _id, user_id from orders where complete = 0 and product_id=?" << productID 
		>> [&](long _id, long user_id, long product_id, int cnt, int cmp) {
			Order ord = { _id, user_id, product_id, cnt, cmp };
			O.push_back(ord);
		};
		return O;
	}


	bool cancelOrder(long id) 
	{
		try {
			db << "delete from orders where _id = ?" << id;
			return true;
		} catch(exception &e) {
			return false;
		}
	}

	bool finishOrder(long orderID)
	{
		try {
			db << "update orders set complete = 1 where _id = ?" << orderID;
			return true;
		} catch (exception &e) {
			return false;
		}
	}
};
#endif
