class IFirst
{
public:
    virtual void First() = 0;
};

class ISecond
{
    virtual void Second() = 0;
};

template <class Derived, class Base>
class SFirstShim
{
public:
    virtual void First() { ... }
};

template <class Derived, class Base>
class SSecondShim
{
public:
    virtual void Second() { ... }
};


class Multiple
    : public SFirstShim<Multiple, IFirst >
    , public SSecondShim<Multiple, ISecond> 
{
};
