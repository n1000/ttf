Text To Frequency
=================

This is a toy program that converts a string into a sound file, where the input
string is visible in the frequency domain.

PCM audio output is sent to standard output by default. The default PCM output
format is mono, signed 16 bit, with a 44.1 kHz sampling rate. The sampling rate
is configurable with the -r option.

Two makefiles are provided, one for BSD (tested on OpenBSD), and one for Linux.

The BSD makefile (`Makefile.bsd`) defaults to using clang, and the Linux
makefile (`Makefile.linux`) defaults to using `gcc`.

## Usage guide

Compile for Linux or Cygwin:

```console
# on Linux (or in Cygwin)
$ make -f Makefile.linux
gcc -c ttf.c -Wall
gcc -o ttf ttf.o -lm
```

Compile on BSD (tested on OpenBSD):

```console
# or, on BSD
$ make -f Makefile.bsd
clang -c ttf.c -Wall
clang -o ttf ttf.o -lm
```

Get command line usage options:

```console
# get basic usage help
$ ./ttf
usage: ttf [-l low_freq (Hz)] [-h high_freq (Hz)] [-r sampling_rate (Hz)] [-d character_duration (sec)] [-g character gap (sec)] string
```

The example below uses `oggenc` to convert the PCM output into the OGG Vorbis
format. `oggenc` can be found in the `vorbis-tools` package on Ubuntu, OpenBSD,
and Cygwin.

Generate a "hello world" example file (the same as the `example.ogg` included
in  this repository):

```console
# generate a "hello world" ogg file
$ ./ttf "hello world" | oggenc -r -q 7 -C 1 - > test.ogg
```

The output can be viewed with a spectrum analyzer, such as [this
one](https://www.dcode.fr/spectral-analysis).
