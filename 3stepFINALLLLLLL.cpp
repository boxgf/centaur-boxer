#include<iostream>
#include<stdlib.h>
#define wR 5
#define nodeReward 5
#define botReward 0
#define pelleteReward -1
#define nothingReward 0
#define costOfForward 200
#define costOfRight 100
#define costOfOverTurn 199
#define scoreOf0 float(costOfForward)/5.0
#define scoreOf1 float(costOfForward)/5.0
#define scoreOf2 float(costOfForward)/11.0
#define scoreOf3 float(costOfForward)/13.0
#define scoreOf4 float(costOfForward)/600.0
#define xSize 4
#define ySize 4
#define maxdepth 4
#define discount 0.8
#define bonus 100*costOfForward
//use destructoors and static vars to find memory use
//make actiontogetHere only to the solution hash remove action to gethere
//botDirection not req
//cost=0 for starting state
//make closed set more good
//initial state make bot take 2 pelletes
//use destructoors and static vars to find memory use
//make actiontogetHere only to the solution hash remove action to gethere
//botDirection not req
//cost=0 for starting state
//make closed set more good
//initial state make bot take 2 pelletes
using namespace std;
double h=0,c=0,avg=0;
int count2=0,cou=0;


class Position
{
      public:
      char P[2];
      bool operator==(Position _2nDoperand)//remove
      {
      if((P[0]== _2nDoperand.P[0])&&(P[1]== _2nDoperand.P[1]))
      return 1;
      return 0;
      }
      
      void display()//remove;
      {
      cout<<P[0]<<" "<<P[1]<<endl;
      }
      Position south()
      {
      Position ret;
          ret=*this ;
         ret.P[1]--;
         return ret;
   
      }
      Position north()
      {
           
        Position ret;
          ret=*this; 
         ret.P[1]++;
         return ret;
   
      }
      Position east()
     {
           
         Position ret;
          ret=*this ;
         ret.P[0]++;
         return ret;
   
      }
      Position west()
     
      {
         Position ret;
          ret=*this ;
         ret.P[0]--;
         return ret;
   
      }
Position(int x,int y)
   {
          P[0]=x;
          P[1]=y;
   }
Position()
{
          P[0]='\0';
          P[1]='\0';
}
};

class Dir
{
      public:
      static int flag;
      char Direc[4];
      void display()//remove
      {
           for (int i=0;i<4;i++)
           cout<<Direc[i]<<" ";
           cout<<endl;
      }
      bool isEmpty()
      {
      for (int i=0;i<4;i++)
      if(Direc[i]!='0')
      return 0;
      return 1;
      
      }
      void push(char in)
      {
      for (int i=0;i<4;i++)
      if(Direc[i]=='0')
      {
      Direc[i]=in;
      return;
      }
      }
      int random()
      {
      return Direc[0];
      }
      Dir()
      {
      for(int i=0;i<4;i++)
      Direc[i]='0';
      }

};

int index(char ch)
{
    switch(ch)
    {
              case 'n':
              return 0;
              case 'e':
              return 1;
              case 's':
              return 2;
              case 'w':
              return 3;
              case 'N':
              return 0;
              case 'E':
              return 1;
              case 'S':
              return 2;
              case 'W':
              return 3;
    }
}


class State
{
public:
 
    char Bot_State[ySize][xSize];
    int cost;
    Position botPos;                        //need not have                                        
    char botDirection;                      //need not have this, can compute
    //char actionToGetHere;
    char operator>>(char dest)
      {
          int ind;
          ind = index(botDirection)-index(dest);
          if(ind == 1|| ind==-3)
          return 'L';
          if(ind ==-1 || ind==3)
          return 'R';
          if(ind == -2 || ind == 2)
          return 'O';
          if(!ind)
          return 'F';   
    }
   bool operator==(State *tobeChecked)  
   {
        for(int i=0;i<ySize;i++)
        for(int j=0;j<xSize;j++)
        if (Bot_State[i][j] != tobeChecked->Bot_State[i][j])
        return 0;
        return 1;
   }
   template <class type>
   type abs(type in)
   {
        if(in<0)
        return -in;
        return in;
   }
   int nearestPelleteDistance()
   {
   int min= 9999,temp;
   for(int i=0;i<ySize;i++)
   for(int j=0;j<xSize;j++)
   if(Bot_State[i][j] =='.'||Bot_State[i][j] =='2'||Bot_State[i][j] =='3'||Bot_State[i][j] =='4')
   {
   temp=abs(botPos.P[0]-j)+abs(botPos.P[1]-i);
   if(temp<min)
   min=temp;
   }
   if (min==9999)
   return 1;
   return min;
   }
    void takeForward()
    {
    
         Bot_State[botPos.P[1]][botPos.P[0]]='0';
         switch(botDirection)
         {
         case 's':
         botPos.P[1]--;
         break;
         case 'w':
         botPos.P[0]--;
         break;
         case 'n':
         botPos.P[1]++;
         break;
         case 'e':
         botPos.P[0]++;
         break;
         }
         Bot_State[botPos.P[1]][botPos.P[0]]=botDirection;     
    }
  
