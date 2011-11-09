from munkres import Munkres, print_matrix, make_cost_matrix
import sys

n, m = map(int, raw_input().split())
matrix = []
for i in xrange(n):
  matrix.append(map(int, raw_input().split()))

cost_matrix = make_cost_matrix(matrix, lambda cost: sys.maxint - cost)
m = Munkres()
indexes = m.compute(cost_matrix)

for i, j in indexes:
  value = matrix[i][j]
  print '%d,%d' % (i, j)
