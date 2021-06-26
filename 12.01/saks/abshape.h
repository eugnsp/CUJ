Listing 7 - An abstract base class for shapes

class shape
	{
public:
	enum palette { BLUE, GREEN, RED };
	shape(palette c);
	virtual double area() const = 0;
	virtual const char *name() const = 0;
	virtual ostream &put(ostream &os) const = 0;
	palette color() const;
private:
	palette _color;
	static const char *color_image[RED - BLUE + 1];
	};

inline ostream &operator<<(ostream &os, const shape &s)
	{
	return s.put(os);
	}
