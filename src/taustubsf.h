/*  
Copyright (c) 2019 University of Oregon
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/* Macro API for option of entirely disabling at compile time */
/* To use this API, set the Macro TAU_USE_STUBS on the command */
/* line or in a config.h file, however your project does it  */

#ifndef TAUSTUBSF_H
#define TAUSTUBSF_H

#if defined(TAU_USE_STUBS)
#define TAU_REGISTER_THREAD() call TauTimer_RegisterThread();
#define TAU_START(_timer_name) call TauTimer_Start(_timer_name//CHAR(0));
#define TAU_STOP(_timer_name) call TauTimer_Stop(_timer_name//CHAR(0));
#define TAU_SAMPLE_COUNTER(_name, _value) call TauTimer_SampleCounter(_name//CHAR(0), _value);
#define TAU_METADATA(_name, _value) call tau_metadata(_name//CHAR(0), _value//CHAR(0));
#else
#define TAU_REGISTER_THREAD()
#define TAU_START(_timer_name)
#define TAU_STOP(_timer_name)
#define TAU_SAMPLE_COUNTER(_name, _value)
#define TAU_METADATA(_name, _value)
#endif

#endif // TAUSTUBS_H
