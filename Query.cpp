#include <iostream>
#include <string.h>
#include "Query.h"
using namespace std;
bool Query::compare_to(Query q)
{
    if (dt==MY_INT && q.dt==MY_INT)
    {
      return (d.my_int==q.d.my_int) ? true:false; 
    }
    else
      if (dt==MY_FLOAT && q.dt==MY_FLOAT)
    {
      return (d.my_float==q.d.my_float) ? true:false; 
    }
    else
      if (dt==MY_STRING && q.dt==MY_STRING)
    {
      return (strcmp(d.my_string,q.d.my_string)==0) ? true:false; 
    }
    
    return false;
}


void Query::print_data()
{
    if (dt==MY_INT)
    {
      cout<<d.my_int;
    }
    else 
      if (dt==MY_STRING)
      {
        cout<<d.my_string;
      }
      else
        if (dt==MY_FLOAT)
        {
          cout<<d.my_float;
        }
}

void Query::reset()
{
  dt=NA;
  cmd=ERR;
}