    bool isnorth()
    {
    if(botPos.P[1]<ySize-1)
    if(Bot_State[botPos.P[1]+1][botPos.P[0]]!='X')
    return 1;
    return 0;
    }
    bool iseast()
    {
    if(botPos.P[0]<xSize-1)
    if(Bot_State[botPos.P[1]][botPos.P[0]+1]!='X')
    return 1;
    return 0;
    }
    bool iswest()
    {
    if(botPos.P[0]>0)
    if(Bot_State[botPos.P[1]][botPos.P[0]-1]!='X')
    return 1;
    return 0;
    }
    bool issouth()
    {
    if(botPos.P[1]>0)
    if(Bot_State[botPos.P[1]-1][botPos.P[0]]!='X')
    return 1;
    return 0;
    }
    bool isValid(Position p)
    {
    if(p.P[0]<xSize&&p.P[1]<ySize&&p.P[0]>=0&&p.P[1]>=0)
    return 1;
    return 0;
    }
  
      bool isPelletes()
    {
    for(int i=0;i<ySize;i++)
    for(int j=0;j<xSize;j++)
    if (Bot_State[i][j]=='.'||Bot_State[i][j]=='2'||Bot_State[i][j]=='3')
    return 1;
    return 0;
    }

     void display()
    {
    for(int i=ySize-1;i>=0;i--)
    {
    cout<<endl;
    for(int j=0;j<xSize;j++)
    cout<<Bot_State[i][j]<<" ";
    }
    cout<<endl;
    }
    float evalFn()
    {
    float score=0;
    for(int i=0;i<ySize;i++)
    for(int j=0;j<xSize;j++)
    switch (Bot_State[i][j])
    {
    case 'X':
         break;
    case '0':
       score+=scoreOf0;
       break; 
    case 'n':
       score+=scoreOf0;
    break;
    case 'w':
       score+=scoreOf0;
       break;
    case 'e':
       score+=scoreOf0;
       break;
    case 's':
       score+=scoreOf0;
       break;
    case '1':
       score+=scoreOf1;
       break; 
    case '2':
       score+=scoreOf2;
       break; 
    case '3':
       score+=scoreOf3;
       break; 
    case '4':
       score+=scoreOf4;
       break;  
    default:
       score+=20/((Bot_State[i][j]-48)*costOfForward*3);
       break;
       }
    ////cout<<score-cost;
    return (score-float(cost)/10.0-nearestPelleteDistance());
    }
    State()
    {
    botDirection='0';

    cost=0;
  
    for(int i=0;i<ySize;i++)
    for(int j=0;j<xSize;j++)
    Bot_State[i][j]='0';
    }
  

};//remove obj


template<class type>
class Q
{
public:
type dat;
Q *link;
Q()
{
link='\0';
}
};

template<class type>
class Queue
{
public:
  Q<type> *first,*last,*temp;
  

 void push(type data) 
      {
	   temp=(class Q<type>*)malloc(sizeof(Q<type>));
		*temp=Q<type>();
      
        temp->dat = data;
        if(first!='\0')
        {
        last->link=temp;
        last=temp;
        }
      
        else
        {
            last=temp;
            first=temp;  
        }
        }  
type pop()
        {
          if(first=='\0')
          {
          ////cout<<"overpoppedqueue";
		   temp=(class Q<type>*)malloc(sizeof(Q<type>));
			*temp=Q<type>();
		
          return temp->dat; 
          }
          temp=first;
          first=first-> link;
          temp->link='\0';
          return temp->dat;    
        }

bool search(type *Pos)
{
     for(temp=first;temp!='\0';temp=temp->link)
     if ((*Pos)== &(temp->dat))
     return 1;
     return 0;
}
void display()//remove;
{
     if(first=='\0')
     {
     ////cout<<"Empty Queue";
     return;
     }
     for(temp=first;temp!='\0';temp=temp->link)
     temp->dat.display();   
}
int count()
{
     if(first=='\0')
     {
  
     return 0;
     }
     int count;
     for(count=0,temp=first;temp!='\0';temp=temp->link,count++);
     return count;
}
bool isThere()
{
      if(first=='\0')
      return 0;
      return 1;
}
Queue()
{
       first='\0';
       last='\0';
       temp='\0';
}  
};

