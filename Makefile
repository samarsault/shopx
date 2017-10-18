css:
	scss styles/main.scss styles/main.css --sourcemap=none
cgi:
	clang++ $(name) -o $(output) -lcgicc -lsql_modern -lsqlite3
serve:	
	python -m CGIHTTPServer