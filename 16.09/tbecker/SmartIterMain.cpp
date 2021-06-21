//
// File: SmartIteratorMain.cpp
// 
// Created: Feb 1998
//
// Author: Thomas Becker
//
// Description:
// ============
//
// Main for testing class CSmartIterator
//

#include<iostream.h>
#include<string>
#include<list>
#include<algorithm>
#include<functional>

#include"SmartIterator.h"
#include"IteratorToMember.h"

// Identifier names get ridiculously long. Disable warning.
#pragma warning(disable: 4786)

// A struct to be placed in containers.
//
typedef struct _customer_info_tag {
  std::string str_name;
  long l_id;
  double dbl_balance;

  // Comparison operators are required by containers
  //
  bool operator==(const _customer_info_tag& rhs) const
  { return l_id == l_id; }
  bool operator!=(const _customer_info_tag& rhs) const
  { return l_id != l_id; }
  bool operator<(const _customer_info_tag& rhs) const
  { return l_id < l_id; }
  bool operator>(const _customer_info_tag& rhs) const
  { return l_id > l_id; }
} CUSTOMER_INFO, *LPCUSTOMER_INFO ;

// Class Balance is a functional that returns the balance from
// a CUSTOMER_INFO struct.
//
class Balance : public std::unary_function<CUSTOMER_INFO, double>{
public:
  double operator() (CUSTOMER_INFO& arg) const
    {return arg.dbl_balance;}
};

///////////////////////////////////////////////////////////////////////
//
// main
//
void main()
{

  // Define a list of customer infos.
  std::list<CUSTOMER_INFO> customer_list;
  
  // Define and initialize three CUSTOMER_INFO structs.
  //
  CUSTOMER_INFO c1;
  CUSTOMER_INFO c2;
  CUSTOMER_INFO c3;
  //
  c1.str_name = "John Doe";
  c1.l_id = 42;
  c1.dbl_balance = 99.0;
  //
  c2.str_name = "John Moe";
  c2.l_id = 43;
  c2.dbl_balance = 98.0;
  //
  c3.str_name = "John Noe";
  c3.l_id = 44;
  c3.dbl_balance = 98.9;

  // Push the three structs on the list.
  //
  customer_list.push_front(c3);
  customer_list.push_front(c2);
  customer_list.push_front(c1);
  
  // Obtain smart iterators to the beginning and end of the list
  // using the functional Balance.
  //
  CSmartIterator<std::list<CUSTOMER_INFO>::iterator, Balance> 
    sit1(
      customer_list.begin(), 
      Balance()
      );
  //
  CSmartIterator<std::list<CUSTOMER_INFO>::iterator, Balance>
    sit2(
      customer_list.end(), 
      Balance()
      );
  
  // Use the smart iterators to find the minimal balance in the list.
  cout << *std::min_element(sit1, sit2)
       << "\n";

  // Find the ID of the customer with the minimal balance by
  // retrieving the original iterator.
  //
  std::list<CUSTOMER_INFO>::iterator iter =
    std::min_element(sit1, sit2).GetIterator();
  //
  cout << iter->l_id
       << "\n";

  // Use the function MakeSmartIterator to pass smart iterators
  // to algorithms without having to worry about template arguments.
  //
  cout << *std::min_element(
    MakeSmartIterator(
      customer_list.begin(),  
      Balance()
      ),
    MakeSmartIterator(
      customer_list.end(),  
      Balance()
      )
    );
      
  cout << "\n";

  // Use the iterators to member to obtain the ID of the customer
  // with the minimal balance without ever having to specify template 
  // arguments.
  //
  cout << std::min_element(
    MakeIteratorToMember(
      customer_list.begin(),  
      & CUSTOMER_INFO::dbl_balance
    ),
    MakeIteratorToMember(
      customer_list.end(),  
      & CUSTOMER_INFO::dbl_balance
    )
  ).GetIterator()->l_id ;

  cout << "\n";
  
  // Modify a balance through an iterator to member.
  *MakeIteratorToMember(
    customer_list.begin(),  
    & CUSTOMER_INFO::dbl_balance
  ) = 88;

  // Use the iterators to member to obtain the minimal balance; it
  // should be 88 now.
  //
  cout << *std::min_element(
    MakeIteratorToMember(
      customer_list.begin(),  
      & CUSTOMER_INFO::dbl_balance
    ),
    MakeIteratorToMember(
      customer_list.end(),  
      & CUSTOMER_INFO::dbl_balance
    )
  ) ;
  
}