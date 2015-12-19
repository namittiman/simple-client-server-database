#include "Query.h"
template <class T>
struct Node
{
  T data;
  Node * next;
};

template <class T>
class LinkedListSize
{
  public:
  int total_size;
  int int_size;
  int float_size;
  int string_size;

  LinkedListSize() {
    total_size=0;
    int_size=0;
    float_size=0;
    string_size=0;
  };

  void reset_sizes()
  {
    total_size=0;
    int_size=0;
    float_size=0;
    string_size=0;
  }

  void increment_sizes(T data)
  {
    total_size++;
    if (data.dt==MY_INT)
    {
      int_size++;
    }
    else if (data.dt==MY_FLOAT)
    {
      float_size++;
    }
    else if (data.dt==MY_STRING)
    {
      string_size++;
    }
  }

    void decrement_sizes(T data)
  {
    total_size--;
    if (data.dt==MY_INT)
    {
      int_size--;
    }
    else if (data.dt==MY_FLOAT)
    {
      float_size--;
    }
    else if (data.dt==MY_STRING)
    {
      string_size--;
    }
  }

  string displaySizes()
  {
    ostringstream oss;
    oss << "Status of LinkedList :: INT : " <<int_size<< " FLOAT : " <<float_size<<" STRING : "<<string_size;
    string var = oss.str();
 
 return var;
  }


};

template <class T>
class LinkedList
{
public:
  LinkedList() : head(NULL){};
  ~LinkedList() {};

  bool addNode(T data)
  {
    try
    {
      Node<T> * tmp = new Node<T>();
      tmp->data = data;
      tmp->next = head;
      head = tmp;
      size.increment_sizes(data);
      return true;
    }
    catch(std::exception & ex)
    {
      return false;
    }
  }


  bool deleteNode(T data)
  {
    Node<T> *curr = head, *prev = NULL;

    while (curr)
    {
      if (data.compare_to(curr->data)) break;
      prev = curr;
      curr = curr->next;
    }

   if (curr)
   {
    if (prev)
    {
      prev->next = curr->next;
    }
    else
    {
      head = curr->next;
    }
    delete(curr);
    size.decrement_sizes(data);
    return true;
  }
  else
  {
    return false;
  }
}


Node<T> * searchNode(T data)
{
  Node<T> * tmp = head;
  while (tmp)
  {
    if (data.compare_to(tmp->data))
    {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}





void printList()
{
  Node<T> * tmp = head;
  bool printNewLine = (tmp) ? true : false;
  while (tmp)
  {
    tmp->data.print_data();
    cout << "|";
    tmp = tmp->next;
  } 

  if (printNewLine)
  {
    cout << endl;
  }
}


void destroyList()
{
  Node<T> * tmp = NULL;
  while (head)
  {
    tmp = head;
    head = head->next;
      //cout << "deleting data " << tmp->data << endl;
    delete(tmp);
    size.reset_sizes();
  }
}


void say_hi()
{
  cout<<"yoyoyoyo";
}

string showCountTypes()
{
  return size.displaySizes();
}


private:
  Node<T> * head;
  LinkedListSize<T> size;




  void print(Node<T> * tmp)
  {
    bool printNewLine = (tmp) ? true : false;
    while (tmp)
    {
      cout << tmp->data << ",";
      tmp = tmp->next;
    } 

    if (printNewLine)
    {
      cout << endl;
    }
  }


  
};

