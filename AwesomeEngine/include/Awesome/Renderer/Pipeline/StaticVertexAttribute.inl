namespace aw
{
	template<uint16 formatIndex, uint8 id>
	inline constexpr VertexAttribute StaticVertexAttribute<formatIndex, id>::ToDescription()
	{
		return VertexAttribute(FormatIndex, Size, Id);
	}
}