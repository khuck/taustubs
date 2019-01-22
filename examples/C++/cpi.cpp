
/* Code from:
https://codereview.stackexchange.com/questions/150749/multithreaded-monte-carlo-pi-approximation-with-own-pseudorandom-number-generato
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <thread>
#include <random>
#include <vector>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <future>
#include "big_wheel.hpp"
#define TAU_USE_STUBS
#include "tautimer.hpp"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

typedef struct
{
    double x, y, z;
} point_t;

static const double PI = 3.1415926535897932384626433832795;
static const std::uint32_t prng_max_value = big_wheel_engine<>::max();

double approximate_pi(std::uint32_t pc,std::uint32_t tp)
{
    return 4.0*(double(pc)/double(tp));
}

double pi_error(double x)
{
    return 100.0*((x-PI)/PI);
}

// Points within circle
std::atomic<std::uint32_t> pcnt(0);

template <class RandEng>
void calculate_pi(std::uint32_t nppt,RandEng rgen)
{
    taustubs::TauTimer::RegisterThread();
    TAUSTUBS_SCOPED_TIMER_FUNC(mytimer);
    point_t rpt;
    std::uint32_t my_pcnt = 0;
    for (std::uint32_t i=0;i<nppt;i++)
    {
        rpt.x = double(rgen())/static_cast<double>(prng_max_value);
        rpt.y = double(rgen())/static_cast<double>(prng_max_value);
        rpt.z = (rpt.x*rpt.x)+(rpt.y*rpt.y);
        if (rpt.z <= 1.0) my_pcnt++;
    }
    pcnt += my_pcnt;
}

int main (int UNUSED(argc), char** UNUSED(argv)) 
{
    TAUSTUBS_SCOPED_TIMER_FUNC(mytimer);
    // Check number of cores
    std::size_t num_cores = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;

    // Thread set
    std::vector<std::thread> thread_set;
    std::vector<std::future<void> > future_set;

    // Generate seeds
    std::random_device rd;
    std::uint32_t seed[16] = {0x0};

    for (int i=0;i<16;i++)
        seed[i] = rd();

    // Initialize PRNG
    big_wheel_engine<> rnd(
        seed[ 0],seed[ 1],seed[ 2],seed[ 3],
        seed[ 4],seed[ 5],seed[ 6],seed[ 7],
        seed[ 8],seed[ 9],seed[10],seed[11],
        seed[12],seed[13],seed[14],seed[15]
    );

    std::uint32_t num_points = 3221225472;
    std::uint32_t num_points_per_thread = num_points/num_cores;

    for (std::size_t i=0;i<num_cores;i++)
    /*
        thread_set.push_back(
            std::thread(
                calculate_pi<big_wheel_engine<>>,
                num_points_per_thread,
                rnd
            )
        );
        */
        future_set.push_back(
            std::async( std::launch::async,
                calculate_pi<big_wheel_engine<>>,
                num_points_per_thread,
                rnd
            )
        );
            

    // Calculated Pi value
    double gen_pi = 0;

    // Calculation error
    double pi_err = 0;

    // Duration
    double dur = 0;

    // Print info
    std::cout << "Seeds:\n";
    for (int iy=0;iy<4;iy++)
    {
        std::cout << " ";
        for (int ix=0;ix<4;ix++)
            std::cout << std::setw(11) << std::setfill(' ') << seed[4*iy+ix] << " ";
        std::cout << "\n";
    }
    std::cout << "Number of points: " << num_points << "\n";
    std::cout << "Number of cores: " << num_cores << "\n";
    std::cout << " ==> Number of points per thread: " << num_points_per_thread << "\n\n";

    std::cout << "Generating random points...\n";

    std::chrono::high_resolution_clock::time_point tpa = std::chrono::high_resolution_clock::now();

    for (std::size_t i=0;i<num_cores;i++)
        future_set[i].get();
        //thread_set[i].join();

    std::chrono::high_resolution_clock::time_point tpb = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> tm = std::chrono::duration_cast<std::chrono::duration<double>>(tpb-tpa);
    dur = tm.count();

    // Approximate pi and calculate error
    gen_pi = approximate_pi(pcnt,num_points);
    pi_err = pi_error(gen_pi);

    std::cout << "\nResults:\n";
    std::cout << "  Number of points within circle: " << pcnt << "\n";
    std::cout << "  Pi approximation:               " << std::setprecision(32) << gen_pi << "\n";
    std::cout << "  Error:                          " << std::setprecision(10) << pi_err << "%\n";

    std::cout << "Time elapsed: " << std::setprecision(10) << dur << " seconds\n";
}


