namespace aw
{
	namespace sdk
	{
		inline LuaScript::LuaScript(const std::string& filename)
		{
			InitInfo(nullptr, filename);
		}

		inline LuaScript::LuaScript(Component* owner, const std::string& filename)
		{
			InitInfo(owner, filename);
		}

		inline void LuaScript::InitInfo(Component* owner, const std::string& path)
		{
			m_owner = owner;

			if (path.empty())
				return;

			LOG_LOG("Loading script %s", path.c_str())

			#if defined(WITH_EDITOR)
				struct stat info;
				stat(path.c_str(), &info);

				m_lastModifiedTime = info.st_mtime;
			#endif

			m_filePath = path;
		}

		template<typename T>
		inline void LuaScript::AddOrSetBinding(const std::string& key, T&& value)
		{
			auto it = m_bindings.find(key);
			if(it != m_bindings.end())
				m_bindings.erase(it);

			auto pair = m_bindings.emplace(key, std::move(value));

			#if defined(WITH_EDITOR)
				using value_type = luwra::internal::PushableT<std::remove_reference_t<T>>;
				std::shared_ptr<value_type> derived_ptr = std::dynamic_pointer_cast<value_type>(pair.first->second.interface);

				m_bindingsVisits[key] = [this, key, derived_ptr](ed::ScriptWidget* widget)
				{
					ed::RefLayout* layout = nullptr;

					if constexpr(std::is_same_v<decltype(derived_ptr->value), bool>)
					{
						ed::BoolRefLayout* refL = new ed::BoolRefLayout(key.c_str(), derived_ptr->value);
						refL->boolLayout->onValueChange = [this, derived_ptr](bool b)
						{
							derived_ptr->value = b;

							Run();
							Start();
						};

						layout = refL;
					}
					else if constexpr(std::is_same_v<decltype(derived_ptr->value), int>)
					{
						ed::IntRefLayout* refL = new ed::IntRefLayout(key.c_str(), derived_ptr->value);
						refL->intLayout->onValueChange = [this, derived_ptr](int i)
						{
							derived_ptr->value = i;

							Run();
							Start();
						};

						layout = refL;
					}
					else if constexpr(std::is_same_v<decltype(derived_ptr->value), float>)
					{
						ed::FloatRefLayout* refL = new ed::FloatRefLayout(key.c_str(), derived_ptr->value);
						refL->floatLayout->onValueChange = [this, derived_ptr](double f)
						{
							derived_ptr->value = static_cast<float>(f);

							Run();
							Start();
						};

						layout = refL;
					}
					else if constexpr(std::is_same_v<decltype(derived_ptr->value), std::string>)
					{
						ed::TextRefLayout* refL = new ed::TextRefLayout(key.c_str(), derived_ptr->value);
						refL->textLayout->onValueChange = [this, derived_ptr](const std::string& str)
						{
							derived_ptr->value = str;

							Run();
							Start();
						};

						layout = refL;
					}
					else if constexpr(std::is_same_v<decltype(derived_ptr->value), Component*>)
					{
						ed::ComponentRefLayout* refL = new ed::ComponentRefLayout(key.c_str(), derived_ptr->value);
						refL->onComponentChange = [this, derived_ptr](Component* comp)
						{
							if (!comp)
								return;

							derived_ptr->value = comp;
						};

						layout = refL;
					}

					if (layout)
					{
						layout->onFieldNameChange = [this, key, derived_ptr](const std::string& newName)
						{
							auto value = derived_ptr->value;
							RemoveBinding(key);

							AddOrSetBinding(newName, value);
						};

						widget->AddBindingLayout(layout);
					}
				};
			#endif
		}

		inline LuaScript::operator const luwra::StateWrapper&() const
		{
			return *m_state;
		}

		inline LuaScript::operator luwra::StateWrapper&()
		{
			return *m_state;
		}
	}
}