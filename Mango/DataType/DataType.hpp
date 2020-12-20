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

		DataType(const_ref<DataType>) = default;
		DataType(rvalue_ref<DataType>) noexcept = default;
		DataType& operator=(const_ref<DataType>) = default;
		DataType& operator=(rvalue_ref<DataType>) noexcept = default;
		~DataType() = default;

	private:
		Value m_Value;

	};
}