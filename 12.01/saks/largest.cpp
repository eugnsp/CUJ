Listing 5 - a function that returns the shape with the largest 
area in a collection of shapes

const shape *largest(const shape *sa[], size_t n)
	{
	const shape *s = 0;
	double m = 0;
	double a;
	for (size_t i = 0; i < n; ++i)
		if ((a = sa[i]->area()) > m)
			{
			m = a;
			s = sa[i];
			}
	return s;
	}
