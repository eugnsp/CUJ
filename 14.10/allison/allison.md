# C++: The Making of a Standard

**Chuck Allison**

*Chuck Allison is a consulting editor (and former columnist) with CUJ and a Senior Software Engineer in the Information and Communication Systems Department of the Church of Jesus Christ of Latter Day Saints in Salt Lake City. He has a B.S. and M.S. in mathematics, has been programming since 1975, and has been teaching and developing in C since 1984. His current interest is object-oriented technology and education. He is a member of X3J16, the ANSI C++ Standards Committee. Chuck can be reached on the Internet at cda@burgoyne.com.*

---

> So far, C++ has escaped the fate of the Vasa; it has not keeled over and disappeared despite much wishful thinking and many dire predictions.

When I attended the March ’96 meeting of the joint C++ Standards Committee (ANSI X3J16/ISO WG21) in Santa Cruz, I found that things hadn’t changed too much in the two years since I had last attended. There were mostly familiar faces, and the talk mostly focused on “When will we get this thing done”? As far as the features that most programmers care about, the specification of the language and library is complete. The first Committee Draft (CD), a formal public review of the working paper which essentially froze the features of the language, took place in the spring of 1995.

The committee took the rest of that year responding to the several thousand comments they received. At the July ’96 meeting in Stockholm, Sweden, the committee voted a final feature freeze. The November ’96 meeting in Kona, Hawaii should result in a second CD. If that meets with general approval from national standards bodies throughout the world, we should see an official C++ standard sometime in early 1998.

To appreciate the work that has gone into the standard, as well as the interview that follows, a little history is in order. For a detailed technical and anecdotal history of C++, see Bjarne Stroustrup, *The Design and Evolution of C++* (Addison-Wesley, 1994). Also known as “D&E”, it is a must-read.

Bjarne Stroustrup, a Dane with a Ph.D. from Cambridge University (England), had used the Simula language for distributed systems simulations in his research. He was disappointed with its poor performance, however, and in 1979 when his new employer, AT&T Bell Labs, said “Do something interesting”, he decided to infuse the C language with some Simula features he had grown accustomed to – most notably classes – and thus “C with Classes” was born. It caught on within AT&T, was dubbed C++, and then proceeded to become a support burden for its inventor.

After the first edition of Stroustrup’s *The C++ Programming Language*(Addison-Wesley, 1985) hit the shelves, however, there was no turning back -- the language just became too popular. And as you might expect, multiple implementations appeared, each with some of its own special features. About the time the C standard became official, the major players in the C++ community were pushing for a C++ standard. ANSI committee X3J16 met for the first time in December 1989, with Dmitry Lenkov of Hewlett-Packard as chair. (Steve Clamage, of Taumetric fame, and now at Sun Microsystems, is the current chair.)

The base documents included the ISO C standard, and Ellis & Stroustrup’s *The Annotated C++ Reference Manual* (the ARM, for short). The latter pretty much reflected AT&T C++ 2.0, along with Bjarne’s ideas for extensions (mainly templates and exceptions). The main goals at the outset were to standardize iostreams, and to add templates and exceptions to the language. Due to the large number of non-US participants, the committee voted to work jointly with ISO Working Group 21 at the Lund, Sweden meeting in June 1991.

It looked like things were winding down at the end of 1993, yet there was still some uneasiness about a lack of robustness in the standard library. At the San Jose meeting in November 1993, Alex Stepanov, then an HP employee, gave a presentation on generic programming which really put templates to good use. By the San Diego meeting the next March, he had refined his and Meng Lee’s “Standard Template Library” (STL) to the point that the committee was ready to seriously consider it, even if it meant delaying the completion of the standard. I remember being one of the conservative skeptics at the time, yet my hand went up in favor of STL, which, along with iostreams, is now the centerpiece of the standard C++ library.

