/*
 * PROJECT:         Aspia Disk Benchmark
 * FILE:            diskbench/graph.c
 * LICENSE:         LGPL (GNU Lesser General Public License)
 * PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
 */

#include "diskbench.h"
#include "helper.h"


#define LEFT_INDENT   40 /* ������ ����� */
#define RIGHT_INDENT  0  /* ������ ������ */
#define TOP_INDENT    0  /* ������ ������ */
#define BOTTOM_INDENT 30 /* ������ ����� */
#define X_GRID        10 /* ���������� ����� ����� �� ��� X */
#define Y_GRID        10 /* ���������� ����� ����� �� ��� Y */

static int old_pos_x = 0;
static int old_pos_y = 0;

/* ������������ �������� ��������� �� ���� */
static int x_max = 100;
static int y_max = 100;

static double y_div = 0;

/* ����� ���� ��������� */
static WCHAR x_name[5] = {0};
static WCHAR y_name[5] = {0};

/*
 * ������� ������ ������� ��� ���������� ������� (���, �����, ������� � ����)
 * HWND hwnd - ����, � ������� ������ ������
 */
VOID
DrawPolygon(HWND hwnd, HDC hdc)
{
    INT Height, Width, count, i;
    HBRUSH hBrush, hOldBrush;
    HPEN hPen, hOldPen;
    WCHAR buf[4];

    Height = GetClientWindowHeight(hwnd);
    Width = GetClientWindowWidth(hwnd);

    /* �������� ����������� ���������� ������, ��� ���������� */
    SetMapMode(hdc, MM_ISOTROPIC);
    /* ����� ���� */
    SetWindowExtEx(hdc, Width, Height, NULL);
    /* ������ ������� ������ */
    SetViewportExtEx(hdc, Width, Height, NULL);
    /* ������������� ������ ��������� */
    SetViewportOrgEx(hdc, 0, 0, NULL);

    /* ������������� ����� ���� */
    SetBkMode(hdc, TRANSPARENT);

    /* ������ ������ ������������� � ����� ���������� */
    hPen = CreatePen(PS_SOLID, 1, RGB(130, 130, 130));
    hBrush = CreateSolidBrush(RGB(0, 0, 0));

    hOldPen = SelectObject(hdc, hPen);
    hOldBrush = SelectObject(hdc, hBrush);

    Rectangle(hdc, 0, 0, Width, Height);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

    /* �������� �������� ��� ��������� */
    hPen = CreatePen(PS_SOLID, 1, RGB(145, 145, 145));
    hOldPen = SelectObject(hdc, hPen);

    /* ������ ��� Y */
    MoveToEx(hdc, LEFT_INDENT, Height - BOTTOM_INDENT, NULL);
    LineTo(hdc, LEFT_INDENT, TOP_INDENT);

    /* ������ ��� X */
    MoveToEx(hdc, LEFT_INDENT, Height - BOTTOM_INDENT, NULL);
    LineTo(hdc, Width - RIGHT_INDENT, Height - BOTTOM_INDENT);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    /* ���������������� � ��������� ����� */
    hPen = CreatePen(PS_DOT, 1, RGB(145, 145, 145));
    hOldPen = SelectObject(hdc, hPen);

    /* ������ ����� �� ��� X */
    count = (Width - LEFT_INDENT - RIGHT_INDENT) / X_GRID;

    /* ������������� ���� ������ */
    SetTextColor(hdc, RGB(255, 255, 255));
    /* ������������� ����� �� ������� */
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));

    for (i = 0; i < X_GRID; i++)
    {
        INT x = (count * i) + LEFT_INDENT;

        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, Height - TOP_INDENT - BOTTOM_INDENT);

        _itow((x_max / X_GRID) * i, buf, 10);
        TextOut(hdc, x - 5, Height - BOTTOM_INDENT + 5, buf, wcslen(buf));
    }

    /* ������ ����� �� ��� Y */
    count = (Height - TOP_INDENT - BOTTOM_INDENT) / Y_GRID;

    for (i = 1; i < Y_GRID; i++)
    {
        INT y = Height - (count * i) - BOTTOM_INDENT;

        MoveToEx(hdc, LEFT_INDENT, y, NULL);
        LineTo(hdc, Width - RIGHT_INDENT, y);

        StringCbPrintf(buf, sizeof(buf), L"%.1f",
            (y_div) ? (double)(((y_max/y_div) / Y_GRID) * i) : ((y_max / Y_GRID) * i));
        TextOut(hdc, LEFT_INDENT - 25, y - 7, buf, wcslen(buf));
    }

    /* ������� ������� ����� ��������� ����� */
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    /* ������� ������� � ��������� ��������� � ���� */
    TextOut(hdc, LEFT_INDENT - 30, TOP_INDENT + 5, y_name, 5); /* Y */
    TextOut(hdc, Width - RIGHT_INDENT - 15, Height - BOTTOM_INDENT + 5, x_name, 2); /* X */
}

