#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

template <typename K, typename T> class HashStack
{
  private:
    std::unordered_map<K, std::vector<T>> m_map;

  public:
    using ReturnType = std::conditional_t<std::is_pointer_v<T>, T, T *>;

    void push(const K &key, const T &value)
    {
        m_map[key].push_back(value);
    }

    std::optional<ReturnType> top(const K &key)
    {
        if (!contains(key))
        {
            return {};
        }

        T &result = m_map[key].back();

        if constexpr (std::is_pointer_v<T>)
        {
            return result;
        }
        else
        {
            return &result;
        }
    }

    std::optional<ReturnType> pop(const K &key)
    {
        std::optional<ReturnType> result = top(key);

        m_map[key].pop_back();

        return result;
    }

    bool contains(const K &key)
    {
        return m_map.contains(key) && m_map[key].size() > 0;
    }
};
