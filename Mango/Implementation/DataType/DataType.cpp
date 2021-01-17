#include "standard_library.hpp"
#include "DataType.hpp"

namespace Mango::Implementation
{
	DataType::DataType() : m_Value(Value::UNDEFINED) 
	{
	}

	DataType::DataType(Value value) : m_Value(value) 
	{
	}

	DataType& DataType::operator=(Value value)
	{
		m_Value = value;

		return *this;
	}

	DataType::DataType(std::string_view stringValue)
	{
		if (stringValue == MANGO_DATA_TYPE_INT_STRING)    { m_Value = Value::INT; return; }
		if (stringValue == MANGO_DATA_TYPE_FLOAT_STRING)  { m_Value = Value::FLOAT; return; }
		if (stringValue == MANGO_DATA_TYPE_STRING_STRING) { m_Value = Value::STRING; return; }
		m_Value = Value::UNDEFINED;
	}

	DataType& DataType::operator=(std::string_view stringValue)
	{
		if (stringValue == MANGO_DATA_TYPE_INT_STRING)    { m_Value = Value::INT; return *this; }
		if (stringValue == MANGO_DATA_TYPE_FLOAT_STRING)  { m_Value = Value::FLOAT; return *this; }
		if (stringValue == MANGO_DATA_TYPE_STRING_STRING) { m_Value = Value::STRING; return *this; }
		m_Value = Value::UNDEFINED; return *this;
	}

	DataType::DataType(char charValue)
	{
		switch (charValue)
		{
			case MANGO_DATA_TYPE_INT_CHAR:    m_Value = Value::INT;       break;
			case MANGO_DATA_TYPE_FLOAT_CHAR:  m_Value = Value::FLOAT;     break;
			case MANGO_DATA_TYPE_STRING_CHAR: m_Value = Value::STRING;    break;
			default:                          m_Value = Value::UNDEFINED;
		}
	}

	DataType& DataType::operator=(char charValue)
	{
		switch (charValue)
		{
			case MANGO_DATA_TYPE_INT_CHAR:    m_Value = Value::INT;       break;
			case MANGO_DATA_TYPE_FLOAT_CHAR:  m_Value = Value::FLOAT;     break;
			case MANGO_DATA_TYPE_STRING_CHAR: m_Value = Value::STRING;    break;
			default:                          m_Value = Value::UNDEFINED;
		}

		return *this;
	}

	std::string DataType::toString() const
	{
		switch (m_Value)
		{
			case Value::INT:    return MANGO_DATA_TYPE_INT_STRING;
			case Value::FLOAT:  return MANGO_DATA_TYPE_FLOAT_STRING;
			case Value::STRING: return MANGO_DATA_TYPE_STRING_STRING;
			default:            return MANGO_DATA_TYPE_UNDEFINED_STRING;
		}
	}

	char DataType::toChar() const
	{
		switch (m_Value)
		{
			case Value::INT:    return MANGO_DATA_TYPE_INT_CHAR;
			case Value::FLOAT:  return MANGO_DATA_TYPE_FLOAT_CHAR;
			case Value::STRING: return MANGO_DATA_TYPE_STRING_CHAR;
			default:            return MANGO_DATA_TYPE_UNDEFINED_CHAR;
		}
	}

	DataType::operator Value() const
	{
		return m_Value;
	}

}