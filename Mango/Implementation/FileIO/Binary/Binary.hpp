#pragma once

namespace Mango::Implementation::FileIO::Binary
{
	template<typename T>
	inline void serializePOD(std::fstream& out, const_ptr<T> value, size_t itemsNumber = 1)
	{
		out.write(reinterpret_cast<const char*>(value), sizeof(T) * itemsNumber);
	}

	template<typename T>
	inline void deserializePOD(std::fstream& in, ptr<T> value, size_t itemsNumber = 1)
	{
		in.read(reinterpret_cast<char*>(value), sizeof(T) * itemsNumber);
	}
}