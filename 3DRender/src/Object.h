#pragma once
#include <glad/glad.h>
// everything is Object in OPENGL
class Object
{
public:
	using Handle = GLuint; 

public:
	Object(Handle handle);
	virtual ~Object();

	// let object non-copyable, avoid the object is deteled twice
	Object(const Object&) = delete;
	void operator = (const Object&) = delete;

	inline Handle GetHandle() const { return m_handle; };

	inline bool IsValid() const { return m_handle != NullHandle; };

	virtual void Bind() const = 0;

protected:
	static const Handle NullHandle = 0;
	inline Handle& GetHandle() { return m_handle; };
#ifndef NDEBUG
	// Check if this Object is currently bound
	virtual bool IsBound() const { return false; }
#endif
private:
	Handle m_handle;
};

