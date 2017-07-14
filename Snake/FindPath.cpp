#include "FindPath.h"
#include <MyTools/CLPublic.h>

CFindPath::CFindPath(_In_ DWORD dwWidth, _In_ DWORD dwHeight) : _dwWidth(dwWidth), _dwHeight(dwHeight)
{
	for (DWORD i = 0; i < _dwWidth; ++i)
	{
		std::vector<Vertex> Vec;
		for (DWORD j = 0; j < _dwHeight; ++j)
		{
			Vertex Vertex_;
			Vertex_.Pt.X = i;
			Vertex_.Pt.Y = j;
			Vec.push_back(std::move(Vertex_));
		}
		_Chess.push_back(std::move(Vec));
	}
}

BOOL CFindPath::GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	ClearChess();
	for (CONST auto& itm : VecSnake)
		_Chess.at(itm.x).at(itm.y).bEnable = false;

	
	return FindPath_AStar(ConvertPoint(VecSnake.at(0)), ConvertPoint(Food), NextDir);
}

BOOL CFindPath::FindPath_AStar(_In_ CONST Point& VecSnakeHead, _In_ CONST Point& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	std::deque<Vertex*> VecSearch;
	Move(VecSnakeHead, nullptr, Food, VecSearch);

	while (VecSearch.size() > 0)
	{
		auto pResult = Heuristic(VecSearch, Food);
		if (pResult->Pt == Food)
		{
			// Head
			NextDir = FindRootChess(VecSnakeHead, pResult);
			return TRUE;
		}

		VecSearch.erase(VecSearch.begin());
		Move(pResult->Pt, pResult, Food, VecSearch);
	}

	return FALSE;
}

CFindPath::Vertex* CFindPath::Heuristic(_In_ std::deque<Vertex*>& VecSearch, _In_ CONST Point& Food) CONST
{
	std::sort(VecSearch.begin(), VecSearch.end(), [Food](CONST Vertex* p1, CONST Vertex* p2)
	{
		// Å·À­¾àÀë
		return p1->fDis < p2->fDis;
	});

	return VecSearch.at(0);
}

VOID CFindPath::Move(_In_ CONST Point& Pt, _In_ CONST Vertex* pFather, _In_ CONST Point& Food, _Out_ std::deque<Vertex *>& VecSearch)
{
	struct DirCalc
	{
		int nX, nY = 0;
	};

	CONST static std::vector<DirCalc> Vec = 
	{
		// Top     // Bottom
		{ 0, -1 },{ 0, 1 },

		// Left    // Right
		{ -1, 0 },{ 1, 0 },
	};

	Point NextStepPoint = Pt;
	for (CONST auto& itm : Vec)
	{
		NextStepPoint.X += itm.nX;
		NextStepPoint.Y += itm.nY;
		if (ExistChess(NextStepPoint))
		{
			Vertex& Vertex_ = _Chess.at(NextStepPoint.X).at(NextStepPoint.Y);
			Vertex_.bEnable = false;
			Vertex_.Father = const_cast<Vertex *>(pFather);
			Vertex_.fDis = MyTools::CLPublic::GetDisBy2D(Vertex_.Pt, Food);
			VecSearch.push_back(&Vertex_);
		}
		NextStepPoint = Pt;
	}
}

BOOL CFindPath::ExistChess(_In_ CONST Point& Pt) CONST
{
	return Pt.X >= 0 && Pt.X != static_cast<decltype(Point::X)>(_dwWidth) && Pt.Y >= 0 && Pt.Y != static_cast<decltype(Point::Y)>(_dwHeight) && _Chess.at(Pt.X).at(Pt.Y).bEnable;
}

CSnake::em_Snake_Direction CFindPath::FindRootChess(_In_ CONST Point& CurPos, _In_ Vertex* Node) CONST
{
	Vertex* Vertex_ = Node;
	while (Vertex_->Father != nullptr)
		Vertex_ = Vertex_->Father;

	int nAbsX = static_cast<int>(CurPos.X) - static_cast<int>(Vertex_->Pt.X);
	int nAbsY = static_cast<int>(CurPos.Y) - static_cast<int>(Vertex_->Pt.Y);

	if (nAbsX == 0)
		return nAbsY > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Top : CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
	else if(nAbsY == 0)
		return nAbsX > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Left : CSnake::em_Snake_Direction::em_Snake_Direction_Right;

	return CSnake::em_Snake_Direction::em_Snake_Direction_None;
}

CFindPath::Point CFindPath::ConvertPoint(_In_ CONST POINT& Pt) CONST
{
	Point Point_;
	Point_.X = static_cast<decltype(Point::X)>(Pt.x);
	Point_.Y = static_cast<decltype(Point::Y)>(Pt.y);

	return std::move(Point_);
}

VOID CFindPath::ClearChess()
{
	for (UINT i = 0;i < _Chess.size(); ++i)
	{
		auto& itm = _Chess.at(i);
		for (UINT j = 0;j < itm.size(); ++j)
		{
			itm.at(j).Clear();
		}
	}
}
