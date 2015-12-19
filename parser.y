%{
	#include <iostream>
	#include <cstdio>
	#include <stdlib.h>
	#include <string.h>
	#include "Query.h"

	using namespace std;
	extern "C" int yylex();
	void yyerror(Query *q,const char *s);
%}

%parse-param {Query*q}
%union
{
	int num;
	float fval;
	char *sval;
};
%token <num>INT
%token <fval>FLOAT
%token <sval>STRING
%token INSERT DELETE1 FIND DELALL SHOW DETAILS EXIT

%%

commands : command commands | command
;
command : INSERT op {q->cmd=INSERTER;} | DELETE1 op {q->cmd=DELETER;} | FIND op {q->cmd=FINDER;} | DELALL {q->cmd=DELETE_ALL;} | SHOW {q->cmd=SHOW_STATUS;} | DETAILS {q->cmd=SHOW_CLIENT_DETAILS;} | EXIT {q->cmd=MY_EXIT;}
;
op 	: INT {q->dt=MY_INT;q->d.my_int=$1;}
| FLOAT  {q->dt=MY_FLOAT;q->d.my_float=$1;}
| STRING {q->dt=MY_STRING;strcpy(q->d.my_string,$1);}
;

%%

void yyerror(Query *q,const char *s)
{
	q->cmd=ERR;
	q->dt=NA;
	cout<<"Parsing error..!! Message : "<<s<<endl;

}

