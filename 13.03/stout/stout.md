# “Olympic” Filtering for Noisy Data

**Bob Stout**

*Bob Stout is Director of Software development for Bridgeway Software in Bellaire, Texas. Familiar as an ex-moderator of the FidoNet C language EchoMail conference and archivist for the "SNIPPETS" collection of free Csource code, Bob is also the author of 28 DOS shareware utilities plus libraries for the Symantec, Watcom, Microsoft, and Borland C and C++ compilers, published by his own company, MicroFirm. Contact him at P.O. Box 428, Alief, TX 77411; bobstout@neosoft.com; or Fidonet 1:06/2000.6.*

---

Compared to the lowly office computer, the typical embedded system lives in a harsh environment. Embedded systems must often interact with events and processes occuring beyond the nearest desktop, or outside the safety of the computer room. The real world works on non-digital principles, so embedded systems typically perform some sort of analog data acquisition, and that analog data is subject to noise. For embedded system to perform appropriately, noise in analog input data streams must therefore be filtered.

In this article I present a simple filtering technique that will work on a small CPU, and that is especially effective with impulse noise – one of the most common and most pernicious types. This technique is not a cure-all, but with it you may avoid much more expensive solutions to your noise problems that would amount to overkill.

## Need a Short Cut?

We already have lots of fixes for noise, both in hardware and in software. The list of available hardware includes traditional analog filters, EMI filters, and Digital Signal Processing (DSP) chips. These are all great solutions *if* they are already installed on your embedded system. If you have to add these components, they stop looking so attractive.

Fancy algorithms seem appealing, at least on the surface. Over the years, many standard types of signal filters have been synthesized in software. These software filters will kill the noise, but they require a powerful CPU. Herein lies a problem. Many embedded systems still rely on 16- and even eight-bit CPUs because of their low cost and low-power operation. Any but the most trivial digital filtering techniques can easily use up much of the available bandwidth of these smaller CPUs, leaving little time left over for doing the work for which they were intended.

The same hardware limitation applies to Finite Impulse Response (FIR), Infinite Impulse Response (IIR), correlation, and frequency-domain analysis. Computing correlations and frequency-domain analyses in real time actually requires more CPU horsepower than most of us have in our desktop systems! The above discussion raises the question: is there a short cut? Can a non-hardware filter remove noise without choking an embedded system CPU? The answer is yes, in some cases.

## Enter “Olympic” Filtering

The noise in analog data will typically be one of two types, AC or impulse. You can hear both of these types of noise in your own home audio system. An example of AC noise is the line frequency hum heard when some component is poorly grounded or shielded. An example of impulse noise is when someone turns on a high-current appliance and you hear a pop or snap though the speakers.

Averaging can work well with AC noise. By careful selection of the sampling frequency and filter interval, the effects of an interfering signal may be readily removed. However, impulse noise will still affect averaging in an unpredictable manner. Using a weighted average can improve the filter's responsiveness and rejection of AC noise, but again does little to help with impulse noise.

What to do?

Many years ago I borrowed an idea from the Olympics, where an athlete's scores were determined by throwing out the highest and lowest scores from the panel of judges and averaging the rest. I asked myself if I could eliminate my "bad" data in the same manner. Implemented in assembly language, the resulting algorithm was simple and proved to be remarkably effective. In the years since, I've used this same simple filtering technique successfully on dozens of embedded projects in several different languages. This technique has worked especially well in equipment mounted adjacent to, and powered from, the same power lines as 60+ HP electric motors that are switched on and off asynchronously at frequent intervals. It has also worked well in environments with analog inputs derived from potentiometer sensors, which get increasingly noisy over time.

Why does olympic filtering work? All averaging filters depend on oversampling, i.e., you always have more data than you really need. When averaging, the redundancy in the "good" data makes a more significant contribution to the result than does the anomalous data. Any additive noise having a mean value of zero – such as AC noise – is a candidate for an averaging filter. Impulse noise presents special problems, however. Impulse noise is intermittent and, when it does occur, typically is not additive. Noise spikes tend to replace normal data rather than simply inducing an offset. Olympic filtering identifies probable anomalous data and removes it before the average is calculated. If there is no anomalous data, then the olympic filter responds the same as a simple averaging filter. Its only added cost is a requirement for oversampling, which provides two more samples per averaging period that will never actually be used.

