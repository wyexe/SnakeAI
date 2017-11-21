#ifndef __SNAKE_SNAKEAI_H__
#define __SNAKE_SNAKEAI_H__

#include "FindPath.h"
#include "EasyFindPath.h"

class CSnakeAI
{
public:
	CSnakeAI(_In_ DWORD dwWidth, _In_ DWORD dwHieght, _In_ CWall& Wall);
	~CSnakeAI() = default;

	BOOL GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir);

	BOOL IsExit() CONST;
private:
	//
	BOOL VitualSnakeMove(_In_ std::deque<POINT>& VecSnake, _In_ CONST POINT& Food);

	//
	BOOL CalcPointByDir(_In_ CONST POINT& CurPoint, _In_ CSnake::em_Snake_Direction& Dir, _Out_ POINT& NewPoint) CONST;

	//
	BOOL HypothesisFarMove(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir);
	
	//
	std::vector<CSnake::em_Snake_Direction> GetFarDir(_In_ CONST POINT& Head, _In_ CONST POINT& Tail) CONST;

	//
	BOOL IsSnakeBody(_In_ CONST POINT& TarPoint, _In_ CONST std::deque<POINT>& VecSnake) CONST;

	//
	BOOL IsAlmostCloseTail(_In_ CONST POINT& Tail, _In_ CONST POINT& Food, _In_ CONST std::deque<POINT>& VecSnake) CONST;

	//
	BOOL TryToUseHamiltonianCycle(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir);

	POINT DirectionToPoint(_In_ CONST POINT& Head, _In_ CSnake::em_Snake_Direction Dir) CONST;

	//
	int PointToIndex(_In_ CONST POINT& Pt) CONST;
private:
	template<class T>
	static float GetDisBy2D(_In_ CONST T& CurPoint, _In_ CONST T& TarPoint)
	{
		// Fastest Speed to Find Tail
		//float fsum = pow(static_cast<float>(CurPoint.x) - static_cast<float>(TarPoint.x), 2.0f) + pow(static_cast<float>(CurPoint.y) - static_cast<float>(TarPoint.y), 2.0f);
		//return sqrt(fsum);
		return static_cast<float>(abs(CurPoint.x - TarPoint.x) + abs(CurPoint.y - TarPoint.y));
	}
private:
	DWORD _dwWidth;
	DWORD _dwHeight;
	CFindPath _FindPath;
	CEasyFindPath _EasyFinPath;
	CWall& _Wall;
};


#endif // !__SNAKE_SNAKEAI_H__
