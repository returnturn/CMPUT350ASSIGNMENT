// list private Grid members here

// ...

int width, height;

struct Node {
    int x,y,h,f,g;
    Node *previous;
    Direction dir;
    //constructor
    Node(int x_, int y_, int h_)
    : x(x_),y(y_),h(h_),f(1e9),g(1e9){
        
    }
};
//compare f value for sorting
struct comp {
    bool operator()(const Node *a, const Node *b){
        return a->f < b->f;
    }
};

struct Tiles {
    Tile tile;
};
//stores tiles type
vector<Tiles> grid;
//check if can fill the area
bool can_fill(int size, int x, int y) const{
    if(x<0 || x+size>width || y<0 || y+size>height){
        return false;
    }
    Tile t = grid[x+y*width].tile;
    for(int i=x;i<=x+size;i++){
        for(int j=y;j<=y+size;j++){
            if(grid[i+j*width].tile!=t){
                return false;
            }
        }
    }
    return true;
}
//flood fill function
void Flood_fill(int size, int x, int y, Tile tile, set<pair<int, int>> &s) const{
    if(x<0 || x+size>width || y<0 || y+size>height || !can_fill(size,x,y) || grid[x+y*width].tile!=tile){
        return;
    }
    if(s.find(make_pair(x, y)) != s.end()){
        return;
    }
    else{
        s.insert(make_pair(x,y));
    }
    Flood_fill(size, x, y+1, tile, s);
    Flood_fill(size, x, y-1, tile, s);
    Flood_fill(size, x-1, y, tile, s);
    Flood_fill(size, x+1, y, tile, s);
}
//calculate 2 points distance
int distance(int x1, int y1, int x2, int y2) const{
    return ceil(sqrt(pow(x1-x2,2)+pow(y1-y2,2))*CARDINAL_COST);
}
