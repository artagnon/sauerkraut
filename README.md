# Sauerkraut

A mishmash of various matrix multiplication algorithms, with hard-coded shape
(square) and type (int) of matrices, for simplicity. Sizes are generated at
compile-time using templates; we use static sizes for simplicity, again.

## Usage

```sh
$ cd sauerkraut-build
$ cmake -GNinja ..
$ ninja check
$ ninja timing
```

This validates all algorithms, and gets timing data.

The tiled parallel multiply takes 1.7 seconds, while the tiled serial one takes
0.8 seconds, on my machine. This is probably due to the cost of spawning threads
for a tiny matrix.

## Primary reference materials
1. [What Every Programmer should know about Memory, Drepper](http://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
2. [Lecture Notes, Mirek](http://www.ccs.neu.edu/home/mirek/classes/2011-F-CS6240/Slides/Lecture2-large.pdf)
3. [Introduction to Parallel Computing in C++, Acar/Chargueraud/Rainey](http://www.cs.cmu.edu/~15210/pasl.html)
