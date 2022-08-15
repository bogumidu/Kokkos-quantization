//
// Created by bogum on 06.06.2022.
//
//
//#include "task_execution_example.h"
//
//#include "Kokkos_Core.hpp"
//#include <cstdio>
//
//
//// Estimate size of the memory pool for given problem
//size_t
//estimate_required_memory(int n) {
//    assert(n >= 0);
//    auto n1 = static_cast<size_t>(n);
//    return (n1 + 1) * (n1 + 1) * 2000;
//}
//
//// Task and execution structure
//template <class Scheduler>
//struct GivenTask {
//
//    using future_type = Kokkos::BasicFuture<int, Scheduler>;
//
//    int n;
//    future_type fn_1;
//    future_type fn_2;
//
//    KOKKOS_INLINE_FUNCTION
//    explicit GivenTask(int num) noexcept : n(num) {}
//
//    template <class TeamMember>
//            KOKKOS_INLINE_FUNCTION
//            void operator()(TeamMember& member, int& result) {
//                auto& scheduler = member.scheduler();
//                // Operation instructions (this is where voxelization will happen)
//            }
//
//};
//
//int main(int argc, char* argv[]) {
//    Kokkos::initialize(argc, argv);
//    {
//        using scheduler_type = Kokkos::TaskScheduler<Kokkos::DefaultExecutionSpace>;
//        using memory_space = typename scheduler_type::memory_space;
//        using memory_pool = typename scheduler_type::memory_pool;
//
//        auto mpool = memory_pool(memory_space{}, estimate_required_memory(n));
//        auto scheduler = scheduler_type(mpool);
//
//        Kokkos::BasicFuture<int, scheduler_type> result;
//
//        Kokkos::Timer timer;
//        {
//            // Execute task
//            result = Kokkos::host_spawn(Kokkos::TaskSingle(scheduler),
//                                        GivenTask<scheduler_type>{n});
//
//            // Wait for all tasks to finish
//            Kokkos::wait(scheduler);
//        }
//        auto time = timer.seconds();
//
//        // Here will be result output method
//    }
//    Kokkos::finalize();
//
//    return 0;
//}