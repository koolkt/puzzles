#include <stdio.h>

#include <utility>
#include <vector>

using namespace std;


void print_path(vector< pair<int, int> >& path, int length)
{
  for (int i = 0; i < length; ++i)
    printf("%d,%d\n", path[i].first, path[i].second);
}

bool expand(vector< vector<int> >& nucleus, int n, int m, int x, 
	    int i, int j,
	    vector< pair<int, int> >& path, int length,
	    vector< vector<short> >& used)
{
  if (length == x) {
    print_path(path, length);
    return true;
  }

  if (nucleus[i][j])
    return false;

  used[i][j] = true;
  path[length] = pair<int, int>(i, j);
  
  if (i > 0 && !used[i - 1][j])
    if (expand(nucleus, n, m, x, i - 1, j, path, length + 1, used))
      return true;
  if (i < n - 1 && !used[i + 1][j])
    if (expand(nucleus, n, m, x, i + 1, j, path, length + 1, used))
      return true;
  if (j > 0 && !used[i][j - 1])
    if (expand(nucleus, n, m, x, i, j - 1, path, length + 1, used))
      return true;
  if (j < n - 1 && !used[i][j + 1])
    if (expand(nucleus, n, m, x, i, j + 1, path, length + 1, used))
      return true;

  used[i][j] = false;

  return false;
}

int main()
{
  int n, m, x;
  scanf("%d", &n);
  scanf("%d", &m);
  scanf("%d", &x);

  vector< vector<int> > nucleus(n, vector<int>(m));
  for (int i = 0; i < n; ++i) {
    char squares[m];
    scanf("%s", squares);
    for (int j = 0; j < m; ++j)
      nucleus[i][j] = (squares[j] == 'x');
  }

  vector< pair<int, int> > path(x);
  vector< vector<short> > used(n, vector<short>(m));

  bool possible = false;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      if (!possible)
	possible = expand(nucleus, n, m, x, i, j, path, 0, used);
  if (!possible)
    printf("impossible\n");
  
  return 0;
}
