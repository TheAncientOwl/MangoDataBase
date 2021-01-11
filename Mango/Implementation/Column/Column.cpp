#include "standard_library.hpp"
#include "Column.hpp"

#include "../FileIO/Binary/Binary.hpp"
using namespace Mango::Implementation::FileIO::Binary;

#include "../../ConsoleColorOutput/ConsoleColorOutput.hpp"

namespace Mango::Implementation
{
#pragma region MANGO_API
	MANGO_API void Column::serializeToConfig(std::fstream& config) const
	{
		size_t nameLength = m_Name.length() + 1;
		serializePOD(config, &nameLength);
		serializePOD(config, m_Name.c_str(), nameLength);

		char c = m_DataType.toChar();
		serializePOD(config, &c);

		serializePOD(config, &m_Size);
	}

	MANGO_API void Column::deserializeFromConfig(std::fstream& config)
	{
		size_t nameLength = 0;
		deserializePOD(config, &nameLength);
		char buffer[MANGO_MAX_COLUMN_NAME_LENGTH + 1]{};
		deserializePOD(config, buffer, nameLength);
		m_Name = buffer;

		char c = ' ';
		deserializePOD(config, &c);
		m_DataType = c;

		deserializePOD(config, &m_Size);
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API std::string_view Column::getName() const
	{
		return m_Name;
	}

	MANGO_PUBLIC_API DataType Column::getDataType() const
	{
		return m_DataType;
	}

	MANGO_PUBLIC_API size_t Column::getSize() const
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
		out << ccolor::light_blue << std::string(col.getName());
		out << ccolor::dark_gray << "] {";
		out << ccolor::lime << col.getDataType().toString() << ccolor::dark_gray << ", " << ccolor::lime << col.getSize() << " BYTES";
		out << ccolor::dark_gray << "}" << ccolor::reset;
		return out;
	}
#pragma endregion

}