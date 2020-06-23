#include "Grid.h"

Grid::Grid(int width_, int height_)
:width(width_), height(height_){
    grid.resize(width*height);
}

Grid::~Grid(){
    
}

int Grid::getWidth() const{
    return width;
}

int Grid::getHeight() const{
    return height;
}

Grid::Tile Grid::getTile(int x, int y) const{
    int i = x+y*width;
    if(i>=0 && i<width*height){
        return grid[x+y*width].tile;
    }
}

void Grid::setTile(int x, int y, Grid::Tile tile){
    int i = x+y*width;
    if(i>=0 && i<width*height){
        grid[i].tile=tile;
    }
}

// Return true iff object with a given size can reside on (x1, y1)
// and move from there to (x2, y2) while staying at the same tile
// type without colliding
//
// This should execute faster than calling findShortestPath()
//
// Also, if the map hasn't changed, subsequent calls with the same
// x1,y1,x2,y2 coordinates SHOULD BE MUCH FASTER. Hint: flood fill + caching
bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const{
    //first check if the start point and destination have the same tile type
    Grid::Tile tile = getTile(x1,y1);
    for(int i=x1;i<=x1+size;i++){
        for(int j=y1;j<=y1+size;j++){
            if(getTile(i,j)!=tile){
                return false;
            }
        }
    }
    for(int i=x2;i<=x2+size;i++){
        for(int j=y2;j<=y2+size;j++){
            if(getTile(i,j)!=tile){
                return false;
            }
        }
    }
    //then use flood fill to check if there is a path between
    set<pair<int,int>> s;
    Flood_fill(size,x1,y1,tile,s);

    if(s.find(make_pair(x2, y2)) == s.end()){
        return false;
    }

    return true;
}

// Compute a shortest path from (x1, y1) to (x2, y2) for an object with a
// given size using A*. Store the shortest path into path, and return the
// cost of that path (using CARDINAL_COST for the moves N, E, S, and W, and
// DIAGONAL_COST for the moves NE, SE, SW, and NW) or -1 if there is no
// path. Reduce initialization time by using the generation counter trick
int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2,
                     std::vector<Direction> &path) const{
    vector<Node*> openSet;
    vector<Node*> world;
    int rh = distance(x1,y1,x2,y2);
    Tile tile = grid[x1+y1*width].tile;
    Node *root = new Node(x1,y1,rh);
    root->g=0;
    root->f=root->h;
    openSet.push_back(root);
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
	    int h = distance(j,i,x2,y2);
            Node *n = new Node(j,i,h);
            world.push_back(n);
        }
    }
    world[x1+y1*width] = root;
    while(!openSet.empty()){
        sort(begin(openSet),end(openSet),comp());
        Node *current = openSet[0];
	//base case
        if(current->x == x2 && current->y == y2){
            int res = current->f;
            while(current!=root){
                path.push_back(current->dir);
                current = current->previous;
            }
            path.push_back(current->dir);
	    reverse(begin(path), end(path));
            return res;
        }
        openSet.erase(openSet.begin());
        int tentative_gScore1 = current->g + CARDINAL_COST;
        int tentative_gScore2 = current->g + DIAGONAL_COST;
        Direction dir1 = N, dir2 = NE, dir3 = E, dir4 = SE, dir5 = S, dir6 = SW, dir7 = W, dir8 = NW;
	bool l=false,r=false,u=false,d=false;
	//move east
	int inx = current->x+current->y*width+1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && can_fill(size, current->x+1, current->y)){
	    r=true;
	    Node *n = world[inx];
            if(tentative_gScore1<n->g){
                n->previous = current;
	    	n->g = tentative_gScore1;
	    	n->f = n->g + n->h;
	    	n->dir = dir3;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move west
	inx = current->x+current->y*width-1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && can_fill(size, current->x-1, current->y)){
	    l=true;
	    Node *n = world[inx];
            if(tentative_gScore1<n->g){
                n->previous = current;
	    	n->g = tentative_gScore1;
	    	n->f = n->g + n->h;
	    	n->dir = dir7;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move south
	inx = current->x+(current->y+1)*width;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && can_fill(size, current->x, current->y+1)){
	    d=true;
	    Node *n = world[inx];
            if(tentative_gScore1<n->g){
                n->previous = current;
	    	n->g = tentative_gScore1;
	    	n->f = n->g + n->h;
	    	n->dir = dir5;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move north
	inx = current->x+(current->y-1)*width;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && can_fill(size, current->x, current->y-1)){
	    u=true;
	    Node *n = world[inx];
            if(tentative_gScore1<n->g){
                n->previous = current;
	    	n->g = tentative_gScore1;
	    	n->f = n->g + n->h;
	    	n->dir = dir1;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move south east
	inx = current->x+(current->y+1)*width+1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && r && d && can_fill(size, current->x+1, current->y+1)){
	    Node *n = world[inx];
            if(tentative_gScore2<n->g){
                n->previous = current;
	    	n->g = tentative_gScore2;
	    	n->f = n->g + n->h;
	    	n->dir = dir4;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move south west
	inx = current->x+(current->y+1)*width-1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && l && d && can_fill(size, current->x-1, current->y+1)){
	    Node *n = world[inx];
            if(tentative_gScore2<n->g){
                n->previous = current;
	    	n->g = tentative_gScore2;
	    	n->f = n->g + n->h;
	    	n->dir = dir6;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move north west
	inx = current->x+(current->y-1)*width-1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && l && u && can_fill(size, current->x-1, current->y-1)){
	    Node *n = world[inx];
            if(tentative_gScore2<n->g){
                n->previous = current;
	    	n->g = tentative_gScore2;
	    	n->f = n->g + n->h;
	    	n->dir = dir8;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
	//move north east
	inx = current->x+(current->y-1)*width+1;
	if(inx >= 0 && inx < width*height && grid[inx].tile==tile && r && u && can_fill(size, current->x+1, current->y-1)){
	    Node *n = world[inx];
            if(tentative_gScore2<n->g){
                n->previous = current;
	    	n->g = tentative_gScore2;
	    	n->f = n->g + n->h;
	    	n->dir = dir2;
		world[inx] = n;
	    	if(find(begin(openSet),end(openSet),n) == openSet.end()){
		    openSet.push_back(n);
	   	}
       	    }
        }
        
    }
    return -1;
}
