#include "SnakeAI.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include <MyTools/CLFile.h>

CSnakeAI::CSnakeAI(_In_ DWORD dwWidth, _In_ DWORD dwHieght, _In_ CWall& Wall) : _FindPath(dwWidth, dwHieght), _dwHeight(dwHieght), _dwWidth(dwWidth), _Wall(Wall), _Exit(FALSE)
{

}

BOOL CSnakeAI::GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	CSnake::em_Snake_Direction NextDir_;
	if (_FindPath.GetNextDirection(VecSnake, Food, NextDir_))
	{
		// Make Virtual Snake to Eat Food
		auto VirtualSnake = VecSnake;
		VitualSnakeMove(VirtualSnake, Food);

		if (VirtualSnake.size() == 1)
		{
			NextDir = NextDir_;
			return TRUE;
		}

		// Remove Tail in Virtual Snake (that A* will set Disable Move in Snake Body, include Tail)
		auto Tail = VirtualSnake.back();
		VirtualSnake.erase(VirtualSnake.end() - 1);

		// Set Snake Tail = Food
		CSnake::em_Snake_Direction VirtualNextDir;
		if (_FindPath.GetNextDirection(VirtualSnake, Tail, VirtualNextDir))
		{
			if (VirtualSnake.size() <= 2 || !IsAlmostCloseTail(Tail, VirtualSnake))
			{
				NextDir = NextDir_;
				return TRUE;
			}
		}
	}

	return HypothesisFarMove(VecSnake, Food, NextDir);
}

BOOL CSnakeAI::IsExit() CONST
{
	return _Exit;
}

BOOL CSnakeAI::VitualSnakeMove(_In_ std::deque<POINT>& VecSnake, _In_ CONST POINT& Food)
{
	POINT Tail = { 0 };
	for (;;)
	{
		auto Head = VecSnake.at(0);
		if (Head.x == Food.x && Head.y == Food.y)
			break;

		CSnake::em_Snake_Direction NextDir;
		if (!_FindPath.GetNextDirection(VecSnake, Food, NextDir))
			return FALSE;

		if (CalcPointByDir(Head, NextDir, Head))
		{
			VecSnake.emplace_front(std::move(Head));
			// 如果吃了一个食物, 那么尾巴是不变的, 所以这里必须少删除一个尾巴
			Tail = VecSnake.back();
			VecSnake.erase(VecSnake.end() - 1);
		}
	}

	if (Tail.x != 0 || Tail.y != 0)
		VecSnake.push_back(std::move(Tail));
	return TRUE;
}

BOOL CSnakeAI::CalcPointByDir(_In_ CONST POINT& CurPoint, _In_ CSnake::em_Snake_Direction& Dir, _Out_ POINT& NewPoint) CONST
{
	NewPoint = CurPoint;
	switch (Dir)
	{
	case CSnake::em_Snake_Direction::em_Snake_Direction_None:
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Top:
		NewPoint.y -= 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Left:
		NewPoint.x -= 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Right:
		NewPoint.x += 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Bottom:
		NewPoint.y += 1;
		break;
	default:
		break;
	}

	
	return !_Wall.IsKnockWall(NewPoint);
}

BOOL CSnakeAI::HypothesisFarMove(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	auto Tail = VecSnake.back();
	auto VecDir = GetFarDir(VecSnake.at(0), Tail);
	for (auto& Dir : VecDir)
	{
		// Make Virtual Snake and Set Max Dis to New Head
		auto VirtualSnake = VecSnake;
		POINT Head;


		if(!CalcPointByDir(VirtualSnake.at(0), Dir, Head))	
			continue;

		if(IsSnakeBody(Head, VirtualSnake))
			continue;

		VirtualSnake.emplace_front(std::move(Head)); 
		VirtualSnake.erase(VirtualSnake.end() - 1);

		// Try to Find Tail
		CSnake::em_Snake_Direction VirtualDir;
		if (_FindPath.GetNextDirection(VirtualSnake, Tail, VirtualDir))
		{
			auto VirtualSnakeHead = VirtualSnake.front();
			if ((VirtualSnakeHead.x != Food.x || VirtualSnakeHead.y != Food.y) || !IsAlmostCloseTail(Tail, VirtualSnake))
			{
				NextDir = Dir;
				return TRUE;
			}
		}
	}

	// Get Max Dis Point to be Next Step
	for (auto& Dir : VecDir)
	{
		POINT Head;
		if (!CalcPointByDir(VecSnake.at(0), Dir, Head))
			continue;

		if (IsSnakeBody(Head, VecSnake))
			continue;

		NextDir = Dir;
		return TRUE;
	}

	_Exit = TRUE;
	return FALSE;
}

std::vector<CSnake::em_Snake_Direction> CSnakeAI::GetFarDir(_In_ CONST POINT& Head, _In_ CONST POINT& Tail) CONST
{
	std::vector<CSnake::em_Snake_Direction> VecDir = 
	{
		CSnake::em_Snake_Direction::em_Snake_Direction_Top,
		CSnake::em_Snake_Direction::em_Snake_Direction_Left,
		CSnake::em_Snake_Direction::em_Snake_Direction_Right,
		CSnake::em_Snake_Direction::em_Snake_Direction_Bottom
	};

	// get Max Dis Dir
	std::sort(VecDir.begin(), VecDir.end(), [=](_In_ CSnake::em_Snake_Direction emDir1, _In_ CSnake::em_Snake_Direction& emDir2) 
	{ 
		POINT p1, p2;
		if (!CalcPointByDir(Head, emDir1, p1) || !CalcPointByDir(Head, emDir2, p2))
			return false;

		return GetDisBy2D(p1, Tail) > GetDisBy2D(p2, Tail);
	});


	return VecDir;
}

BOOL CSnakeAI::IsSnakeBody(_In_ CONST POINT& TarPoint, _In_ CONST std::deque<POINT>& VecSnake) CONST
{
	return MyTools::CLPublic::Deque_find_if(VecSnake, static_cast<POINT*>(nullptr), [TarPoint](_In_ CONST POINT& itm) { return itm.x == TarPoint.x && itm.y == TarPoint.y; });
}

BOOL CSnakeAI::IsAlmostCloseTail(_In_ CONST POINT& Tail, _In_ CONST std::deque<POINT>& VecSnake) CONST
{
	auto& Head = VecSnake.at(0);
	if (Head.x == Tail.x)
		return abs(Head.y - Tail.y) == 1;
	else if (Head.y == Tail.y)
		return abs(Head.x - Tail.x) == 1;
	
	return FALSE;
}