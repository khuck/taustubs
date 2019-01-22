/*
Copyright (c) 2019 University of Oregon
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef _INIT_H_
#define _INIT_H_

#ifdef __cplus_plus 
extern "C" {
#endif /* __cplus_plus */

extern void initialize(double **matrix, int rows, int cols);

#ifdef __cplus_plus 
}
#endif /* __cplus_plus */

#endif /* _INIT_H */
