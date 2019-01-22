# TAUSTUBS 

*This is work in progress...*

The idea behind taustubs is that an application can have permanent TAU
instrumentation in it, but not have TAU as a build dependency.  Basically, the
instrumentation relies on function pointers that are initialized to NULL, and
then set at program start if the TAU functions are defined in the program
space (i.e. if the TAU preload libraries are used when running with `tau_exec`).
Admittedly, that requires adding this project (or it's code) as a dependency,
but without the headache of dealing with a highly customizable performance
measurement framework like TAU.  For more information, see [the TAU
website](http://tau.uoregon.edu).  To download the latest TAU code, see [the
TAU public mirror](http://github.com/UO-OACISS/tau2).

see [https://rafalcieslak.wordpress.com/2013/04/02/dynamic-linker-tricks-using-ld_preload-to-cheat-inject-features-and-investigate-programs/] for some more interesting ideas.

## How to use this code:

To just test out this code, edit (if necessary) and run the `go.sh` script.
This will build the library and the examples.  Then just run the programs and
see the output.  You should see the programs execute normally, with some
warning output that the TAU libraries weren't found.

```
khuck@ktau:~/src/taustubs$ ./build/bin/matmult_C 
TAU libraries not loaded, TAU support unavailable
/home/khuck/src/taustubs/build/lib/libtaustubs.so.0.1: undefined symbol: Tau_init
Done.
```

To run with TAU (see
http://github.com/UO-OACISS/tau2), configure TAU with:

```
./configure -pthread -bfd=download
make -j install
```

then run the same examples with `tau_exec` and use `pprof` to dump the profile
using text output (the *serial* option for TAU means non-MPI):

```
khuck@ktau:~/src/taustubs$ tau_exec -T serial,pthread ./build/bin/matmult_C
Done.
khuck@ktau:~/src/taustubs$ pprof -s
Reading Profile files in profile.*

FUNCTION SUMMARY (total):
---------------------------------------------------------------------------------------
%Time    Exclusive    Inclusive       #Call      #Subrs  Inclusive Name
              msec   total msec                          usec/call 
---------------------------------------------------------------------------------------
100.0        0.593       11,900           4           4    2975091 .TAU application
 99.8           31       11,875           4          32    2968856 do_work
 74.9        0.011        8,917           3           3    2972337 threaded_func
 57.6        6,860        6,860           4           0    1715062 compute
 40.0        4,762        4,762           4           0    1190562 compute_interchange
 25.1        0.398        2,982           1           7    2982759 main
  1.8          218          218          12           0      18226 allocateMatrix
  0.2           23           23           3           0       7898 pthread_join
  0.0            2            2          12           0        248 freeMatrix
  0.0        0.246        0.246           3           0         82 pthread_create

FUNCTION SUMMARY (mean):
---------------------------------------------------------------------------------------
%Time    Exclusive    Inclusive       #Call      #Subrs  Inclusive Name
              msec   total msec                          usec/call 
---------------------------------------------------------------------------------------
100.0        0.148        2,975           1           1    2975091 .TAU application
 99.8            7        2,968           1           8    2968856 do_work
 74.9      0.00275        2,229        0.75        0.75    2972337 threaded_func
 57.6        1,715        1,715           1           0    1715062 compute
 40.0        1,190        1,190           1           0    1190562 compute_interchange
 25.1       0.0995          745        0.25        1.75    2982759 main
  1.8           54           54           3           0      18226 allocateMatrix
  0.2            5            5        0.75           0       7898 pthread_join
  0.0        0.743        0.743           3           0        248 freeMatrix
  0.0       0.0615       0.0615        0.75           0         82 pthread_create

```

## How to use this code in *your* project

To use this code in your project, either add the files in the `src` directory
to your project, or add the taustubs library as a build dependency.  To add
instrumentation to your project, include `tautimer.hpp` (C++), `taustubs.h`
(C), or `taustubsf.h` (Fortran) to your source file (making sure to define
`USE_TAU_STUBS` first - otherwise the macros will be compiled away), and add
the API calls.  See the examples for details, but the API includes:

* `TAU_REGISTER_THREAD()` - the first thing a spawned pthread should do, lets TAU
  know that there is a new thread, in case `pthread_create()` wasn't captured.
* `TAU_START(_timer_name)` - start a timer with the name `_timer_name`.
* `TAU_STOP(_timer_name)` - stop a timer with the name `_timer_name`.
* `TAU_START_FUNC()` - start a timer with the name of the current function (using
  the `__func__` preprocessor macro).
* `TAU_STOP_FUNC()` - start a timer with the name of the current function (using
  the `__func__` preprocessor macro).
* `TAU_SAMPLE_COUNTER(_name, _value)` - sample a counter with the name `_name` and
  the value `_value`
* `TAU_METADATA(_name, _value)` - capture metadata as a key/value pair with
  the name `_name` and the value `_value`

The C++ API also has scoped timers for convenience:

* `TAUSTUBS_SCOPED_TIMER(__var,__name)` - start a timer with the name `__name` and
  create a stack variable `__var`.  When `__var` goes out of scope, stop the timer
  in the destructor of `__var`.
* `TAUSTUBS_SCOPED_TIMER_FUNC(__var)` - start a timer with the name of the current
  function (using the `__func__` preprocessor macro) and create a stack
  variable `__var`.  When `__var` goes out of scope, stop the timer in the
  destructor of `__var`.

