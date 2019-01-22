! Copyright (c) 2019 University of Oregon
! Distributed under the Boost Software License, Version 1.0. (See accompanying
! file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define TAU_USE_STUBS 1
#include "taustubsf.h"

      subroutine HELLOWORLD(iVal)
        integer iVal
        TAU_START('HelloWorld()')
! Do something here...
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

