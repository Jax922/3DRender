#pragma once
#include "BufferObject.h"
#include "../Data.h"
class VertexBufferObject : public BufferObjectBase<BufferObject::ArrayBuufer>
{
public:
	VertexBufferObject();
	using BufferObject::AllocateData;

	void AllocateData(size_t size);
	void AllocateData(std::span<const std::byte> data);
	template<typename T>
	void AllocateData(std::span<const T> data, Mode mode = Mode::StaticDraw);
	template<typename T>
	inline void AllocateData(std::span<T> data, Mode mode = Mode::StaticDraw) { AllocateData(std::span<const T>(data), mode); }

	using BufferObject::UpdateData;
	template<typename T>
	void UpdateData(std::span<const T> data, size_t offsetBytes = 0);
	template<typename T>
	inline void UpdateData(std::span<T> data, size_t offsetBytes = 0) { UpdateData(std::span<const T>(data), offsetBytes); }

};

template<typename T>
void VertexBufferObject::AllocateData(std::span<const T> data, Mode mode)
{
	AllocateData(Data::GetBytes(data), mode);
}

template<typename T>
void VertexBufferObject::UpdateData(std::span<const T> data, size_t offsetBytes)
{
	UpdateData(Data::GetBytes(data), offsetBytes);
}
