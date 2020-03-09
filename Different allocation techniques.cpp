#include <iostream>
#include <queue>
#include <string>

using namespace std;

//class for processes
class process{
public:
        //Constructors
        process(int i){last=this;}//At the begining(when declaring the object in main ) the last process is the first one
        process(){}//used when dynamically allocated objects (when using new)
        //getters
        int get_size(){return size;}
        process *get_next(){return next;}
        process *get_last(){return last;}
        string get_name(){return pName;}
        //setters
        void set_size(int i ){size = i;}
        void set_name(string i ){pName = i;}
        void set_next(process *i){next = i;}
        void set_last(process *i){last = i;}
        //finctions 
        void add(int i,int j);//add a memory block
private:
        int size=-1;//when = -1 means that there is only one block
        string pName="  ";//used When Printing the memory layout
        process *next=nullptr;//Next Process
        process *last=nullptr;//Last Process
        
};

void process :: add(int i ,int j){
        // if the last item is the first one 
        if (last == this && this->get_size()==-1){last->size=i;last->pName="p"+std::to_string(j);}
        else {
            //create new item using the second constructor,then set its parameters,then its the last item
            process *nPro=new process();
            nPro->size=i;
            nPro->set_name("p"+std::to_string(j));
            last->set_next(nPro);
            last=nPro;
            
        }
    
}

//used when there is no free memory for certain process
queue<process> waitingList;

class memory{
public:
    memory (int i ){set_last(this);}
    memory (){};
    //getters
    int get_size(){return size;}
    int get_rem(){return rem;}
    memory *get_next(){return next;}
    memory *get_last(){return last;}
    process *get_p(){return p;}
    int get_address(){return address;}    
    //setters
    void set_size(int i){size=i;}
    void set_address(int i){address=i;}
    void set_rem(int i){rem=i;}
    void set_next(memory *i){next=i;}
    void set_last(memory *i){last=i;}
    void set_p(process *i){p=i;}
    //functions
    void add(int i,int j);
    void FF(process i);
    void BF(process i);
private :
    int size, rem,address=-1;//address is =-1 when its the first element(the object we declared in main)
    memory *next=nullptr;
    memory *last=nullptr;
    process *p =nullptr;//pointing to the process in this block
};

void pBox (memory *m);//do not care about that
void print(memory m);//you will use this function to print
void case0(memory *&p,memory *&p2);//do not care about that

//First Fit
void memory::FF(process i){
    
    bool foundPlace =false,finished=false;
    memory *ptr=this;//to iterate over memory blocks
    process *pPtr=&i;//to iterate over processes 
    memory *ptr2=nullptr;//used when creating new object
    while ( !finished){
          foundPlace =false;
          while (!foundPlace && ptr!=nullptr){//until finding suitable place in memory
                //if there is a suitable free space and its not used by another process
                if (ptr->get_rem() >= pPtr->get_size() && ptr->get_p()==nullptr){
                    
                    ptr->set_p(pPtr);
                    ptr->set_rem(ptr->get_rem()-pPtr->get_size());
                    if (ptr->get_rem()!= 0){//take the desired space from the memory block and the free space remained will be as a new memory block
                        case0( ptr,ptr2);
                    }
                    if (pPtr->get_next()!=nullptr)//if the processes not finished
                    pPtr=pPtr->get_next();
                    else finished=true;
                    foundPlace=true;
                    print(*this);
                    }
                else
                ptr=ptr->get_next();
                
        }//While #2
        //if did not find free or suitable space in memory the process goes to the waiting list
        //if it was  the last process finish, if not then go to next process
        if (ptr==nullptr){
            
        waitingList.push(*pPtr);
        if (pPtr->get_next()!=nullptr)pPtr=pPtr->get_next();else finished=true;
        
        }
        //resetting variables :D 
        ptr=this;  
        ptr2=nullptr;
    }//while #1
    
}
//Best Fit 
void memory::BF(process i){
    
    bool finished=false;
    memory *ptr=this;//to iterate over memory blocks
    process *pPtr=&i;//to iterate over processes 
    memory *ptr2=nullptr;//used when creating new object, and to store the address of suitable memory block 
    int min=10e6;//just a big number 
    while ( !finished){
          while (ptr!=nullptr){//until the list of the memory blocks is finished
            //if there is a suitable free space and its not used by another process
                if (ptr->get_rem() >= pPtr->get_size() && ptr->get_p()==nullptr){
                    
                    if (ptr->get_size()-pPtr->get_size() <min)
                        {
                        ptr2=ptr;
                        min= ptr->get_size()-pPtr->get_size();
                        }
                        ptr=ptr->get_next();
                }
                else ptr=ptr->get_next();
                
        }//While #2
                    if (min == 10e6){waitingList.push(*pPtr);}//if no suitabe space found
                   
                    else {
                        
                    ptr=ptr2;
                    ptr->set_p(pPtr);
                    ptr->set_rem(ptr->get_rem()-pPtr->get_size()); 
                      
                      if (ptr->get_rem()!= 0){
                        case0( ptr,ptr2);
                    } 
                    print(*this);
                    }//else
                    if (pPtr->get_next()!=nullptr)pPtr=pPtr->get_next();else finished=true;
                    ptr2=nullptr;
                    ptr=this;
                    min=10e6;
                    }
                    
}

