#include<functional>

class Balance : public std::unary_function<CUSTOMER_INFO, double>{
public:
  double operator() (CUSTOMER_INFO& arg) const
    {return arg.dbl_balance;}
};
