namespace aw
{
	namespace sdk
	{
		template<typename Loader, typename ...Args>
		inline auto ResourceManager::Load(const std::string& filename, Args&&... args)
		{
			//if constexpr (
			//	std::is_same_v<
			//		std::invoke_result<
			//			decltype(Loader::Load),
			//			decltype(filename),
			//			Args&&...>, void>)
			//	Loader::Load(filename, std::forward<Args>(args)...);
			////std::enable_if<std::invoke_result_t<Loader::Load, filename, std::forward<Args>(args)...>>
			//else
			return Register(filename, Loader::Load(filename, std::forward<Args>(args)...));
			//return Register(filename, std::enable_if_t<std::is_same_v<
			//	std::invoke_result<
			//	decltype(Loader::Load),
			//	decltype(filename),
			//	Args&&...>, void>, Loader::Load(filename, std::forward<Args>(args)...)>);
		};

		//template<typename ...Args>
		//inline void ResourceManager::Load<GLTFLoader, Args...>(const std::string& filename, Args&&... args)
		//{
		//	Loader::Load(filename, std::forward<Args>(args)...);
		//}

		//template<typename Loader, typename ...Args>
		//std::enable_if_t<!std::is_same_v<void, std::invoke_result_t<decltype(Loader::Load), const std::string&, Args&&...>>, std::invoke_result_t<decltype(Loader::Load), const std::string&, Args&&...>> ResourceManager::Load(const std::string& filename, Args&&... args)
		//{
		//	return Register(filename, Loader::Load(filename, std::forward<Args>(args)...));
		//}

		//template<typename Loader, typename ...Args>
		//std::enable_if_t<std::is_same_v<void, std::invoke_result_t<decltype(Loader::Load), const std::string&, Args&&...>>, void> ResourceManager::Load(const std::string& filename, Args&&... args)
		//{
		//	Loader::Load(filename, std::forward<Args>(args)...);
		//}
	}
}