#pragma once
#include "BufferObject.h"
#include "../Data.h"

class ElementBufferObject : public BufferObjectBase<BufferObject::ElementArrayBuffer>
{
	ElementBufferObject();

    template<typename T>
    void AllocateData(size_t elementCount, Mode mode = Mode::StaticDraw);
    template<typename T>
    void AllocateData(std::span<const T> data, Mode mode = Mode::StaticDraw);
    template<typename T>
    inline void AllocateData(std::span<T> data, Mode mode = Mode::StaticDraw) { AllocateData(std::span<const T>(data), mode); }

    template<typename T>
    void UpdateData(std::span<const T> data, size_t offsetBytes = 0);


#ifndef NDEBUG
// Check if a data type is supported to be used as index
static bool IsSupportedType(Data::Type type);
#endif
};

// Call the base implementation with the buffer size, computed with elementCount and size of T
template<typename T>
void ElementBufferObject::AllocateData(size_t elementCount, Mode mode)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::AllocateData(elementCount * sizeof(T), mode);
}

// Call the base implementation with the span converted to bytes
template<typename T>
void ElementBufferObject::AllocateData(std::span<const T> data, Mode mode)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::AllocateData(Data::GetBytes(data), mode);
}

// Call the base implementation with the span converted to bytes
template<typename T>
void ElementBufferObject::UpdateData(std::span<const T> data, size_t offsetBytes)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::UpdateData(Data::GetBytes(data), offsetBytes);
}
