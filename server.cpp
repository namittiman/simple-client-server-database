#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "parser.tab.h" 
#include "parser_header.h"
#include <time.h>
using namespace std;
#include <string>
#include <sstream>
#include <map>
#include "LinkedList.h"

typedef struct
{
  int* csock;  
  LinkedList<Query> *list_ptr;
} thread_args;

map<string,string> details_map;

string m[] = {
  "Query OK, 1 value Inserted", "Query OK, But value Insetion Unsuccessful",
  "Query OK, 1 value Deleted", "Query OK, 0 values Deleted",
  "Query OK, Value Found","Query OK, Value NOT Found","Query OK, Linked List Deleted",
  "Query OK, But Value Already Exists, 0 Values Inserted" ,"Error in Syntax"
};
void* ClientHandler(void*);
void* DetailsHandler(void*);
int yyparse(Query*);

int main(int argc, char** argv)
{
  if (argc<2)
  {
    printf("\n invoke as \n ./server port# \n\n");
    exit(0);
  }
  int host_port= atoi(argv[1]);
  struct sockaddr_in my_addr;
  int hsock;
  int * p_int ;
  int err;

  socklen_t addr_size = 0;
  int* csock;
  sockaddr_in sadr;
  pthread_t thread_id=0;
  int *i; 

  pthread_t detail_thread_id=0;
  pthread_create(&detail_thread_id,0,&DetailsHandler, (void*)i);
  time_t rawtime;
  struct tm * timeinfo;




  hsock = socket(AF_INET, SOCK_STREAM, 0);
  if(hsock == -1)
  {
    printf("Error initializing socket %d\n", errno);
    exit(0);
  }
  p_int = (int*)malloc(sizeof(int));
  *p_int = 1;
  if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
    (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) )
  {
    printf("Error setting options %d\n", errno);
    free(p_int);
    exit(0);
  }
  free(p_int);
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(host_port);
  memset(&(my_addr.sin_zero), 0, 8);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  if(bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
  {
    fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
    exit(0);
  }
  if(listen( hsock, 10) == -1 )
  {
    fprintf(stderr, "Error listening %d\n",errno);
    exit(0);
  }
  //Now lets do the server stuff
  addr_size = sizeof(sockaddr_in);
  
  LinkedList<Query> list;

  thread_args my_arg;
  my_arg.list_ptr=&list;

  while(true)
  {
    printf("waiting for a connection\n");
    csock = (int*)malloc(sizeof(int));
    if((*csock = accept( hsock, (sockaddr*)&sadr, &addr_size))!= -1)
    {

      time (&rawtime);
      timeinfo = localtime (&rawtime);
      printf ("Current local time and date: %s ip : %s", asctime(timeinfo),inet_ntoa(sadr.sin_addr));
      details_map.insert (pair<string,string>(inet_ntoa(sadr.sin_addr),asctime(timeinfo) ));

      my_arg.csock=csock;
      printf("------------\nReceived connection from %s\n",inet_ntoa(sadr.sin_addr));
      pthread_create(&thread_id,0,&ClientHandler, (void*)&my_arg);
      pthread_detach(thread_id);
    }
    else
    {
      fprintf(stderr, "Error accepting %d\n", errno);
    }
  }
}

void* DetailsHandler(void* yo)
{
  string input;
  while(1)
  {  
  getline(cin,input);
  if(input.compare("SHOW CLIENT DETAILS")==0 || input.compare("show client details")==0)
  {
    cout<<"<Client IP>    <Connection Time>\n";
    for(map<string,string>::iterator it = details_map.begin(); it != details_map.end(); ++it)
    {
      cout << it->first <<"\t"<< it->second<<"\n";
    }
  }
  else
  {
    cout<< "Invalid Command\n";
  }
  fflush(stdout);
}
}



