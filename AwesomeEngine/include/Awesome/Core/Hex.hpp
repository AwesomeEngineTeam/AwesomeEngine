#pragma once

#ifndef AWESOME_CORE_HEX_GUARD
#define AWESOME_CORE_HEX_GUARD

#include <Awesome/Core/Config.hpp>
#include <unordered_map>

namespace aw
{
	struct HexConverter
	{
	private:
		static std::unordered_map<int, char> decToHex;
		static std::unordered_map<char, int> hexToDec;

	public:
		AWESOME_CORE_API static std::vector<unsigned char> ToByte(const void* src, size_t size);
		AWESOME_CORE_API static std::string ToHexString(std::vector<unsigned char> bytes);
		AWESOME_CORE_API static void FromHexString(std::string hexString, void* dst);
	};
}

#endif // !GUARD