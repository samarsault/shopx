#include <iostream>
#include "data.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	ItemsDB items;
	// vector<Item> v = db.userItems(1);
	// for (int i = 0;i < v.size();i++)
	// 	cout << v[i].name << endl;
	string query = "Fruits", op;
	db << "select description from items where name like \'%?%\';" << query >> op;
	cout << op;
}