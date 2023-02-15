#include "BufferObject.h"

BufferObject::BufferObject() : Object(NullHandle)
{
	Handle& handle = GetHandle();
	glGenBuffers(1, &handle);
}

BufferObject::~BufferObject()
{
	Handle& handle = GetHandle();
	glDeleteBuffers(1, &handle);
}

void BufferObject::AllocateData(size_t size, Mode mode)
{
	Target target = GetTarget();
	glBufferData(target, size, nullptr, mode);
}

void BufferObject::AllocateData(std::span<const std::byte> data, Mode mode)
{
	Target target = GetTarget();
	glBufferData(target, data.size_bytes(), data.data(), mode);
}

void BufferObject::UpdateData(std::span<const std::byte> data, size_t offset)
{
	Target target = GetTarget();
	glBufferSubData(target, offset, data.size_bytes(), data.data());
}

void BufferObject::Bind(Target target) const
{
	Handle handle = GetHandle();
	glBindBuffer(target, handle);
}

void BufferObject::Unbind(Target target)
{
	Handle handle = NullHandle;
	glBindBuffer(target, handle);
}
