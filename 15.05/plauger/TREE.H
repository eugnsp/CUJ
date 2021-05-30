[Caption: Listing 1: Template class Tree]

			// TEMPLATE CLASS Tree
template<class K, class Ty, class Kfn, class Pr, class A>
	class Tree {
protected:
///	typedef typename A::rebind<void>::other::pointer
///		Genptr;
	typedef void *Genptr;	///
	enum Redbl {Red, Black};
	struct Node;
	friend struct Node;
	struct Node {
		Genptr Left, Parent, Right;
		Ty Value;
		Redbl Color;
		};
///	typedef typename A::rebind<Node>::other::pointer
///		Nodeptr;
///	typedef typename A::rebind<Nodeptr>::other::reference
///		Nodepref;
///	typedef typename A::rebind<const K>::other::reference
///		Keyref;
///	typedef typename A::rebind<Redbl>::other::reference
///		Rbref;
///	typedef typename A::rebind<Ty>::other::reference
///		Vref;
	typedef Node *Nodeptr;	///
	typedef Nodeptr& Nodepref;	///
	typedef const K& Keyref;	///
	typedef Redbl& Kbref;	///
	typedef Ty& Vref; 	///
	static Rbref Color(Nodeptr P)
		{return ((Rbref)(*P).Color); }
	static Keyref Key(Nodeptr P)
		{return (Kfn()(Value(P))); }
	static Nodepref Left(Nodeptr P)
		{return ((Nodepref)(*P).Left); }
	static Nodepref Parent(Nodeptr P)
		{return ((Nodepref)(*P).Parent); }
	static Nodepref Right(Nodeptr P)
		{return ((Nodepref)(*P).Right); }
	static Vref Value(Nodeptr P)
		{return ((Vref)(*P).Value); }
public:
	typedef Tree<K, Ty, Kfn, Pr, A> Myt;
	typedef K key_type;
	typedef Ty value_type;
	typedef typename A::size_type size_type;
	typedef typename A::difference_type difference_type;
///	typedef typename A::rebind<Ty>::other::pointer
///		Tptr;
///	typedef typename A::rebind<const Ty>::other::pointer
///		Ctptr;
///	typedef typename A::rebind<Ty>::other::reference
///		reference;
///	typedef typename A::rebind<const Ty>::other::reference
///		const_reference;
	typedef Ty *Tptr;	///
	typedef const Ty *Ctptr;	///
	typedef Ty& reference;	///
	typedef const Ty& const_reference;	///
		// CLASS iterator
	class iterator;
	friend class iterator;
	class iterator : public Bidit<Ty, difference_type,
		Tptr, reference> {
	public:
		iterator()
			{}
		iterator(Nodeptr P)
			: Ptr(P) {}
		reference operator*() const
			{return (Value(Ptr)); }
///		Tptr operator->() const
///			{return (&**this); }
		iterator& operator++()
			{Inc();
			return (*this); }
		iterator operator++(int)
			{iterator Tmp = *this;
			++*this;
			return (Tmp); }
		iterator& operator--()
			{Dec();
			return (*this); }
		iterator operator--(int)
			{iterator Tmp = *this;
			--*this;
			return (Tmp); }
		bool operator==(const iterator& X) const
			{return (Ptr == X.Ptr); }
		bool operator!=(const iterator& X) const
			{return (!(*this == X)); }
		void Dec()
			{if (Color(Ptr) == Red
				&& Parent(Parent(Ptr)) == Ptr)
				Ptr = Right(Ptr);
			else if (Left(Ptr) != Nil)
				Ptr = Max(Left(Ptr));
			else
				{Nodeptr P;
				while (Ptr == Left(P = Parent(Ptr)))
					Ptr = P;
				Ptr = P; }}
		void Inc()
			{if (Right(Ptr) != Nil)
				Ptr = Min(Right(Ptr));
			else
				{Nodeptr P;
				while (Ptr == Right(P = Parent(Ptr)))
					Ptr = P;
				if (Right(Ptr) != P)
					Ptr = P; }}
		Nodeptr Mynode() const
			{return (Ptr); }
	protected:
		Nodeptr Ptr;
		};
		// CLASS const_iterator
	class const_iterator;
	friend class const_iterator;
	class const_iterator : public Bidit<Ty, difference_type,
		Ctptr, const_reference> {
	public:
		const_iterator()
			{}
		const_iterator(Nodeptr P)
			: Ptr(P) {}
		const_iterator(const iterator& X)
			: Ptr(X.Mynode()) {}
		const_reference operator*() const
			{return (Value(Ptr)); }
///		Ctptr operator->() const
///			{return (&**this); }
		const_iterator& operator++()
			{Inc();
			return (*this); }
		const_iterator operator++(int)
			{const_iterator Tmp = *this;
			++*this;
			return (Tmp); }
		const_iterator& operator--()
			{Dec();
			return (*this); }
		const_iterator operator--(int)
			{const_iterator Tmp = *this;
			--*this;
			return (Tmp); }
		bool operator==(const const_iterator& X) const
			{return (Ptr == X.Ptr); }
		bool operator!=(const const_iterator& X) const
			{return (!(*this == X)); }
		void Dec()
			{if (Color(Ptr) == Red
				&& Parent(Parent(Ptr)) == Ptr)
				Ptr = Right(Ptr);
			else if (Left(Ptr) != Nil)
				Ptr = Max(Left(Ptr));
			else
				{Nodeptr P;
				while (Ptr == Left(P = Parent(Ptr)))
					Ptr = P;
				Ptr = P; }}
		void Inc()
			{if (Right(Ptr) != Nil)
				Ptr = Min(Right(Ptr));
			else
				{Nodeptr P;
				while (Ptr == Right(P = Parent(Ptr)))
					Ptr = P;
				if (Right(Ptr) != P)
					Ptr = P; }}
		Nodeptr Mynode() const
			{return (Ptr); }
	protected:
		Nodeptr Ptr;
		};
	typedef reverse_iterator<const_iterator>
		const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;
	typedef pair<iterator, bool> Pairib;
	typedef pair<iterator, iterator> Pairii;
	typedef pair<const_iterator, const_iterator> Paircc;
	explicit Tree(const Pr& Parg, bool Marg, const A& Al)
		: allocator(Al),
///		Myal(Al), Myalp(Al),
		key_compare(Parg), Multi(Marg)
		{Init(); }
	Tree(const Ty *F, const Ty *L,
		const Pr& Parg, bool Marg, const A& Al)
		: allocator(Al),
///		Myal(Al), Myalp(Al),
		key_compare(Parg), Multi(Marg)
		{Init();
		insert(F, L); }
	Tree(const Myt& X)
		: allocator(X.allocator),
///		Myal(X.allocator), Myalp(X.allocator),
		key_compare(X.key_compare), Multi(X.Multi)
		{Init();
		Copy(X); }
	~_Tree()
		{erase(begin(), end());
		Freenode(Head);
		Head = 0, Size = 0;
			{Lockit Lk;
			if (--_Nilrefs == 0)
				{Freenode(Nil);
				Nil = 0; }}}
	Myt& operator=(const Myt& X)
		{if (this != &X)
			{erase(begin(), end());
			key_compare = X.key_compare;
			Copy(X); }
		return (*this); }
	iterator begin()
		{return (iterator(Lmost())); }
	const_iterator begin() const
		{return (const_iterator(Lmost())); }
	iterator end()
		{return (iterator(Head)); }
	const_iterator end() const
		{return (const_iterator(Head)); }
	reverse_iterator rbegin()
		{return (reverse_iterator(end())); }
	const_reverse_iterator rbegin() const
		{return (const_reverse_iterator(end())); }
	reverse_iterator rend()
		{return (reverse_iterator(begin())); }
	const_reverse_iterator rend() const
		{return (const_reverse_iterator(begin())); }
	size_type size() const
		{return (Size); }
	size_type max_size() const
		{return (allocator.max_size()); }
	bool empty() const
		{return (size() == 0); }
	A get_allocator() const
		{return (allocator); }
	Pr key_comp() const
		{return (key_compare); }
	Pairib insert(const value_type& V)
		{Nodeptr X = Root();
		Nodeptr Y = Head;
		bool Ans = true;
		while (X != Nil)
			{Y = X;
			Ans = key_compare(Kfn()(V), Key(X));
			X = Ans ? Left(X) : Right(X); }
		if (Multi)
			return (Pairib(Insert(X, Y, V), true));
		iterator P = iterator(Y);
		if (!_Ans)
			;
		else if (P == begin())
			return (Pairib(Insert(X, Y, V), true));
		else
			--_P;
		if (key_compare(Key(P.Mynode()), Kfn()(V)))
			return (Pairib(Insert(X, Y, V), true));
		return (Pairib(P, false)); }
	iterator insert(iterator P, const value_type& V)
		{if (size() == 0)
			;
		else if (P == begin())
			{if (key_compare(Kfn()(V), Key(P.Mynode())))
				return (Insert(Head, P.Mynode(), V)); }
		else if (P == end())
			{if (key_compare(Key(Rmost()), Kfn()(V)))
				return (Insert(Nil, Rmost(), V)); }
		else
			{iterator Pb = P;
			if (key_compare(Key((--_Pb).Mynode()), Kfn()(V))
				&& key_compare(Kfn()(V), Key(P.Mynode())))
				if (Right(Pb.Mynode()) == Nil)
					return (Insert(Nil, Pb.Mynode(), V));
				else
					return (Insert(Head, P.Mynode(), V)); }
		return (insert(V).first); }
	void insert(iterator F, iterator L)
		{for (; F != L; ++F)
			insert(*F); }
	void insert(const value_type *F, const value_type *L)
		{for (; F != L; ++F)
			insert(*F); }
	iterator erase(iterator P)
		{Nodeptr X;
		Nodeptr Y = (P++).Mynode();
		Nodeptr Z = Y;
		if (Left(Y) == Nil)
			X = Right(Y);
		else if (Right(Y) == Nil)
			X = Left(Y);
		else
			Y = Min(Right(Y)), X = Right(Y);
		if (Y != Z)
			{Parent(Left(Z)) = Y;
			Left(Y) = Left(Z);
			if (Y == Right(Z))
				Parent(X) = Y;
			else
				{Parent(X) = Parent(Y);
				Left(Parent(Y)) = X;
				Right(Y) = Right(Z);
				Parent(Right(Z)) = Y; }
			if (Root() == Z)
				Root() = Y;
			else if (Left(Parent(Z)) == Z)
				Left(Parent(Z)) = Y;
			else
				Right(Parent(Z)) = Y;
			Parent(Y) = Parent(Z);
			STD swap(Color(Y), Color(Z));
			Y = Z; }
		else
			{Parent(X) = Parent(Y);
			if (Root() == Z)
				Root() = X;
			else if (Left(Parent(Z)) == Z)
				Left(Parent(Z)) = X;
			else
				Right(Parent(Z)) = X;
			if (Lmost() != Z)
				;
			else if (Right(Z) == Nil)
				Lmost() = Parent(Z);
			else
				Lmost() = Min(X);
			if (Rmost() != Z)
				;
			else if (Left(Z) == Nil)
				Rmost() = Parent(Z);
			else
				Rmost() = Max(X); }
		if (Color(Y) == Black)
			{while (X != Root() && Color(X) == Black)
				if (X == Left(Parent(X)))
					{Nodeptr W = Right(Parent(X));
					if (Color(W) == Red)
						{Color(W) = Black;
						Color(Parent(X)) = Red;
						Lrotate(Parent(X));
						W = Right(Parent(X)); }
					if (Color(Left(W)) == Black
						&& Color(Right(W)) == Black)
						{Color(W) = Red;
						X = Parent(X); }
					else
						{if (Color(Right(W)) == Black)
							{Color(Left(W)) = Black;
							Color(W) = Red;
							Rrotate(W);
							W = Right(Parent(X)); }
						Color(W) = Color(Parent(X));
						Color(Parent(X)) = Black;
						Color(Right(W)) = Black;
						Lrotate(Parent(X));
						break; }}
				else
					{Nodeptr W = Left(Parent(X));
					if (Color(W) == Red)
						{Color(W) = Black;
						Color(Parent(X)) = Red;
						Rrotate(Parent(X));
						W = Left(Parent(X)); }
					if (Color(Right(W)) == Black
						&& Color(Left(W)) == Black)
						{Color(W) = Red;
						X = Parent(X); }
					else
						{if (Color(Left(W)) == Black)
							{Color(Right(W)) = Black;
							Color(W) = Red;
							Lrotate(W);
							W = Left(Parent(X)); }
						Color(W) = Color(Parent(X));
						Color(Parent(X)) = Black;
						Color(Left(W)) = Black;
						Rrotate(Parent(X));
						break; }}
			Color(X) = Black; }
		Destval(&Value(Y));
		Freenode(Y);
		--_Size;
		return (P); }
	iterator erase(iterator F, iterator L)
		{if (size() == 0 || F != begin() || L != end())
			{while (F != L)
				erase(F++);
			return (F); }
		else
			{Erase(Root());
			Root() = Nil, Size = 0;
			Lmost() = Head, Rmost() = Head;
			return (begin()); }}
	size_type erase(const K& X)
		{Pairii P = equal_range(X);
		size_type N = 0;
		Distance(P.first, P.second, N);
		erase(P.first, P.second);
		return (N); }
	void erase(const K *F, const K *L)
		{for (; F != L; ++F)
			erase(*F); }
	void clear()
		{erase(begin(), end()); }
	iterator find(const K& Kv)
		{iterator P = lower_bound(Kv);
		return (P == end()
			|| key_compare(Kv, Key(P.Mynode()))
				? end() : P); }
	const_iterator find(const K& Kv) const
		{const_iterator P = lower_bound(Kv);
		return (P == end()
			|| key_compare(Kv, Key(P.Mynode()))
				? end() : P); }
	size_type count(const K& Kv) const
		{Paircc Ans = equal_range(Kv);
		size_type N = 0;
		Distance(Ans.first, Ans.second, N);
		return (N); }
	iterator lower_bound(const K& Kv)
		{return (iterator(Lbound(Kv))); }
	const_iterator lower_bound(const K& Kv) const
		{return (const_iterator(Lbound(Kv))); }
	iterator upper_bound(const K& Kv)
		{return (iterator(Ubound(Kv))); }
	const_iterator upper_bound(const K& Kv) const
		{return (iterator(Ubound(Kv))); }
	Pairii equal_range(const K& Kv)
		{return (Pairii(lower_bound(Kv), upper_bound(Kv))); }
	Paircc equal_range(const K& Kv) const
		{return (Paircc(lower_bound(Kv), upper_bound(Kv))); }
	void swap(Myt& X)
		{STD swap(key_compare, X.key_compare);
		if (allocator == X.allocator)
			{STD swap(Head, X.Head);
			STD swap(Multi, X.Multi);
			STD swap(Size, X.Size); }
		else
			{Myt Ts = *this; *this = X, X = Ts; }}
	friend void swap(Myt& X, Myt& Y)	///
		{X.swap(Y); }	///
protected:
	static Nodeptr Nil;
	static size_t Nilrefs;
	void Copy(const Myt& X)
		{Root() = Copy(X.Root(), Head);
		Size = X.size();
		if (Root() != Nil)
			{Lmost() = Min(Root());
			Rmost() = Max(Root()); }
		else
			Lmost() = Head, Rmost() = Head; }
	Nodeptr Copy(Nodeptr X, Nodeptr P)
		{Nodeptr R = X;
		for (; X != Nil; X = Left(X))
			{Nodeptr Y = Buynode(P, Color(X));
			if (R == X)
				R = Y;
			Right(Y) = Copy(Right(X), Y);
			Consval(&Value(Y), Value(X));
			Left(P) = Y;
			P = Y; }
		Left(P) = Nil;
		return (R); }
	void Erase(Nodeptr X)
		{for (Nodeptr Y = X; Y != Nil; X = Y)
			{Erase(Right(Y));
			Y = Left(Y);
			Destval(&Value(X));
			Freenode(X); }}
	void Init()
		{	{Lockit Lk;
			if (Nil == 0)
				{Nil = Buynode(0, Black);
				Left(Nil) = 0, Right(Nil) = 0; }
			++Nilrefs; }
		Head = Buynode(Nil, Red), Size = 0;
		Lmost() = Head, Rmost() = Head; }
	iterator Insert(Nodeptr X, Nodeptr Y, const Ty& V)
		{Nodeptr Z = Buynode(Y, Red);
		Left(Z) = Nil, Right(Z) = Nil;
		Consval(&Value(Z), V);
		++Size;
		if (Y == Head || X != Nil
			|| key_compare(Kfn()(V), Key(Y)))
			{Left(Y) = Z;
			if (Y == Head)
				{Root() = Z;
				Rmost() = Z; }
			else if (Y == Lmost())
				Lmost() = Z; }
		else
			{Right(Y) = Z;
			if (Y == Rmost())
				Rmost() = Z; }
		for (X = Z; X != Root()
			&& Color(Parent(X)) == Red; )
			if (Parent(X) == Left(Parent(Parent(X))))
				{Y = Right(Parent(Parent(X)));
				if (Color(Y) == Red)
					{Color(Parent(X)) = Black;
					Color(Y) = Black;
					Color(Parent(Parent(X))) = Red;
					X = Parent(Parent(X)); }
				else
					{if (X == Right(Parent(X)))
						{X = Parent(X);
						Lrotate(X); }
					Color(Parent(X)) = Black;
					Color(Parent(Parent(X))) = Red;
					Rrotate(Parent(Parent(X))); }}
			else
				{Y = Left(Parent(Parent(X)));
				if (Color(Y) == Red)
					{Color(Parent(X)) = Black;
					Color(Y) = Black;
					Color(Parent(Parent(X))) = Red;
					X = Parent(Parent(X)); }
				else
					{if (X == Left(Parent(X)))
						{X = Parent(X);
						Rrotate(X); }
					Color(Parent(X)) = Black;
					Color(Parent(Parent(X))) = Red;
					Lrotate(Parent(Parent(X))); }}
		Color(Root()) = Black;
		return (iterator(Z)); }
	Nodeptr Lbound(const K& Kv) const
		{Nodeptr X = Root();
		Nodeptr Y = Head;
		while (X != Nil)
			if (key_compare(Key(X), Kv))
				X = Right(X);
			else
				Y = X, X = Left(X);
		return (Y); }
	Nodeptr& Lmost()
		{return (Left(Head)); }
	Nodeptr& Lmost() const
		{return (Left(Head)); }
	void Lrotate(Nodeptr X)
		{Nodeptr Y = Right(X);
		Right(X) = Left(Y);
		if (Left(Y) != Nil)
			Parent(Left(Y)) = X;
		Parent(Y) = Parent(X);
		if (X == Root())
			Root() = Y;
		else if (X == Left(Parent(X)))
			Left(Parent(X)) = Y;
		else
			Right(Parent(X)) = Y;
		Left(Y) = X;
		Parent(X) = Y; }
	static Nodeptr Max(Nodeptr P)
		{while (Right(P) != Nil)
			P = Right(P);
		return (P); }
	static Nodeptr Min(Nodeptr P)
		{while (Left(P) != Nil)
			P = Left(P);
		return (P); }
	Nodeptr& Rmost()
		{return (Right(Head)); }
	Nodeptr& Rmost() const
		{return (Right(Head)); }
	Nodeptr& Root()
		{return (Parent(Head)); }
	Nodeptr& Root() const
		{return (Parent(Head)); }
	void Rrotate(Nodeptr X)
		{Nodeptr Y = Left(X);
		Left(X) = Right(Y);
		if (Right(Y) != Nil)
			Parent(Right(Y)) = X;
		Parent(Y) = Parent(X);
		if (X == Root())
			Root() = Y;
		else if (X == Right(Parent(X)))
			Right(Parent(X)) = Y;
		else
			Left(Parent(X)) = Y;
		Right(Y) = X;
		Parent(X) = Y; }
	Nodeptr Ubound(const K& Kv) const
		{Nodeptr X = Root();
		Nodeptr Y = Head;
		while (X != Nil)
			if (key_compare(Kv, Key(X)))
				Y = X, X = Left(X);
			else
				X = Right(X);
		return (Y); }
///	Nodeptr Buynode(Nodeptr Parg, Redbl Carg)
///		{Nodeptr S = Myal.allocate(1, (void *)0);
///		Myalp.construct(&Acc::Parent(S), Parg);
///		Myalp.construct(&Acc::Color(S), Carg);
///		Myalp.construct(&Acc::Left(S), 0);
///		Myalp.construct(&Acc::Right(S), 0);
///		return (S); }
///	void Consval(Tptr P, const Ty& V)
///		{allocator.construct(P, V); }
///	void Destval(Tptr P)
///		{allocator.destroy(P); }
///	void Freenode(Nodeptr S)
///		{Myalp.destroy(&Acc::Parent(S));
///		Myalp.destroy(&Acc::Color(S));
///		Myalp.destroy(&Acc::Left(S));
///		Myalp.destroy(&Acc::Right(S));
///		Myal.deallocate(S, 1); }
///	A::rebind<Node>::other Myal;
///	A::rebind<Nodeptr>::other Myalp;
	Nodeptr Buynode(Nodeptr Parg, Redbl Carg)	///
		{Nodeptr S = (Nodeptr)allocator.Charalloc(	///
			1 * sizeof (Node));	///
		Parent(S) = Parg;	///
		Color(S) = Carg;	///
		return (S); }	///
	void Consval(Tptr P, const Ty& V)	///
		{Construct(&*P, V); }	///
	void Destval(Tptr P)	///
		{Destroy(&*P); }	///
	void Freenode(Nodeptr S)	///
		{allocator.deallocate(S, 1); }	///
	A allocator;
	Pr key_compare;
	Nodeptr Head;
	bool Multi;
	size_type Size;
	};
