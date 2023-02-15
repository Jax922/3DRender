#pragma once
#include "../Object.h"
#include <span>

class BufferObject : public Object
{
public:
	enum Target : GLenum
	{
		ArrayBuufer = GL_ARRAY_BUFFER,
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
	};
	
	// render mode
	enum Mode : GLenum
	{
		StaticDraw = GL_STATIC_DRAW, StaticRead = GL_STATIC_READ, StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW, DynamicRead = GL_DYNAMIC_READ, DynamicCopy = GL_DYNAMIC_COPY,
		StreamDraw = GL_STREAM_DRAW, StreamRead = GL_STREAM_READ, StreamCopy = GL_STREAM_COPY
	};

public:
	BufferObject();
	virtual ~BufferObject();
	void AllocateData(size_t size, Mode mode);
	void AllocateData(std::span<const std::byte> data, Mode mode);
	void UpdateData(std::span<const std::byte> data, size_t offset = 0);
	virtual Target GetTarget() const = 0;

protected:
	void Bind(Target target) const;
	static void Unbind(Target target);
};

template<BufferObject::Target T>
class BufferObjectBase : public BufferObject
{
public:
	inline BufferObjectBase() {}

	// Return the templated enum value T
	inline Target GetTarget() const override { return T; }

	// When binding this object, we bind it to the corresponding Target
	void Bind() const override;

	// When unbinding this class, we unbind the corresponding Target
	static void Unbind();

#ifndef NDEBUG
	// Check if there is any BufferObject currently bound to this target
	inline static bool IsAnyBound() { return s_boundHandle != Object::NullHandle; }
#endif

protected:
#ifndef NDEBUG
	// Check if this BufferObject is currently bound to this target
	inline bool IsBound() const override { return s_boundHandle == GetHandle(); }

	// Handle of the buffer object that is currently bound to this target
	static Handle s_boundHandle;
#endif
};


#ifndef NDEBUG
template<BufferObject::Target T>
Object::Handle BufferObjectBase<T>::s_boundHandle = Object::NullHandle;
#endif

template<BufferObject::Target T>
void BufferObjectBase<T>::Bind() const
{
	BufferObject::Bind(T);
#ifndef NDEBUG
	s_boundHandle = GetHandle();
#endif
}

template<BufferObject::Target T>
void BufferObjectBase<T>::Unbind()
{
	BufferObject::Unbind(T);
#ifndef NDEBUG
	s_boundHandle = NullHandle;
#endif
}