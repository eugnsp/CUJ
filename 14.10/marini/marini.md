# A Class Hierarchy for Random Number Generation

**Marc Marini**

*Marc Marini is an engineering group leader with Motorola Semiconductor Products Sector. He received a BSEE from Texas A&M University and is currently working on his MSCS from NTU. He has seven years of experience in test engineering and is a registered professional engineer in the State of Texas.*

---

Class derivation makes sense as a way of inheriting mathematical properties, not just common data.

## Introduction

C++ classes often come in handy for numerical applications because of their ability to encapsulate state information. Such is the case, for example, with random number generation. A random number generator needs to remember the last value it produced, and a C++ class can store that value as a private or protected data member. But that’s not all. Inheritance and polymorphism can be used to good effect in this type of application as well. In the case of random number generation, derived classes can adapt a uniform distribution – as produced by the base class – to produce new distributions, such as exponential, polynomial, etc. Given a good base class, a user can derive his/her own classes to produce a tailor-made distribution.

The class hierarchy presented here can be used to generate random variates from different distributions. The base class provides a uniform distribution over the interval from `0` to `1` (`U(0, 1)`). The subclasses use substreams from this generator to calculate variates from their distributions. The three distributions presented in this article are uniform, exponential, and triangle. Additional distributions, such as normal and gamma, can be implemented as subclasses as well.

The generator used by the base class to produce a uniform distribution is a multiplicative linear congruential generator (MLCG). An MLCG produces random numbers based on the recursive formula

<code>
Z<sub>i</sub> = (aZ<sub>i-1</sub>) mod m
</code>

