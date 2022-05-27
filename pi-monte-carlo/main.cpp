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
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

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

void calc_pi_multiprocessing(uintmax_t n)
{
    const auto no_of_processes = 12;
    std::cout << "No of processes: " << no_of_processes << "\n";

    auto t_start = std::chrono::high_resolution_clock::now();

    const auto n_per_process = n / no_of_processes;

    // Shared memory allocation
    uintmax_t *shared_hits = static_cast<uintmax_t *>(
        mmap(0, sizeof(uintmax_t) * no_of_processes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0));

    pid_t pid;

    for(size_t i = 0; i < no_of_processes; ++i)
    {
        pid = fork(); //############################## creating a subprocess (child process with pid == 0)

        if (pid < 0)
            std::terminate();

        /**************************************************************
         * child process implementation
         *************************************************************/
        if (pid == 0)
        {
            // std::cout << "New child process... #" << getpid() << std::endl;
            uintmax_t hits = count_hits(n_per_process);
            // std::cout << "Hits#" << getpid() << hits << "\n";
            shared_hits[i] = hits;
            _exit(0);
        }
    }

    // waiting for all children processes
    while (wait(NULL) != -1)
    {
    }

    uintmax_t total_hits = 0;

    for(int i = 0; i < no_of_processes; ++i)
        total_hits += shared_hits[i];

    double pi = 4 * (static_cast<double>(total_hits) / n);

    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << "ms\n";

    // shared memory deallocation
    if (munmap(shared_hits, sizeof(uintmax_t) * no_of_processes) == -1)
    {
        std::terminate();
    }
}

int main()
{
    calc_pi_single_thread(N);

    std::cout << "\n-----------------------\n";

    calc_pi_multithreading(N);

    std::cout << "\n-----------------------\n";

    calc_pi_multithreading_with_mutex(N);
    
    std::cout << "\n-----------------------\n";

    calc_pi_multiprocessing(N);
}
