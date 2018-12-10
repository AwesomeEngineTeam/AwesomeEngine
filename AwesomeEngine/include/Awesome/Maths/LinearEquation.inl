namespace aw
{
	inline LinearEquation::LinearEquation(std::string str)
	{
		size_t t = str.find_first_of("+-*/0123654789") + 1;

		float rec = std::stof(str, &t + 1);

		std::cout << rec << std::endl;
	}
}