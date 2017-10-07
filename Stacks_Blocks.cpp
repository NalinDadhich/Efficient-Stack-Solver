#include<iostream>
#include<vector>
#include<list>
#include<algorithm>
#include<random>
#include<fstream>
#include<queue>

using namespace std;

class Compare;
class node;

typedef vector< list <char> > state;
typedef priority_queue <node* , vector<node*>, Compare> PQ;

class node
{
public:
    int total_stacks;
    int total_blocks;
    node(int a, int b);
    state current_state;
    node *parent;
    int g_n;
    void generate_goal(char elements_in_stack[]);
    void show_current_config(ofstream &myfile);
    void generate_all_possibilities(int total_stacks, int total_blocks, node *first_child_q, PQ *stl_q, node *visited_state[], int *queue_size_achieved, int *visited_state_itr, ofstream myfile);		//to generate all the child of a state
};


node::node(int a, int b)
{
    total_stacks=a;
    total_blocks=b;
    g_n=0;
    parent=NULL;
}
class Compare
{
public:
    bool operator() (node *state1, node *state2)
    {
        int h_n_1=0;
        int ch=65;
        int ch_to_find=65;
        int total_in_0=0;
        int total_ordered_0=0;
        int flag=0;
        int depth_of_to_find;
        int height_of_not_found;
        int dist_of_stack;
        list<char> list_check;
        list<char>::iterator itr;
        
        list_check=((*state1).current_state)[0];
        for(itr=list_check.begin(); itr!=list_check.end(); ++itr)
        {
            if(*itr==(char)ch and flag==0)
            {
                total_ordered_0++;
                ch_to_find++;
            }
            else
                flag=1;
            ch++;
            total_in_0++;
            
        }
        
        flag=0;
        for(int i=0;i <state1->total_stacks; i++)
        {
            if(flag==1)
                break;
            height_of_not_found=0;
            list_check=((*state1).current_state)[i];
            for(itr=list_check.begin(); itr!= list_check.end(); ++itr)
            {
                if(*itr==(char)ch_to_find and flag==0)
                {
                    if(i==0)
                        dist_of_stack=0;
                    else
                        dist_of_stack=1;
                    flag=1;
                    break;
                    
                }
                height_of_not_found++;
            }
        }
        depth_of_to_find=list_check.size()-(height_of_not_found+1) ;
        h_n_1=(total_in_0-2*total_ordered_0)*(state1->total_blocks) + depth_of_to_find + dist_of_stack;
        
        
        int h_n_2=0;
        ch=65;
        ch_to_find=65;
        total_in_0=0;
        total_ordered_0=0;
        flag=0;
        list_check=((*state2).current_state)[0];
        for(itr=list_check.begin(); itr!=list_check.end(); ++itr)
        {
            if(*itr==(char)ch and flag==0)
            {
                total_ordered_0++;
                ch_to_find++;
            }
            else
                flag=1;
            ch++;
            total_in_0++;
        }
        
        
        flag=0;
        for(int i=0;i <state2->total_stacks; i++)
        {
            if(flag==1)
                break;
            height_of_not_found=0;
            list_check=((*state2).current_state)[i];
            for(itr=list_check.begin(); itr!= list_check.end(); ++itr)
            {
                if(*itr==(char)ch_to_find and flag==0)
                {
                    if(i==0)
                        dist_of_stack=0;
                    else
                        dist_of_stack=1;
                    flag=1;
                    break;
                }
                height_of_not_found++;
            }
        }
        depth_of_to_find=list_check.size()-(height_of_not_found+1) ;
        h_n_2=(total_in_0-2*total_ordered_0)*(state1->total_blocks) + depth_of_to_find + dist_of_stack;
        
        if(h_n_1 + state1->g_n>= h_n_2 + state2->g_n)
            return true;
        else
            return false;
    }
};



void node::generate_goal(char elements_in_stack[])
{
    list<char> goal_state;
    list<char> empty_lists;
    
    for(int i=0;i<total_blocks;i++)
    {
        goal_state.push_back(elements_in_stack[i]);
    }
    current_state.push_back(goal_state);
    int total_empty_reqd=total_stacks-1;
    while(total_empty_reqd)
    {
        current_state.push_back(empty_lists);		//create other empty stacks here
        total_empty_reqd--;
    }
}

