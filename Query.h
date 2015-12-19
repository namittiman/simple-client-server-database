typedef enum
{    
    MY_EXIT,
    ERR,
    INSERTER,
    DELETER,
    FINDER,
    DELETE_ALL,
    SHOW_STATUS,
    SHOW_CLIENT_DETAILS
}Command;

typedef enum
{   
    NA,
    MY_INT,
    MY_FLOAT,
    MY_STRING
}Data_type;


typedef union
{
    int my_int;
    float my_float;
    char my_string[100];
} Data;

class Query
{   
   public:
    Command cmd;
    Data_type dt;
    Data d;

     bool compare_to(Query);
     void print_data();
     
     void reset();
};