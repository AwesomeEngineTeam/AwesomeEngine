namespace aw
{
	template<typename T>
	constexpr T Sum(T v)
	{
		return v;
	}

	template<typename T, typename... Args>
	constexpr T Sum(T v, Args... args)
	{
		return Sum(v) + Sum(args...);
	}

	template<typename Out, typename In>
	constexpr Out Assemble(const In& lhs, const In& rhs)
	{
		return (static_cast<Out>(lhs) << (sizeof(In) * 8)) | static_cast<Out>(rhs);
	}

	template<typename Out, typename In>
	constexpr Out DisassembleLHS(const In& assembled)
	{
		return assembled >> (sizeof(Out) * 8);
	}

	template<typename Out, typename In>
	constexpr Out DisassembleRHS(const In& assembled)
	{
		return Out(assembled);
	}

	template<typename Out, typename In>
	constexpr void Disassemble(const In& assembled, Out* lhs, Out* rhs)
	{
		*lhs = DisassembleLHS<Out, In>(assembled);
		*rhs = DisassembleRHS<Out, In>(assembled);
	}

	template <typename T, T... args>
	constexpr bool PackContains(const T& i)
	{
		bool res = false;
		using expand = int[];
		(void)expand
		{
			0,
				(res = !(args - i) ? true : res, 0)...
		};

		return res;
	}

	template <typename T, T... args>
	constexpr T SumParamsUntilEquals(const T& i)
	{
		int32 res = 0;
		bool found = false;
		using expand = int[];
		(void)expand
		{
			0,
				(res += (found ? 0 : (!(int32(args) - int32(i)) ? ((found = true) && 0) : int32(args))), 0)...
		};

		return T(res);
	}
}