# The Column that Needs a Name: Understanding C++ Declarations

**Dan Saks**

*Dan Saks is the president of Saks&Associates, which offers consulting and training in C++ and C. He is secretary of the ANSI and ISO C++ committees. Dan is coauthor of C++ Programming Guidelines, and codeveloper of the Plum Hall Validation Suite for C++ (both with Thomas Plum). You can reach him at 393 Leander Dr., Springfield OH, 45504-4906, by phone at (513)324-3601, or electronically at dsaks@wittenberg.edu.*

---

Last month, I began an in-depth look at the structure of C++ declarations. (See “[The Column That Needs a Name: Understanding C++ Declarations](../../13.12/saks/saks.htm)”, *CUJ*, December 1995). This month, I continue by examining the most perplexing part of a declaration, namely, the declarator. My objective here is to get you to the point where you will never again look at an error message that says *“declaration syntax error”* and react by mindlessly throwing in a `*` or pair of parentheses in the hope that the error will go away. I’m not sure we can get to “never”, but maybe we can get to “rarely”.

## Declarators in Declarations

I’ll begin by reviewing the role of declarators in declarations. As always, I will use EBNF (Extended Backus-Naur Form) to describe syntax rules:

```
Each Production has the form
N = E . where means “an N is defined to be a E”

E is an expression which may contain the following operators:
X | Y   means “either an X or a Y”
[ X ]   means 0 or 1 occurrence(s) of an X”
{ X }   means “0 or more occurrences of an X”

also
( )     group sub-expressions
" "     enclose a terminal symbol
```

The C++ Draft Standard uses the term *simple-declaration* to describe the dominant form for object and function declarations. For example,
```cpp
static const char *table[N];
```
is a *simple-declaration*.

A simple-declaration has two main parts: the *decl-specifier-seq* and the *init-declarator-list*. In the previous example,
```cpp
static const char
```
is the *decl-specifier-seq*, and
```cpp
*table[N];
```
is the *init-declarator-list*. In this case, the *init-declarator-list* is just a single declarator. The general form for a simple declaration is
```
simple-declaration =
    decl-specifier_seq
[ init-declarator-list ] ";" .
```
where
```
decl-specifier-seq =
    { decl-specifier } .
```

In other words, a *simple-declaration* is a sequence of zero or more *decl-specifier*s followed by an optional *init-declarator-list* and then a mandatory semicolon. A *decl-specifier* can be a *type-specifier* (a keyword or identifier that names a type), or any of the numerous other specifiers, such as `extern`, `static`, `register`, `mutable`, `inline`, `virtual`, `friend`, or `typedef`, to name a few. See the productions for *decl-specifier* in Table 2:

```
decl-specifier-seq =
    { decl-specifier } .

decl-specifier =
    storage-class-specifier |
    function-specifier |
    type-specifier |
    "friend" |
    "typedef" .

storage-class-specifier =
    "auto" | "register" | "static" | "extern" | "mutable" .

function-specifier =
    "inline" | "virtual" | "explicit" .

type-specifier =
    cv-qualifier |
    simple-type-specifier |
    class-specifier |
    enum-specifier |
    elaborated-type-specifier .

cv-qualifier =
    "const" | "volatile" .

simple-type-specifier =
    "bool" | "char" | "int" | "wchar_t" |
    "double" | "float" |
    "void" |
    "long" | "short" |
    "signed" | "unsigned" |
    type-name .

type-name =
    class-name | enum-name | typedef-name .
```

An *init-declarator-list* is a sequence of one or more *init-declarator*s separated by commas, where an *init-declarator* is a declarator followed by an optional initializer. See the productions for *init-declarator-list* and *init-declarator* in Table 3:
```
init-declarator-list =
    init-declarator { "," init-declarator } .

init-declarator =
    declarator [ initializer ] .

declarator =
    direct-declarator | ptr-operator declarator .

direct-declarator =
    ( declarator-id | "(" declarator ")" ) { array_suffix | function_suffix } .

declarator-id =
    identifier | other-id .

other-id =
    conversion-function-id |
    operator-function-id |
    "~" class-name |
    qualified-id |
    template-id .

array_suffix =
    "[" [ constant-expression ] "]" .

function_suffix =
    "(" [ parameter-clause ] ")" cv-qualifier-seq .

ptr-operator =
    "&" | [ class-name "::" ] "*" cv-qualifier-seq .

cv-qualifier-seq =
    { cv-qualifier } .
```

