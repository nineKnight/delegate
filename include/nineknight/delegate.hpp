#include <functional>
#include <mutex>
#include <vector>

namespace nineknight {

namespace priv {

class dummy_lock
{
  public:
    void lock() {}
    void unlock() {}
};

}    // namespace priv

using st_policy = priv::dummy_lock;
using mt_policy = std::mutex;

using def_thread_policy = st_policy;

template <typename T, typename ThreadPolicy = def_thread_policy>
class delegate;

template <typename R, typename... Args, typename ThreadPolicy>
class delegate<R(Args...), ThreadPolicy>
{
    ThreadPolicy policy;
    std::vector<std::function<R(Args...)>> functions;

    delegate(const delegate &);
    delegate &operator=(const delegate &);

  public:
    delegate() = default;
    void operator()(Args... args)
    {
        std::unique_lock<ThreadPolicy> lock(policy);
        std::vector<std::function<R(Args...)>> executors(functions);
        lock.unlock();
        for (auto &executor : executors) {
            executor(std::forward<Args>(args)...);
        }
    }
    bool Add(std::function<R(Args...)> cb)
    {
        // for (auto it = functions.begin(); it != functions.end(); it++) {
        //     if (it->target_type() == cb.target_type()) {
        //         return false;
        //     }
        // }
        std::unique_lock<ThreadPolicy> lock(policy);
        functions.emplace_back(cb);
        return true;
    }
    void Remove(std::function<R(Args...)> cb)
    {
        std::unique_lock<ThreadPolicy> lock(policy);
        for (auto it = functions.begin(); it != functions.end();) {
            if (it->target_type() == cb.target_type()) {
                it = functions.erase(it);
            } else {
                ++it;
            }
        }
        return;
    }
    bool operator+=(std::function<R(Args...)> cb) { return Add(cb); }
    void operator-=(std::function<R(Args...)> cb) { return Remove(cb); }
};

}    // namespace nineknight