# Tuning Up Math Functions

**Tim Prince**

*Tim Prince has a B.A. in physics from Harvard and a Ph.D. in mechanical engineering from the University of Cincinnati. He has 25 years of experience in aerodynamic design and computational analysis. He can be reached at Box 3224, Rancho Santa Fe CA 92075.*

---

With the publication of P.J. Plauger's book *The Standard C Library*, satisfactory source code is readily available for C library functions. While Plauger's code does not treat accuracy as the only criterion for success, most of the functions can be made faster on most computers. Since most of the speedup techniques are the same for any code, I concentrate on those functions for which both better performance and accuracy can be obtained.

## Accuracy

Accuracy in the math functions is affected most by the quality of the numerical approximations used and by organizing the arithmetic to minimize roundoff error. In both of these areas, compromises can be made to improve speed at the expense of accuracy. Using one less term in a polynomial or skipping circuitous schemes to get more precision in the arithmetic will still give satisfactory results most of the time. This route is easier to follow than the route which attempts to get the last available bit of precision. Unless the math functions are taking a large percentage of the time in an application, the slight extra time consumed will be much less annoying than the possibility of incorrect results.

Just as a floating-point number must be specified to 17 decimal digits to assure that it can be converted to the closest IEEE P754 standard `double`, polynomial approximations should be good to 17 decimal digits. More accurate approximations make no difference, but series which may be off even by half of the least-significant bit will produce more roundoff errors.

A slightly different approach can be taken for functions of `float` precision, using series which are accurate to nine decimal digits, with `double` arithmetic avoiding the need for any special precautions against roundoff error. For the common case of applications which need both `sinf` and `cosf` of the same argument, schemes using the half-angle formulae can produce both results in little more time than `cosf` alone. These schemes are much more favorable for vectorization. This method can be used internally in coprocessors that have efficient `long double` arithmetic, but it limits the accuracy to a little over 15 decimal digits if used in standard `double` precision. Software implementations of `long double` are so slow that their existence may as well be ignored. They don't even compete with programs like `bc` which are useful for getting the series coefficients.

Rational approximations, even more than polynomial approximations, demand that the formulae be organized so that the roundoff error in the division is washed out by later addition of larger terms. This means that the code should be written so that the final operation is addition rather than multiplication or division. For an odd function (one for which `f(-x) == -f(x)`), this often produces a `+=` assignment.

## `sin` Function

Review of Plauger's `sin` function showed that the basic approach is excellent, but the accuracy of the polynomial approximation can be improved slightly. The `bc` program, [Listing 1](listing1.bc), takes three minutes on an HP720 to calculate the coefficients for approximations up to 28 digits precision. Replacing the approximated function `s(x)/x` by `c(x)` will produce the other set of coefficients. These series give more accuracy than Plauger's with one less term in the `sin` series.

In certain applications, the most critical point of accuracy is whether the functions satisfy the algebraic identity
```
cos(x) * cos(x) + sin(x) * sin(x) == 1
```
With the function written as shown, errors in range reduction do not affect this test, which is good, except that other tests are needed to check range reduction. With these coefficients and the change to make the `sin` series use the `+=` operator, the identity is satisfied more often than with the library functions provided by Sun and HP.

Unless one is trying to simulate IEEE handling of `NaN` (Not a Number) and `Inf` (infiity) on a system which does not support them, there is no need in a function like this to write code which looks for these special bit patterns. The basic system must trap and handle them appropriately without our wasting time on them. Since we aren't performing anything but normal arithmetic, any `NaN` or `Inf` will be propagated automatically. These exceptions should happen so seldom that code which shortcuts the full formulae would take more time in the normal case than it could save in the exceptional cases. Likewise, any modern system can handle underflows appropriately without specific user code.

I show two ways of handling arguments with magnitude greater than `INT_MAX` faster than by using Plauger's `_Dint`. In `_Sin`, [Listing 2](listing2.c), as did Plauger, I add or subtract multiples of <code>2&pi;</code>, but I use the value of `DBL_EPSILON` from `<float.h>` to write arithmetic directly to get an appropriate multiple. In the cases of `FLT_ROUNDS <= 0` or `FLT_ROUNDS == 3`, the results should be identical to `_Dint`. For `FLT_ROUNDS == 1`, the integer will be one greater about half the time, but this only slightly worsens the accuracy for cases in which not much accuracy is possible anyway.

