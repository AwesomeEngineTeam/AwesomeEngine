#include <Awesome/Core/Hex.hpp>

#include <iostream>

namespace aw
{
	std::unordered_map<int, char> HexConverter::decToHex
	{
		{ 0, '0' },
		{ 1, '1' },
		{ 2, '2' },
		{ 3, '3' },
		{ 4, '4' },
		{ 5, '5' },
		{ 6, '6' },
		{ 7, '7' },
		{ 8, '8' },
		{ 9, '9' },
		{ 10, 'A' },
		{ 11, 'B' },
		{ 12, 'C' },
		{ 13, 'D' },
		{ 14, 'E' },
		{ 15, 'F' }
	};

	std::unordered_map<char, int> HexConverter::hexToDec
	{
		{ '0', 0 },
		{ '1', 1 },
		{ '2', 2 },
		{ '3', 3 },
		{ '4', 4 },
		{ '5', 5 },
		{ '6', 6 },
		{ '7', 7 },
		{ '8', 8 },
		{ '9', 9 },
		{ 'A', 10 },
		{ 'B', 11 },
		{ 'C', 12 },
		{ 'D', 13 },
		{ 'E', 14 },
		{ 'F', 15 }
	};

	std::vector<unsigned char> HexConverter::ToByte(const void* src, size_t size)
	{
		std::vector<unsigned char> bytes;
		bytes.resize(size);
		std::memcpy(bytes.data(), src, size);

		return bytes;
	}

	std::string HexConverter::ToHexString(std::vector<unsigned char> bytes)
	{
		std::string hex = "";
		int s = static_cast<int>(bytes.size());

		for (int i = 0; i < s; ++i)
		{
			int dec = static_cast<int>(bytes[i]);
			int rst = dec;
			int div = 0;

			if (dec >= 16)
			{
				div = dec / 16;
				rst = dec - div * 16;
			}

			hex += decToHex[div];
			hex += decToHex[rst];
		}

		return hex;
	}

	void HexConverter::FromHexString(std::string hexString, void* dst)
	{
		if (dst == nullptr)
			return;

		int hexSize = static_cast<int>(hexString.size());
		int byteSize = hexSize / 2;
		std::vector<unsigned char> bytes;
		bytes.reserve(byteSize);

		for (int i = 0; i < hexSize; i += 2)
			bytes.push_back(hexToDec[hexString[i]] * 16 + hexToDec[hexString[i + 1]]);

		std::memcpy(dst, bytes.data(), byteSize);
	}
}