#pragma once

#define MANGO_ESCAPE_CHAR '/'

namespace Mango::Implementation::StringUtils
{
	std::vector<std::string_view> splitAtChar(std::string_view str, char c);
	std::vector<std::string_view> splitAtCharWithEscape(std::string_view str, char c);
	void splitInCleanStringsAt(std::string_view str, char c, ref<std::vector<std::string>> out);
	std::string_view trimWhiteSpaces(std::string_view str);
	std::string cleanString(std::string_view str, char escapedChar);
	void removeEscapeChar(ref<std::string> str, char escapedChar);
	bool isValidIdentifier(std::string_view str);
}