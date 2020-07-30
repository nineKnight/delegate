#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <typeindex>

namespace nineknight {

template <typename>
class delegate;

template <typename R, typename... Args>
class delegate<R(Args...)>
{
    std::mutex lock;
    std::map<std::type_index, std::function<R(Args...)>> functions;

    delegate(const delegate&);
    delegate& operator=(const delegate&);

  public:
    delegate() = default;
    void operator()(Args... args)
    {
        std::lock_guard<std::mutex> guard(lock);
        for (auto& function : functions) {
            function.second(args...);
        }
    }
    bool Add(std::function<R(Args...)> cb)
    {
        std::lock_guard<std::mutex> guard(lock);
        return functions.emplace(std::type_index(cb.target_type()), cb).second;
    }
    void Remove(std::function<R(Args...)> cb)
    {
        std::lock_guard<std::mutex> guard(lock);
        functions.erase(std::type_index(cb.target_type()));
        return;
    }
    bool operator+=(std::function<R(Args...)> cb) { return Add(cb); }
    void operator-=(std::function<R(Args...)> cb) { return Remove(cb); }
};

}