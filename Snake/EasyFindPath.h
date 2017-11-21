#ifndef __SNAKE_EASYFINDPATH_H__
#define __SNAKE_EASYFINDPATH_H__

#include "Snake.h"
class CEasyFindPath
{
private:
	enum class em_Vertex_Corner
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		None,
	};
public:
	CEasyFindPath(_In_ DWORD dwWidth, _In_ DWORD dwHeight);
	~CEasyFindPath() = default;;

	CSnake::em_Snake_Direction FindNextDirection(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir);
private:
	CSnake::em_Snake_Direction NoneMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir);

	CSnake::em_Snake_Direction TopMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir);

	CSnake::em_Snake_Direction BottomMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir);

	em_Vertex_Corner GetHeadCorner(_In_ CONST POINT& Head) CONST;
private:
	DWORD _dwWidth;
	DWORD _dwHeight;
	CSnake::em_Snake_Direction _emDir;
};



#endif // !__SNAKE_EASYFINDPATH_H__
