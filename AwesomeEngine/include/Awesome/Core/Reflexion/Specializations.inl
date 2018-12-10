namespace aw
{
	namespace reflect
	{
		/// ___________________________________std::vector<T>_________________________________________

		struct TypeDescriptor_StdVector : TypeDescriptor
		{
			TypeDescriptor* itemType;
			size_t (*getSize)(const void*);
			const void* (*getItem)(const void*, size_t);

			template <typename ItemType>
			TypeDescriptor_StdVector(ItemType*) :
				TypeDescriptor("std::vector<>", sizeof(std::vector<ItemType>)),
				itemType{ TypeResolver<ItemType>::Get() }
			{
				getSize = [](const void* vecPtr) -> size_t
				{
					const auto& vec = *static_cast<const std::vector<ItemType>*>(vecPtr);
					return vec.size();
				};

				getItem = [](const void* vecPtr, size_t index) -> const void*
				{
					const auto& vec = *static_cast<const std::vector<ItemType>*>(vecPtr);
					return &vec[index];
				};
			}

			std::string GetFullName() const override
			{
				return std::string("std::vector<") + itemType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				int numItems = static_cast<int>(getSize(obj));
				int index = 0;

				os << '[';

				for (; index < numItems - 1; ++index)
				{
					itemType->Dump(getItem(obj, index), os);
					os << '-';
				}

				if (index < numItems)
					itemType->Dump(getItem(obj, index), os);

				os << ']';
			}

			void Undump(void* obj, std::istream& is = std::cin) const override {}
		};

		template <typename T>
		class TypeResolver<std::vector<T>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
				return &typeDesc;
			}
		};

		/// __________________________________________________________________________________________
		/// ________________________________std::unique_ptr<T>________________________________________

		struct TypeDescriptor_StdUniquePtr : TypeDescriptor
		{
			TypeDescriptor* targetType;
			const void* (*getTarget)(const void*);

			template <typename TargetType>
			TypeDescriptor_StdUniquePtr(TargetType*) :
				TypeDescriptor{ "std::unique_ptr<>", sizeof(std::unique_ptr<TargetType>) },
				targetType{ TypeResolver<TargetType>::Get() }
			{
				getTarget = [](const void* uniquePtrPtr) -> const void*
				{
					const auto& uniquePtr = *static_cast<const std::unique_ptr<TargetType>*>(uniquePtrPtr);
					return uniquePtr.get();
				};
			}

			std::string GetFullName() const override
			{
				return std::string("std::unique_ptr<") + targetType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				const void* targetObj = getTarget(obj);

				if (targetObj == nullptr)
					os << 'N';
				else
					targetType->Dump(targetObj, os);
			}

			void Undump(void* obj, std::istream& is = std::cin) const override {}
		};

		template <typename T>
		class TypeResolver<std::unique_ptr<T>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdUniquePtr typeDesc{ (T*) nullptr };
				return &typeDesc;
			}
		};

		/// __________________________________________________________________________________________
		/// ______________________________std::shared_ptr<T>__________________________________________

		struct TypeDescriptor_StdSharedPtr : TypeDescriptor
		{
			TypeDescriptor* targetType;
			const void* (*getTarget)(const void*);

			template <typename TargetType>
			TypeDescriptor_StdSharedPtr(TargetType*) :
				TypeDescriptor{ "std::shared_ptr<>", sizeof(std::shared_ptr<TargetType>) },
				targetType{ TypeResolver<TargetType>::Get() }
			{
				getTarget = [](const void* sharedPtrPtr) -> const void*
				{
					const auto& sharedPtr = *static_cast<const std::shared_ptr<TargetType>*>(sharedPtrPtr);
					return sharedPtr.get();
				};
			}

			std::string GetFullName() const override
			{
				return std::string("std::shared_ptr<") + targetType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				const void* targetObj = getTarget(obj);

				if (targetObj == nullptr)
					os << 'N';
				else
					targetType->Dump(targetObj, os);
			}

			void Undump(void* obj, std::istream& is = std::cin) const override {}
		};

		template <typename T>
		class TypeResolver<std::shared_ptr<T>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdSharedPtr typeDesc{ (T*) nullptr };
				return &typeDesc;
			}
		};

		/// __________________________________________________________________________________________
		/// _________________________________std::list<T>_____________________________________________

		struct TypeDescriptor_StdList : TypeDescriptor
		{
			TypeDescriptor* itemType;
			size_t(*hops)(const void*);
			const void* (*getItem)(const void*, size_t);

			template <typename ItemType>
			TypeDescriptor_StdList(ItemType*) :
				TypeDescriptor("std::list<>", sizeof(std::list<ItemType>)),
				itemType{ TypeResolver<ItemType>::Get() }
			{
				hops = [](const void* listPtr) -> size_t
				{
					if (listPtr == nullptr)
						return 0;

					const auto& list = *(const std::list<ItemType>*) listPtr;
					return std::distance(list.begin(), list.end());
				};

				getItem = [](const void* listPtr, size_t index) -> const void*
				{
					const auto& list = *(const std::list<ItemType>*) listPtr;
					typename std::list<ItemType>::const_iterator it = list.begin();

					for (size_t i = 0; i < index; ++i)
						it++;

					return it.operator->();
				};

				//clear = [](const void* listPtr)
				//{
				//	const auto& list = *(const std::list<ItemType>*) listPtr;
				//	list.clear();
				//};

				//resize = [](const void* listPtr, size_t size)
				//{
				//	const auto& list = *(const std::list<ItemType>*) listPtr;
				//	list.resize(size);
				//}
			}

			std::string GetFullName() const override
			{
				return std::string("std::list<") + itemType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				if (obj == nullptr)
					return;

				size_t h = hops(obj);
				size_t index = 0u;

				TypeResolver<size_t>::Get()->Dump(&h, os);

				for (; index < h; ++index)
				{
					os << '-';
					itemType->Dump(getItem(obj, index), os);
				}
			}

			void Undump(void* obj, std::istream& is = std::cin) const override 
			{
				if (obj == nullptr)
					return;

				std::string s;

				is >> s;
				std::stringstream ss = CutString(s, 0, sizeof(size_t) * 2);

				size_t size;
				TypeResolver<size_t>::Get()->Undump(&size, ss);

				for (size_t i = 0; i < size; ++i)
				{

				}
			}
		};

		template <typename T>
		class TypeResolver<std::list<T>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdList typeDesc{ (T*) nullptr };
				return &typeDesc;
			}
		};

		/// __________________________________________________________________________________________
		/// _______________________________std::map<T1, T2>___________________________________________

		struct TypeDescriptor_StdMap : TypeDescriptor
		{
			TypeDescriptor* keyType;
			TypeDescriptor* itemType;
			size_t (*hops)(const void*);
			std::pair<const void*, const void*> (*getPair)(const void*, size_t);

			template <typename KeyType, typename ItemType>
			TypeDescriptor_StdMap(KeyType*, ItemType*) :
				TypeDescriptor("std::map<>", sizeof(std::map<KeyType, ItemType>)),
				keyType{ TypeResolver<KeyType>::Get() },
				itemType{ TypeResolver<ItemType>::Get() }
			{
				hops = [](const void* mapPtr) -> size_t
				{
					if (mapPtr == nullptr)
						return 0;

					const auto& map = *static_cast<const std::map<KeyType, ItemType>*>(mapPtr);
					return std::distance(map.begin(), map.end());
				};

				getPair = [](const void* mapPtr, size_t index) -> std::pair<const void*, const void*>
				{
					const auto& map = *static_cast<const std::map<KeyType, ItemType>*>(mapPtr);
					typename std::map<KeyType, ItemType>::const_iterator it = map.begin();

					for (size_t i = 0; i < index; ++i)
						it++;

					return std::make_pair<const void*, const void*>(&(it->first), &(it->second));
				};
			}

			std::string GetFullName() const override
			{
				return std::string("std::map<") + keyType->GetFullName() + ", " + itemType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				int h = static_cast<int>(hops(obj));
				int index = 0;

				os << '{';

				for (; index < h - 1; ++index)
				{
					std::pair<const void*, const void*> pair = getPair(obj, index);
					keyType->Dump(pair.first, os);
					os << ',';
					itemType->Dump(pair.second, os);
					os << '-';
				}

				if (index < h)
				{
					std::pair<const void*, const void*> pair = getPair(obj, index);
					keyType->Dump(pair.first, os);
					os << ',';
					itemType->Dump(pair.second, os);
				}

				os << '}';
			}

			void Undump(void* obj, std::istream& is = std::cin) const override {}
		};

		template <typename KeyType, typename ItemType>
		class TypeResolver<std::map<KeyType, ItemType>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdMap typeDesc{ (KeyType*) nullptr, (ItemType*) nullptr };
				return &typeDesc;
			}
		};

		/// __________________________________________________________________________________________
		/// ________________________________std::atomic<T>____________________________________________

		struct TypeDescriptor_StdAtomic : TypeDescriptor
		{
			TypeDescriptor* valueType;
			const void* (*getValue)(const void*);

			template <typename ValueType>
			TypeDescriptor_StdAtomic(ValueType*) :
				TypeDescriptor("std::atomic<>", sizeof(ValueType)),
				valueType{ TypeResolver<ValueType>::Get() }
			{
				getValue = [](const void* atomicPtr) -> const void*
				{
					const std::atomic<ValueType>& atomic = *static_cast<const std::atomic<ValueType>*>(atomicPtr);
					return &atomic._My_val;
				};
			}

			std::string GetFullName() const override
			{
				return std::string("std::atomic<") + valueType->GetFullName() + ">";
			}

			void Dump(const void* obj, std::ostream& os = std::cout) const override
			{
				valueType->Dump(getValue(obj), os);
			}

			void Undump(void* obj, std::istream& is = std::cin) const override {}
		};

		template <typename ValueType>
		class TypeResolver<std::atomic<ValueType>>
		{
		public:
			static TypeDescriptor* Get()
			{
				static TypeDescriptor_StdAtomic typeDesc{ (ValueType*) nullptr };
				return &typeDesc;
			}
		};
	}
}