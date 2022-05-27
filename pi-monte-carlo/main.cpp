#include <array>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

const uintmax_t N = 100'000'000;
const uint8_t no_of_threads = 8;

int count_hits(uintmax_t n)
{
    std::random_device rd;
    std::mt19937_64 rnd_gen(rd());
    std::uniform_real_distribution<double> rnd_distr {0.0, 1.0};

    uintmax_t hits {};
    for (uintmax_t i = 0; i < n; ++i)
    {
        double x = rnd_distr(rnd_gen);
        double y = rnd_distr(rnd_gen);

        if (x * x + y * y < 1.0)
            ++hits;
    }

    return hits;
}

void calc_pi_single_thread(uintmax_t n)
{
    auto t_start = std::chrono::high_resolution_clock::now();

    auto hits = count_hits(n);

    double pi = 4 * (static_cast<double>(hits) / n);

    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << "ms\n";
}

void calc_pi_multithreading(uintmax_t n)
{
    auto no_of_cores = std::thread::hardware_concurrency();
    std::cout << "No of cores: " << no_of_cores << "\n";

    const uintmax_t n_per_thread = n / no_of_cores;

    auto t_start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> thds;
    std::vector<uintmax_t> partial_hits(no_of_cores);

    for (uint8_t i = 0; i < no_of_cores; ++i)
    {
        thds.push_back(std::thread([i, n_per_thread, &partial_hits]
            { partial_hits[i] = count_hits(n_per_thread); }));
    }

    for (auto& thd : thds)
    {
        thd.join();
    }

    uintmax_t hits {};
    for (const auto& ph : partial_hits)
        hits += ph;

    double pi = 4 * (static_cast<double>(hits) / n);

    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << "ms\n";
}

void calc_pi_multithreading_with_mutex(uintmax_t n)
{
    auto no_of_cores = std::thread::hardware_concurrency();
    std::cout << "No of cores: " << no_of_cores << "\n";

    const uintmax_t n_per_thread = n / no_of_cores;

    auto t_start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> thds;
    uintmax_t hits = 0;
    std::mutex mtx_hits;

    for (uint8_t i = 0; i < no_of_cores; ++i)
    {
        thds.push_back(std::thread([i, n_per_thread, &hits, &mtx_hits] { 
            auto local_hits = count_hits(n_per_thread);
            
            // mtx_hits.lock(); // Critical section starts
            // hits += local_hits;
            // mtx_hits.unlock(); // Critical section ends

            {
                std::lock_guard<std::mutex> lk{mtx_hits}; // mtx_hits.lock();
                hits += local_hits;
            } // mtx_hits.unlock();
        }));
    }

    for (auto& thd : thds)
    {
        thd.join();
    }

    double pi = 4 * (static_cast<double>(hits) / n);

    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << "ms\n";
}

int main()
{
    calc_pi_single_thread(N);

    std::cout << "\n-----------------------\n";

    calc_pi_multithreading(N);

    std::cout << "\n-----------------------\n";

    calc_pi_multithreading_with_mutex(N);
}
