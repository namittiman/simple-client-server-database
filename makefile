LDFLAGS = -L/usr/local/opt/flex/lib
CPPFLAGS = -I/usr/local/opt/flex/include
default:
	bison -d parser.y
	flex --header-file="parser_header.h" parser.l
	g++ parser.tab.c lex.yy.c server.cpp Query.cpp -o server $(LDFLAGS) -lfl  -lpthread 
	g++ $(CPPFLAGS) client.cpp -o client
clean:
	rm -f server client lex.l lex.yy.c parser_header.h parser.tab.c parser.tab.h yacc.y