void node::show_current_config(ofstream &myfile)
{
    list <char> temp;
    list<char>::iterator j;
    for(int i=0;i<current_state.size();i++)
    {
        cout<<"\n"<<i<<" || ";
        myfile<<"\n"<<i<<" || ";
        temp=current_state[i];
        for(j = temp.begin(); j != temp.end(); ++j)
        {
            cout << *j << " ";
            myfile<<*j<<" ";
        }
    }
    cout<<"\n";
    myfile<<"\n";
}


void andha_randomize(node *child)
{
    list<char>::iterator int_itr;
    int total_push_pops; //to store the number of pushes and pops found by dis(gen)%current_state[0].size()
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<short> dis;
    total_push_pops=dis(gen)%1000;		//randomly generated total push and pops I will do on goal list
    
    while(total_push_pops)
    {
        int popped_stack_no=dis(gen)%(*child).total_stacks;
        int pushed_stack_no=dis(gen)%(*child).total_stacks;
        
        if(popped_stack_no==pushed_stack_no or ((*child).current_state)[popped_stack_no].empty()==true)
            continue;
        
        char goal_state_popped=((*child).current_state)[popped_stack_no].back();
        ((*child).current_state)[popped_stack_no].pop_back();							//randomly push elements into any of the other stacks
        ((*child).current_state)[pushed_stack_no].push_back(goal_state_popped);
        total_push_pops--;
    }
}

int here(node *temp_node, node *visited_state[], int *visited_state_itr)
{
    for(int i=0; i<*visited_state_itr; i++)
    {
        if((*visited_state[i]).current_state==(*temp_node).current_state)
            return 1;
    }
    return 0;
}

void show_stl_q(PQ *stl_q, int *queue_size_achieved, ofstream &myfile)
{
    PQ g=*stl_q;
    cout<<"\nSize of: "<<g.size();
    myfile<<"\nSize of: "<<g.size();
    (g.top())->show_current_config(myfile);
    g.pop();
    *queue_size_achieved-=1;
    
}


void generate_all_possibilities(int total_stacks, int total_blocks, node *first_child_q, PQ *stl_q, node *visited_state[], int *queue_size_achieved, int *visited_state_itr, ofstream &myfile)
{
    node *temp_node=new node(total_stacks,total_blocks);
    (*temp_node).current_state=(*first_child_q).current_state;
    int pushed_stack_no,popped_stack_no;
    list<char> temp_list;
    list<char>::iterator itr;
    
    for(pushed_stack_no=0;pushed_stack_no<total_stacks;pushed_stack_no++)
    {
        for(popped_stack_no=0;popped_stack_no<total_stacks;popped_stack_no++)
        {
            node *temp_node=new node(total_stacks,total_blocks);
            (*temp_node).current_state =(*first_child_q).current_state;
            
            if(popped_stack_no==pushed_stack_no or ((*temp_node).current_state)[popped_stack_no].empty()==true)
                continue;
            
            char goal_state_popped=((*temp_node).current_state)[popped_stack_no].back();
            (((*temp_node).current_state)[pushed_stack_no]).push_back(goal_state_popped);
            ((*temp_node).current_state)[popped_stack_no].pop_back();							//randomly push elements into any of the other stacks
            if(here(temp_node, visited_state, visited_state_itr)==0)
            {
                (*temp_node).g_n=(*first_child_q).g_n+1;
                (*temp_node).parent=first_child_q;
                (*stl_q).push(temp_node);
                *queue_size_achieved+=1;
            }
        }
    }
}

void trace_back(node *top, ofstream &myfile)
{
    while(top->parent!=NULL)
    {
        top->show_current_config(myfile);
        top=top->parent;
    }
}

int is_it_goal(node *first_child_q)
{
    int ch=65;
    list<char> check= (first_child_q->current_state)[0];
    list<char>::iterator itr=check.begin();
    for(int i=0;i<first_child_q->total_blocks; i++)
    {
        if(*itr==(char)ch)
        {	++itr;
            ch++;
            continue;
        }
        else
            return 0;
    }
    return 1;
}


