cc Copyright (c) 2019 University of Oregon
cc Distributed under the Boost Software License, Version 1.0. (See accompanying
cc file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "taustubsf.h"

      subroutine HELLOWORLD(iVal)
        integer iVal
        TAU_START('HelloWorld()')
cc Do something here...
        print *, "Iteration = ", iVal
        TAU_STOP('HelloWorld()')
      end

      program main
        integer i
        TAU_START('main()')
        print *, "test program"

        do 10, i = 1, 10
          call HELLOWORLD(i)
10      continue
        TAU_STOP('main()')
      end

