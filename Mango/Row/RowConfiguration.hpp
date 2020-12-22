#pragma once
namespace Mango
{
	class Row;
	class Table;
}
#include "../DataType/DataType.hpp"

namespace Mango
{
	class RowConfiguration
	{
	private: /// Fields
		std::vector<size_t> m_Offsets;
		std::vector<size_t> m_Sizes;
		std::vector<DataType> m_DataTypes;

	private: /// API
		void PRIVATE_API pushBack(size_t size, DataType dataType);

		size_t PRIVATE_API offsetAt(int index) const;
		size_t PRIVATE_API sizeAt(int index) const;
		DataType PRIVATE_API dataTypeAt(int index) const;

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
		friend class Mango::Table;
	};
}