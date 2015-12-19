#include <iostream>
#include "LinkedList.h"
using namespace std;

template <class T>
bool addNode(T data)
{
  try
  {
    Node<T> * tmp = new Node<T>();
    tmp->data = data;
    tmp->next = head;
    head = tmp;
    ++size;
    return true;
  }
  catch(std::exception & ex)
  {
    return false;
  }
}

template <class T>
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
      --size;
      return true;
    }
    else
    {
      return false;
    }
  }

template <class T>
  Node<T> * searchNode(T data)
  {
    Node<T> * tmp = head;
    while (tmp)
    {
      if (tmp->data == data)
      {
        return tmp;
      }
      tmp = tmp->next;
    }
    return NULL;
  }

template <class T>
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

template <class T>
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

template <class T>
  void destroyList()
  {
    Node<T> * tmp = NULL;
    while (head)
    {
      tmp = head;
      head = head->next;
      //cout << "deleting data " << tmp->data << endl;
      delete(tmp);
    }
  }

template <class T>
void say_hi()
{
  cout<<"yoyoyoyoyoyoyoyoyoyoyyoyoyoyoyoyoyoyoyoyo";
}

 


/*  int main()
  {
    LinkedList<Query> l;
    Query q;
    q.cmd=INSERT_FLOAT;
    q.d.my_float=0;
    l.addNode(q);
    q.d.my_float=1.4;
    l.addNode(q);
    q.d.my_float=5;
    l.addNode(q);
    q.d.my_float=3;
    l.addNode(q);

    q.d.my_float=3.3;
    l.addNode(q);
    q.d.my_float=4;
    l.addNode(q);
    l.printList();

    q.d.my_float=1.4;
    l.deleteNode(q);
    l.printList();
 
    l.deleteNode(q);
    l.printList();
    q.d.my_float=3.5;
    l.deleteNode(q);
    l.printList();
    q.d.my_;float=3.3;
    l.deleteNode(q);

    l.printList();
 

  if (l.searchNode(2))
    {
      std::cout << "2 found \n";
    }

  if (!l.searchNode(5))
    {
      std::cout << "5 not found \n";
    }
    l.destroyList();
l.printList();
    return 0;
  }*/