# Wagner

![badge.cpp](https://img.shields.io/badge/c++-14-008080.svg?style=flat-square)
[![license][badge.license]][license]

[badge.license]: https://img.shields.io/badge/License-MIT%202.0-7ec0ee.svg

[license]: https://github.com/PhDP/wagner/blob/master/LICENSE

A theoretical model to study macroevolution and phylogeographies.

Named after Moritz Wagner (1813-1887), who made important contributions to our
understanding of speciation.

The program is written in C++14. Tested on Linux 64-bit with clang++ 4.0
and g++ 6.3.0.

## Reference

Working paper: http://arxiv.org/abs/1203.1790

## Dependencies

* [boost](http://www.boost.org/) [optional]. Will default to the much slower
  std::set/std::map if boost is unavailable.
* [cmake](https://cmake.org/) for building

usage and options
-----------------
Compile with make (requires a modern ANSI C++14 compiler), e.g.:

    $ mkdir build && cd $_
    $ cmake ..
    $ make

 and execute with

    $ ./src/wagner_exe

You can change the model with:

    -model

The current version supports four models:

    0   Neutral model
    1   Phylogenetic distance
    2   Euclidean distance with traits [default]
    3   Fuzzy distance with traits

You can also use the following options [default values]:

    -threads    Number of threads to launch [number of available cores].
    -seed       Seed for the random number generator [6].
    -c          Number of communities (or vertices, or nodes, or patches) [64].
    -t          Number of time steps. Needs to be a power of two [512].
    -e          Per population extinction rate [0.05].
    -m          Max migration rate [0.04].
    -n          Number of traits for models 2-3 [10].
    -w          Standard deviation of the white noise applied to traits after each time step [0.001].
    -a          Aleph for models 1-2 [10.0].
    -s          Speciation rate [0.04].
    -r          Radius of the random geometric network [0.2].

Options not followed by an argument

    -shuffle    After t/2 time steps, shuffle all populations [false].

For example:

    $ ./ẃagner -model 1 -a 50 -seed 88

Will run the model 1 with aleph = 50, and seed = 88. The order of the options
doesn't matter and all parameters should be printed in the xml output file. If
the number of time steps supplied is not a power of two, the problem will find
the largest power of two that fits in this number.

license
-------
[MIT](https://github.com/PhDP/wagner/blob/master/LICENSE) <http://opensource.org/licenses/MIT>

author
------
Philippe Desjardins-Proulx <philippe.d.proulx@gmail.com>