For example,
```cpp
unsigned int *p, &r = u, n;
```
has an *init-declarator-list* consisting of three *init-declarator*s. Neither the first declarator, `*p`, nor the third declarator, `n`, has an initializer. The second declarator, `&r`, has `= u` as its initializer. For now, I’m not concerned about the initializers. I’ve mentioned the *init-declarator*s just to establish the context for declarators.

A declarator is the name being declared, along with all the `*s`, `()s`, `[]s` and (in the case of C++) `&s` that modify that name. The name declared in a declarator is called the *declarator-id*. That name can have forms other than just a simple identifier. See the productions for *declarator-id* and *other-id* in Table 3.

For example, in a declaration such as
```cpp
size_t string::length() const;
```
the *declarator-id* is `string::length` (a *qualified-id*). In
```cpp
string &operator+=(char);
```
the *declarator-id* is `operator+=` (an *operator-function-id*). And, in
```cpp
~string();
```
the *declarator-id* is `~string` (a destructor name).

A declarator might not have any operators in it at all; it might be just a *declarator-id*. In that case, the *declarator-id* designates an entity with the type specified by the associated *decl-specifier*s. For example, if the declarator is just `x` and the *decl-specifier*s designate type `T`, then `x` has type `T`.

## Operator Precedence

Each declarator operator embellishes the type of the *declarator-id* in a unique way. The `*` operator embellishes the type by adding “pointer to”. That is, if the *decl-specifier*s designate type `T`, then the declarator `*x` declares `x` to have type “pointer to `T`”. Similarly, `&x` declares `x` to have type “reference to `T`”, `x[]` declares `x` to have type “array of `T`”, and `x()` declares `x` to have type “function with no parameters returning `T`”, or more simply “function returning `T`”.

That was the easy part. It gets harder when you start combining more than one operator in a declarator. Is
```cpp
T *x[]
```
a “pointer to array of `T`” or an “array of pointer to `T`”? Well, maybe that one isn’t so tough either. Most of you know that it’s the latter. But how do you explain this to a neophyte other than to say “Well, I just happen know this?” That sort of explanation does not scale up very well to cover declarations such as
```cpp
T A::* const x[]
```

You need a deeper understanding.

Here’s one helpful insight. One of C’s original design concepts was that a declarator should look just like (or, at least, a lot like) a sub-expression that uses the identifier from that declarator (the *declarator-id*). For example, given
```cpp
int **x[N];
```
then `**x[i]` appearing in an expression has type `int`. In the same vein, `*x[i]` appearing in an expression has type `int *` (pointer to `int`). Whenever you see `x` in a sub-expression involving declarator operators, it appears that you can “erase” that part of `x`’s declaration that looks like the sub-expression, and you’re left with the type of the sub-expression. For example, erasing `x[i]` from
```cpp
int **x[N]
```
leaves `int **` (pointer to pointer to `int`). Unfortunately, as with most things relating to declarators (not to mention the rest of C++), the actual rules are rarely as simple as we’d like. The fact is, `*x` does not have type `int *[N]` (array with `N` elements of type pointer to `int`), but rather it has type `int **` (pointer to pointer to `int`). I explain why a little later. Even though it falls apart on some of the details, I still think understanding the original design concept is helpful.

Unfortunately, few of the C++ extensions to C’s declarator syntax preserve the original design. For example, the `&` that appears in a reference declaration does not appear in expressions that use the reference. Similarly, *cv-qualifier*s (`const` and `volatile`) appear in declarations, but not in expressions (other than as casts). To top it off, pointer-to-member declarations look nothing like pointer-to-member expressions. For example,
```cpp
int T::*pm;
```
declares `pm` as a “pointer to member of `T` with type `int`”. However, you use `pm` in expressions such as `x.*pm` (where `x` is an object of type `T`) or `p->*pm` (where `p` has type “pointer to `T`”), which bear little resemblance to the declarator.