/*
 * ������� ������ ����� �������
 * x, y - ���������� � ������� ����� �������� �����
 * ���������, ��� ��� ������ ������ ������� ��������� ����������
 * ����������� � x = 0, y = 0.
 */
VOID
DrawDiagram(HWND hwnd, double x, double y)
{
    INT Height = GetClientWindowHeight(hwnd);
    INT Width = GetClientWindowWidth(hwnd);
    HPEN hPen, hOldPen;
    HDC hdc = GetDC(hwnd);

    SetMapMode(hdc, MM_ANISOTROPIC);

    /* ����� ���� */
    SetWindowExtEx(hdc,
                   x_max, /* ����� ����� 100% */
                   y_max, /* 100 MB/s */
                   NULL);

    /* ������ ������� ������ */
    SetViewportExtEx(hdc,
                     Width - LEFT_INDENT,
                     -(Height - BOTTOM_INDENT),
                     NULL);

    /* ������������� ������ ��������� */
    SetViewportOrgEx(hdc,
                     LEFT_INDENT,
                     Height - BOTTOM_INDENT,
                     NULL);

    /* ������������� ����� ���� */
    SetBkMode(hdc, TRANSPARENT);

    hPen = CreatePen(PS_SOLID, 0, RGB(6, 236, 0));
    hOldPen = SelectObject(hdc, hPen);

    MoveToEx(hdc, old_pos_x, old_pos_y, NULL);

    old_pos_x = x;
    old_pos_y = (!y_div) ? y : y * y_div;

    LineTo(hdc, old_pos_x, old_pos_y);

    /* ������� ������� ����� ��������� ����� */
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

/* ������������� ����� ��� ���� ��������� */
VOID
GraphSetCoordNames(WCHAR *x, WCHAR *y)
{
    StringCbCopy(x_name, sizeof(x_name), x);
    StringCbCopy(y_name, sizeof(y_name), y);
}

/* ������������� ������������ �������� �� ���� ��������� */
VOID
GraphSetCoordMaxValues(HWND hwnd, double x, double y)
{
    DebugTrace(L"x = %f, y = %f", x, y);

    if (y < 5.0)
    {
        y_div = 10;
        y_max = (int)(y * y_div);
    }
    else if (y < 2.0)
    {
        y_div = 100;
        y_max = (int)(y * y_div);
    }
    else
    {
        y_max = (int)y;
        y_div = 0;
    }

    x_max = x;

    DrawPolygon(hwnd, GetDC(hwnd));
}

/*
 * ������������� ������ ��������� ����� (�� ������� ���������� ���������)
 * � ���������� x � y
 */
VOID
GraphSetBeginCoord(int x, int y)
{
    old_pos_x = x;
    old_pos_y = y;
}

/* ������������� ������ ��������� � 0-0 � ������� �� ������� ������� */
VOID
GraphClear(HWND hwnd)
{
    GraphSetBeginCoord(0, 0);
    DrawPolygon(hwnd, GetDC(hwnd));
}

/*
 * ������� ��� ������� � ���������� ��������� WM_PAINT
 * HWND hwnd - ����, � ������� ����� �������� ������
 */
VOID
GraphOnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc;

    hdc = BeginPaint(hwnd, &ps);

    DrawPolygon(hwnd, hdc);
    ValidateRect(hwnd, NULL);

    EndPaint(hwnd, &ps);
}
