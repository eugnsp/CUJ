Listing 4 - class rectangle derived from shape

class rectangle : public shape
	{
public:
	rectangle(palette c, double h, double w);
	double area() const;
	const char *name() const;
	ostream &put(ostream &os) const;
private:
	double height, width;
	};

rectangle::rectangle(palette c, double h, double w)
	: shape(c), height(h), width(w) { }

double rectangle::area() const
	{
	return height * width;
	}

const char *rectangle::name() const
	{
	return "rectangle";
	}

ostream &rectangle::put(ostream &os) const
	{
	return shape::put(os) << " with height = " << height
		<< " and width = " << width;
	}