where `a` is referred to as the multiplier and `m` is the modulus. To find out more about MLCGs (and LCGs) see the [sidebar](#sidebar). You may also want to consult the references at the end of this article.

## Base Implementation

### Selecting MLCG Constants

In general, we want random number generators to have the longest possible period, so the value of m should be maximized. The multiplier a must be chosen with care to produce good statistical properties. For my class hierarchy I selected as the modulus the largest prime signed integer available from the compiler: `2,147,483,647` (<code>2<sup>31</sup> - 1</code>). Based on the results presented in [1], I selected a multiplier of `62,089,911`. Other acceptable multipliers are `742,938,285`, `950,706,376`, `1,226,874,159`, and `1,343,714,438`.

### Base Class

The base random number generator class `RandomGenerator` ([Listing 1](random.h)) produces a uniform distribution of random numbers from `0` to `1`. Each instance of this base class seeds its random number generator with the 100,000<sup>th</sup> number from the previously instantiated generator. (Thanks to a shortcut, the previous generator does not have to run through 100,000 calculations to produce this seed. The shortcut is given below.) This separation of seeds ensures that the various substream distributions appear to be independent.

The base class constructor ([Listing 2](random.cpp)) examines the static value `stream_count` to determine if the current instance is the first instance of the RandomGenerator class. If it is, the static `base_seed` value becomes the seed for the class. (This value must be set prior to instantiation of the class, via member function `RandomGenerator::SelectSeed`.) Otherwise, the constructor calculates a new seed value from the equation

<code>
S<sub>new</sub> = (S<sub>old</sub> * A<sup>100000</sup>) mod m
</code>

This is the shortcut mentioned previously. The constructor then initializes the instance variables `my_start_seed` and `current_seed` to the `base_seed` value.

### Random Number Generation

A call to `RandomGenerator`’s `Rand` member function produces a double-precision value in the range of `0` to `1`. Before using this class, make sure the platform you are using can handle a 38-bit mantissa in its double-precision floating-point variables. If it can’t, this class will not work, due to a loss of precision when calculating the next seed value. This routine will compute

the next random number based on the MLCG algorithm presented above. If your platform is capable of handling 38-bit integers, you may want to change the floating-point math to integer math for a speed increase.

### Other Class Members

The protected data member, `current_seed`, is <code>Z<sub>i-1</sub></code> in the random number algorithm. This member is updated with each call to the `Rand` function. The remaining data members were described above.

The `Reset` function reloads the `current_seed` variable with the `my_base_seed` variable. This function allows the program to reuse the same sequence of random numbers. There are two static functions, which should be called before any class instances are created. The first function, `ResetStreamCount`, loads the `stream_count` variable with zero. The second function, `SelectSeed`, was already mentioned. `SelectSeed` provides a mechanism for explicitly specifying the seed for the next created stream.

## Derived Classes

The base class `RandomGenerator` produces uniformly distributed sequences of pseudo-random numbers from `0` to `1` as I mentioned earlier. I derive three classes from this base class, which modify the sequences produced by the basic class to produce new distributions. These new distributions are briefly described as follows.

The `UniformRandomGenerator` class generates uniformly distributed random numbers over a user-specified interval. The user specifies the interval by passing the values to the constructor. This derived class `Rand` function produces a sequence according to the formula

```
Z = A + (B - A) * U(0, 1)
```

where `A` (default is `0`) is the low interval limit, `B` (default is `1`) is the upper interval limit, and `U(0, 1)` is the uniform distribution from `0` to `1` as produced by the base class `Rand` functions.

The `ExponentialRandomGenerator` class generates exponentially distributed random numbers about a user-specified mean `M`. The mean is set by the constructor; its default value is `1`. The distribution returned by `Rand` function follows the formula

```
Z = -M * ln(U(0, 1))
```

The `TriangleRandomGenerator` class generates triangularly distributed random numbers about a user-specified peak (`cut_value`). The user also specifies the distribution range `(A, B)` around the peak. This class’s `Rand` function returns

```
Z = sqrt(C * U(0, 1))
```

if `U(0, 1)` is less than or equal to `C`, and

```
Z = 1 - sqrt((1 - C) * (1 - U(0, 1))
```

if `U(0, 1)` is greater than `C`. The value of `C` is computed from `C = (cut_value - A) / (B - A)`.

## Summary

These classes provide statistically well-behaved random numbers for a variety of distributions. They also provide the ability to generate multiple random numbers from seemingly independent streams. Finally, the class set may be expanded to provide random variates from other distributions.

## References

1. Fishman and Moore. “An Exhaustive Analysis of Multiplicative Congruential Random Number Generators with Modulus 2<sup>31</sup> - 1”, SIAM Journal of Science and Statistical Computing, Vol. 7, No. 1, January 1986.
2. Law and Kelton. Simulation and Modeling Analysis, Second Edition, chapter 7.
3. Jerry Dwyer and K.B. Williams. “Testing Random Number Generators, Parts [1](../../14.06/dwyer/dwyer.md) and [2](../../14.08/dwyer/dwyer.md)”, C/C++ Users Journal, June 1996 and August 1996.

---

## Sidebar – Linear Congruential Generators

The code in this article uses a common type of random number generator known
as the linear congruential generator (LCG). This generator produces random
numbers based on the recursive formula

<code>
Z<sub>i</sub> = (aZ<sub>i-1</sub> + c) mod m
</code>

where `m > 0`, `0 < a < m`, `0 <= c < m`, and <code>0 < Z<sub>0</sub> < m</code>. In this formula, `a` is referred to as the multiplier and `c` as the increment. A good generator will produce each value in the range of `0` to `m` before a repeated value occurs. This behavior is referred to as full cycle generation. Of course, this algorithm, like any algorithm run on a computer, cannot generate truly random numbers. This becomes clear when you realize that every <code>Z<sub>i</sub></code> can be determined from <code>Z<sub>0</sub></code>. However, through careful selection of `m`, `a`, and `c`, the numbers generated will appear to be independent and identically distributed. For an LCGto produce this sort of distribution, the following conditions must hold [2]:
1. The only positive integer that divides both `m` and `c` is `1`.
2. If `q` is a prime number that divides `m`, then `q` divides `a - 1`.
3. If `4` divides `m`, then `4` divides `a - 1`.

Consider the case of `m = 8` and select `a = 5`, `c = 3`, and <code>Z<sub>0</sub> = 5</code>. The random number sequence generated would be 5, 4, 7, 6, 1, 0, 3, 2, 5.... Notice that each integer is generated once before the pattern restarts. The seed value determines the starting point in the pattern. In order to generate the variates in a different order either `a`, `c`, or both must be modified. Finally, observing the rules defined above does not guarantee that the generator will be statistically acceptable. See reference [1] for information on testing random
number generators.

### Multiplicative LCGs

The multiplicative LCG (MLCG) is a special case of the LCG for which the increment is assigned the value `0`. Thus, the algorithm for random number generation becomes

<code>
Z<sub>i</sub> = (aZ<sub>i-1</sub>) mod m
</code>

To achieve a period of size `m - 1` for this algorithm, `m` must be a prime number. An MLCG based on a prime modulus is known as a PMMLCG (prime modulus MLCG).
