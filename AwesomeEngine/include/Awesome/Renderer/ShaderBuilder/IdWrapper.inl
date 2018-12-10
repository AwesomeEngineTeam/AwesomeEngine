namespace aw
{
	namespace sb
	{
		inline IdWrapper::IdWrapper(spv::Id id) :
			id{ id }
		{
		}

		inline void IdWrapper::AddDecoration(spv::Decoration decoration, int32 num)
		{
			ShaderBuilder::GetBuilder().addDecoration(id, decoration, num);
		}

		inline void IdWrapper::AddMemberDecoration(uint32 member, spv::Decoration decoration, int32 num)
		{
			ShaderBuilder::GetBuilder().addMemberDecoration(id, member, decoration, num);
		}

		inline IdWrapper::operator spv::Id()
		{
			return id;
		}

		inline IdWrapper::operator const spv::Id() const
		{
			return id;
		}
	}
}