The major features added since C++ 2.0 include templates, exceptions, user namespaces, and run-time type identification (RTTI). They are “major” because they directly affect the overall structure of your programs. Minor features, which are less intrusive but very powerful in their own right, include new-style casts, a new Boolean data type, being able to overload on enumerated types, support for wide characters, and alternative tokens to support foreign keyboards (e.g., `or` for `||`). In addition to STL, the standard library includes renovated stream classes, a string class template with specializations for wide and narrow characters, and the infrastructure for RTTI and overloading and overriding operator new and operator delete.

The major topic of debate at the Santa Cruz meeting was the template compilation model. Bjarne originally defined templates so that their definitions could reside in separate compilation units, just like ordinary functions can. Compiler vendors soon discovered, however, that it was often easier to instantiate and optimize templates that were defined totally within an include file. These two approaches are known respectively as the separation and inclusion models of template compilation.

Certain individuals found some problems with the separation model, and in Santa Cruz proposed that we drop it altogether from the language. Although it passed a straw vote handily, it didn’t go over very well with enough people (including the star of this article – see below) that the issue tabled the vote to release a second CD to the next meeting (Stockholm, July ’96). A very promising compromise, authored by voices on both sides of the question, was the focus of the Stockholm meeting. Since that compromise was accepted, it looks like the end of the standards effort is (finally) upon us.

Over 100 talented professionals have contributed directly to the standard, working out the relentless details of fitting new features into a complex language in a mutually supporting way. Through all the challenges, both technical and political, Bjarne Stroustrup has been a force for reason and civility. I remember one time, when tempers were flaring a bit on the committee e-mail reflector, he sent out a vignette from the movie Dr. Strangelove, where military planners were in violent disagreement. The presiding officer firmly counseled, “gentlemen, you can’t fight here. This is the War Room”.

More than once Bjarne’s vision has brought things back to a sound footing. When I asked if he would write for this journal, he had to decline, due to a backlog of heavy commitments. He did, however, consent to an online interview, the transcript of which occupies the remainder of this article. Enjoy!

## Online interview

> I know you have a Ph.D. in Applied Math. What were your other degrees?

Not quite, my Masters degree (Cand. Scient.) from the University of Aarhus (Denmark) is in “Mathematics with computer science”. I took the math part because that was the only way to do computer science there in those days. My Ph.D. from Cambridge (England, of course) is in Computer Science. I’m a very poor mathematician, but I guess that’s better than not being one at all.

> How did you get into computing?

By signing up for math and computer science at the university. I have tried to remember why I did that, but I really don’t know. I certainly hadn’t seen a computer by the time I signed up. I guess the combination of scientific and practical aspects attracted me.

> When/how did you notice that C with Classes was becoming something that was going to consume much of your time, if not your career (in other words, not just a temporary interest)?

Somewhere in 1982, I realized that support of the C with Classes user community was becoming an unacceptable drain on me. As I saw it, the user population had become too large for one person to serve well while also doing research, but too small to support an infrastructure. I had to decide whether to develop C with Classes into a more flexible and expressive language, or to drop it. Fortunately, I didn’t think of a third alternative – which people point out is the conventional solution – which is to gather more users through hype. Through C with Classes, I decided to further develop the combination of valuable ideas from C and Simula that I had experienced. The result was C++. I tried to “escape” to other work on several later occasions, but always the growth of the C++ community and the challenges of new applications kept me involved.

> What do you do for your day job now?

I’m trying to build up a research group to focus on large-scale programming – that is, to do research on the use of programming in large programs rather than just language design, just the study of small (student) programs, or the exclusive focus on design and/or process. I think that programming technique, programming language, and the individual programmer have a central role in the development of large systems. Too often, either the scale of industrial projects or the role of programming is ignored. This research will involve work on libraries and tools.

Naturally, C++ will play a major role here, but I have no plans to limit my interests to just C++. My department is part of AT&T Research. In the breakup of AT&T, AT&T kept half of the information sciences research of the old AT&T Bell Labs (the other half and the name went to Lucent Technologies). The AT&T part is now called AT&T Research and we aim to be a research organization second to none. To a considerable degree, C++ is still my “day job”. I work on standards issues, help users, write papers, give talks, write books, etc. Occasionally, I even get to write some code – though not as much as I’d like.

> What would you like to accomplish before you “retire”.

