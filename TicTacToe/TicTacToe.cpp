#include <bits/stdc++.h>
using namespace std;

class Symbol{
private:
    char mark;
public:
    Symbol(char m){
        this->mark=m;
    }
    char getMark(){
        return mark;
    }
};

class Board{
private:
    vector<vector<Symbol*>> grid;
    int size;
    Symbol* empty;
public:
    Board(int s,Symbol* e){
        this->size=s;
        this->empty=e;
        grid.resize(size);
        for(int i=0;i<size;i++){
            grid[i].resize(size,empty);
        }
    }
    ~Board(){
        delete empty;
         for (auto& row : grid) {
            for (auto ptr : row) {
                delete ptr; 
            }
        }
        grid.clear();
    }
    bool isCellEmpty(int r,int c){
        return grid[r][c]==empty;
    }
    Symbol* getCell(int r,int c){
        return grid[r][c];
    }
    void markCell(int r,int c,Symbol* s){
        grid[r][c]=s;
    }
    int getSize(){
        return size;
    }
    void display(){
        for(int j=0;j<size+1;j++){
            cout<<"___";
        }
        cout<<endl;
        for(int i=0;i<size;i++){
            cout<<"|";
            for(int j=0;j<size;j++){
                cout<<grid[i][j]->getMark()<<"|";
            }
            cout<<endl;
            for(int j=0;j<size+1;j++){
                cout<<"__";
            }
            cout<<endl;
        }
    }
};
class Rule{
    public:
    virtual bool checkWin(Board* b,Symbol *s)=0;
    virtual bool checkDraw(Board* b)=0;
    virtual bool isValidMove(Board* b,int r,int c)=0;
};
class StandardRule:public Rule{
    public:
    bool checkWin(Board* b,Symbol *s) override {
        int n=b->getSize();
        for(int i=0;i<n;i++){
            int count=0;
            for(int j=0;j<n;j++){
                if(b->getCell(i,j)==s)count++;
            }
            if(count==n)return true;
        }
        for(int i=0;i<n;i++){
            int count=0;
            for(int j=0;j<n;j++){
                if(b->getCell(j,i)==s)count++;
            }
            if(count==n)return true;
        }
        int count=0;
        for(int i=0;i<n;i++){
            if(b->getCell(i,i)==s)count++;
        }
        if(count==n)return true;
        count=0;
        for(int i=0;i<n;i++){
            if(b->getCell(i,n-1-i)==s)count++;
        }
        if(count==n)return true;
        return false;
        
    }
    bool checkDraw(Board *b) override {
        for(int i=0;i<b->getSize();i++){
            for(int j=0;j<b->getSize();j++){
                if(b->isCellEmpty(i,j))return false;
            }
        }
        return true;
    }
    bool isValidMove(Board* b,int r,int c){
        if( r<0 || r>=b->getSize() || c<0 || c>=b->getSize())return false;
        return b->isCellEmpty(r,c);
    }
};

class Player{
private:
    int id;
    string name;
    Symbol* symbol;
public:
    static int idCounter;
    Player(string n,char mark){
        id=idCounter++;
        name=n;
        symbol=new Symbol(mark);
    }
    ~Player(){
        delete symbol;
    }
    Symbol* getSymbol(){
        return symbol;
    }
    string getName(){
        return name;
    }
};
int Player::idCounter=0;
class IObserver{
public:
    virtual void update(string msg)=0;
};
class ConsoleList: public IObserver{
    void update(string msg) override{
        cout<<msg<<endl;
    }
};

class Game{
private:
    Board* board;
    Rule* rule;
    deque<Player*> players;
    vector<IObserver*>Observerlist;
    bool gameOver;
public:
    Game(Rule* r){
        board=new Board(3,new Symbol('*'));
        rule=r;
    }
    void notify(string msg){
        for(int i=0;i<Observerlist.size();i++){
            Observerlist[i]->update(msg);
        }
    }
    void addPlayer(Player* p){
        players.push_back(p);
    }
    void addObserver(IObserver* o){
        Observerlist.push_back(o);
    }
    void play(){
        while(!rule->checkDraw(board)){
            Player* currPlayer=players.front();
            players.pop_front();
            int x,y;
            do{
                cout<<currPlayer->getName()<<"Enter x and y "<<endl;
                cin>>x>>y;
            }while(!rule->isValidMove(board,x,y));
            board->markCell(x,y,currPlayer->getSymbol());
            notify(currPlayer->getName()+" marked cell "+to_string(x)+", "+to_string(y)+" with "+currPlayer->getSymbol()->getMark());
            board->display();
            if(rule->checkWin(board,currPlayer->getSymbol())){
                notify(currPlayer->getName()+" wins!!!");
                return;
            }
            players.push_back(currPlayer);
        }
        notify("Game draw!!");
    }
};

enum class GameType{
    STANDARD
};

class GameFactory{
public:
    static Game* createGame(GameType gt){
        Rule* r;
        if(gt==GameType::STANDARD){
            r= new StandardRule();
        }
        return new Game(r);
    }
};

int main()
{
    Player* a=new Player("Yash",'X');
    Player* b=new Player("Sanskar",'O');
    
    Game * g=GameFactory:: createGame(GameType::STANDARD);
    g->addPlayer(a);
    g->addPlayer(b);
    g->addObserver(new ConsoleList());
    g->play();
    
}