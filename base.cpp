#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
const int MAX_NAME = 20, MAX_GROUP=6;

ifstream in;

struct list
{
    char* group;
    char* name;
    char* surname;
    struct list* next;
};

struct list* init()
{
    struct list* groups = new struct list;
    groups->next = NULL;
    return groups;
}

void print(struct list* all)
{
    for(;all->next!=NULL;all=all->next)
        cout << all->group << '\t' << all->name << '\t' << all->surname << endl;
}

struct list* create()
{
    struct list* element = new struct list;
    element->group = new char;
    element->name = new char;
    element->surname = new char;
    cout << "Group: ";
    cin >> element->group;
    cout << "Name: ";
    cin >> element->name;
    cout << "Surname: ";
    cin >> element->surname;
    return element;
}

struct list* add(struct list* all, struct list* _new, int position) //Добавить элемент
{
    if(position == 0)
    {
        _new->next = all;
        return _new;
    }
    struct list* head = all;
    struct list* prev = new struct list;
    for(int i=0;i<position && all->next!=NULL;i++)
    {
        prev = all;
        all=all->next;
    }
    prev->next = _new;
    _new->next = all;
    return head;
}

struct list* addFromCin(struct list* all) //Добавить что-то из вводимого
{
    struct list* _new = create();
    all = add(all,_new,0);
    return all;
}

struct list* remove(struct list* all, int index) //Удаление элемента
{
    if(index < 0 || all->next == NULL)
        return all;
    if(index == 0)
    {
        all=all->next;
        return all;
    }
    struct list* head = all;
    struct list* previous = new struct list;
    for(int i=0;all->next!=NULL && i<index; i++)
    {
        previous = all;
        all = all->next;
    }
    previous->next = all->next;
    return head;
}

bool sortGroups (struct list* one, struct list* two)
{
    for(int i=0; i<MAX_GROUP; i++)
    {
        if(one->group[i] > two->group[i])
            return false;
        if(one->group[i] < two->group[i])
            return true;
    }
    return false;
}

bool sortNames (struct list* one, struct list* two)
{
    for(int i=0; i<MAX_NAME ; i++)
    {
        if(one->name[i]=='\0')
            return true;
        if(two->name[i]=='\0')
            return false;
        if(one->name[i] > two->name[i])
            return false;
        if(one->name[i] < two->name[i])
            return true;
    }
    return false;
}

bool sortSurnames (struct list* one, struct list* two)
{
    for(int i=0; i<MAX_NAME; i++)
    {
        if(one->surname[i] > two->surname[i])
            return false;
        if(one->surname[i] < two->surname[i])
            return true;
    }
    return false;
}

struct list* sort(struct list* all, int index, bool(*greater)(struct list*, struct list*))
{    
    struct list* head = all;
    for(int i=0; i<index && all->next!=NULL; i++)
        all=all->next;
    if(all->next == NULL)
        return head;

    int index_maximum = index;
    struct list* maximum = all;

    for(int i=index; all->next!=NULL;all=all->next, i++)
        if(greater(maximum, all))
        {
            maximum = all;
            index_maximum = i;
        }
    head = remove(head, index_maximum);
    head = add(head, maximum, 0);
    index++;
    head = sort(head, index, greater);
    return head;
}

void giveBaseToFile(struct list* all)
{
    ofstream out;
    out.open("database.txt");
    for(;all->next!=NULL;all=all->next)
        out << all->group << '\t' << all->name << '\t' << all->surname << endl;
    out << "@";
    out.flush();
    out.close();
}

struct list* getOneStringFromFile (struct list* all, int* ident)
{
    struct list* _new = new struct list;
    _new->group = new char;
    _new->name = new char;
    _new->surname = new char;
    in >> _new->group;
    if(_new->group[0]=='@')
        {
        *ident  = -1;
        return all;
        }
    in >> _new->name;
    in >> _new->surname;
    all = add (all, _new, 0);
    return all;
}

struct list* getBaseFromFile(struct list* all)
{
    int i = 0;
    in.open("database.txt");
    while(i == 0)
    {
        all = getOneStringFromFile(all, &i);
    }
    in.close();
    return all;
}

int main()
{
    struct list* groups = init();
    char k, a;
    int size;
    while(true)
    {
    cout << "Ваши действия: " << endl;
    cin >> a;
    switch (a)
    {
    case 'e':
        return 0;
    case 'a':
        cout << "Сколько человек?" << endl;
        cin >> size;
        for(int i=0;i<size;i++)
            groups = addFromCin(groups);
        break;
    case 'd':
        cout << "Позиция для удаления: ";
        cin >> size; // k - натуральное
        groups = remove(groups, size-1);
        break;
    case 's':
        cout << "Сортировка по полю";
        cin >> k;
        switch (k)
        {
        case 'g':
            groups = sort(groups,0, sortGroups);
            break;
        case 'n':
            groups = sort(groups,0, sortNames);
            break;
        case 's':
            groups = sort(groups,0, sortSurnames);
            break;
        }
        break;
    case 'f':
        cout << "Работа с файлами: ";
        cin >> k;
        switch (k)
        {
        case 'i':
            giveBaseToFile(groups);
            break;
        case 't':
            groups = getBaseFromFile(groups);
            break;
        }
        break;
    case 'p':
        print(groups);
        break;
    }
    }
    return 0;
}