Retire from C++ development, or for real? I’m too young to retire, so I’ll assume that you mean from C++. If the Stockholm meeting of the standards committee accomplishes what I hope it will, I will consider the language and its standard library complete and sound so that that phase of the evolution is over with. Unless something goes seriously wrong (and by the time you publish this we will know), ISO C++ will be a better approximation of my ideals than any earlier version. The improvements to templates and the inclusion of the STL into the standard library were the key to getting C++ to where I wanted it.

> What do you hope will be accomplished in Stockholm? What could go seriously wrong, and what would be the impact, in your opinion?

I was referring to the problems with separate compilation of templates. The ARM, my 2nd edition, and the original draft standard working paper allowed separate compilation of templates and disallowed the inclusion of a non-inline template functions in more than one translation unit. This led to implementation inefficiencies and it became popular to include non-inline template function definitions in header files. Consequently the draft standard was amended to allow that approach also. The general problem and the original compromise is described in some detail in D&E.

In what I consider an excess of enthusiasm for the “simple” scheme where the user “simply” `#include`s all the information needed to do template instantiation wherever an implementation might need it, some members then proposed to ban separate compilation of templates. In Santa Cruz, that proposal was almost accepted (over my dead body) and it could still be accepted in Stockholm. *[It was not. – pjp]* That would be the first time the committee banned a fundamental feature.

The reasons given for banning separate compilation of templates have varied: hard to implement, leads to slow compiles, hard to use, hard to specify precisely, hard to give good error messages for, etc. The reasons for separate compilation are better separation of interface and implementation, maintaining the standard separate compilation model found in the rest of the language, potentially faster compilation of very large programs, and the fact that separate compilation has become part of every language used for large programs, independently of what its designer(s) thought ideal from a language-technical point of view.

Consider the program shown in Listing 1:

```cpp
// sum.c:

#define INCR 2
void trace(void*);
static int count;

template<class T> T sum(vector<T>& v)
{
    count += INCR;
    trace(v);
    T r = 0;
    for (int i = 0; i < v.size(); i++)
        r += v[i];
}
```

```cpp
// user.c (inclusion style)

#define trace my_tracer
#define INCR(v) (v)++
extern int count;

#include "sum.c"

void g(vector<int>& vi)
{
    int s = sum(vi);
}
```

Mind-boggling, eh? It is not what I would call good programming, but it is the kind of warped example that a standards committee must spend much time on. The legality and meaning of every program must be pinned down. Including the definition of `sum` into the user code has the effect that the declarations in `sum.c` interfere with the meaning of names used in `user.c` (e.g., `INCR` and `count`) and the declarations from `sum.c` interfere with the meaning of names used in the template declaration. For programs relying on large amounts of information in header files, all sorts of strange effects can result. In particular, there is no protection against macros when one `#include`s one file into another. The alternative is to use separate compilation to keep the template definition context as separate as possible from the context of its use, as shown in Listing 2:

```cpp
// user.c (separation style)

#define trace my_tracer
#define INCR(v) (v)++
extern int count;

template<class T> T sum(vector<T>& v);

void g(vector<int>& vi)
{
    int s = sum(vi);
}
```

Some of the problems with “name leakage” in the inclusion model can be compensated for through the use of namespaces, but not those caused by macros. The separation model requires attention to the problem of how names are selected from the contexts involved. However, the problems relating to context and name binding when composing a program out of templates from different source files are roughly equivalent to the problems you face composing the program out of templates in separate namespaces. This is natural because namespaces and translation units are two ways of expressing logical separation. The implementor must face similar challenges in the two cases.

Currently, the committee is deadlocked. However, work that I consider promising is in progress. The general idea is to simplify the lookup rules to the point where templates could even be put into DLLs. That roughly means that dependencies on the instantiation context should be expressed in terms of the global and namespace symbol tables, so that the actual source code of a translation unit need not be examined to instantiate a template – though potentially better code could be generated if it was. This is a tricky, but very important issue. I would consider a C++ without separate compilation of templates incomplete. I hope for a solution that is technically superior to both the current separation and inclusion models.

