Text To Frequency
=================
This is a toy program that converts a string into a sound file, where the input
string is visible in the frequency domain.

PCM output is sent to stdout by default.

Example usage:

```console
$ make
clang -c ttf.c -Wall
clang -o ttf ttf.o -lm
$ ./ttf "hello world" | oggenc -r -q 7 -C 1 - > test.ogg
```
