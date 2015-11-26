#include <iostream>
using namespace std;

const int MAX_NAME=20;

struct list {
    int group;
    char name[MAX_NAME];
    char surname[MAX_NAME];
    struct list* next;
};

struct list* form()
{
    struct list* into=new struct list;
    cout << "Group: ";
    cin >> into->group;
    cout << "Name: ";
    cin >> into->name;
    cout << "Surname: ";
    cin >> into->surname;
    return into;
}

struct list* get(struct list* base, int index)
{
    for(int i=0; i<index; i++)
        base=base->next;
    return base;
}

struct list* add(struct list* base, struct list* into) //Добавление в 0-позицию
{
    into->next=base;
    return into;
}

struct list* remove(struct list* base, int position)
{
    struct list* head = base;
    if(base->next==NULL)
        return head;
    struct list* trash = get(head, position);
    if(position==0)
        head=trash->next;
    else{
        struct list* prev_trash =  get(head, position-1);
        prev_trash->next=trash->next;
    }
    return head;
}

bool greats_str(char* maybe_str, char* still_str)
{
    for(int i=0;i<MAX_NAME;i++)
    {
        if (maybe_str[i]>still_str[i])
            return true;
        else
            return false;
    }
    return true;
}

bool greats(struct list* maybe, struct list* still)
{
    bool name, surname;
    if(maybe->group > still->group)
        return true;
    else
    {
        name=greats_str(maybe->name,still->name);
        surname=greats_str(maybe->surname,still->surname);
        if(name || (!name && surname))
            return true;
        else
            return false;
    }
}

struct list* sort(struct list* base, int k)
{
    struct list* head = base;
    base=get(base,k);
    struct list* last = get(head, k);
    if(last->next==NULL)
        return head;
    int i =k, max=k;
    for(i=k; base->next!=NULL; i++)
    {
        if(greats(get(head,i),get(head,max))==true)
            max=i;
        base=base->next;
    }
    struct list* maximum =get(head,max);
    head=remove(head, max);
    head=add(head, maximum);
    head=sort(head,k+1);
    return head;
}

struct list* init()
{
    struct list* head = new struct list;
    head->next = NULL;
    return head;
}

void print(struct list* base)
{
    for(;base->next!=NULL;base=base->next)
        cout << base->group << '\t' << base->name << '\t' << base->surname << endl;
    return;
}

//add, replace, remove, sort
int main()
{
    char a;
    struct list* head = init();
    while(true)
    {
        cin >> a;
        switch (a)
        {
            case 'e':
                return 0;
            case 's':
                head=sort(head,0);//++
                break;
            case 'a':
                {
                struct list* new_stud = form();
                head=add(head, new_stud);
                break;
                }
            case 'p':
                print(head);
                break;
            case 'd':
                head=remove(head,0);
                break;
    }
    }

    return 0;
}

