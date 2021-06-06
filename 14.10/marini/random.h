/*
 * random.h
 * Header file for the implementation of a random number
 * generator class.  This class is based on the algorithm defined
 * by Z(i) = AZ(i-1)modM.  Where M is the prime number 2^31-1 =
 * 2,147,483,647 and A is the value 62089911.
 */
#include <values.h>
#include <math.h>

// Unform Random Generator Class Definition (PMMLCG)
class RandomGenerator { // Generate U(0,1)
    public:
        RandomGenerator(void);
//        virtual ~RandomGenerator(void);
        virtual double Rand(void);
        void Reset(void);
        static void ResetStreamCount();
        static void SelectSeed(long int seed);
    protected:
        long int current_seed;
    private:
        static int stream_count;
        static long int base_seed;
        long int my_start_seed;
};

// RandomGenerator Inline Functions

inline void RandomGenerator::ResetStreamCount(void)
{
    stream_count = 0;
}


inline void RandomGenerator::SelectSeed(long int seed)
{
    base_seed = seed;
}

inline void RandomGenerator::Reset(void)
{
    current_seed = my_start_seed;
}
// End of RandomGenerator


// Uniform Random Generator Class Definition -- Generate U(a,b)
class UniformRandomGenerator : public RandomGenerator {
    public:
        UniformRandomGenerator(double high_limit = 1.0,
                               double low_limit = 0.0);
//        virtual ~UniformRandomGenerator(void);
        double Rand(void);
    private:
        double A;
        double B;
};
 
// UniformRandomGenerator Inline Functions
inline
UniformRandomGenerator::UniformRandomGenerator(double high_limit,
    double low_limit) : RandomGenerator(), A(low_limit),
                        B(high_limit)
{    // Constructor does nothing special
}
 
inline double UniformRandomGenerator::Rand(void)
{ // return Z = A + (B-A)*U(0,1)
    return A + ((B - A) * RandomGenerator::Rand());
}
// End of UniformRandomGenerator
 
// Exponential Random Generator Class Def. -- Gen. Exp(a)
class ExponentialRandomGenerator : public RandomGenerator {
    public:
        ExponentialRandomGenerator(double m = 1.0);
//        virtual ~ExponentialRandomGenerator(void);
        double Rand(void);
    private:
        double mean;
};
 
// ExponentialRandomGenerator Inline Functions
inline 
ExponentialRandomGenerator::ExponentialRandomGenerator(double m) :
    RandomGenerator(), mean(m)
{    // Constructor does nothing special
}

inline double ExponentialRandomGenerator::Rand(void)
{ // return Z = -1*mean*ln(U(0,1))
    return -1.0 * mean * log(RandomGenerator::Rand());
}
// End of ExponentialRandomGenerator
 
// Triangle Random Generator Class Def. -- Generate Triang(a,b,c)
class TriangleRandomGenerator : public RandomGenerator {
    public:
        TriangleRandomGenerator(double high_limit = 1.0,
                                double low_limit = 0.0,
                                double cut_value = 0.5);
//        virtual ~TriangleRandomGenerator(void);
        double Rand(void);
    private:
        double A;
        double B;
        double C;
};
 
// TriangleRandomGenerator Inline Function
inline
TriangleRandomGenerator::TriangleRandomGenerator(
    double high_limit,double low_limit, double cut_value) : 
        RandomGenerator(), A(low_limit), B(high_limit)
{    // Calculate bend value
     C = (cut_value - A) / (B - A);
}
 
inline double TriangleRandomGenerator::Rand(void)
{
    /*
     * If U(0,1) <= C, then return A + (B-A)*sqrt(C*U(0,1)) 
     * otherwise
     * return A + (B-A)*(1 - sqrt((1-C) * (1 - U(0,1))))
     */
    double x = RandomGenerator::Rand();
    if (x <= C)
        return A + (B - A) * sqrt(C * x);
    else
        return A + (B - A) * (1.0 - sqrt((1.0 - C)*(1.0 - x)));
}
// End of ExponentialRandomGenerator