## Implementation

Implementing an olympic filter is simple. Store the acquired data in a simple circular buffer. To obtain an output value, sum the data, subtract out the high and low values, and divide the sum by the buffer size minus two. I've always used buffer sizes that were two plus an integral power of two, so that after removing the high and low values, I could calculate the average by shifting rather than division.

In [Listing 1](CIRCBUF.H) and [Listing 2](CIRCBUF.C) I show a standard C implementation, since good quality C compilers are available for virtually any target platform you might choose. However, for many low-end CPUs, assembly language might be a better choice. I think that for embedded PCs, C++ would clearly be superior. (Circular buffer management in general, and filtering algorithms in particular, are especially well suited to a C++ implementation when writing for embedded platforms with available C++ compilers. Although omitted here for space, the following files are available on this month's code disk and online sources: [`CIRCBUF.HPP`](CIRCBUF.HPP), a template-based circular-buffer class; [`OLYMFILT.HPP`](OLYMFILT.HPP), an olympic filter derived from the circular-buffer base class; [`AVGFILT.HPP`](AVGFILT.HPP), another straight averaging filter for comparison, also derived from the circular-buffer base class; and [`FILTDEMO.CPP`](FILTDEMO.CPP), a demonstration program.) The underlying algorithm retains its efficiency regardless of target environment or implementation tools.

[Listing 1](CIRCBUF.H) shows the header file `CIRCBUF.H`. Note that the buffer is embedded in a `struct` object containing a pointer to the actual buffer. This `struct` also contains the buffer's size, a pointer to the next location to fill, and a flag to indicate whether the buffer has been filled. This last variable is important. In any filtering algorithm, running the filter before the buffer has been filled will greatly skew the result. The filter output will only be valid to the extent that the buffer data are valid.

[Listing 2](CIRCBUF.C) defines the three functions essential to the filter's implementation: allocating a buffer of the required size, adding data to it, and running the olympic filter on it. `CIRCBUF.C` also includes a straight averaging filter for comparison and a sample `main` function which may be used for testing by simply defining the macro `TEST`.

Both the olympic and averaging filters take a "snapshot", copying the buffer to temporary storage. In a typical system, the sample acquisition (and hence the `add` function) will run at intervals based on timer interrupts. Since these interrupts may occur asynchronously, good coding practice dictates that you somehow stabilize the data set before processing. Other alternatives would be to simply disable the timer interrupt while processing the buffer, or to perform data acquisition synchronously with other tasks. As written, the code presented really needs modification to disable the timer interrupts while taking the snapshot.

## Testing and Tuning

Compiling the sample demo with the `TEST` macro defined easily demonstrates the differences between olympic and "straight" averaging filters. Using well-behaved data and a six sample buffer size results in comparable performance from the two filters. For example, entering
```none
CIRCBUF 1 2 3 4 5 6 7 8 9 10
```
shows no difference between the results of the two types of filters, whereas entering
```none
CIRCBUF 3 4 4 5 -200 1000
```
shows dramatically different results typical of using an olympic filter in the presence of significant impulse noise.

Olympic filtering is clearly superior to an averaging filter on data containing significant impulse noise. Like other averaging filters, olympic filters can also be tuned for good performance in the presence of AC noise by setting a sample rate equal to `1/(noise_frequency * buffer_size)`. For example, if you are using an olympic buffer containing six samples, a sampling interval of 2.78 msec will provide your system with good immunity from induced 60 Hz noise. Choice of buffer size also dramatically affects the outcome. Small buffers reject the most noise and respond to changing conditions more quickly, at the expense of potentially discarding the most good data. Large buffers take longer to fill and respond more slowly, but eliminate more AC noise.

## Give it a Try

No one type of signal filtering works for all applications. I've briefly discussed some of the most common algorithms and their suitability to smaller embedded systems projects. The olympic filter is a simple algorithm with a successful track record operating in some of the noisiest environments imaginable. Although I've never read of this algorithm being published elsewhere, it's been in my bag of tricks for years. Try it on your next embedded project when noise is a problem. You may find, as I often have, that it's all you need.
