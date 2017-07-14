#ifndef __SNAKE_FINDPATH_H__
#define __SNAKE_FINDPATH_H__

#include "Snake.h"

class CFindPath
{
public:
	struct Point
	{
		int X = 0;
		int Y = 0;

		bool operator == (CONST Point& p) CONST
		{
			return X == p.X && Y == p.Y;
		}
		bool operator != (CONST Point& p) CONST
		{
			return X != p.X || Y != p.Y;
		}
	};

	struct Vertex
	{
		Point Pt;
		float fDis = 0;
		bool bEnable = true;

		struct Vertex* Father = nullptr;

		VOID Clear()
		{
			fDis = 0;
			bEnable = true;
			Father = nullptr;
		}
	};
public:
	CFindPath(_In_ DWORD dwWidth, _In_ DWORD dwHeight);
	~CFindPath() = default;

	BOOL GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir);
private:
	// A*
	BOOL FindPath_AStar(_In_ CONST Point& VecSnakeHead, _In_ CONST Point& Food, _Out_opt_ CSnake::em_Snake_Direction& NextDir);

	// 
	Vertex* Heuristic(_In_ std::deque<Vertex*>& VecSearch, _In_ CONST Point& Food) CONST;

	//
	VOID Move(_In_ CONST Point& Pt, _In_ CONST Vertex* pFather, _In_ CONST Point& Food, _Out_ std::deque<Vertex*>& VecSearch);

	//
	BOOL ExistChess(_In_ CONST Point& Pt) CONST;

	//
	CSnake::em_Snake_Direction FindRootChess(_In_ CONST Point& CurPos, _In_ Vertex* Node) CONST;

	//
	Point ConvertPoint(_In_ CONST POINT& Pt) CONST;

	//
	VOID ClearChess();
private:
	DWORD _dwWidth;
	DWORD _dwHeight;
	std::vector<std::vector<Vertex>> _Chess;
};



#endif // !__SNAKE_FINDPATH_H__
