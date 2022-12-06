#ifndef HSSTSQUEUE_H
#define HSSTSQUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "hss_iqueue.hpp"

namespace hss
{

  template <typename T>
  class thread_safe_queue : public IQueue<T>
  {
  private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

  public:
    thread_safe_queue() : IQueue<T>()
    {
    }

    thread_safe_queue(thread_safe_queue const &other) : IQueue<T>()
    {
      std::lock_guard<std::mutex> lk(other.mut);
      data_queue = other.data_queue;
    }

    void push(T & new_value)
    {
      std::lock_guard<std::mutex> lk(mut);
      data_queue.push(new_value);
      data_cond.notify_one();
    }

    void push(T && new_value)
    {
      std::lock_guard<std::mutex> lk(mut);
      if(!std::is_move_constructible<T>::value)
          throw std::runtime_error("hss::thread_safe_queue::push(T && new_value) but T seems not move_constructible.");
      data_queue.push(std::move(new_value));
      data_cond.notify_one();
    }


    void push_moving(T && new_value)
    {
      std::lock_guard<std::mutex> lk(mut);
      data_queue.push(std::move(new_value));
      data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk, [this]
                     { return !data_queue.empty(); });
      value = data_queue.front();
      data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk, [this]
                     { return !data_queue.empty(); });
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
    }

    bool try_pop(T &value)
    {
      std::lock_guard<std::mutex> lk(mut);
      if (data_queue.empty())
        return false;
      value = std::forward<T>(data_queue.front());
      data_queue.pop();
      return true;
    }

    std::shared_ptr<T> try_pop_shared()
    {
      std::lock_guard<std::mutex> lk(mut);
      if (data_queue.empty())
        return std::shared_ptr<T>();
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
    }

    bool try_pop_ptr(T & value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
          return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    bool try_pop_moving(T && value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
          return false;
        value.reset(std::move(data_queue.front()));
        data_queue.pop();
        return true;
    }

    bool empty() const
    {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.empty();
    }
  };
} // end namespace hss
#endif // HSSTSQUEUE_H
