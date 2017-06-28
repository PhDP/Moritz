Wagner
======
A theoretical model to study macroevolution and phylogeographies.

Named after Moritz Wagner (1813-1887), who made important contributions to our
understanding of speciation.

The program is written in ANSI C++11. Tested on Linux 64-bit with clang++ 3.0
and g++ 4.7.0.

reference
---------
Working paper: http://arxiv.org/abs/1203.1790

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
    1   Model with aleph (see arXiv paper) [default]
    2   Model with logistic speciation (decreases with the total number of species).
    3   Model with both aleph and logistic speciation.
    4   Model with traits (set option n for the number of traits)

You can also use the following options [default values]:

    -seed       Seed for the random number generator [6].
    -c          Number of communities (or vertices, or nodes, or patches) [64].
    -t          Number of time steps. Needs to be a power of two [512].
    -e          Per population extinction rate [0.05].
    -m          Max migration rate [0.04].
    -n          Number of traits for the traits model [10].
    -w          Standard deviation of the white noise applied to traits after each time step [0.001].
    -a          Aleph for models 1 and 3 [10.0].
    -s          Speciation rate (or max speciation rate for logistic models) [0.04].
    -se         With logistic speciation and n species, the speciation rate is 2s/(1 + se^n) [1.02].
    -r          Radius of the random geometric network [0.2].

Options not followed by an argument

    -shuffle    After t/2 time steps, shuffle all populations [false].

For example:

    $ ./ẃagner -model 1 -a 50 -seed 88

Will run the model 1 with aleph = 50, and seed = 88. The order of the options
doesn't matter and all parameters should be printed in the xml output file. If
the number of time steps supplied is not a power of two, the problem will find
the largest power of two that fits in this number.

language
--------
ANSI C++11.

I use many features from the C++11 standard so it requires a modern compiler.

license
-------
MIT <http://opensource.org/licenses/MIT>

author
------
Philippe Desjardins-Proulx <philippe.d.proulx@gmail.com>
