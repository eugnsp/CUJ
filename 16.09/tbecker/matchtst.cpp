#include<functional>

class name_pred : public std::unary_function<CUSTOMER_INFO, bool>
{
pubic:  
  name_pred(string str_name) : m_str_name(str_name){}
  bool operator() (CUSTOMER_INFO arg) const
    { return m_str_name == arg.m_str_name; }
private:
  string m_str_name;
};