void memory::add(int i ,int j){
        if (last == this && this->get_address()==-1){
            
            last->size=i;
            last->rem=i;
            last->address = j;    
        }
        else {
            
            memory *nMem=new memory();
            nMem->size=i;
            nMem->rem=i;
            nMem->address=j;
            last->set_next(nMem);
            last=nMem;
            
        }
    
}




int main (){
    
  int address,holeSize, numberOfProcesses,psize;
  char methodOfAllocation;
  memory M(1);
  process P(1);

  cout << "Please Enter Hole Size,Or Negative Number  To Quit!\n";
  cin>>holeSize;
  if (holeSize <=0){goto A4;}
  while (holeSize > 0){
    A3:
    cout << "Please Enter The Address Of The Hole\n";
    cin >> address;
    if(address < 0){goto A3;}
    M.add(holeSize,address);
    cout << "Please Enter Hole Size, Or Negative Number  To Finish Input!\n";
    cin>>holeSize;
  }
  A2:  
  cout << "Please Enter The Number Of Processes\n";
  cin >> numberOfProcesses;
  if (numberOfProcesses<0){goto A2;}
  for (int i = 0 ; i< numberOfProcesses;i++){
      
        cout << "P"<<i<<" Size Is:";
        cin >> psize;
        P.add(psize,i);
        
   }
A1:   
   cout << "Please Enter (f) for First Fit Allocation, or (b) for Best Fit Allocation\n";
   cin >>methodOfAllocation;
   
   switch (methodOfAllocation){
       
    case 'f':
    M.FF(P);
    break;
    case 'b':
    M.BF(P);
    break;
    default:
    goto A1;
    }
A4:
return 0;
}//end main



void print(memory m){
    memory *ptr=&m;
    while (ptr != nullptr){
        
        cout <<"Address:";
        cout <<ptr->get_address();
        cout  <<endl;    
        pBox(ptr);
        ptr=ptr->get_next();
        
        }
      cout <<"\t=======================\n";

    
}

void pBox (memory *m){
    process *p=m->get_p();
    cout << " ";
   for (int i = 0 ; i< 9 ; i++){cout <<"_";}  
   cout << endl;
   cout <<"|";
   for (int i = 0 ; i< 9 ; i++){cout <<" ";}     
   cout <<"|\n|";
   for (int i = 0 ; i< 4 ; i++){cout <<" ";}
   if (m->get_p()!=nullptr)
   cout << p->get_name();
   else {cout << "  ";}
   for (int i = 0 ; i< 3 ; i++){cout <<" ";} 
   cout <<"|\t"<<m->get_size()<<"\n|"    ;
   for (int i = 0 ; i< 9 ; i++){cout <<"_";}     
   cout << "|\n";
   
}

void case0(memory *&ptr,memory *&ptr2){
    
      ptr->set_size(ptr->get_size()-ptr->get_rem());
                        ptr2=new memory();
                        ptr2->set_address(ptr->get_address()+ptr->get_size());
                        ptr2->set_size(ptr->get_rem());
                        ptr2->set_rem(ptr2->get_size());
                        ptr2->set_next(ptr->get_next());
                        ptr->set_rem(0);
                        ptr->set_next(ptr2);
    
    
    }

 
