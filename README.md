Text To Frequency
=================
This is a toy program that converts a string into a sound file, where the input
string is visible in the frequency domain.

PCM audio output is sent to standard output by default. The default PCM output
format is mono, signed 16 bit, with a 44.1 kHz sampling rate by default. The
sampling rate is configurable with the -r option.

Two makefiles are provided, one for BSD (tested on OpenBSD), and one for Linux.

The BSD makefile (`Makefile.bsd`) defaults to using clang, and the Linux
makefile (`Makefile.linux`) defaults to using `gcc`.

Example usage (the makefile defaults to the clang compiler, and is a BSD
makefile):

```console
# on Linux
$ make -f Makefile.linux
clang -c ttf.c -Wall
clang -o ttf ttf.o -lm

# on BSD
make -f Makefile.bsd

# generate a "hello world" ogg file
$ ./ttf "hello world" | oggenc -r -q 7 -C 1 - > test.ogg
```

The output can be viewed with a spectrum analyzer, such as [this
one](https://www.dcode.fr/spectral-analysis).
