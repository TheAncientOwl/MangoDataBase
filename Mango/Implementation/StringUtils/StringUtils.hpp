#pragma once

#define MANGO_ESCAPE_CHAR '/'

namespace Mango::Implementation::StringUtils
{
	namespace Impl
	{
		template<typename T, size_t Size>
		struct Set
		{
			std::array<T, Size> data;

			[[nodiscard]] constexpr bool contians(const T& value) const
			{
				const auto it = std::find_if(std::begin(data), std::end(data),
											 [&value](const auto& v) { return v == value; });

				return it != std::end(data);
			}
		};

		static constexpr std::array<char, 31> invalidIdentifierChars
		{
			'`', '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '+', '=', '{', '}', '[', ']',
			'|', '\\', ';', ':', '\'', '"', ',', '.', '<', '>', '/', '?'
		};
	}

	std::vector<std::string_view> splitAtChar(std::string_view str, char c);
	std::vector<std::string_view> splitAtCharWithEscape(std::string_view str, char c);
	void splitInCleanStringsAt(std::string_view str, char c, ref<std::vector<std::string>> out);
	std::string_view trimWhiteSpaces(std::string_view str);
	std::string cleanString(std::string_view str, char escapedChar);
	void removeEscapeChar(ref<std::string> str, char escapedChar);
	bool isValidIdentifier(std::string_view str);
}