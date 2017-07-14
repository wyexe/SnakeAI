#include "Wall.h"
#include <iostream>
#include <MyTools/CLPublic.h>

CWall::CWall() : _dwWidth(NULL), _dwHeight(NULL)
{
	//pChess = nullptr;
}

VOID CWall::CreateWall(_In_ DWORD dwHeight, _In_ DWORD dwWidth)
{
	_dwHeight = dwHeight;
	_dwWidth = dwWidth;

	HideCursor();
	// Print Left
	for (DWORD i = 1; i <= dwHeight; ++i)
		PrintByPoint(POINT{ static_cast<LONG>(0), static_cast<LONG>(i) }, em_PrintType::em_PrintType_LeftWall);

	// Print Top
	for (DWORD i = 1; i <= dwWidth; ++i)
		PrintByPoint(POINT{ static_cast<LONG>(i), static_cast<LONG>(0) }, em_PrintType::em_PrintType_TopWall);

	// Print Right
	for (DWORD i = 1; i <= dwHeight; ++i)
		PrintByPoint(POINT{ static_cast<LONG>(dwWidth + 1), static_cast<LONG>(i) }, em_PrintType::em_PrintType_RightWall);

	// Print Bottom
	for (DWORD i = 1; i <= dwWidth; ++i)
		PrintByPoint(POINT{ static_cast<LONG>(i), static_cast<LONG>(dwHeight + 1) }, em_PrintType::em_PrintType_BottomWall);

	for (DWORD i = 0;i < dwWidth; ++i)
	{
		std::vector<BOOL> Vec;
		for (DWORD j = 0; j < dwHeight; ++j)
			Vec.push_back(TRUE);

		_VecChess.push_back(Vec);
	}
}

DWORD CWall::GetWidth() CONST _NOEXCEPT
{
	return _dwWidth;
}

DWORD CWall::GetHeight() CONST _NOEXCEPT
{
	return _dwHeight;
}

VOID CWall::SetPointAttribute(_In_ CONST POINT& Tar, BOOL bEnable)
{
	if (_VecChess.size() < static_cast<UINT>(Tar.x) && _VecChess.at(0).size() < static_cast<UINT>(Tar.y))
	{
		_VecChess.at(Tar.x).at(Tar.y) = bEnable;
	}
}

std::vector<POINT> CWall::GetVecUsefulPoint(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food) CONST
{
	std::vector<POINT> VecUsefulPoint;

	std::vector<std::vector<BOOL>> VecChess = _VecChess;
	for (CONST auto& itm : VecSnake)
		VecChess.at(itm.x).at(itm.y) = FALSE;

	VecChess.at(Food.x).at(Food.y) = FALSE;

	for (DWORD X = 0;X < _dwWidth; ++X)
	{
		for (DWORD Y = 0; Y < _dwHeight; ++Y)
		{
			if(!VecChess.at(X).at(Y))
				continue;

			VecUsefulPoint.push_back(POINT {static_cast<decltype(POINT::x)>(X),static_cast<decltype(POINT::y)>(Y)});
		}
	}

	return VecUsefulPoint;
}

BOOL CWall::IsKnockWall(_In_ CONST POINT& Tar) CONST
{
	return Tar.x >= static_cast<LONG>(_dwWidth) || Tar.x < 0 || Tar.y >= static_cast<LONG>(_dwHeight) || Tar.y < 0;
}

VOID CWall::Clear()
{
	for (DWORD i = 0;i < _dwWidth + 1; ++i)
	{
		for (DWORD j = 0;j < _dwHeight + 1; ++j)
			PrintByPoint(POINT{ static_cast<LONG>(i),static_cast<LONG>(j) }, em_PrintType::em_PrintType_Empty);
	}
}

VOID CWall::PrintByPoint(_In_ POINT TarPoint, _In_ em_PrintType emPrintType)
{
	static HANDLE hOut = INVALID_HANDLE_VALUE;
	if (hOut == INVALID_HANDLE_VALUE)
		hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);


	COORD crd = { static_cast<SHORT>(TarPoint.x * 2), static_cast<SHORT>(TarPoint.y) };

	::SetConsoleCursorPosition(hOut, crd);

	struct PrintContent
	{
		em_PrintType emType;
		WCHAR        wchText;
	};

	CONST static std::vector<PrintContent> VecContent = 
	{
		{ em_PrintType::em_PrintType_TopWall, L'▁'},{ em_PrintType::em_PrintType_LeftWall, L'▕' },
		{ em_PrintType::em_PrintType_RightWall, L'▏' },{ em_PrintType::em_PrintType_BottomWall, L'▔' },
		{ em_PrintType::em_PrintType_SnakeHead_Dir_Top, L'▲' },{ em_PrintType::em_PrintType_SnakeHead_Dir_Bottom, L'▼' },
		{ em_PrintType::em_PrintType_SnakeHead_Dir_Left, L'◀' },{ em_PrintType::em_PrintType_SnakeHead_Dir_Right, L'▶' },
		{ em_PrintType::em_PrintType_SnakeBody, L'■'},
		{ em_PrintType::em_PrintType_Food, L'★' },{ em_PrintType::em_PrintType_Empty, L' '},
	};

	auto pElement = MyTools::CLPublic::Vec_find_if_Const(VecContent, [emPrintType](CONST PrintContent& Content) { return Content.emType == emPrintType; });
	if (pElement != nullptr)
	{
		std::wcout << pElement->wchText;
	}
}

VOID CWall::PrintSnakeByPoint(_In_ POINT TarPoint, _In_ em_PrintType emPrintType)
{
	TarPoint.x += 1;
	TarPoint.y += 1;
	PrintByPoint(TarPoint, emPrintType);
}

VOID CWall::HideCursor()
{
	HANDLE hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO Info = { 1,0 };
	::SetConsoleCursorInfo(hOut, &Info);
}
