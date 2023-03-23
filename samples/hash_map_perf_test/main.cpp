/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/jerk-thread
 */

#include <unordered_set>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <string>
#include <numeric>
#include <functional>

#include "jerk-thread/containers/hashmap/hash_set.h"
#include "jerk-thread/synchronization/spinlock.h"

template<typename TContainer, bool bLock = false>
auto run_test(std::size_t writers_count, std::size_t readers_count, std::size_t iterations) {
    std::vector<std::thread> ts;
    jt::rw_spinlock guard;
    TContainer set;

    auto&& emplace = [&](auto&& to){
        set.emplace(to);
    };

    auto&& find = [&](auto&& to) {
        const auto volatile res = set.find(to);
    };

    for (std::size_t i{ 0 }; i < iterations; ++i) {
        const auto to_emp = std::to_string(i);
        set.emplace(to_emp);
    }

    auto&& perform = [&] (auto&& f, auto&& measure, auto lockt) {
        for (std::size_t t{ 0 }; t < measure.size(); ++t) {
            ts.emplace_back([&, t] {
                for (std::size_t i{ 0 }; i < iterations; ++i) {
                    const auto to_emp = std::to_string(i) + "_" + std::to_string(t);
                    auto&& start = std::chrono::high_resolution_clock::now();

                    if constexpr (bLock) {
                        decltype(lockt) lock(guard);
                        f(to_emp);
                    }
                    else
                    {
                        f(to_emp);
                    }

                    auto&& elapsed = std::chrono::high_resolution_clock::now() - start;
                    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                    measure[t] += duration;

                    //if (measure.size() == 1)
                        //std::this_thread::yield();
                }
            });
        }
    };

    std::vector<long long> writers_measure;
    writers_measure.resize(writers_count);
    std::vector<long long> readers_measure;
    readers_measure.resize(readers_count);

    perform(emplace, writers_measure, std::unique_lock<jt::rw_spinlock>{});
    perform(find, readers_measure, std::shared_lock<jt::rw_spinlock>{});

    for (auto&& t : ts)
        t.join();

    return std::pair{ std::move(readers_measure), std::move(writers_measure) };
}

int main() {
    auto&& [r1, w1] = run_test<jt::hash_set<std::string>>(10, 80, 1000);
    auto&& [r2, w2] = run_test<std::unordered_set<std::string>, true>(10, 80, 1000);

    std::cout << "Read std: " << std::accumulate(std::begin(r2), std::end(r2), (long long)0, std::plus<long long>{})
        << " Read jt: " << std::accumulate(std::begin(r1), std::end(r1), (long long)0, std::plus<long long>{}) << std::endl;

    std::cout << "Write std: " << std::accumulate(std::begin(w2), std::end(w2), (long long)0, std::plus<long long>{})
        << " Write jt: " << std::accumulate(std::begin(w1), std::end(w1), (long long)0, std::plus<long long>{}) << std::endl;
} 