#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

void background_work(size_t id, const std::string& text, std::chrono::milliseconds delay)
{
    std::cout << "bw#" << id << " has started..." << std::endl;

    for (const auto& c : text)
    {
        std::cout << "bw#" << id << ": " << c << std::endl;

        std::this_thread::sleep_for(delay);
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

void deamon()
{
    while(true)
    {
        std::cout << "Deamon works..." << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

class BackgroundWork
{
    const int id_;
    const std::string text_;

public:
    BackgroundWork(int id, std::string text)
        : id_{id}
        , text_{std::move(text)}
    {
    }

    void operator()(std::chrono::milliseconds delay) const
    {
        std::cout << "BW#" << id_ << " has started..." << std::endl;

        for (const auto& c : text_)
        {
            std::cout << "BW#" << id_ << ": " << c << std::endl;

            std::this_thread::sleep_for(delay);
        }

        std::cout << "BW#" << id_ << " is finished..." << std::endl;
    }
};

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    std::thread thd_empty;
    std::cout << "Empty thread id: " << thd_empty.get_id() << std::endl;

    std::thread thd_1{&background_work, 1, "Radmor", 250ms};
    std::thread thd_2{&background_work, 2, text, 500ms};
    std::thread thd_3{BackgroundWork{3, "Functor"}, 300ms};
    std::thread thd_4{[] { background_work(4, "Lambda", 500ms); }};
    std::thread thd_deamon{&deamon};

    if (thd_1.joinable())
        thd_1.join();
    thd_2.join();
    thd_3.join();
    thd_4.join();
    thd_deamon.detach();
    assert(thd_deamon.joinable() == false);

    std::this_thread::sleep_for(5s);

    std::cout << "Main thread ends..." << std::endl;
}
