#ifndef SPARSE_SET_HPP
#define SPARSE_SET_HPP

#include <vector>
#include <limits>
#include <cstdint>

typedef uint64_t EntityID;
constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

namespace mtrs::comp
{

template<typename TypeComponent>
class SparseSet
{
    std::vector<TypeComponent> _dense;
    std::vector<EntityID> _sparse;
    std::vector<EntityID> _entities;
    size_t _size = 0;

public:
    SparseSet() = default;

    void reserve(size_t capacity)
    {
        _dense.reserve(capacity);
        _entities.reserve(capacity);
    }

    TypeComponent &add(EntityID entity, TypeComponent component)
    {
        if (entity >= _sparse.size())
        {
            _sparse.resize(entity + 1, NULL_ENTITY);
        }
        
        if (_sparse[entity] != NULL_ENTITY)
        {
            _dense[_sparse[entity]] = std::move(component);
            return _dense[_sparse[entity]];
        }
        
        _sparse[entity] = _size;
        _entities.push_back(entity);
        _dense.push_back(std::move(component));
        _size++;
        return _dense[_sparse[entity]];
    }

    TypeComponent *get(EntityID entity)
    {
        if (entity >= _sparse.size()) return nullptr;
        EntityID dense_index = _sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return &_dense[dense_index];
    }
    
    const TypeComponent *get(EntityID entity) const
    {
        if (entity >= _sparse.size()) return nullptr;
        EntityID dense_index = _sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return &_dense[dense_index];
    }

    bool has(EntityID entity) const
    {
        return entity < _sparse.size() && _sparse[entity] != NULL_ENTITY;
    }

    void remove(EntityID entity)
    {
        if (!has(entity)) return;
        
        EntityID dense_index = _sparse[entity];
        EntityID last_entity = _entities.back();
        
        if (dense_index != _size - 1)
        {
            _dense[dense_index] = std::move(_dense.back());
            _entities[dense_index] = last_entity;
            
            _sparse[last_entity] = dense_index;
        }
        
        _dense.pop_back();
        _entities.pop_back();
        _sparse[entity] = NULL_ENTITY;
        _size--;
    }

    class Iterator
    {
        std::vector<EntityID>& _entities;
        std::vector<TypeComponent>& _dense;
        size_t _index;
        
    public:
        Iterator(std::vector<EntityID>& entities, std::vector<TypeComponent>& dense, size_t index) 
        : _entities(entities), _dense(dense), _index(index)
        {}
            
        std::pair<EntityID, TypeComponent&> operator*()
        {
            return {_entities[_index], _dense[_index]};
        }
        
        Iterator& operator++() { _index++; return *this; }
        bool operator!=(const Iterator& other) { return _index != other._index; }

        bool operator!=(const Iterator& other) const
        {
            return _index != other._index;
        }

        EntityID get_entity() const
        {
            return _entities[_index];
        }
    };

    Iterator begin() const
    {
        return Iterator(_entities, _dense, 0);
    }

    Iterator end() const
    {
        return Iterator(_entities, _dense, _size);
    }
    

    const std::vector<EntityID>& get_entities() const
    {
        return _entities;
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    size_t capacity() const
    {
        return _dense.capacity();
    }

    void clear()
    {
        _dense.clear();
        _entities.clear();
        for (auto& idx : _sparse)
        {
            idx = NULL_ENTITY;
        }
        _size = 0;
    }
};

}

#endif