> What improvements to templates were you referring to, and why?

In the ARM, a rather restrictive version of templates was specified. I was worried about defining something that would be too hard to implement efficiently, so I imposed restrictions that I hoped we could later relax. Over the years such relaxations were made in response to problems found using templates. Consider a list:

```cpp
template<class T> class list { ... };
```

Unless most unusually clever, an implementation would replicate the code of `list` functions for each instantiation of `list`. For example:

```cpp
list<int> li;
list<complex> lc;
list<shape*> lps;
list<char*> lpc;
list<node*> lpn;
```

would generate five copies of the list code. This can be a real problem to projects. Having lists of dozens of different element types is not uncommon, and this can lead to serious code bloat. The solution that I have recommended from the very first paper on templates is to use a single shared implementation for all lists of pointers. For example:

```cpp
// specialization
template<> class list<void*> { ... };

class plist : private list<void*> { ... };
```

Now every list of pointers (`plist`) is implemented using a list of `void*` and there will be no code bloat. Unfortunately, the user now has to remember to distinguish between `list`s and `plist`s. Experience shows that programmers fail to do that and suffer code bloat as a result. What we need is a way to define a special (shared) implementation for lists of pointers without introducing a new name. That is we need to provide a special implementation for lists with template arguments that happen to be pointers. This is expressed using partial specialization:

```cpp
template<class T> class list<T*> :
    partial list<void*> { ... };
```

The `<T*>` means use this specialization for every instantiation for which the template argument is a pointer. This allows users to vary the implementation of lists without affecting the list interface. There are several such minor extensions to templates that together represent a major improvement to what we can express in C++. Here, I’ll just mention default template arguments. As an example consider sorting strings of arbitrary characters. A (too) simple version looks like this:

```cpp
template<class T>
int cmp(string<T>& a, string<T>& b)
{
    for (int i = 0; i < a.length() && i < b.length(); i++)
        if (a[i] != b[i])
		    return a[i] < b[i];
    return b.length() - a.length();
}
```

The problem is that `operator<` may not be defined for some character type `T`. Also for different string types, you might want to compare using different comparison criteria. An example would be comparing strings of ordinary characters with and without case sensitivity. Though the traditional and obvious solution is to make the comparison criteria a function argument, a more elegant and efficient method is to make the comparison criteria a template argument:

```cpp
template<class T, class Comp = CMP<T> >
int cmp(string<T>& a, string<T>& b)
{
    for (int i = 0; i < a.length() && i < b.length(); i++)
        if (!Comp::eq(a[i],b[i]))
		    return Comp::lt(a[i],b[i]);
	return b.length() - a.length();
}
```

Naturally, the default comparison criterion `CMP` would be defined to supply the usual comparison operators:

```cpp
template<class T> class CMP
{
public:
    static bool eq(const T& a, const T& b)
	{
	    return a == b;
	}

    static bool lt(const T& a, const T& b)
    {
        return a < b;
    }
};
```

A case-insensitive version might be defined as:

```cpp
template<class T> class ONE_CASE
{
public:
    static bool eq(const T& a, const T& b)
    {
        return toupper(a) == toupper(b);
    }

	static bool lt(const T& a, const T& b)
    {
        return toupper(a) < toupper(b);
    }
};
```

We could then write:

```cpp
string<char> sc;
string<char> sc2;

// case-sensitive comparison
int c = cmp(sc, sc2);

// case-insensitive comparison
int c = cmp<char, ONE_CASE>(sc, sc2);
```

The `cmp<char, ONE_CASE>` notation is explicit qualification of a template function call. When the template parameters are not explicitly specified, they are deduced from the call or taken from the defaults specified in the template declaration, thus

```cpp
cmp(sc, sc2);
```

is equivalent to
```cpp
cmp<char, CMP<char> >(sc, sc2);
```

The reason it is better to supply “policies” through template parameters than through function arguments is that operations supplied through a template parameter are trivially inlined. I guess the main point is that these extensions are minor, yet they provide direct support for fundamental programming and design notions such as providing a variety of suitable implementations without affecting the user interface and providing parameterization without requiring the user to take any special action in the most common case.

