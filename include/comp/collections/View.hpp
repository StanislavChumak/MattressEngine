#ifndef VIEW_HPP
#define VIEW_HPP

#include "SparseSet.hpp"
#include <tuple>
#include <utility>
#include <unordered_set>

namespace mtrs::comp
{

template<typename... ComponentTypes>
class View
{
    const std::unordered_set<EntityID>& _disabled_ids;
    std::tuple<SparseSet<ComponentTypes>&...> _sets;

    template<size_t I = 0>
    static size_t min_size_index(
        const std::tuple<SparseSet<ComponentTypes>&...>& sets,
        size_t cur_min_index = 0,
        size_t cur_min_size = std::numeric_limits<size_t>::max())
    {
        if constexpr (I == sizeof...(ComponentTypes))
        {
            return cur_min_index;
        }
        else
        {
            size_t sz = std::get<I>(sets).size();
            if (sz < cur_min_size)
            {
                return min_size_index<I + 1>(sets, I, sz);
            }
            else
            {
                return min_size_index<I + 1>(sets, cur_min_index, cur_min_size);
            }
        }
    }

public:
    explicit View(const std::unordered_set<EntityID>& disabled_ids,SparseSet<ComponentTypes>&... sets)
    : _disabled_ids(disabled_ids), _sets(sets...)
    {}

    class Iterator
    {
        friend class View;

        const std::deque<EntityID>* _entities = nullptr;
        size_t _index = 0;
        size_t _size = 0;
        const std::unordered_set<EntityID>* _disabled_ids = nullptr;
        const std::tuple<SparseSet<ComponentTypes>&...>* _sets = nullptr;

        bool has_all_components(EntityID entity) const
        {
            if(_disabled_ids->find(entity) != _disabled_ids->end()) return false;
            return (std::get<SparseSet<ComponentTypes>&>(*_sets).has(entity) && ...);
        }

        void advance_to_valid()
        {
            while (_index < _size && !has_all_components((*_entities)[_index])) 
            {
                _index++;
            }
        }

        Iterator(const std::deque<EntityID>* entities, size_t size,
            const std::tuple<SparseSet<ComponentTypes>&...>* sets,
            const std::unordered_set<EntityID>* disabled_ids,
            size_t start_index)
        : _entities(entities), _size(size), _disabled_ids(disabled_ids)
        , _sets(sets), _index(start_index)
        {
            advance_to_valid();
        }

    public:
        bool operator!=(const Iterator& other) const
        {
            return _index != other._index;
        }

        Iterator& operator++()
        {
            ++_index;
            advance_to_valid();
            return *this;
        }

        auto operator*() const
        {
            EntityID entity = (*_entities)[_index];
            return std::tuple_cat(
                std::make_tuple(entity),
                std::make_tuple(std::get<SparseSet<ComponentTypes>&>(*_sets).get(entity)...));
        }
    };

private:
    template<size_t... Is>
    Iterator make_begin_impl(size_t min_index, std::index_sequence<Is...>) const
    {
        const std::deque<EntityID>* entities = nullptr;
        size_t set_size = 0;
        (void)((min_index == Is ?
        (
            entities = &std::get<Is>(_sets).get_entities(),
            set_size = std::get<Is>(_sets).size(),
            true
        ) : false) || ...);
        return Iterator(entities, set_size, &_sets, &_disabled_ids, 0);
    }

    template<size_t... Is>
    Iterator make_end_impl(size_t min_index, std::index_sequence<Is...>) const
    {
        const std::deque<EntityID>* entities = nullptr;
        size_t set_size = 0;
        (void)((min_index == Is ?
        (
            entities = &std::get<Is>(_sets).get_entities(),
            set_size = std::get<Is>(_sets).size(),
            true
        ) : false) || ...);
        return Iterator(entities, set_size, &_sets, &_disabled_ids, set_size);
    }

public:
    Iterator begin() const
    {
        size_t min_index = min_size_index(_sets);
        return make_begin_impl(min_index, std::index_sequence_for<ComponentTypes...>{});
    }

    Iterator end() const
    {
        size_t min_index = min_size_index(_sets);
        return make_end_impl(min_index, std::index_sequence_for<ComponentTypes...>{});
    }
};

}

#endif
