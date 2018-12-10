namespace aw
{
	namespace sdk
	{
		inline Text2D::Text2D(Actor* actor, uint32 typeId, const std::string& name) :
			Component(actor, typeId, name)
		{
			//SetContent(" ");
		}

		inline Text2D::Text2D(Actor* actor) :
			Text2D(actor, uint32(ComponentID::Text2D), "Unnamed Text2D")
		{
		}

		inline std::string Text2D::GetContent() const
		{
			m_mutex.lock();
			std::string str = m_content;
			m_mutex.unlock();

			return str;
		}

		inline void Text2D::SetContent(std::string content)
		{
			m_mutex.lock();
			m_content = std::move(content);
			m_mutex.unlock();

			m_updated = false;
		}
	}
}