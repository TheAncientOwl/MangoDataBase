#pragma once
namespace Mango
{
	class Table;
	namespace Queries
	{
		class InsertIntoQuery;
	}
}
#include "RowConfiguration.hpp"

namespace Mango
{
	class Row
	{
	private: /// Fields
		std::byte* m_Data;
		std::shared_ptr<RowConfiguration> m_Config;

	private: /// API
		void PRIVATE_API setDataAt(int index, const void* value, size_t size);
		void PRIVATE_API setDataAt(int index, const_ref<std::string> value);
		const_ptr<std::byte> PRIVATE_API data() const;
		ptr<std::byte> PRIVATE_API data();
		size_t PRIVATE_API size() const;

	public: /// Interface
		int getInt(int index) const;
		float getFloat(int index) const;
		std::string_view getString(int index) const;

	public: /// Constructors
		Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config);

		Row() = default;
		Row(const Row&) = delete;
		Row(Row&& rhs) noexcept;
		Row& operator=(const Row&) = delete;
		Row& operator=(Row&& rhs) noexcept;
		~Row();

	private: /// Friends
		friend class Mango::Table;
		friend class Queries::InsertIntoQuery;
	};
}