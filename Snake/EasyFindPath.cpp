#include "EasyFindPath.h"

CEasyFindPath::CEasyFindPath(_In_ DWORD dwWidth, _In_ DWORD dwHeight) : _dwWidth(dwWidth), _dwHeight(dwHeight)
{
	_emDir = CSnake::em_Snake_Direction::em_Snake_Direction_None;
}

CSnake::em_Snake_Direction CEasyFindPath::FindNextDirection(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir)
{
	switch (_emDir)
	{
	case CSnake::em_Snake_Direction::em_Snake_Direction_None:
		return NoneMove(Head, emLastDir);
	case CSnake::em_Snake_Direction::em_Snake_Direction_Top:
		return TopMove(Head, emLastDir);
	case CSnake::em_Snake_Direction::em_Snake_Direction_Bottom:
		return BottomMove(Head, emLastDir);
	default:
		break;
	}
	return emLastDir;
}

CSnake::em_Snake_Direction CEasyFindPath::NoneMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir)
{
	switch (GetHeadCorner(Head))
	{
	case em_Vertex_Corner::BottomLeft: case em_Vertex_Corner::TopLeft:
		return CSnake::em_Snake_Direction::em_Snake_Direction_Right;
	case em_Vertex_Corner::BottomRight:
		if (emLastDir != CSnake::em_Snake_Direction::em_Snake_Direction_Right)
		{
			_emDir = CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
			return CSnake::em_Snake_Direction::em_Snake_Direction_Left;
		}
		break;
	case em_Vertex_Corner::TopRight:
		if (emLastDir != CSnake::em_Snake_Direction::em_Snake_Direction_Right)
		{
			_emDir = CSnake::em_Snake_Direction::em_Snake_Direction_Top;
			return CSnake::em_Snake_Direction::em_Snake_Direction_Left;
		}
		break;
	default:
		break;
	}

	if (Head.y == 0)
		return emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Right ? CSnake::em_Snake_Direction::em_Snake_Direction_Bottom : CSnake::em_Snake_Direction::em_Snake_Direction_Right;
	else if(Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 1))
		return emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Right ? CSnake::em_Snake_Direction::em_Snake_Direction_Top : CSnake::em_Snake_Direction::em_Snake_Direction_Right;

	return emLastDir;
}

CSnake::em_Snake_Direction CEasyFindPath::TopMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir)
{
	if (Head.y == 0)
	{
		if (Head.x == 0)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;

		if (emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Left)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Left;

		if(emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Top)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Right;


		return CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
	}

	if (Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 2))
	{
		if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 2))
			return CSnake::em_Snake_Direction::em_Snake_Direction_Top;
		if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1))
			return CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;

		switch (emLastDir)
		{
		case CSnake::em_Snake_Direction::em_Snake_Direction_Bottom:
			return CSnake::em_Snake_Direction::em_Snake_Direction_Right;
		case CSnake::em_Snake_Direction::em_Snake_Direction_Right:
			return CSnake::em_Snake_Direction::em_Snake_Direction_Top;
		default:
			break;
		}
	}

	if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1) && Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 1))
	{
		_emDir = CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
		return CSnake::em_Snake_Direction::em_Snake_Direction_Left;
	}

	return emLastDir;
}

CSnake::em_Snake_Direction CEasyFindPath::BottomMove(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction emLastDir)
{
	if (Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 1))
	{
		if (Head.x == 0)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Top;

		if (emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Bottom)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Right;

		if (emLastDir == CSnake::em_Snake_Direction::em_Snake_Direction_Right)
			return CSnake::em_Snake_Direction::em_Snake_Direction_Top;

		return CSnake::em_Snake_Direction::em_Snake_Direction_Left;
	}

	if (Head.y == 1)
	{
		if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 2))
			return CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
		if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1))
		{
			return CSnake::em_Snake_Direction::em_Snake_Direction_Top;
		}

		switch (emLastDir)
		{
		case CSnake::em_Snake_Direction::em_Snake_Direction_Top:
			return CSnake::em_Snake_Direction::em_Snake_Direction_Right;
		case CSnake::em_Snake_Direction::em_Snake_Direction_Right:
			return CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
		default:
			break;
		}
	}

	if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1) && Head.y == 0)
	{
		_emDir = CSnake::em_Snake_Direction::em_Snake_Direction_Top;
		return CSnake::em_Snake_Direction::em_Snake_Direction_Left;
	}

	return emLastDir;
}

CEasyFindPath::em_Vertex_Corner CEasyFindPath::GetHeadCorner(_In_ CONST POINT& Head) CONST
{
	if (Head.x == 0 && Head.y == 0)
		return em_Vertex_Corner::TopLeft;

	if (Head.x == 0 && Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 1))
		return em_Vertex_Corner::BottomLeft;

	if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1) && Head.y == 0)
		return em_Vertex_Corner::TopRight;

	if (Head.x == static_cast<decltype(POINT::x)>(_dwWidth - 1) && Head.y == static_cast<decltype(POINT::y)>(_dwHeight - 1))
		return em_Vertex_Corner::BottomRight;

	return em_Vertex_Corner::None;
}
