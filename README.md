![Fury of Dracula](https://www.cse.unsw.edu.au/~cs2521/20T2/ass/ass2/Pics/drac.png)

![Travis-CI Build Status](https://travis-ci.com/lucasbrsa/fury-of-dracula.svg?token=T88Zm2yy8Q4icx3jiDrh&branch=master)

# fury-of-dracula

### CS2521 Assignment 

Group Name: stephen-eric-lucas-debbie

Tutorial Code: T17A

| Github Name  | Real Name | zID |
| -------------      | -------------   | ------------- |
| Stephenmorrisunsw  | Stephen Morris  | z5230723  |
| ericholm           | Eric Holmstrom | z5310814  |
| lucasbrsa         | Lucas Barbosa   | z5259433  |

All we do is code and win.


### Usage

Run `make` in the view folder to generate all the executables. This should generate
the following executables:
```
our_test testGameView testHunterView testDraculaView testMap
```
The last 4 are the base implementations given to use by the base template. These contain a couple
of tests we can use to determine whether we have implemented the view properly.

The `our_test` executable is from the `our_test.c` which contains some miscellaneous testing such as path finding testing
and ADT implementation test.


Additionally, there is a test script which runs the supplied tests to run these combined tests run ```run_tests.sh```. Also
a `.travis.yml` file has been added which when this repository is connected up to Travis CI will allow us to run automated testing
upon every commit to master to ensure that any of the latest changes haven't broken anything.

Lucas Barbosa | Stephen Morris | Eric Holmstrom | Debbie-Tommy

