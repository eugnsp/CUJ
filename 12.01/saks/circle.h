Listing 3 - class circle derived from shape

class circle : public shape
	{
public:
	circle(palette c, double r);
	double area() const;
	const char *name() const;
	ostream &put(ostream &os) const;
private:
	double radius;
	};

circle::circle(palette c, double r) : shape(c), radius(r) { }

double circle::area() const
	{
	const double pi = 3.1415926;
	return pi * radius * radius;
	}

const char *circle::name() const
	{
	return "circle";
	}

ostream &circle::put(ostream &os) const
	{
	return shape::put(os) << " with radius = " << radius;
	}
