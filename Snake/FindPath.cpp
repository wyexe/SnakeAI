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

UINT CFindPath::GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	ClearChess();

	for (CONST auto& itm : VecSnake)
	{
		_Chess.at(itm.x).at(itm.y).bEnable = false;
	}

	_Chess.at(Food.x).at(Food.y).bEnable = true;
	
	return FindPath_AStar(ConvertPoint(VecSnake.at(0)), ConvertPoint(Food), NextDir);
}

UINT CFindPath::GetNextDirection(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _Out_ std::vector<Point>& VecPoint)
{
	ClearChess();

	for (CONST auto& itm : VecSnake)
	{
		_Chess.at(itm.x).at(itm.y).bEnable = false;
	}

	_Chess.at(Food.x).at(Food.y).bEnable = true;

	return FindPath_AStar_Far(ConvertPoint(VecSnake.at(0)), ConvertPoint(Food), VecPoint);
}

CFindPath::Point CFindPath::GetMinestDisVertex(_In_ CONST std::vector<Point>& VecPoint, _In_ CONST Point& Food) CONST
{
	auto Vec = VecPoint;
	std::sort(Vec.begin(), Vec.end(), [=](CONST Point& p1, CONST Point& p2)
	{
		return Get2DDis(p1, Food) < Get2DDis(p2, Food);
	});

	return Vec.at(0);
}

BOOL CFindPath::FindPath(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food, _In_ CONST POINT& Tail, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	// Head to Food
	CSnake::em_Snake_Direction emDirHeadToFood = CSnake::em_Snake_Direction::em_Snake_Direction_None;
	if (!GetNextDirection(VecSnake, Food, emDirHeadToFood))
		return FALSE;

	if (VecSnake.size() <= 2)
	{
		NextDir = emDirHeadToFood;
		return TRUE;
	}

	// Head to Tail
	std::vector<Point> VecPointHeadToTail;
	if (GetNextDirection(VecSnake, Tail, VecPointHeadToTail) == 0)
		return FALSE;

	
	// Minest Distance Vertex in (Head to Tail) to Food
	auto MinestPoint = GetMinestDisVertex(VecPointHeadToTail, ConvertPoint(Food));
	
	if (Get2DDis(MinestPoint, ConvertPoint(Food)) <= Get2DDis(ConvertPoint(VecSnake.at(0)), ConvertPoint(Food)))
	{
		// Move to Food
		NextDir = emDirHeadToFood;
		return TRUE;
	}

	// Move to Tail
	NextDir = ComprDirection(ConvertPoint(VecSnake.at(0)), VecPointHeadToTail.back());
	return TRUE;
}

UINT CFindPath::FindPath_AStar(_In_ CONST Point& VecSnakeHead, _In_ CONST Point& Food, _Out_ CSnake::em_Snake_Direction& NextDir)
{
	std::deque<Vertex*> VecSearch;
	Move(VecSnakeHead, nullptr, Food, VecSearch);

	while (VecSearch.size() > 0)
	{
		auto pResult = Heuristic(VecSearch, Food);
		if (pResult->Pt == Food)
		{
			// Head
			UINT uCount = 0;
			NextDir = FindRootChess(VecSnakeHead, pResult, uCount);
			return uCount;
		}

		VecSearch.erase(VecSearch.begin());
		Move(pResult->Pt, pResult, Food, VecSearch);
	}

	return 0;
}

UINT CFindPath::FindPath_AStar_Far(_In_ CONST Point& VecSnakeHead, _In_ CONST Point& Food, _Out_opt_ std::vector<Point>& VecPoint)
{
	std::deque<Vertex*> VecSearch;
	Move(VecSnakeHead, nullptr, Food, VecSearch);

	while (VecSearch.size() > 0)
	{
		auto pResult = Heuristic_Far(VecSearch, Food);
		if (pResult->Pt == Food)
		{
			// Head
			VertexToVecDir(pResult, VecPoint);
			return VecPoint.size();
		}

		VecSearch.erase(VecSearch.begin());
		Move(pResult->Pt, pResult, Food, VecSearch);
	}

	return 0;
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

CFindPath::Vertex* CFindPath::Heuristic_Far(_In_ std::deque<Vertex*>& VecSearch, _In_ CONST Point& Food) CONST
{
	std::sort(VecSearch.begin(), VecSearch.end(), [Food](CONST Vertex* p1, CONST Vertex* p2)
	{
		// Å·À­¾àÀë
		return p1->fDis > p2->fDis;
	});

	return VecSearch.at(0);
}

float CFindPath::Get2DDis(_In_ CONST Point& p1, _In_ CONST Point& p2) CONST
{
	return  static_cast<float>(abs(p1.X - p2.X) + abs(p1.Y - p2.Y));;
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
			// Å·¼¸Àï¾àÀë
			//Vertex_.fDis = MyTools::CLPublic::GetDisBy2D(Vertex_.Pt, Food);
			// Âü¹þ¶Ù¾àÀë
			Vertex_.fDis = static_cast<float>(abs(Vertex_.Pt.X - Food.X) + abs(Vertex_.Pt.Y - Food.Y));
			VecSearch.push_back(&Vertex_);
		}
		NextStepPoint = Pt;
	}
}

BOOL CFindPath::ExistChess(_In_ CONST Point& Pt) CONST
{
	return Pt.X >= 0 && Pt.X != static_cast<decltype(Point::X)>(_dwWidth) && Pt.Y >= 0 && Pt.Y != static_cast<decltype(Point::Y)>(_dwHeight) && _Chess.at(Pt.X).at(Pt.Y).bEnable;
}

CSnake::em_Snake_Direction CFindPath::FindRootChess(_In_ CONST Point& CurPos, _In_ Vertex* Node, _Out_ UINT& uCount) CONST
{
	Vertex* Vertex_ = Node;
	while (Vertex_->Father != nullptr)
	{
		uCount += 1;
		Vertex_ = Vertex_->Father;
	}

	uCount += 1;
	return ComprDirection(CurPos, Vertex_->Pt);
}
 
VOID CFindPath::VertexToVecDir(_In_ Vertex* Node, _Out_ std::vector<Point>& VecPoint)
{
	Vertex* Vertex_ = Node;
	while (Vertex_->Father != nullptr)
	{
		VecPoint.push_back(Vertex_->Pt);
		Vertex_ = Vertex_->Father;
	}
	VecPoint.push_back(Vertex_->Pt);
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

CSnake::em_Snake_Direction CFindPath::ComprDirection(_In_ CONST Point& CurPoint, _In_ CONST Point& TarPoint) CONST
{
	int nAbsX = static_cast<int>(CurPoint.X) - static_cast<int>(TarPoint.X);
	int nAbsY = static_cast<int>(CurPoint.Y) - static_cast<int>(TarPoint.Y);

	if (nAbsX == 0)
		return nAbsY > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Top : CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
	else if (nAbsY == 0)
		return nAbsX > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Left : CSnake::em_Snake_Direction::em_Snake_Direction_Right;

	return CSnake::em_Snake_Direction::em_Snake_Direction_None;
}
