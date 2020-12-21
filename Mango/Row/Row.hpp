#pragma once
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
		std::byte* PRIVATE_API data();

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
	};
}