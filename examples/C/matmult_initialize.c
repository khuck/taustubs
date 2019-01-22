/*
Copyright (c) 2019 University of Oregon
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "matmult_initialize.h"
#include "taustubs.h"

void initialize(double **matrix, int rows, int cols) {
  TAU_START(__func__);
  int i,j;
#pragma omp parallel private(i,j) shared(matrix)
  {
    //set_num_threads();
    /*** Initialize matrices ***/
#pragma omp for nowait
    for (i=0; i<rows; i++) {
      for (j=0; j<cols; j++) {
        matrix[i][j]= i+j;
      }
    }
  }
  TAU_STOP(__func__);
}

