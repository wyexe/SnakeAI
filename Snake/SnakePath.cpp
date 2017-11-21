#include "SnakePath.h"

CSnakePath::CSnakePath(_In_ DWORD dwWidth, _In_ DWORD dwHeight) : _dwWidth(dwWidth), _dwHeight(dwHeight)
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

BOOL CSnakePath::FindPath(_In_ CONST std::deque<POINT>& , _In_ CONST POINT& , _Out_ CSnake::em_Snake_Direction& )
{

	return TRUE;
}

CSnakePath::Point CSnakePath::ConvertPoint(_In_ CONST POINT& Pt) CONST
{
	Point Point_;
	Point_.X = static_cast<decltype(Point::X)>(Pt.x);
	Point_.Y = static_cast<decltype(Point::Y)>(Pt.y);

	return std::move(Point_);
}

VOID CSnakePath::ClearChess()
{
	for (UINT i = 0; i < _Chess.size(); ++i)
	{
		auto& itm = _Chess.at(i);
		for (UINT j = 0; j < itm.size(); ++j)
		{
			itm.at(j).Clear();
		}
	}
}

CSnake::em_Snake_Direction CSnakePath::ComprDirection(_In_ CONST Point& CurPoint, _In_ CONST Point& TarPoint) CONST
{
	int nAbsX = static_cast<int>(CurPoint.X) - static_cast<int>(TarPoint.X);
	int nAbsY = static_cast<int>(CurPoint.Y) - static_cast<int>(TarPoint.Y);

	if (nAbsX == 0)
		return nAbsY > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Top : CSnake::em_Snake_Direction::em_Snake_Direction_Bottom;
	else if (nAbsY == 0)
		return nAbsX > 0 ? CSnake::em_Snake_Direction::em_Snake_Direction_Left : CSnake::em_Snake_Direction::em_Snake_Direction_Right;

	return CSnake::em_Snake_Direction::em_Snake_Direction_None;
}

CSnake::em_Snake_Direction CSnakePath::FindRootChess(_In_ CONST Point& CurPos, _In_ Vertex* Node, _Out_ UINT& uCount) CONST
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

BOOL CSnakePath::ExistChess(_In_ CONST Point& Pt) CONST
{
	return Pt.X >= 0 && Pt.X != static_cast<decltype(Point::X)>(_dwWidth) && Pt.Y >= 0 && Pt.Y != static_cast<decltype(Point::Y)>(_dwHeight) && _Chess.at(Pt.X).at(Pt.Y).bEnable;
}
