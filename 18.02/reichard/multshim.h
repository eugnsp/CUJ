class IBase
{
public:
    virtual void First() = 0;
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

class Nested 
    : public SFirstShim<Nested, SSecondShim<Nested, IBase> >
{
};
