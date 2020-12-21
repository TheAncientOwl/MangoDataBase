#include "standard_library.hpp"
#include "Column.hpp"

namespace Mango
{
	Column::Column(std::string_view name, DataType dataType, size_t size)
		: m_Name(name), m_DataType(dataType), m_Size(size)
	{
	}

	void PRIVATE_API Column::serialize(std::fstream& file) const
	{
		size_t nameLength = m_Name.length() + 1;
		serializePOD(file, &nameLength);
		serializePOD(file, m_Name.c_str(), nameLength);

		char c = m_DataType.toChar();
		serializePOD(file, &c);

		serializePOD(file, &m_Size);
	}

	void PRIVATE_API Column::deserialize(std::fstream& file)
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

	std::string_view Column::name() const
	{
		return m_Name;
	}

	DataType Column::dataType() const
	{
		return m_DataType;
	}

	size_t Column::size() const
	{
		return m_Size;
	}

	std::ostream& operator<<(std::ostream& out, const Column& col)
	{
		out << ccolor::dark_gray << "[";
		out << ccolor::light_blue << std::string(col.name());
		out << ccolor::dark_gray << "] {";
		out << ccolor::lime << col.dataType().toString() << ccolor::dark_gray << ", " << ccolor::lime << col.size() << " BYTES";
		out << ccolor::dark_gray << "}" << ccolor::reset;
		return out;
	}
}