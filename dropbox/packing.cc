/*
The main idea of my solution is the concept of "nodes", i.e. positions where
rectangles can be placed.
We start with (0,0) and each time we place a new rectangle we remove its 
position from the current nodes and we add the remaining three corners.
In the greedy algorithm, rectangles are placed sequentially from the largest 
to the smallest, at each step minimizing the area of the current enclosing 
rectangle over all available nodes.
In the recursive (backtracking) version, all possible combinations are 
evaluated by trying to place each available rectangle at each of the current 
possible nodes, and continuing to build the solution until either we've 
placed all the rectangles or it's already proven to be sub-optimal.
It can be shown that this representation (where rectangles can only be placed 
at corners of other rectangles) always contains an optimal solution, so the 
backtracking algorithm gives the smallest enclosing area.
On the other hand, generating ALL equivalent optimal configurations would 
involve considering all the points in the enclosing area instead of just 
three points for each already placed rectangle. Doing this would make the 
running time dependent not only on the number of rectangles, but also on 
their size, which is clearly inefficient.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

#define GREEDY_THRESHOLD 7 // if n < GREEDY_THRESHOLD we use backtracking, 
                           // otherwise we use the faster greedy solver

struct Node {
  int x, y;

  Node() {};

  Node(int _x, int _y) : x(_x), y(_y) {}

  Node(const Node& n) : x(n.x), y(n.y) {}
  
  const Node operator+(const Node& n)
  {
    return Node(x + n.x, y + n.y);
  }

  bool operator==(const Node& n) const
  {
    return (x == n.x && y == n.y);
  }

};

struct Rect {
  int width, height, area;
  int x, y;
  
  Rect() {}

  Rect(int w, int h) : width(w), height(h)
  {
    area = width * height;
  }

  void place(Node n)
  {
    x = n.x;
    y = n.y;
  }

  bool operator==(const Rect& r) const
  {
    return (width == r.width && height == r.height);
  }

  bool operator!=(const Rect& r) const
  {
    return !operator==(r);
  }

  bool operator<(const Rect& r) const
  {
    return (area > r.area);
  }

  bool intersects(Rect& r)
  {
    return  (x < r.x + r.width && y < r.y + r.height && r.x < x +
             width && r.y < y + height);
  }

  void reverse()
  {
    int t = width;
    width = height;
    height = t;
  }

};

enum {
  Empty, Corner, Hedge, Vedge
};

const char sym[4] = {' ', '+', '-', '|'};

struct Solution {
  vector<Rect> rect;
  int width, height, area;

  vector<Solution> btsol;
  bool bt;

  Solution(vector<Rect>& r, int w = 0, int h = 0) : rect(r), width(w), 
                                                   height(h)
  {
    area = width * height;
    bt = false;
  }

  bool operator==(const Solution& s) const
  {
    if (width != s.width || height != s.height)
      return false;
    Rect pieces1[width][height];
    Rect pieces2[width][height];
    int x1[rect.size()];
    int y1[rect.size()];
    for (int i = 0; i < rect.size(); ++i) {
      pieces1[rect[i].x][rect[i].y] = rect[i];
      x1[i] = rect[i].x;
      y1[i] = rect[i].y;
    }
    for (int i = 0; i < s.rect.size(); ++i)
      pieces2[s.rect[i].x][s.rect[i].y] = s.rect[i];
    for (int i = 0; i < rect.size(); ++i)
      if (pieces1[x1[i]][y1[i]] != pieces2[x1[i]][y1[i]])
	return false;
    return true;
  }

  void greedy()
  {    
    sort(rect.begin(), rect.end());

    width = height = 0;

    vector<Node> nodes;
    nodes.push_back(Node(0, 0));
    
    for (int i = 0; i < rect.size(); ++i) {
      int best_width, best_height;
      int best_area = INT_MAX;
      int best_node;
      bool reverse = false;
      for (int node = 0; node < nodes.size(); ++node) {
	rect[i].place(nodes[node]);
	bool valid_node = true;
	for (int j = 0; j < i; ++j)
	  if (rect[i].intersects(rect[j])) { 
	    valid_node = false;
	    break;
	  }
	if (valid_node) {
	  int new_width = max(width, rect[i].x + rect[i].width);
	  int new_height = max(height, rect[i].y + rect[i].height);
	  int new_area = new_width * new_height;
	  if (new_area < best_area) {
	    best_node = node;
	    best_width = new_width;
	    best_height = new_height;
	    best_area = new_area;
	    reverse = false;
	  }
	}
	rect[i].reverse();
	valid_node = true;
	for (int j = 0; j < i; ++j)
	  if (rect[i].intersects(rect[j])) { 
	    valid_node = false;
	    break;
	  }
	if (valid_node) {
	  int new_width = max(width, rect[i].x + rect[i].width);
	  int new_height = max(height, rect[i].y + rect[i].height);
	  int new_area = new_width * new_height;
	  if (new_area < best_area) {
	    best_node = node;
	    best_width = new_width;
	    best_height = new_height;
	    best_area = new_area;
	    reverse = true;
	  }
	}
	rect[i].reverse();
      }
      if (reverse)
	rect[i].reverse();
      rect[i].place(nodes[best_node]);
      nodes.push_back(nodes[best_node] + Node(rect[i].width, 0));
      nodes.push_back(nodes[best_node] + Node(0, rect[i].height));
      nodes.push_back(nodes[best_node] + Node(rect[i].width, 
                                              rect[i].height));
      nodes.erase(nodes.begin() + best_node);
      width = best_width;
      height = best_height;
      area = best_area;
    }
  }
  
  void btrec(Solution& sol, vector<Node>& nodes, bool *used, int cnt)
  {
    if (cnt == sol.rect.size()) {
      if (btsol.size() == 0)
	  btsol.push_back(sol);
      else if (sol.area < btsol[0].area) {
	btsol.clear();
	btsol.push_back(sol);
      }
      else if (sol.area == btsol[0].area && 
               find(btsol.begin(), btsol.end(), sol) == btsol.end())
	btsol.push_back(sol);
    }
    else {
      for (int i = 0; i < sol.rect.size(); ++i)
	if (!used[i]) {
	  used[i] = true;

	  for (int node = 0; node < nodes.size(); ++node) {
	    sol.rect[i].place(nodes[node]);

	    bool valid_node = true;
	    for (int j = 0; j < sol.rect.size(); ++j)
	      if (j != i && used[j] && 
                  sol.rect[i].intersects(sol.rect[j])) { 
		valid_node = false;
		break;
	      }
	    if (valid_node) {
	      Node cur_node(nodes[node]);
	      nodes.erase(nodes.begin() + node);
	      Node new_nodes[3];
	      new_nodes[0] = cur_node + Node(sol.rect[i].width, 0);
	      new_nodes[1] = cur_node + Node(0, sol.rect[i].height);
	      new_nodes[2] = cur_node + Node(sol.rect[i].width, 
                                             sol.rect[i].height);
	      nodes.push_back(new_nodes[0]);
	      nodes.push_back(new_nodes[1]);
	      nodes.push_back(new_nodes[2]);
	      int old_width = sol.width;
	      int old_height = sol.height;
	      int old_area = sol.area;
	      sol.width = max(sol.width, sol.rect[i].x + sol.rect[i].width);
	      sol.height = max(sol.height, 
                               sol.rect[i].y + sol.rect[i].height);
	      sol.area = sol.width * sol.height;

	      if (btsol.size() == 0 || sol.area <= btsol[0].area)
		btrec(sol, nodes, used, cnt + 1);
	      
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[0]));
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[1]));
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[2]));
	      nodes.insert(nodes.begin(), cur_node);
	      sol.width = old_width;
	      sol.height = old_height;
	      sol.area = old_area;
	    }	    
	    sol.rect[i].reverse();

	    valid_node = true;
	    for (int j = 0; j < sol.rect.size(); ++j)
	      if (j != i && used[j] && 
                  sol.rect[i].intersects(sol.rect[j])) { 
		valid_node = false;
		break;
	      }
	    if (valid_node) {
	      Node cur_node(nodes[node]);
	      nodes.erase(nodes.begin() + node);
	      Node new_nodes[3];
	      new_nodes[0] = cur_node + Node(sol.rect[i].width, 0);
	      new_nodes[1] = cur_node + Node(0, sol.rect[i].height);
	      new_nodes[2] = cur_node + Node(sol.rect[i].width, 
                                             sol.rect[i].height);
	      nodes.push_back(new_nodes[0]);
	      nodes.push_back(new_nodes[1]);
	      nodes.push_back(new_nodes[2]);
	      int old_width = sol.width;
	      int old_height = sol.height;
	      int old_area = sol.area;
	      sol.width = max(sol.width, sol.rect[i].x + sol.rect[i].width);
	      sol.height = max(sol.height, 
                               sol.rect[i].y + sol.rect[i].height);
	      sol.area = sol.width * sol.height;
	      
	      if (btsol.size() == 0 || sol.area <= btsol[0].area)
		btrec(sol, nodes, used, cnt + 1);
	      
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[0]));
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[1]));
	      nodes.erase(find(nodes.begin(), nodes.end(), new_nodes[2]));
	      nodes.insert(nodes.begin(), cur_node);
	      sol.width = old_width;
	      sol.height = old_height;
	      sol.area = old_area;
	    }	    
	    sol.rect[i].reverse();	    
	  }
	  used[i] = false;
	}
    }
  }
  
  void backtrack()
  {
    bt = true;
   
    Solution sol(rect);
    bool used[rect.size()];
    for (int i = 0; i < rect.size(); ++i)
      used[i] = false;
    vector<Node> nodes;
    nodes.push_back(Node(0, 0));
    btrec(sol, nodes, used, 0);
    area = btsol[0].area;
  }

  void draw()
  {
    if (!bt) {
      int sol[width][height];
      for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y)
	  sol[x][y] = Empty;
      for (int i = 0; i < rect.size(); ++i) {
	int x, y, w, h;
	x = rect[i].x;
	y = rect[i].y;
	w = rect[i].width;
	h = rect[i].height;
	sol[x][y] = sol[x + w - 1][y] = sol[x][y + h - 1] = 
          sol[x + w - 1][y + h - 1] = Corner;
	for (int p = x + 1; p < x + w - 1; ++p)
	  sol[p][y] = sol[p][y + h - 1] = Hedge;
	for (int p = y + 1; p < y + h - 1; ++p)
	  sol[x][p] = sol[x + w - 1][p] = Vedge;
	for (int p = x + 1; p < x + w - 1; ++p)
	  for (int q = y + 1; q < y + h - 1; ++q)
	    sol[p][q] = Empty;
      }
    
      cerr << width << "x" << height << ":" << endl;
      for (int y = 0; y < height; ++y) {
	for (int x = 0; x < width; ++x)
	  cerr << sym[sol[x][y]] << " ";
	cerr << endl;
      }
      cerr << endl;
    }
    else
      for (int i = 0; i < btsol.size(); ++i)
	btsol[i].draw();
  }
    
  double ratio() // for testing purposes only
  {
    double sum = 0;
    for (int i = 0; i < rect.size(); ++i)
      sum += rect[i].area;
    return sum / area; 
  }

};

int main()
{
  vector<Rect> rect;
  int n;

  cin >> n;
  for (int i = 0; i < n; ++i) {
    int w, h;
    cin >> w >> h;
    rect.push_back(Rect(w, h));
  }

  Solution sol(rect);
  
  if (n >= GREEDY_THRESHOLD)
    sol.greedy();
  else
    sol.backtrack();

  cout << sol.area << endl;
  sol.draw();
  
  return 0;
}