In [Listing 3](listing3.c), I save time by not testing the size of the argument, but going straight to a remaindering scheme which works for arguments with magnitude up to <code>&pi; / (2 * DBL_EPSILON)</code>. Outside of this range, there is no accuracy anyway, and we should in principle report `ERANGE`. [The C Standars says we can't, however. – pjp] The code is set up so that the argument sign can be tested in parallel with the multiplication by <code>2 / &pi;</code>, saving time on pipelined machines. Plauger's scheme of testing the sign bit directly with integer operators requires the argument to be copied to an `int` register, which on many architectures includes an intermediate copy to memory, so it is faster as well as more portable simply to use a `double` comparison operator. Rounding should work correctly for any valid `FLT_ROUNDS` including those which vary at run time, but more efficient code will be generated if `FLT_ROUNDS` is constant.

Some systems have trouble casting to `int` a floating-point value that exceeds `INT_MAX`. (You need to determine the least-significant integer bits to determine the correct octant. I have included code that works correctly even for large values, then used a `#if` to skip over it. But this approach is quite slow on many machines (although not as slow as the [Listing 2](listing2.c) code). We avoid any attempts to cast from `double` to `int` and back to `double`, which few compilers optimize.

One might think that the boundaries could be widened slightly by using casts to `unsigned long`, possibly with `fabs`. But typical architectures do not have hardware support for casts directly from `double` to `unsigned long`, so our goal of saving time is defeated. More general ways of rounding to an integer, such as a FORTRAN `ANINT` function or a PA-RISC `FRND` pseudo-instruction, must trap to a function which spends a lot of time checking for cases that we don't care about. These short cuts have never caused me any trouble, but they aren't totally satisfactory for black-box code, which isn't under the control of the user.

Finally, [Listing 3](listing3.c) breaks the polynomials into groups to make more efficient use of architectures that can perform `double` add or multiply operations in pipeline or parallel fashion. When the separate polynomial chunks are combined, there is a roundoff that is different from a straight Horner-polynomial evaluation. The change is swamped, as mentioned above, by arranging for it to occur before the largest term is added. This roundoff is slightly more favorable than the one in the Horner evaluation. Tests on a SPARC 2 show that the pipelineable polynomial takes 10% longer to evaluate, as would be expected from the requirement for two additional multiplications. On an HP720, [Listing 3](listing3.c) runs 30% faster than it would without the parallelization, as the compiler make effective use of the parallel multiply-and-add instruction.

## `exp` Function

The most serious criticism of Plauger's `_Exp` function is that it deals with negative arguments by unthinking application of the symmetry
```
exp(-x) = 1 / exp(x)
```

This adds the roundoff error of a final division whenever the argument is negative. [It wasn't unthinking. The acceptance tests I had to pass worried more about this symmetry than the roundoff error. – pjp] The corrected code [Listing 4](listing4.c) is simpler, but it involves casting the argument to `int` with rounding, both for positive and negative arguments.

As the argument has already been forced to memory by passing it in by reference, the sign bit can be isolated quickly and used as an index into an array `{.5, -.5}` of rounding constants. This array can be `float` or `double`. On 32-bit architectures, it may be faster to get a `float` from memory and cast it to `double`. With the indexing scheme, no conditionals are required, avoiding breaks in the instruction pipeline.

Plauger's `exp` function can be speeded up, as he suggests in his book, by changing `_Dtest` to a macro, and avoiding operations that require the argument to be copied through memory. This macro will even detect a `NaN` if IEEE arithmetic is implemented:
```c
#define _Dtest(x) ((x) > DBL_MAX || (x) < -DBL_MAX ? INF: \
                   (x) != (x) ? NAN: \
                   (x) !=0 ? FINITE : 0)
```
Caution: in this context, `x != x` is **not** the same as `!(x == x)`.

If VAX compatibility is not required, `_Dscale` and similar functions can be implemented using `long` instead of `short`, as most architectures use the same byte order for storage of all 32-bit quantities. For full compatibility with Plauger's book, change all `long` and `int` declarations to `short`.

We can't use 21 digits of precision, so an approximation with one less term than Plauger's gives identical results. I set up the coefficients thinking that I could get more speed by making the smallest coefficient `1`, but this introduces an additional roundoff error that can be avoided by calculating the numerator with `+=` as shown. The scale of the coefficients used by Plauger was set for best accuracy on an IBM 360 with base-16 arithmetic, a handicap which we will do without. The change presented here is slightly faster on architectures that can perform add and multiply in parallel.

The formula which ends with the addition of `0.5` is algebraically equivalent to one of the form `(even + odd) / (even - odd)`, which has the same symmetry as `exp` as mentioned earlier. This form will run faster on certain pipelined machines but doesn't have the desired swamping effect on the division roundoff error. The speed of the formula stems from this identity of terms in the numerator and denominator.

Awareness of standards seems to have brought about a sameness in implementations of `exp`, and we find little difference between the performance of these functions from various vendors' libraries.

## `pow` Function

`pow` is so complicated that there is at least one way per programmer to implement it. With just one change, Plauger's code is as accurate as any. The problem, as with his `_Exp` function, is the inversion which is used to take care of negative powers. Instead of inverting and then multiplying, the code must be changed to divide in this situation. [Same comment as before. – pjp] This requires elimination of the conditionals which appear to be intended to save time in the cases of integral powers and bases which are exact powers of 2. In most applications where these special cases occur, use of `pow` may be avoided entirely, so there is no point in trying to speed up these cases at the expense of the general case.

In terms of performance, the greatest problem is the excessive time spent in the low-level support functions. I suggest using the `_Dtest` macro to isolate the special cases, with appropriate changes in the `pow` code ([Listing 5](listing5.c)). The only remaining special case that would be dealt with in `_Dunscale` is the subnormal, which can be handled quicker by detecting it and multiplying by <code>2<sup>DBL_MANT_DIG</sup></code> which equals `1 / DBL_EPSILON`.

The union `_Dvar`is identical to `_Dconst` except that the `const` qualifier is omitted. On most current architectures, working on a union of `double` and `int` requires copying through memory, so the generated code is the same as if it were done with pointer casts. The MIPS is an exception to this, and it may be possible using a variation on the `_Dvar` union to get the compiler to copy directly between `float` and general registers.

The call to `_Dscale` can be eliminated by using the scaling which Plauger built into `_Exp`. Casts from `double` to `int` to `double` have been collected so that they should be recognized by a compiler's peephole optimizer. (This will be discussed later.) Code for raising the fractional part to an integer power has been revised to reduce the number of comparisons. As most C compilers will not register allocate a variable whose address has been taken, care has been taken to copy to register eligible `doubles` where this will improve the code.

Some commercial libraries achieve better speed by separating completely the integer power case and using `exp(log(x) * y)` for the usual case. The more cautious approach which we have taken from Plauger's book should give more accurate results for `|y| > 4`, but many applications confine their use of `pow` to the range where these precautions are not needed.

## `atan` Function

The basic scheme employed in the 4.3BSD library ([Listing 6](listing6.c)) has proven more accurate than the one shown in Plauger's text. Making the rotations by angles whose inverse tangents are multiples of 0.5 reduces roundoff error. The corresponding angle adjustments are made by splitting the adjustment in a way analogous to the precautions taken by Plauger in some of the other math functions. A small adjustment is made to the third-power term of the series, such that the large adjustment at the end can be done by an "exact" number. The roundoff error in the third-power term is swamped by the later additions of larger "exact" quantities. The 1985 Berkeley code had these adjustments in certain of the ranges. I found that it should be extended to the other ranges.

I changed the coefficients to a series obtained by a Chebyshev fit to `atan(x) / x`. Tests in 53-bit IEEE precision showed no difference from the results obtained with the coefficients in the BSD version, but it should not be necessary to add a term to obtain 56-bit precision. I omitted precautions taken in the Berkeley code which are unnecessary when using correctly rounded arithmetic, as well as precautions taken against old-style compilers which may ignore parentheses.

As with the `sin` function, greater speed is obtained on pipelined machines with the series written to allow some parallel computation. While more parallel computation is available by further grouping of terms, no more speed is possible on the HP720. Grouping terms at the level shown makes no difference in roundoff error.

The normal way to get `atan` would be with a masking macro
```c
#define atan(x) atan2(x, 1.)
```

This costs little time compared to a separate `atan` function. In those cases where a division occurs in the argument of `atan`, it is faster and more accurate to use `atan2` so that the division can be postponed to the internals of the math function.

`asin` and `acos` can be written as macros, using `atan2` and `sqrt`. As most applications make little use of these functions, this gives satisfactory results when there is a hardware square root. It should also put you on the road to simplifying expressions such as `cos(asin(x))` which cost a surprising amount of time and accuracy.

## Improving Compiler Optimization

The current SPARC, MIPS, and PARISC compilers generate redundant code for casts from `double` to `int` to `double`, which occur frequently in the math functions. These architectures first produce an `int` result in a floating-point register, which must be copied to a general register before it can be used for integer arithmetic. When it occurs in isolation, the cast from `int` to `double` must copy the `int` value to a floating-point register, where it is not otherwise allowed to go. When the `int` value has just come from truncating a floating-point value, the necessary value already exists in a floating-point register, but the compilers don't take advantage of it.

In source code for the Free Software Foundation gcc compiler, the Machine Description file includes the ingredients to produce a `define_peephole` pattern which will match these pairs of casts and generate code without the redundant copy from general to floating-point register. On the SPARC and PA-RISC architectures, this copy has to go through memory, as there is no register-to-register move between general and floating-point registers. At least two instructions and one register allocation can be eliminated. In addition, the floating-point processor can continue on without waiting for results to arrive in the general register, so four or more clock cycles may be saved.

## Conclusion

When striving for maximum accuracy, any operation that produces roundoff errors at the level of the least-significant bit of the result should be avoided. The best way to accomplish this is to make the final operation the addition of an exact larger quantity, such as the original argument or an exact constant.

Making execution of small sections of code conditional should be avoided when the results are harmless. It doesn't save time in the majority of cases. Modern architectures handle underflow automatically and don't require special code.

Most current machines use coprocessor architectures, which are slow in transferring data between floating-point and general registers. Mixed `int` and `double` operations on the same data should be avoided. Such operations which depend on bit patterns are non-portable and difficult to read, so should be used sparingly.

## References

* Press, Flannery, et al. 1988. *Numerical Recipes in C*, Cambridge University Press.
(`bc` code actually was adapted from 1986 FORTRAN edition.)
* Plauger, P.J. 1992. *The Standard C Library*. Prentice-Hall.