Another useful insight is that the operators appearing in declarators follow the same precedence rules as operators in expressions. For example, `[]` has higher precedence than unary `*`, so that when a compiler sees the expression `*x[i]`, it generates code to evaluate `x[i]` first and then dereference the result. When `*x[N]` appears as a declarator, the compiler uses the same precedence rules to associate `[N]` with `x` first. Thus, `*x[N]` is an “array of pointers” rather than “a pointer to an array”.

| Precedence | Operator | Meaning in Declarator | Meaning in Expression |
|:----------:|:--------:|:---------------------:|:---------------------:|
| Higher     | `::`     | member of             | Member of             |
|            |          |                       |                       |
|            | `[]`     | array of              | subscript             |
|            | `()`     | funtion returning     | call                  |
|            |          |                       |                       |
|            | `*`      | pointer to            | dereference           |
| Lower      | `&`      | reference to          | address of            |

Table 4 shows the precedence for all the operators that can appear in a declarator in C++. The order of the operators between a pair of horizontal lines is immaterial. That is, `[]` and `()` have the same precedence (lower than `::`), and `*` and `&` have the same precedence (lower than `[]` and `()`). The `()` operator in Table 4 is the function call operator, as in `f()`, and not the cast operator, as in `(char *)p`. You can find a complete operator precedence table showing all the C++ operators (not just the ones for declarators) in either Stroustrup [1] or Lippman [2].

As in expressions, you can override the operator precedences by using parentheses to explicitly group operands with operators. For instance,
```cpp
T *f();
```
declares `f` as “function returning pointer to `T`”, but
```cpp
T (*f)();
```
declares `f` as “pointer to function returning `T`”.

As in expressions, adding extra parentheses to a declarator does not change the type of the *declarator-id* provided the parentheses don’t change the grouping. Such extra parentheses are said to be redundant. For example,
```cpp
T (*(f))();
    ^ ^
```
the indicated parentheses are redundant, as are
```cpp
T (((*f))());
  ^^    ^  ^
  1|    |  1
   2    2
```
Both declare `f` as “pointer to function returning `T`”. However, there are other contexts, such as *abstract-declarators* and *new-declarators*, where seemingly redundant parentheses can change the meaning, so be careful.

The `::` is the scope resolution operator. It is both unary and binary. For example,
```cpp
const ::T *p;
```
declares `p` as a “pointer to `const T`”, where `T` must be in the global namespace. Here `::` is a unary operator which tells the compiler to look for `T` in the global namespace and ignore any others entities named `T` occurring in any class scope or block scope that enclose `p`’s declaration. A declaration such as
```cpp
const X::T *p;
```
also declares `p` as a “pointer to `const T`”, but to a different `T`. This `T` must be a type declared in a class or namespace named `X`. Here `::` appears as a binary operator which tells the compiler to look for `T` in the scope of `X`.

When a `::` appears as part of a type name (as in the previous two examples), it’s not a declarator operator. A type name is part of the *decl-specifier-seq*, not the declarator. The syntax does not allow parentheses in the *decl-specifier-seq*, so you can’t throw in redundant parentheses. For instance, adding extra parentheses, as in
```cpp
const (X::T) *p;
```
causes a syntax error.

`::` appears as a declarator operator only when used to declare a *pointer-to-member*. In that case, the `::` is neither unary nor binary. `::` combines a class name with a `*` operator to form a *pointer-to-member* operator. For example, in
```cpp
int X::*pm;
```
the *decl-specifier-seq* is just `int`, and `X::*pm` is the declarator. `X::*` is a declarator operator that embellishes the *declarator-id* with “pointer to member of `X` with type ...”. A pointer-to-member operator such as `X::*` has the same precedence as a plain `*` or `&`. Thus,
```cpp
int X::*pm();
```
declares `pm` as “function returning pointer to member of `X` with type `int`”. To declare `pm` as a pointer to a member function, you need parentheses to alter the bindings, as in
```cpp
int (X::*pm)();
```
This declares `pm` as “pointer to member of `X` with type function returning `int`”.

