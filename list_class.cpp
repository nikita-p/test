#include <iostream>

using namespace std;

class list
{
    int number, mark;
    list* next;
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
    void add(int nmbr, int mrk)
    {
        list* b = new list(number, mark, next);
        mark =mrk;
        number = nmbr;
        next = b;
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
};

int main()
{
    list firstCourse;
    firstCourse.add(cin, cout);
    firstCourse.add(cin, cout);
    firstCourse.add(cin, cout);
    firstCourse.del(2);
    firstCourse.print(cout);
    return 0;
}

