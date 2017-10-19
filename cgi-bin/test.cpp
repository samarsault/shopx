#include <iostream>
#include "data.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	ItemsDB items;
	// vector<Item> v = db.userItems(1);
	// for (int i = 0;i < v.size();i++)
	// 	cout << v[i].name << endl;
	vector<Item> v = items.userItems(1);
	cout << v[0].name;
}