Each of the symbols, `X`, `::`, `*`, and `p` is a separate token, so C++ doesn’t care how much or little whitespace appears between them. However, any intervening parentheses will introduce a syntax error. For instance, both
```cpp
int (X::*)pm();
int X::(*pm)();
```
are erroneous.

Earlier, I introduced the notion that you can determine the type of a declarator-like sub-expression by “erasing” that part of the declarator that looks like the sub-expression. For example, given
```cpp
int **x[N];
```
then `**x[i]` appearing in an expression has type `int`, and `*x[i]` appearing in an expression has type “pointer to `int`”. I said this notion seems to break down because `*x` has type “pointer to pointer to `int`” rather than “array of pointer to `int`”.

The real cause of the breakdown is that arrays “decay” to pointers in most contexts. As declared, `x` has type “array of ...”, but when you use it an expression, that use of `x` almost always converts to type “pointer to...”. `x` itself remains “array of...”; just the use of `x` has type “pointer to...”. Both C and C++ define the `[]` operator as a pointer operator, not an array operator. `[]` works for arrays because arrays decay to pointers.

Array decay gives rise to the famous identity relationship:
```cpp
a[i] == *(a + i)
```

In the special case where `i` is zero, the identity is:
```cpp
a[0] = *a
```

Therefore, when you see `*x` in an expression, you should transform `*x` to `x[0]`, and then the “erasing” rule works. The sub-expression `x[0]` “looks like” `x[N]` in the declarator, so you erase `x[N]` and you’re left with `int **` (pointer to pointer to `int`).

How do you know when to convert `*x` to `x[0]`, or vice versa? First, add redundant parentheses to the declaration to make the operator bindings in the declarator explicit. (Don’t put the parentheses in your code; just do it mentally.) Then just make sure you only erase a pair of matching parentheses and everything that lies in between. For example, the declaration
```cpp
int **x[N];
```
is equivalent to the fully-parenthesized declaration
```cpp
int (*(*(x[N])));
```

Thus, you can’t obtain the type of `*x` by simply erasing `*x` from the declarator because `*x` straddles a left parenthesis. On the other hand, if you convert `*x` to `x[0]` then you can erase `x[N]` because `x[N]` completely fills a pair of matching parentheses. That leaves you with `int **`, which is indeed the type of `*x`.

## Mining the Grammar

In the course of explaining declarators, I’ve made several bald assertions about their syntactic structure. For example, I asserted that, with respect to a declarator, `X::*` is a single pointer-to-member operator, and it won’t tolerate any parentheses in it. How do I know this? I read the grammar in Table 3.

In particular, the production for declarator is
```
declarator =
    direct-declarator | ptr-operator declarator .
```
which shows, among other things, that a declarator prefixed with a *ptr-operator* is still a declarator. The production for *ptr-operator*
```
ptr-operator = "&" |
    [ class-name "::" ] "*"
	cv-qualifier-seq .
```
shows that, even though the *class-name* and `::` are optional, they are part of a single *ptr-operator*. Nowhere does it allow parentheses. The grammar allows grouping parentheses in a *direct-declarator*:
```
direct-declarator = ( declarator-id |
    "(" declarator ")" )
	{ array_suffix |
	function_suffix } .
```

Note that this production has two pairs of parentheses. Each inner parenthesis is enclosed in double quotes, indicating that the parentheses are actual symbols that may appear in a *direct-declarator*. The outer parentheses are meta-symbols; they change the grouping of the operators used in describing *direct-declarator*.

This part of the right-hand side:
```
"(" declarator ")"
```
shows that whatever it is that’s inside the parentheses must be able to stand alone as a declarator. Since a declarator can be a *ptr-operator* followed by a declarator, then
```
( ptr-operator declarator )
```
is a declarator, as is
```
ptr-operator ( declarator ) .
```
and clearly, a left parenthesis cannot appear anywhere inside the *ptr-operator*.

