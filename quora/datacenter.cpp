#include <stdio.h>
#include <vector>

using namespace std;

enum {
  Free,
  Busy,
  Source,
  Target
};

int w, h;
int count = 0;

void expand(const vector< vector<int> >& grid, vector< vector<bool> >& used, int x, int y, int remaining)
{
  used[x][y] = true;
  --remaining;

  if (grid[x][y] == Target) {
    if (!remaining)
      ++count;
  }
  else {
    if (x > 0 && grid[x - 1][y] != Busy && !used[x - 1][y])
      expand(grid, used, x - 1, y, remaining);
    if (x < w - 1 && grid[x + 1][y] != Busy && !used[x + 1][y])
      expand(grid, used, x + 1, y, remaining);
    if (y > 0 && grid[x][y - 1] != Busy && !used[x][y - 1])
      expand(grid, used, x, y - 1, remaining);
    if (y < h - 1 && grid[x][y + 1] != Busy && !used[x][y + 1])
      expand(grid, used, x, y + 1, remaining);
  }

  used[x][y] = false;
  ++remaining;
}

int main()
{
  scanf("%d %d", &w, &h);
  vector< vector<int> > grid(w, vector<int>(h));
  vector< vector<bool> > used(w, vector<bool>(h));
  int source_x, source_y;
  int length;
  for (int x = 0; x < w; ++x)
    for (int y = 0; y < h; ++y) {
      int room;
      scanf("%d", &room);
      if (room == Source) {
        source_x = x;
        source_y = y;
      }
      if (room != Busy)
        ++length;
      grid[x][y] = room;
    }
  expand(grid, used, source_x, source_y, length);
  printf("%d\n", count);
  return 0;
}
