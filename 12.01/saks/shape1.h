Listing 1 - A base class for shapes

class shape
	{
public:
	enum palette { BLUE, GREEN, RED };
	shape(palette c);
	virtual double area() const;
	virtual const char *name() const;
	virtual ostream &put(ostream &os) const;
	palette color() const;
private:
	palette _color;
	static const char *color_image[RED - BLUE + 1];
	};

inline ostream &operator<<(ostream &os, const shape &s)
	{
	return s.put(os);
	}
