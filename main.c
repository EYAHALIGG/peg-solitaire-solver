#include <iostream>
using namespace std;
typedef unsigned long long ulong;
typedef ulong board;
#define xmask   0b111
#define xmove1  0b011
#define xmove2  0b110
#define ymask   0b10000000100000001
#define ymove1  0b00000000100000001
#define ymove2  0b10000000100000000
#define PIECE_LIMIT 3 // (3 PIECES -> under 1 second) ( 2 PIECES -> 1,5 Hours) (1 PIECE -> 6,5 Hours)
int pieceCount(board);
void printBoard(board);
bool solvePuzzle(board);
const board unused =
~(28 | (28 << 8) |
    ((board)28 << (5 * 8)) |
    ((board)28 << (6 * 8)) |
    (127 << (2 * 8)) |
    (127 << (3 * 8)) |
    ((board)127 << (4 * 8)));
_declspec(naked) int pieceCount(board b)
{
    __asm
    {
        popcnt eax, dword ptr[esp + 4]
        popcnt edx, dword ptr[esp + 8]
        add eax, edx
        ret
    }
}
void printBoard(board b)
{
    for (int i = 0; i < 8; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            int index = k + i * 8;
            cout << "-*"[(b >> (63 - index)) & 1];
        }
        cout << "" << endl;
    }
}
bool solvePuzzle(board b)
{
#define routine() printBoard(b); cout << "" << endl
    if (pieceCount(b) <= PIECE_LIMIT) { routine(); return true; }
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            int i, k;
            i = x + y * 8;
            if (!(((board)xmask << i)&unused))
            {
                k = (b >> i)&xmask;
                if ((k == xmove1) || (k == xmove2)) { if (solvePuzzle(((board)xmask << i) ^ b)) { routine(); return true; } }
            }
            i = y + x * 8;
            if (!(((board)ymask << i)&unused))
            {
                k = (b >> i)&ymask;
                if ((k == ymove1) || (k == ymove2)) { if (solvePuzzle(((board)ymask << i) ^ b)) { routine(); return true; } }
            }
        }
    }
    return false;
#undef routine
}
int main()
{
    board b =
        (28 | (28 << 8) |
        ((board)28 << (5 * 8)) |
        ((board)28 << (6 * 8)) |
        (127 << (2 * 8)) |
        (127 << (3 * 8)) |
        ((board)127 << (4 * 8))) ^
        ((board)1 << (3 + 3 * 8));
    solvePuzzle(b);
    system("pause");
    return 0;
}