> How would you summarize the impact/benefit of STL?

My greatest hope is that it will teach many more people to use C++ as a high-level language rather than a glorified assembly language. My aim with C++ was – and is – to raise the level of abstraction in code so that the program text directly reflect the concepts of the application and general computer science concepts wherever reasonable. Yet, far too many people get lost fiddling with bits and pointers. The standard library allows people to take `string`, `vector`, `list`, and `map` as given and delay worrying about C-style strings and the pointer-level implementation of basic data structures until necessary.

> You have often used the story of the Vasa to encourage taking the simple route in defining the C++ language. But it is pretty much common public opinion that C++ is one the most complicated languages in existence. Any comment on that? If there are different “levels” of C++ usage, how would you characterize them?

Clearly there was a danger; why else would I bother telling a cautionary tale? Construction of the Vasa started in 1625, at the request of King Gustav. It was originally intended to be a regular warship, but during the construction the king saw bigger and better ships elsewhere and changed his mind. He insisted on a flagship with two gundecks; he also insisted on a lot of statues suitable for a Royal flagship. The result was (and is) very impressive. Rather top heavy, though. On its maiden voyage the Vasa only made it half way across Stockholm harbor before a gust of wind blew it over; it sank killing about 50 people.

It has been raised and you can now see it in a museum in Stockholm. It is a beauty to behold – far more beautiful at the time than its unextended first design and far more beautiful today than if it had suffered the usual fate of a 17th century battle ship – but that is no consolation to its designer, builders, and intended users.

So far, C++ has escaped the fate of the Vasa; it has not keeled over and disappeared – despite much wishful thinking and many dire predictions. Some of the newer “extensions” are seen as generalizations that actually make life easier for C++ programmers. My programs have certainly become shorter, clearer, and easier to write over the years. I simply don’t have to go through as many contortions to express good design notions as I used to. Naturally, if your view is that the best programming language is the one that lets a novice get a demo running fastest, C++ doesn’t fare so well, because it certainly takes longer to master. However, my primary concern is production code, and in competent hands, C++ is a delight.

ISO C++ is a closer approximation to my ideas than the C++ described in the ARM. It might be worth noting that in one respect King Gustav was right: had the Vasa as originally designed come alongside a “modern” two-gundeck battleship, it would have reached the bottom almost as fast as the Vasa actually built did. The flaw wasn’t to add a gundeck; that gundeck was essential for the Vasa to fulfill its mission. The fault was to add that gundeck in the wrong way. Given that perspective, we might also take a kinder view of the extra time the committee took to make sure that the extensions to C++ were properly designed and tried out.

C++ is of course quite complex when compared to, say the original Pascal, but so is every other modern programming language. Compared to the programming environments we use and the systems we program for, however, C++ is still simple. For a novice, it would be a serious mistake to try to first learn all of C++ and then start using it. Instead, it is best to start with a subset and expand one’s repertoire as needed. I recommend starting with a C-like subset, but using the standard `vector`, `list`, `map`, and `string` classes instead of fiddling with C-style arrays and strings. Naturally, macros should be avoided and also unions and bitfields. However, you can write very nice code with that subset of C and C++ plus the four standard library classes I mentioned.

Soon after, one can start experimenting with simple classes, simple templates, and simple class hierarchies. I recommend focusing on abstract classes before designing any non-trivial class hierarchy. Very simple exception handling can also be useful early on. The most important thing to keep in mind is to focus on the problem to be solved, rather than the language-technical aspects of C++. It is always easier to learn a language if you have an experienced – and non-prejudiced – friend or colleague to help.

> What is the next step in the evolution of C++?

Tools/environments and library design. I’d like to see incremental compilers and linkers for C++. Something like two seconds is a suitable time for re-compiling and re-linking a medium-sized C++ program after a change that is localized to a few functions. I’d like to see browsers and analysis tools that know not only syntax but also the type of every entity of a program. I’d like to see optimizers that actually take notice of C++ constructs and do a decent job of optimizing them, rather than simply throwing most of the useful information away and giving the rest to an optimizer that basically understands only C. I’d like to see debuggers integrated with the incremental compiler so that the result approximates a C++ interpreter. (I’d also like to see a good portable C++ interpreter.) None of this is science fiction; in fact, I have seen experimental versions of most of what I suggest – and more. We still suffer from first-generation C++ environments and tools.

