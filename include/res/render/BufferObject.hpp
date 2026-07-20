#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include <cstdint>

namespace mtrs::res
{

class BufferObject
{
    friend class SpriteBatch;

    uint32_t _id;
    uint32_t _mode;
public:
    BufferObject();
    ~BufferObject() noexcept;

    BufferObject(const BufferObject &) = delete;
    BufferObject &operator=(const BufferObject &) = delete;
    BufferObject(BufferObject &&other) noexcept;
    BufferObject &operator=(BufferObject &&other) noexcept;

    uint32_t id() const noexcept { return _id; }

    void init(const uint32_t &mode, const void *data, const int64_t size, const uint32_t usage);
    void update(const void *data, const int64_t size, const int64_t offset) const;

    void bind() const;
    void bind_base(const uint32_t index);
    void unbind() const noexcept;
};

}

#endif
