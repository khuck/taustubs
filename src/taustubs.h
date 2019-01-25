/*
    Copyright (c) 2019 University of Oregon
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TAUSTUBS_H
#define TAUSTUBS_H

/* regular C API */

void TauTimer_RegisterThread();
void TauTimer_Start(const char * timer_name);
void TauTimer_Stop(const char * timer_name);
void TauTimer_SampleCounter(const char * name, const double value);
void TauTimer_MetaData(const char * name, const char * value);

/*
    Macro API for option of entirely disabling at compile time
    To use this API, set the Macro TAU_USE_STUBS on the command
    line or in a config.h file, however your project does it
 */

#define TAU_USE_STUBS
#if defined(TAU_USE_STUBS)
#define TAU_REGISTER_THREAD() TauTimer_RegisterThread();
#define TAU_START(_timer_name) TauTimer_Start(_timer_name);
#define TAU_STOP(_timer_name) TauTimer_Stop(_timer_name);
#define TAU_START_FUNC() TauTimer_Start(__func__);
#define TAU_STOP_FUNC() TauTimer_Stop(__func__);
#define TAU_SAMPLE_COUNTER(_name, _value) TauTimer_SampleCounter(_name, _value);
#define TAU_METADATA(_name, _value) TauTimer_MetaData(_name, _value);
#else
#define TAU_REGISTER_THREAD()
#define TAU_START(_timer_name)
#define TAU_STOP(_timer_name)
#define TAU_START_FUNC()
#define TAU_STOP_FUNC()
#define TAU_SAMPLE_COUNTER(_name, _value)
#define TAU_METADATA(_name, _value)
#endif

#endif // TAUSTUBS_H
