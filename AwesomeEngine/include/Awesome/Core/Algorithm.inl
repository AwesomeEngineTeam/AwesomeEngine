namespace aw
{
	inline void ToLower(std::string& str)
	{
		for (char& c : str)
		{
			if (c >= 'A' && c <= 'Z')
				c += 32;
		}
	}

	inline void ToUpper(std::string& str)
	{
		for (char& c : str)
		{
			if (c >= 'a' && c <= 'z')
				c -= 32;
		}
	}
}