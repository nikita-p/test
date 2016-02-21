#include <iostream>

using namespace std;

class list
{
    int number, mark;
    list* next;


    void add(int nmbr, int mrk)
    {
        list* b = new list(number, mark, next);
        mark =mrk;
        number = nmbr;
        next = b;
    }
    void add(int nmbr, int mrk, int pos, int count)
    {
        if(next == NULL)
            return;
        if(pos-1 == count || next->next == NULL)
        {
            list* b = new list(nmbr, mrk, next);
            next = b;
        }
        else
            next->add(nmbr,mrk,pos,count+1);
    }

public:
    list()
    {
        this->number=0;
        this->mark=0;
        this->next=NULL;
    }
    list(int nmbr, int mrk)
    {
        this->number = nmbr;
        this->mark = mrk;
        this->next = NULL;
    }
    list(int nmbr, int mrk, list* nxt)
    {
        this->number = nmbr;
        this->mark = mrk;
        this->next = nxt;
    }
    void print(ostream& o)
    {
        if(next == NULL)
            return;
        o << number << '\t' << mark << endl;
        next->print(o);
    }

    void add(int nmbr, int mrk, int pos)
    {
        if(pos==0)
            add(nmbr, mrk);
        else
            add(nmbr, mrk, pos, 0);
    }
    void add(istream& a, ostream& o)
    {
        int nmbr, mrk;
        o << "Number: ";
        a >> nmbr;
        o << "Mark: ";
        a >> mrk;
        this->add(nmbr, mrk);
    }
    void del(int pos)
    {
        if(pos==0)
        {
            list* b = next;
            mark=next->mark;
            number = next->number;
            next = next->next;
            delete b;
        }
        del(pos, 0);
    }
    void del(int pos, int i)
    {
        if(next == NULL)
            return;
        if(i==pos-1)
        {
            list* b = next;
            next = next->next;
            delete b;
            return;
        }
        next->del(pos,i+1);
    }

    bool great()
    {
        if(next->number > next->next->number)
            return true;
        return false;
    }

    void sort(int& len)
    {
        if(next->next->next==NULL)
        {
            return;
        }
        if(great())
        {
            list* tmp = next;
            next = next->next;
            tmp->next = next->next;
            next->next = tmp;
        }
        len++;
        next->sort(len);
    }
    void sort()
    {
        add(0,0,0);
        int len=0, j;
        sort(len);
        j=len;
        for(int i=0;i<j; i++) //Здесь можно оптимизировать
            sort(len);
        del(0);
    }
};

int main()
{
    list firstCourse;
    firstCourse.add(1, 1, 0);
    firstCourse.add(2, 2, 0);
    firstCourse.add(3, 3, 0);
    firstCourse.add(12,1,12);
    firstCourse.add(4, 3, 0);
    firstCourse.del(2);
    firstCourse.sort();
    firstCourse.print(cout);
    return 0;
}

