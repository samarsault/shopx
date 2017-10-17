css:
	scss styles/main.scss styles/main.css --sourcemap=none
cgi:
	g++ $(name) -o $(output) $(pkg-config --libs --cflags sqlite_modern_cpp cgicc)
serve:	
	python -m CGIHTTPServer