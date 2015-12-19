%{
	#include <iostream>
	#include <cstdio>
	#include <stdlib.h>
	#include <string.h>
	#include "Query.h"

	using namespace std;

	extern "C" int yylex();
	
	void yyerror(Query *q,const char *s);

	
	int token_type=0;
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
%token INSERT DELETE1 FIND ALL SHOW DETAILS DISPLAY

%%

commands : command commands | command
; 

command : INSERT op { 
	if(token_type == 1)
	{
		q->cmd=INSERT_INT;
	}
	else if(token_type == 2)
	{
		q->cmd=INSERT_FLOAT;
	}
	else
	{
		q->cmd=INSERT_STRING;
	}
	//			
}
| DELETE1 op { if(token_type == 1)
	//			f1.deleteNode(a);
	;
	else if(token_type == 2)
	//			f2.deleteNode(b);
	;		      else if(token_type == 3)
	//			f3.deleteNode(c);
	;		      else
	cout<<"Deleting all elements\n"; 
}
| FIND op   { if(token_type == 1)
	//f1.searchNode(a);
	;		      else if(token_type == 2)
	//f2.searchNode(b);
	;		      else ;
	//f3.searchNode(c); 
}
| SHOW op   { 
	cout<<"Integer = ";//f1.size<<"\tFloat = "<<f2.size<<"\t String = "<<f3.size<<"\n\n";
}	 
| DISPLAY   {   cout<<"Integer Linked List";
//f1.printList(); 
cout<<"Floating Value Linked List";
//f2.printList(); 
cout<<"String Linked List";
//f3.printList(); 
}
;
op 	: INT    { token_type=1; q->d.my_int=$1;}
| FLOAT  { token_type=2; q->d.my_float=$1;}
| STRING { token_type=3; strcpy(q->d.my_string,$1);}
| ALL    { token_type=4; }
| DETAILS{ 	   }
;
%%

void yyerror(Query *q,const char *s)
{
	cout<<"Parsing error..!! Message : "<<s<<endl;
}

