#include <stdio.h>

#include <algorithm>
#include <vector>

using namespace std;


int main()
{
  int k, m;
  scanf("%d", &k);
  scanf("%d", &m);

  vector< vector<char> > sequences(k, vector<char>(m));
  for (int i = 0; i < k; ++i) {
    char sequence[m];
    scanf("%s", sequence);
    copy(sequence, sequence + m, sequences[i].begin());
  }

  vector< vector<short> > valid(k, vector<short>(m + 1));
  for (int i = 0; i < k; ++i)
    valid[i][0] = true;

  char c;
  int p = 0;
  while (scanf("%c", &c) == 1) {
    for (int i = 0; i < k; ++i) {
      for (int j = m - 1; j >= 0; --j)
	valid[i][j + 1] = (valid[i][j] && c == sequences[i][j]);
      if (valid[i][m])
	printf("%d,%d\n", i, p - m);
    }
    ++p;
  }

  return 0;
}
