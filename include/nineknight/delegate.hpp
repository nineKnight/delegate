#include <functional>
#include <list>

namespace nineknight {

template <typename>
class delegate;

template <typename R, typename... Args>
class delegate<R(Args...)>
{
    std::list<std::function<R(Args...)>> functions;

    delegate(const delegate&);
    delegate& operator=(const delegate&);

  public:
    delegate() = default;
    void operator()(Args... args)
    {
        for (auto& function : functions) {
            function(args...);
        }
    }
    bool Add(std::function<R(Args...)> cb)
    {
        // for (auto it = functions.begin(); it != functions.end(); it++) {
        //     if (it->target_type() == cb.target_type()) {
        //         return false;
        //     }
        // }
        functions.emplace_back(cb);
        return true;
    }
    void Remove(std::function<R(Args...)> cb)
    {
        for (auto it = functions.begin(); it != functions.end(); it++) {
            if (it->target_type() == cb.target_type()) {
                it = --functions.erase(it);
            }
        }
        return;
    }
    bool operator+=(std::function<R(Args...)> cb) { return Add(cb); }
    void operator-=(std::function<R(Args...)> cb) { return Remove(cb); }
};

}    // namespace nineknight
