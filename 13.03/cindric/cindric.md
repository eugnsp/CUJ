# When the “Best” Algorithm Isn’t

**Blase B. Cindric**

*Currently in his tenth year of teaching computer science at the undergraduate level, Blase has spent the past six years as a faculty member at Westminster College, New Wilmington, PA. In addition to his full-time teaching responsibilities, he is a part-time Ph.D. student in computer science at the University of Pittsburgh, where his areas of interest are theory of computation and algorithmic analysis. He holds a Masters degree in computer science from Penn State, lives very happily with his wife, and is at the beck and call of their two cats. He can be reached via e-mail as bbc@cheese.westminster.edu.*

---

## Introduction

When computer scientists analyze the performance of algorithms, they attempt to make the analysis as general as possible by focusing on large data sets. Also, no assumptions are made concerning any properties that the data might have. The idea behind this approach is to make the resulting rankings of the algorithms valid for the largest variety of data possible.

So, for instance, the consensus of the computer science community is that the best sorting algorithm, on average, is the Quicksort algorithm. A programmer without much experience who sees such a claim is likely to accept the wisdom of the ancients as the whole truth and nothing but the truth. But there is a catch. We programmers are always working on some specific problem, and sometimes we have extra information about our data sets that may allow us to obtain better performance than the “best” algorithm. For our purposes, we can do better than the “best”.

Such a situation arose in an application I developed several years ago. I present here a simplified version. We have an array that must contain unique values (no duplicates allowed), with the currently used positions of the array containing sorted values. After the end of this sorted data, several unsorted additions are made to the array.

Our task is to sort the array so the entire data set is sorted. So we have an array that is mostly sorted, with some unsorted stuff at the end. In this situation, it turns out that there is a sorting method that outperforms Quicksort for some array configurations, a variant of the insertion sort routine.

## When Simpler is Better

Fans of algorithmic analysis may balk at this claim. “How can lowly insertion sort, whose average behavior is <code>O(n<sup>2</sup>)</code>, outperform the mighty Quicksort routine, with its `O(n * log n)` average time?” The secret lies in the fact that if we treat the array as two subarrays, one part sorted and the other part containing data to be inserted in order into the sorted part, then our algorithm need only perform a few insertions into the sorted part to accomplish our sorting task.

Quicksort will process the entire array, without exploiting the fact that most of the sorting task is already done. For certain proportions of the size of the sorted and unsorted portions of the array, this insertion sort method outperforms Quicksort.

[Listing 1](INSSORT.C) contains source code for this special insertion sort routine. This code assumes that the array contains unsigned integers, and is to be sorted in ascending order. The idea is simple. For each element in the unsorted portion of the array, first copy the value into a temporary variable. Then work backwards through the sorted portion of the array, copying each value that is larger than the item to be inserted down one slot in the array.

When control exits the inner `for` loop, `list[j]` is the array element that contains the largest value in the array that is not larger than the item to be inserted, and `list[j + 1]` has been copied into `list[j + 2]`, with all subsequent values following in sequence. In essence, the code has moved all of the values greater than our item down one position in the array, opening up a slot `list[j + 1]` where that value should be placed to maintain sorted order.

## Performance

This special insertion sort routine performs best when the number of insertions to be made is small. As the size of the unsorted portion of the array grows relative to the sorted portion, the performance advantage decreases. Eventually, for a certain number of unsorted additions, the time taken to painstakingly move each unsorted value into its rightful place exceeds the time needed if we ignore our special array properties and just use Quicksort. This crossover point depends in each case on the sizes of the two portions.

I've run tests for different array sizes and proportions of sorted to unsorted elements, with the results shown in Table 1.

| Size of Array | Size of Sorted Portion | Size of Unsorted Portion | Proportion |
|---------------|------------------------|--------------------------|------------|
| 500           | 489                    | 11                       | 2.25%      |
| 1000          | 979                    | 21                       | 2.15%      |
| 2000          | 1952                   | 48                       | 2.46%      |
| 5000          | 4900                   | 100                      | 2.04%      |
| 7500          | 7368                   | 132                      | 1.79%      |
| 10000         | 9812                   | 188                      | 1.92%      |
| 15000         | 14692                  | 308                      | 2.10%      |
| 20000         | 19564                  | 436                      | 2.23%      |

*Table 1. Sizes of sorted and unsorted portions of arrays. For each table entry, a smaller unsorted portion results in the special insertion sort outperforming Quicksort, while a larger unsorted portion makes Quicksort the better performer.*

Roughly speaking, if the number of unsorted elements to be inserted is less than 2 percent of the size of the sorted portion, then this special sort method gives us a performance advantage over blindly using Quicksort. The solution I employed for my application calculates this proportion, and uses this special insertion sort technique if the number of additions is below this 2 per cent threshold. It uses Quicksort if there are lots of additions to be made.

The whole point here is that knowledge of special characteristics of the data set to be processed can alter the choice of algorithm that yields the best performance in practice. The moral of this story? Take advantage of any special properties you know about the data set when choosing an algorithm, and you too can do better than the “best”.