> Certainly a large part of C++’s success is that it was built on C. (We’re all familiar with the phrase “As close as possible to C, but no closer”, which is a twist on Einstein’s “As simple as possible, but no simpler”.) But C compatibility has certainly been a challenge, and has brought about a number of compromises as the upcoming ISO standard has evolved. What are the major compromises that you recall?

I think most of the compromises were in place long before the standards committee was convened. (See my answer to the next question). The C++ specific major features such as classes, namespaces, templates, and exceptions were constrained by a wish to be able to generate very compact and efficient code, and to coexist with code written in other languages, rather than anything C-specific. Actually, most “compromises” can be seen as driven by the “zero-overhead principle”, which says that any feature you don’t use shouldn’t cost you anything in time or space. That’s what keeps C++ a viable systems programming language, and what has kept it from evolving towards something more convenient for toy examples, but less useful as a tool for everyday programming.

> Suppose for a moment that C compatibility was not an issue. How would C++ be different, in your view?

That is not really a fair question because “As close to C as possible, but no closer” really was a basic design aim, delivering definite technical benefits to C++ users. It was not just a political or commercial (advertising) decision. Had C not been there and been more than adequate for my needs, I would have found some other language to be compatible with. I see no virtue in designing yet another Algol-family language.

Also, the sheer effort of finding and maintaining the appropriate degree of C compatibility is not to be underestimated. C compatibility was one of the hardest decisions to make and put into practice, in addition to being one of the most important. However, there are many aspects of C that I’m no fan of, so had there been a language as efficient, flexible, and available as C without those features, I would have chosen to be compatible with that. For example, I consider the C declaration syntax an experiment that failed, and in general, the C syntax for declarations is too lenient. Note that C++ has dropped the “implicit `int`” rule:

```c
static T;
```

is no longer valid C++; you must write

```cpp
static int T;
```

if you really mean to declare an integer.

There are many such details that make life unnecessarily hard for the compiler writer and for the casual reader of real life C and C++ code (as opposed to trivial examples like the one above). I suspect my dislike for the preprocessor is well known. Cpp is essential in C programming, and still important in conventional C++ implementations, but it is a hack, and so are most of the techniques that rely on it. It has been my long-term aim to make Cpp redundant. However, I wouldn’t dream of banning it until long after it had been made genuinely redundant – and that hasn’t happened yet.

Templates, consts, inlines, and namespaces make most uses of macros redundant (and counterproductive), but we have no widely available alternative to several common uses of `#ifdef` (so far). The preprocessor is one of the main factors that has led to the lack of more sophisticated C program development environments: the fact that the source text seen by the programmer isn’t the text seen by the compiler is a crippling handicap. I think the time has come to be serious about macro-free C++ programming.

I also find C-style arrays too low-level for most uses. However, I don’t consider C’s success an accident. It was – and often still is – superior to alternatives in enough ways to be the right choice for many projects. Except, of course, when the alternative is C++. I have never found an application where C was a better choice than C++ for any reason but the lack of availability of a C++ compiler. If there is some non-C feature of C++ that someone finds unsuitable for a project, it can simply go unused there.

> What is your take on the Java revolution?

What Java revolution? Java is (at least) two distinct things: A fairly standard modern programming language with a superficial resemblance to C++, and a rather interesting system for downloading code into other people’s computers when prompted through their web browsers. The latter addresses hard, interesting, and important problems. If the serious security problems with Java and Javascript can be solved, this could be very important. It could be very important even if the security holes are left open, because it seems that most people generally don’t care about security anyway.

