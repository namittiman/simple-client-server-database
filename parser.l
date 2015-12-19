%{
#include <cstdio> 
#include <iostream>
#include "parser.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
%}
%option case-insensitive
%%
[ \t|\n|\0]+       {}

(insert)				{return INSERT;}
(delete)				{return DELETE1;}
(find)					{return FIND;}
("delete all")			{return DELALL;}
("show status")			{return SHOW;}
("show client details")	{return DETAILS;}
(exit)				{return EXIT;}

[0-9]+  		   {yylval.num=atoi(yytext);return INT;}
[0-9]+\.[0-9]+     {yylval.fval=atof(yytext);return FLOAT;}
[a-zA-Z0-9]+       {yylval.sval=strdup(yytext);return STRING;}
.	      	 	   {cout<<"Invalid Command\n\n";}
%%

	