The grammar also answers all sorts of other questions about *cv-qualifier*s in declarations. *cv-qualifier*s (`const` and `volatile`) can appear in either a *decl-specifier-seq* or a declarator, or both. For example, in

```cpp
char const *const p;
```
the *decl-specifier-seq* is “`char const`”, and the declarator is “`*const p`”. The production for *ptr-operator* shows that any *cv-qualifier*s in the declarator are an inseparable part of a pointer operator. In other words, if you explicitly parenthesize the declaration above, the `*` and the `const` to its right must remain together, either inside or outside a pair of matching parentheses. For example, any of
```cpp
char const (*const p);
char const *const (p);
char const (*const (p));
```
are valid, and all the parentheses are redundant. However,
```cpp
char const *(const p);
```
is an error because it breaks up the *ptr-operator*.

```cpp
char const (*const) p;
```
is also an error because parentheses around a *ptr-operator* must also enclose the declarator. *cv-qualifier*s can also participate in pointer-to-member operators, and the same rules apply as with plain pointer operators. For example, in
```cpp
volatile sig_atomic_t X::*const p;
```
the declarator is `X::*const p`. Syntactically, `X::*const` is one declarator operator. As declared, `p` has type “`const` pointer to member of `X` with type `volatile sig_atomic_t`”. *cv-qualifier*s can also appear after the parameter list in a function declarator. For example, in
```cpp
const char *data() const;
```
the declarator is `*data() const`. This declarator contains two operators:
1. the pointer operator, `*`, and
2. the *function-suffix*, `() const`.

The production
```
function_suffix =
    "(" [ parameter-clause ] ")"
	cv-qualifier-seq .
```
shows that the parenthesized *parameter-clause* and the *cv-qualifier*s (if any) are part of a single declarator operator. The *cv-qualifier*s are valid only in member function declarations, so a *function-suffix* with a trailing `const` qualifier embellishes its declarations with “`const` member function returning ...” rather than just “`const` function returning ...”. The `()` in a *function-suffix* has higher precedence than a *ptr-operator*, so
```cpp
const char *data() const;
```
declares data as “`const` member function returning pointer to `const char`”. You can add redundant parentheses, as in
```cpp
const char *(data() const);
```
without changing the meaning. However,
```cpp
const char (*data)() const;
```
overrides the operator precedences, so it declares data as “pointer to `const` member function returning `const char`”. Placing parentheses around the *function-suffix* also changes the meaning rather dramatically:
```cpp
const char *data(() const);
```
In this case, `() const` becomes data’s *parameter-clause*, and data’s type becomes “[non-`const`] function returning pointer to `const char`”. Since `() const` is an ill-formed parameter declaration, your compiler will produce a diagnostic, but it probably won’t be very edifying.

## A Note of Caution

The grammars in Tables 2 and 3 are sufficiently detailed to answer many questions about the structure of declaration, but they are incomplete or over-simplified in a few places. For example, Table 2 does not show that a *type-name* can be a qualified name such as `::T`, `X::T`, or `::X::T`. Nor does it show that a *decl-specifier* can be a *class-specifier* or *enum-specifier*, as in
```cpp
enum color { BLACK, WHITE } c;
```

Table 3 mentions, but does not define, initializers and *parameter-clause*s. It doesn’t even mention exception-specifications which occur at the end of a *function-suffix*. Rest assured I will include exception-specification in my upcoming article(s) on exception handling.

Nonetheless, the grammar in Tables 2 and 3 is detailed enough to guide the construction of a program that parses (reads and verifies) fairly-complicated C++ declarations. I will show you such a program next month.

## References

1. Bjarne Stroustrup. The C++ Programming Language, 2nd. Ed. (Addison-Wesley, 1991).
2. Stanley B. Lippman. C++ Primer, 2nd. Ed. (Addison- Wesley, 1991).
