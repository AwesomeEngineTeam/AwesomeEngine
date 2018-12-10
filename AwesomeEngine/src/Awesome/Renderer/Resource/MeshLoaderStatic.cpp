//#include <Awesome/Renderer/MeshLoaderStatic.hpp>

namespace aw
{
	/*int64 MeshLoaderStatic::GetNextFaceIndex(std::string& faceToken) // TODO modifying string is heavy use const char*& instead
	{
		if (faceToken[0] == '/')
			return 0;

		int64 index = std::stoll(faceToken); // stoll stops when it encounter a non numeric character ("42/666/-1" -> 42)
		faceToken.erase(0, faceToken.find('/') + 1);

		return index;
	}*/
}