namespace aw
{
	inline Color3f::Color3f(float r, float g, float b) :
		red{ r },
		green{ g },
		blue{ b }
	{
	}

	inline Color3f::Color3f(const std::array<float, 3>& arr) :
		red{ arr[0] },
		green{ arr[1] },
		blue{ arr[2] }
	{
	}

	inline Color3f::Color3f(const float arr[3]) :
		red{ arr[0] },
		green{ arr[1] },
		blue{ arr[2] }
	{
	}

	inline Color4f::Color4f(float r, float g, float b, float a) :
		red{ r },
		green{ g },
		blue{ b },
		alpha{ a }
	{
	}

	inline Color4f::Color4f(const std::array<float, 4>& arr) :
		red{ arr[0] },
		green{ arr[1] },
		blue{ arr[2] },
		alpha{ arr[3] }
	{
	}

	inline Color4f::Color4f(const float arr[4]) :
		red{ arr[0] },
		green{ arr[1] },
		blue{ arr[2] },
		alpha{ arr[3] }
	{
	}

	inline Color4f::operator const std::array<float, 4>&() const
	{
		return reinterpret_cast<const std::array<float, 4>&>(*this);
	}
}