int main()
{
    ofstream myfile("output.txt");
    PQ stl_q;
    int a,b;
    char elements_in_stack[100];
    node *visited_state[100];
    int visited_state_itr=0;
    int queue_size_achieved=0;
    cout<<"Total Stacks: ";
    myfile<<"Total Stacks: ";
    cin>>a;
    myfile<<a<<"\n";
    cout<<"Total Blocks: ";
    myfile<<"Total Blocks: ";
    cin>>b;
    myfile<<b;
    cout<<"Enter Elements (Only Capital Letters): ";
    for(int i=0;i<b; i++)
        cin>>elements_in_stack[i];
    
    //GENERATE my_goal
    node *my_goal=new node(a, b);
    my_goal->generate_goal(elements_in_stack);
    cout<<"\nGoal Node: \n";
    myfile<<"\nGoal Node: \n";
    my_goal->show_current_config(myfile);
    
    //GENERATE a first_child_q and randomize it
    node *first_child_q=new node(a, b);
    first_child_q=my_goal;
    /*list<char> list0={'D'};
     list<char> list1={'E', 'F', 'I', 'J'};
     list<char> list2={'B', 'G'};
     list<char> list3={'C','H'};
     list<char> list4={'A'};
     
     
     (*first_child_q).current_state[0]=list0;
     (*first_child_q).current_state[1]=list1;
     (*first_child_q).current_state[2]=list2;
     (*first_child_q).current_state[3]=list3;
     (*first_child_q).current_state[4]=list4;*/
    
    andha_randomize(first_child_q);
    
    cout<<"\nInitial State: \n ";
    myfile<<"\nInitial State: \n ";
    first_child_q->show_current_config(myfile);
    visited_state[visited_state_itr]=first_child_q;
    visited_state_itr++;
    
    generate_all_possibilities(a, b, first_child_q, &stl_q, visited_state, &queue_size_achieved, &visited_state_itr, myfile);
    
    int count_pop=0;
    node *top;
    while(1)
    {
        cout<<"\nPop number: "<<count_pop<<"\n";
        myfile<<"\nPop number: "<<count_pop<<"\n";
        
        if(stl_q.size()==0)
        {
            cout<<"\nEmpty queue!";
            myfile<<"\nEmpty queue!";
        }
        top=stl_q.top();
        
        if(top->current_state==my_goal->current_state)
        {
            cout<<"\nDone !!";
            myfile<<"\nDone !!";
            break;
        }
        first_child_q=stl_q.top();
        first_child_q->show_current_config(myfile);
        
        int res=is_it_goal(first_child_q);
        if(res==1)
        {
            cout<<"\nDone !";
            myfile<<"\nDone !";
            break;
        }
        stl_q.pop();
        queue_size_achieved-=1;
        
        visited_state[visited_state_itr++]=first_child_q;
        generate_all_possibilities(a,b, first_child_q, &stl_q, visited_state, &queue_size_achieved, &visited_state_itr, myfile);
        if(count_pop==30000)
        {
            cout<<"\nCouldn't find! ";
            myfile<<"\nCouldn't find! ";
            break;
        }
        count_pop++;
    }
    
    cout<<"\nMax queue size: "<<queue_size_achieved-1<<"\n";
    cout<<"Max depth: "<<top->g_n;
    cout<<"\nOptimal Path length: "<<count_pop-1<<"\n";
    
    myfile<<"\nMax queue size: "<<queue_size_achieved-1<<"\n";
    myfile<<"Max depth: "<<top->g_n;
    myfile<<"\nOptimal Path length: "<<count_pop-1<<"\n";
    
    int input;
    cout<<"\nDo you want to see trace_back?(1/0) ";
    myfile<<"\nDo you want to see trace_back?(1/0) ";
    cin>>input;
    myfile<<"\n"<<input;
    if(input==1)
    {
        
        trace_back(top, myfile);
    }
    return 0;
}
