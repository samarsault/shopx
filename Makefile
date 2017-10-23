cc = clang++-3.8 -std=c++1y -lcgicc -lsqlite3 # on osx use clang++
css:
	scss styles/main.scss styles/main.css --sourcemap=none
cpp:
	$(cc) src/cart.cpp -o cgi-bin/cart
	$(cc) src/items.cpp -o cgi-bin/items
	$(cc) src/login.cpp -o cgi-bin/login
	$(cc) src/order.cpp -o cgi-bin/order
serve:	
	python -m CGIHTTPServer
