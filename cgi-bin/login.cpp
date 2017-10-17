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

class User {
	// all function
};

int main(int argc, char **argv)
{
	try {
		Cgicc cgi;
		cgicc::form_iterator name = cgi.getElement("user");
		cgicc::form_iterator pass = cgi.getElement("pass");

		database db("users.db");

		// Write here
		
	}
	catch(exception& e) {
		// handle any errors - omitted for brevity
	}
}