void* ClientHandler(void* my_arg)
{
  thread_args* ptr=(thread_args*) my_arg;
  int *csock = ptr->csock;

  char buffer[1024];

  int buffer_len = 1024;
  int bytecount;
  Query q;
  Query* find_ptr;
  
  LinkedList<Query> *xx = ptr->list_ptr;
  // List* list=ptr->list;


  while(1)
  {

    q.reset();
    memset(buffer, 0, buffer_len);
    buffer_len=1024;
    if((bytecount = recv(*csock, buffer, buffer_len, 0))== -1)
    {
      fprintf(stderr, "Error receiving data %d\n", errno);
      free(csock);
      exit(0);
    }
    //cout<<"\nthe buffer :: "<<buffer<<endl;
    //cout<<"\nthe buffer len :: "<<strlen(buffer)<<endl;
    //yy_scan_buffer(buffer,buffer_len);
    YY_BUFFER_STATE yybs=yy_scan_string(buffer);
    yy_switch_to_buffer(yybs);
    yyparse(&q);

    printf("\nReceived bytes %d\nReceived string \"%s\"\n", bytecount, buffer);

    if (q.cmd==INSERTER)
    {
      cout<<"INSERTER"<<endl;
      //Insert only if not present already
      if (xx->searchNode(q)==NULL)
      {
        if(xx->addNode(q))
        {
        //send successful
          if((bytecount = send(*csock, m[0].c_str(), strlen(m[0].c_str()), 0))== -1)
          {
            fprintf(stderr, "Error sending data %d\n", errno);
            free(csock);
            exit(0);
          }      
          printf("Sent bytes %d\n", bytecount);
        }
        else
        {
        //send unsuccessful
          if((bytecount = send(*csock, m[1].c_str(), strlen(m[1].c_str()), 0))== -1)
          {
            fprintf(stderr, "Error sending data %d\n", errno);
            free(csock);
            exit(0);
          }      
          printf("Sent bytes %d\n", bytecount);
        }
      }
      else
      {
        if((bytecount = send(*csock, m[7].c_str(), strlen(m[7].c_str()), 0))== -1)
        {
          fprintf(stderr, "Error sending data %d\n", errno);
          free(csock);
          exit(0);
        }      
        printf("Sent bytes %d\n", bytecount);
      }



    }
    else if (q.cmd==DELETER)
    {
      cout<<"DELETER"<<endl;
      if(xx->deleteNode(q))
      {
        //send successful
        if((bytecount = send(*csock, m[2].c_str(), strlen(m[2].c_str()), 0))== -1)
        {
          fprintf(stderr, "Error sending data %d\n", errno);
          free(csock);
          exit(0);
        }      
        printf("Sent bytes %d\n", bytecount);
      }
      else
      {
        //send unsuccessful
        if((bytecount = send(*csock, m[3].c_str(), strlen(m[3].c_str()), 0))== -1)
        {
          fprintf(stderr, "Error sending data %d\n", errno);
          free(csock);
          exit(0);
        }      
        printf("Sent bytes %d\n", bytecount);
      }
    } 
    else if (q.cmd==FINDER)
    {
      cout<<"FINDER"<<endl;
      find_ptr=(Query*)xx->searchNode(q);
      if (find_ptr==NULL)
      {
        // send found
        if((bytecount = send(*csock, m[5].c_str(), strlen(m[5].c_str()), 0))== -1)
        {
          fprintf(stderr, "Error sending data %d\n", errno);
          free(csock);
          exit(0);
        }      
        printf("Sent bytes %d\n", bytecount);
      }
      else
      {
        //send not found
        if((bytecount = send(*csock, m[4].c_str(), strlen(m[4].c_str()), 0))== -1)
        {
          fprintf(stderr, "Error sending data %d\n", errno);
          free(csock);
          exit(0);
        }      
        printf("Sent bytes %d\n", bytecount);
      }   
    }
    else if (q.cmd==DELETE_ALL)
    {
      cout<<"DELETE_ALL"<<endl;
      xx->destroyList();
      if((bytecount = send(*csock, m[6].c_str(), strlen(m[6].c_str()), 0))== -1)
      {
        fprintf(stderr, "Error sending data %d\n", errno);
        free(csock);
        exit(0);
      }      
      printf("Sent bytes %d\n", bytecount);
    }
    else if (q.cmd==SHOW_STATUS)
    {

      cout<<"SHOW_STATUS"<<endl;
      string msg = xx->showCountTypes();
      if((bytecount = send(*csock, msg.c_str(), strlen(msg.c_str()), 0))== -1)
      {
        fprintf(stderr, "Error sending data %d\n", errno);
        free(csock);
        exit(0);
      }
      printf("Sent bytes %d\n", bytecount);
    }
    else if (q.cmd==ERR)
    {
      cout<<"ERR"<<endl;
        //send invalid command
      if((bytecount = send(*csock, m[8].c_str(), strlen(m[8].c_str()), 0))== -1)
      {
        fprintf(stderr, "Error sending data %d\n", errno);
        free(csock);
        exit(0);
      }      
      printf("Sent bytes %d\n", bytecount);


    }
    else if (q.cmd==MY_EXIT)
    {
      cout<<"MY_EXIT"<<endl;
      free(csock);
      return 0;
    }




    xx->printList();


    printf("-----------------------------------------------\n");
  }
  return 0;
}
