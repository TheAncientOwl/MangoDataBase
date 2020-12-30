#include "standard_library.hpp"
#include "Column.hpp"

#include "../BinaryUtils/BinaryUtils.hpp"
using namespace Mango::Implementation::BinaryUtils;

namespace Mango::Implementation
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void Column::serialize(std::fstream& file) const
	{
		size_t nameLength = m_Name.length() + 1;
		serializePOD(file, &nameLength);
		serializePOD(file, m_Name.c_str(), nameLength);

		char c = m_DataType.toChar();
		serializePOD(file, &c);

		serializePOD(file, &m_Size);
	}

	MANGO_PRIVATE_API void Column::deserialize(std::fstream& file)
	{
		size_t nameLength = 0;
		deserializePOD(file, &nameLength);
		char buffer[MANGO_MAX_COLUMN_NAME_LENGTH + 1]{};
		deserializePOD(file, buffer, nameLength);
		m_Name = buffer;

		char c = ' ';
		deserializePOD(file, &c);
		m_DataType = c;

		deserializePOD(file, &m_Size);
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API std::string_view Column::name() const
	{
		return m_Name;
	}

	MANGO_PUBLIC_API DataType Column::dataType() const
	{
		return m_DataType;
	}

	MANGO_PUBLIC_API size_t Column::size() const
	{
		return m_Size;
	}

	MANGO_PUBLIC_API Column::Column(std::string_view name, DataType dataType, size_t size)
		: m_Name(name), m_DataType(dataType), m_Size(size)
	{
	}

	MANGO_PUBLIC_API std::ostream& operator<<(std::ostream& out, const Column& col)
	{
		out << ccolor::dark_gray << "[";
		out << ccolor::light_blue << std::string(col.name());
		out << ccolor::dark_gray << "] {";
		out << ccolor::lime << col.dataType().toString() << ccolor::dark_gray << ", " << ccolor::lime << col.size() << " BYTES";
		out << ccolor::dark_gray << "}" << ccolor::reset;
		return out;
	}
#pragma endregion

}