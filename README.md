# ShopX

An experimental shopping site prototype using a CGI Interface

![ShopX](http://i.imgur.com/SDiUGu2r.jpg)

## Running

Once, you've installed the libraries in the C++ Dependencies section,

```sh
$ make cpp # compile C++
$ make css # compile sass
$ make serve
```

## Database

SQLite is used as a database engine.
The following databases exist:

1. User
   ID, Name, Address, Email, ID, Auth_Token(Password), Type(Buyer/Seller)

2. Items
    Name, Price, Description, Discount(FLOAT), IMAGE, SellerID, Inventory

3. Orders
    OrderID, UserID, ProductID

## C++ Dependencies

CGICC: http://ftp.gnu.org/gnu/cgicc/

SQLITE C++ Parser: https://github.com/aminroosta/sqlite_modern_cpp

JSON: https://github.com/nlohmann/json