Queue<State> randomClosedSet;
Dir getLegalDirections(State *givenState)
{
Dir retDirections;//make dir class a linked lists
int count=0;
if(givenState->iseast())
retDirections.Direc[count++]='E';
if(givenState->isnorth())
retDirections.Direc[count++]='N';
if(givenState->iswest())
retDirections.Direc[count++]='W';
if(givenState->issouth())
retDirections.Direc[count]='S';
return retDirections;
}
int cost(char in)
{
switch(in)
{
           case 'F':
                return costOfForward;
           case 'R':
                return costOfRight+costOfForward;
           case 'L':
                return costOfRight+costOfForward;
            case 'O':
                return costOfOverTurn+costOfForward;
                
}
}
State generateSuccessorState(State *givenState, char direction)
{
State newState;
newState=*givenState;

char action = (*givenState)>>direction;//remove action can cal at end

newState.Bot_State[newState.botPos.P[1]][newState.botPos.P[0]] = direction+32;
newState.botDirection=direction+32;
{
newState.takeForward();
newState.cost = givenState->cost+cost(action);
Position i;
char j;
switch(newState.botDirection)
{
  case 's':  
  for(i = newState.botPos.south() ,j='1'; newState.isValid(i);i=i.south(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
   for(i = newState.botPos.east(),j='1'; newState.isValid(i);i=i.east(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
   for(i = newState.botPos.west(),j='1'; newState.isValid(i);i=i.west(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
   

  break;
  case 'w':  
 for(i = newState.botPos.west(),j='1'; newState.isValid(i);i=i.west(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.north(),j='1'; newState.isValid(i);i=i.north(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.south(),j='1'; newState.isValid(i);i=i.south(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  break;  
  case 'e':  
  for(i = newState.botPos.east(),j='1'; newState.isValid(i);i=i.east(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.south(),j='1'; newState.isValid(i);i=i.south(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.north(),j='1'; newState.isValid(i);i=i.north(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  break;  
  case 'n':  
  for(i = newState.botPos.north(),j='1'; newState.isValid(i);i=i.north(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.east(),j='1'; newState.isValid(i);i=i.east(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  for(i = newState.botPos.west(),j='1'; newState.isValid(i);i=i.west(),j++)
  if(((newState.Bot_State[i.P[1]][i.P[0]]<='4'&& newState.Bot_State[i.P[1]][i.P[0]]>j)||newState.Bot_State[i.P[1]][i.P[0]]=='.')&&newState.Bot_State[i.P[1]][i.P[0]]!='X')
  newState.Bot_State[i.P[1]][i.P[0]]=j;
  break;                       
}
}
  

return newState;
}
bool isNotGoalState(State checkState)
{
if(checkState.isPelletes())
return 1;
return 0;
}

Queue<Position> makeChildrenOfPos(Position *coordinates,State *inState)
{
Queue<Position> childQ;
Position childs;
 int flag=0;
 if(coordinates->P[0]<xSize-1)
 if(!(inState->Bot_State[coordinates->P[1]][coordinates->P[0]+1]=='X'))
 {
 childs.P[0]=coordinates->P[0]+1;
 childs.P[1]=coordinates->P[1];
 childQ.push(childs);
 } 
 if(coordinates->P[1]<ySize-1)
 if(!(inState->Bot_State[coordinates->P[1]+1][coordinates->P[0]]=='X'))
 {
 childs.P[0]=coordinates->P[0];
 childs.P[1]=coordinates->P[1]+1;
 childQ.push(childs);
 } 
 if(coordinates->P[0]>0)
 if(!(inState->Bot_State[coordinates->P[1]][coordinates->P[0]-1]=='X'))
 {
 childs.P[0]=coordinates->P[0]-1;
 childs.P[1]=coordinates->P[1];
 childQ.push(childs);
 } 
 if(coordinates->P[1]>0)
 if(!(inState->Bot_State[coordinates->P[1]-1][coordinates->P[0]]=='X'))
 {
 childs.P[0]=coordinates->P[0];
 childs.P[1]=coordinates->P[1]-1;
 childQ.push(childs);
 } 


return childQ;                      
}


void fillPellets(State *inputState)
{
Position currentPos,posToPush,poppedChild;

Queue<Position> orderQ,child;
int i,n;//remove n;
//set clasedSet;
int Nodes=0;
Dir actions;
currentPos = inputState->botPos;
orderQ.push(currentPos);
inputState->Bot_State[currentPos.P[1]][currentPos.P[0]]='.';
while(orderQ.isThere())
{
currentPos=orderQ.pop();
Nodes++;
inputState->Bot_State[currentPos.P[1]][currentPos.P[0]]=='.';
child=makeChildrenOfPos(&currentPos,inputState);
while(child.isThere())
{
poppedChild=child.pop();
if((inputState->Bot_State[poppedChild.P[1]][poppedChild.P[0]])!='.')
{
inputState->Bot_State[poppedChild.P[1]][poppedChild.P[0]]='.';
orderQ.push(poppedChild);
}
}
}
}
template <class type>
type maxt(type in1, type in2)
{
if(in1==-989898989)
return in2;
if(in2==-989898989)
return in1;
if(in1>in2)
return in1;
return in2;
}

float maxiMax(State &instate,int depth=0)//stop thrashing with eval and set2
{


if(depth>= maxdepth)
return instate.evalFn();
else if(!isNotGoalState(instate))
return instate.evalFn()+bonus;

float val=-989898989;
Dir legaldirs;
State successor;
legaldirs = getLegalDirections(&instate);

if(depth==0)
{
depth++;
float tempval;
Dir bestdir;


for(int i=0; (i<4)&&(legaldirs.Direc[i]!='0');i++)
{
        successor=generateSuccessorState(&instate,legaldirs.Direc[i]);
       
        {
        tempval=maxt(maxiMax(successor,depth),val);

        if(val!=tempval)
        {
        bestdir=Dir();
        bestdir.push(legaldirs.Direc[i]);

        }
        val=tempval; 
        }  
}
if(bestdir.isEmpty())
return (legaldirs.random());
 //bestdir.display();
 return(bestdir.random());
}
depth++;
for(int i=0; (i<4)&&(legaldirs.Direc[i]!='0');i++)
{
        successor=generateSuccessorState(&instate,legaldirs.Direc[i]);
      //if(!closed.search(&successor))
      val=maxt(maxiMax(successor,depth),val);    
}
return discount*(val)+instate.evalFn(); 
}


void generateRandom(State *incoming)
{
     State temp;
     temp=*incoming;
     temp.Bot_State[0][0]='n';
     while(randomClosedSet.search(&temp))
     {
     temp=*incoming;                          
     for(int i=0;i<ySize;i++)
     for(int j=0;j<xSize;j++)
     {
     temp.Bot_State[i][j]='X';
     if(rand()%5)
     temp.Bot_State[i][j]='-';
     }
     temp.Bot_State[0][0]='n';
     temp.Bot_State[1][0]='-';
     
     }
     randomClosedSet.push(temp);
     *incoming=temp;
     
}
main()
{

    
State Maze,*p;
int pc;
State current;
char bestDirection,bestAction,co;


for(int i=0;i<ySize;i++)//change this check if node is there                                                               //change this check if node is there
for(int j=0;j<xSize;j++)
Maze.Bot_State[i][j]='-';
Maze.botPos=Position(0,0);

//NODES HERE...
Maze.Bot_State[3][0]='X';
Maze.Bot_State[2][1]='X';
Maze.Bot_State[1][2]='X';






fillPellets(&Maze);
Maze.botPos=Position(0,-1);
Maze.botDirection='n';

Maze=generateSuccessorState(&Maze,'N');
current=Maze;
current.display();
while(isNotGoalState(current))
{
bestDirection=maxiMax(current,0);
bestAction=current>>bestDirection;
current=generateSuccessorState(&current,bestDirection);
current.cost=0;
cout<<bestAction<<" ";
}
cout<< "\n-------------------------   :"<< count2<<"   ";
//current.display();


}