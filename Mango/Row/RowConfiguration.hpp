#pragma once
namespace Mango
{
	class Row;
}
#include "../DataType/DataType.hpp"

#define MANGO_ROW_CONFIG_DEBUG

namespace Mango
{
	class RowConfiguration
	{
	private: /// Fields
		std::vector<size_t> m_Offsets;
		std::vector<size_t> m_Sizes;
#ifdef MANGO_ROW_CONFIG_DEBUG
		std::vector<DataType> m_DataTypes;
#endif // ROW_CONFIG_DEBUG	

	private: /// API
#ifdef MANGO_ROW_CONFIG_DEBUG
		void PRIVATE_API pushBack(size_t size, DataType dataType);
#else
		void PRIVATE_API pushBack(size_t size);
#endif // MANGO_ROW_CONFIG_DEBUG

		size_t PRIVATE_API offsetAt(int index) const;
		size_t PRIVATE_API sizeAt(int index) const;

#ifdef MANGO_ROW_CONFIG_DEBUG
		DataType PRIVATE_API dataTypeAt(int index) const;
#endif // MANGO_ROW_CONFIG_DEBUG

	public: /// Interface
		size_t totalSize() const;

	public: /// Constructors
		RowConfiguration() = default;
		RowConfiguration(const RowConfiguration&) = delete;
		RowConfiguration(RowConfiguration&&) = default;
		RowConfiguration& operator=(const RowConfiguration&) = delete;
		~RowConfiguration() = default;

	private: /// Friends
		friend class Mango::Row;
	};
}