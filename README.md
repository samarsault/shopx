# ShopX

A shopping site prototype

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
