#pragma once

#define MANGO_DATA_TYPE_UNDEFINED_STRING "UNDEFINED"
#define MANGO_DATA_TYPE_STRING_STRING    "STRING"
#define MANGO_DATA_TYPE_FLOAT_STRING     "FLOAT"
#define MANGO_DATA_TYPE_INT_STRING       "INT"

#define MANGO_DATA_TYPE_UNDEFINED_CHAR 'u'
#define MANGO_DATA_TYPE_STRING_CHAR    's'
#define MANGO_DATA_TYPE_FLOAT_CHAR     'f'
#define MANGO_DATA_TYPE_INT_CHAR       'i'

namespace Mango
{
	class DataType
	{
	public:
		enum class Value : uint8_t { UNDEFINED, INT, FLOAT, STRING };

		DataType();
		DataType(Value value);
		DataType(std::string_view stringValue);
		DataType(char charValue);

		std::string toString() const;
		char toChar() const;
		
		operator Value() const;

		DataType(ConstRef<DataType>) = default;
		DataType(RRef<DataType>) noexcept = default;
		DataType& operator=(ConstRef<DataType>) = default;
		DataType& operator=(RRef<DataType>) noexcept = default;

	private:
		Value m_Value;

	};
}