Listing 2 - Member function and static member data definitions for 
class shape

shape::shape(palette c) : _color(c) { }

shape::palette shape::color() const
	{
	return _color;
	}

double shape::area() const
	{
	return 0;
	}

const char *shape::name() const
	{
	return "point";
	}

ostream &shape::put(ostream &os) const
	{
	return os << color_image[_color] << ' ' << name();
	}

const char *shape::color_image[shape::RED - shape::BLUE + 1] =
	{ "blue", "green", "red" };