I guess you meant to ask me about Java as a programming language and its relationship to C++, but I’d rather not say too much about that, because language comparisons that are not based on significant user experience are rarely fair. In light of your previous question, I will point out, though, that Java is certainly not the language I would have designed if I had had no compatibility constraints. It is amazing, though, what Sun marketing dollars have been able to achieve. That is a lesson that will not be forgotten, and one that is ominous to the individual programmer, the small company, and the academic. If people insist on comparing C++ and Java – as they seem to do – I suggest they look through D&E to see why C++ is the way it is, and consider both languages in the light of the design criteria I set for C++. The differences between C++ and Java are more than skin-deep, and not every advantage goes to the same language.

> In your inaugural address to X3J16 in December, 1989, you stated that if the committee took more than five years to come up with a standard, it will have failed. It looks it’s going to be almost eight years when all is said and done. Any comment?

I guess some “weasel wording” is in order because I think that the committee has done a great job – even if it has taken too long to do it. I seriously underestimated the time it takes to create a consensus among an impressively diverse membership. However, look back a couple of years, say to March 1995, when my five-year time allotment ran out. Every major language feature and every major standard library part were in place. Unless something nasty happens with template compilation, we could – if we wanted to take a sympathetic view – write the last three years off to polishing and ISO rules. One thing that I had hoped for, and that actually happened, was that the implementations started reflecting the standard long before the ink was dry. Much of the good work of the standards work is already in the hands of C++ programmers.

> How will the existence of a standard change things in the C++ community? And for you personally?

For the community: stability, better compilers, better tools, better libraries, better teaching materials, and better techniques. For me: finally a chance to use C++ as I meant to use it, without being distracted by standards work and language design concerns. The important and interesting topic is programming, not programming languages. I find far too many people belonging to one of two camps: The ones who think that programming languages are of no importance, and only get in the way of systems builders (many C programmers are in this camp), and those who think that a programming language can perform miracles for them, if only some very specific, language-technical aspects are “just right”. (They never are, so all efforts are expended on designing the perfect programming language.)

I belong to a third camp: I know that a good language can be most helpful for an individual programmer and for a group, but that much more good code has been written in languages deemed bad than in languages proclaimed great! What matters most of all is the programmer’s understanding of the problem to be solved and the techniques needed to solve it; a programming language can help express clear ideas and even help clarify ideas that are “almost right” by providing a suitable framework. I think C++ does that for a wider range of applications than any other current language, and that C++ will become an even better tool if people do themselves the favor of taking the time to learn the techniques made possible by the features provided by ISO C++. However, without a clear idea of what you are doing and how, you’re lost in whatever language you choose to program in.

## Postscript

As this article goes to press, the C++ committee meeting in Stockholm has already occurred, with generally encouraging results. To answer a few questions raised by this interview, Bjarne offers this commentary on the Stockholm meeting:

> In Stockholm, the committee decided on a proposal for cleaning up lookup rules and dependency rules for templates that leaves the language simpler and better specified. This, in the majority’s opinion, answered the concerns raised about separate compilation of templates, so separate compilation need not be banned. I’m very happy about this decision (which was 6-to-1 among the ISO national delegations). Most of the credit for this resolution goes to people at Silicon Graphics (SGI), who initiated the work that lead to the resolution, and worked hard on the details.
>
> Many people felt that the large number of issues resolved in Stockholm – the library working groups were particularly productive – necessitated a delay in the vote for CD, since the Working Paper resulting from the meeting would contain so many changes. Consequently, the committee didn’t vote to submit the (yet to be composed) document as the CD in Stockholm, but it did  vote unanimously to submit the document produced by incorporating the Stockholm resolutions and their resulting editorial adjustments as a CD at the November meeting in Hawaii, without any further technical changes to the language or library.
>
> What this means for the schedule is not easy to estimate, because that depends on interlocking schedules of other groups and committees (such as ANSI and ISO), but estimates vary from none at all to 4 months delay. What matters in my mind is that we – finally – know exactly what the C++ language and standard library consists of. Naturally, improvements could still be made, but I’m very happy with what we have now, and it “time to ship”. As the saying goes: “the best is the enemy of the good”, and although what we have now isn’t necessarily perfect, it is very good. I’m looking forward to a period of stability and productivity.
>
> *Bjarne Stroustrup*
