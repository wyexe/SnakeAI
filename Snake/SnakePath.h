#ifndef __SNAKE_SNAKEPATH_H__
#define __SNAKE_SNAKEPATH_H__

#include "Snake.h"

class CSnakePath
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
	CSnakePath(_In_ DWORD dwWidth, _In_ DWORD dwHeight);
	~CSnakePath() = default;

	BOOL FindPath(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& EndPoint, _Out_ CSnake::em_Snake_Direction& NextDir);

private:
	//
	Point ConvertPoint(_In_ CONST POINT& Pt) CONST;

	//
	VOID ClearChess();

	//
	CSnake::em_Snake_Direction ComprDirection(_In_ CONST Point& CurPoint, _In_ CONST Point& TarPoint) CONST;

	//
	CSnake::em_Snake_Direction FindRootChess(_In_ CONST Point& CurPos, _In_ Vertex* Node, _Out_ UINT& uCount) CONST;

	//
	BOOL ExistChess(_In_ CONST Point& Pt) CONST;
private:
	DWORD _dwWidth;
	DWORD _dwHeight;
	std::vector<std::vector<Vertex>> _Chess;
};



#endif // !__SNAKE_SNAKEPATH_H__
