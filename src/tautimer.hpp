/*
Copyright (c) 2019 University of Oregon
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#pragma once

#include <memory>
#include <string>

namespace taustubs {

class TauTimer
{
public:
    static void RegisterThread(void);
    static void Start(const char * timer_name);
    static void Start(const std::string &timer_name);
    static void Stop(const char * timer_name);
    static void Stop(const std::string &timer_name);
    static void SampleCounter(const char * name, const double value);
    static void MetaData(const char * name, const char * value);
    // The only way to get an instance of this class
    static TauTimer &get(void) { 
         static std::auto_ptr<TauTimer> instance( new TauTimer );
         return *instance;
    }
    // destructor
    ~TauTimer(void);
    bool initialized;
private:
    // Private constructor to prevent construction
    TauTimer(void);
    // Prevent copies
    TauTimer(const TauTimer &old);
    const TauTimer &operator = (const TauTimer &old);
};

class scoped_timer {
    private:
        std::string _name;
    public:
        scoped_timer(const std::string &name) : _name(name) { 
            TauTimer::Start(_name);
        }
        ~scoped_timer() {
            TauTimer::Stop(_name);
        }
}; 

}; // namespace taustubs

#if defined(TAU_USE_STUBS)
#define TAUSTUBS_SCOPED_TIMER(__var,__name) taustubs::scoped_timer __var(__name);      
#define TAUSTUBS_SCOPED_TIMER_FUNC(__var) taustubs::scoped_timer __var(__func__);      
#else
#define TAUSTUBS_SCOPED_TIMER(__var,__name)
#define TAUSTUBS_SCOPED_TIMER_FUNC(__var)
#endif

