# if max_val <= bound1 we always use dynamic programming, whose running time
# is roughly O(n*max_val)
# else if n <= n_max or max_val > bound2 we use the O(2^(n/2)) solution
# (split into two halves, generate all subsets of the two, recombine them)
# in all remaining cases we use dynamic programming

from heapq import merge

n_max = 35
bound1 = 1000
bound2 = 20000

act = []
n = int(raw_input())
max_val = 0
for i in range(n):
    a, v = raw_input().split()
    v = int(v)
    act.append((a, v))
    max_val = max(max_val, abs(v))

def exp_time():
    part1 = [(0, frozenset())]
    for i in range(n/2):
        part1 = [t for t in merge(part1,
                                  [(val+act[i][1], s.union({act[i][0]}))
                                   for (val,s) in part1])]      
    part2 = [(0, frozenset())]
    for i in range(n/2, n):
        part2 = [t for t in merge(part2,
                                  [(val+act[i][1], s.union({act[i][0]}))
                                   for (val,s) in part2])]
    i = 0
    j = len(part2) - 1
    while i < len(part1) and j >= 0:
        sum = part1[i][0] + part2[j][0]
        if (sum > 0):
            j -= 1
        elif (sum < 0):
            i += 1
        elif len(part1[i][1]) == 0 and len(part2[j][1]) == 0:
            i += 1 # or j -= 1, it's indifferent
        else:
            for x in part1[i][1].union(part2[j][1]):
               print i
            exit(0)

def dyn_prog():
    cache = {}

    def s(i, sum):
        if i >= n:
            return (sum == 0)
        if (i, sum) not in cache:
            cache[(i, sum)] = s(i + 1, sum) + s(i + 1, sum - act[i][1])
        return cache[(i, sum)]

    if s(0, 0) > 0:
        sum = 0
        sol = set()
        for i in range(n):
            if s(i + 1, sum - act[i][1]) > 0:
                sol.add(act[i][0])
                sum -= act[i][1]
        if len(sol) > 0:
            for x in sol:
               print x
            exit(0)

if max_val <= bound1:
    dyn_prog()
elif n <= n_max or max_val > bound2:
    exp_time()
else:
    dyn_prog()

print "no solution"


    
