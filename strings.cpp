#include <iostream>
#include <fstream>
using namespace std;

struct WinString{
    char* data;
    int len;
};

int length_str(char* word)
{
    int i=0;
    while(word[i]!='\0')
        i++;
    return i;
}

struct WinString* cr_n_str(char* word)
{
    int len = length_str(word);
    struct WinString* word_str = new struct WinString;
    word_str->data=new char[len];
    for(int i =0;i<len;i++)
        word_str->data[i]=word[i];
    word_str->len=len;
    return word_str;
}

struct WinString* absolutely_str()
{
    char* x = new char [255];
    cin >> x;
    struct WinString* s = cr_n_str(x); //Создали строку
    return s;
}

struct WinString* united_str(struct WinString* x, struct WinString* y)
{
    struct WinString* z = new struct WinString;
    int old_len=x->len;
    z->len=x->len + y->len;
    int new_len=z->len;
    z->data=new char[new_len];
    for(int i=0;i<old_len;i++)
        z->data[i]=x->data[i];
    for(int i=old_len;i<new_len;i++)
        z->data[i]=y->data[i-old_len];
    z->data[new_len]='\0';
    return z;
}

int search(struct WinString* str, char* part, int index)
{
    int coin=0; //Число совпадений
    int len_part=length_str(part);
    int j=0;
    for(int i=index;i<str->len;i++)
    {
        if (str->data[i]==part[j])
            j++;
        else
            while (j>0 && str->data[i]!=part[j-1])
                j--;
        if (j==len_part)
        {
            j=0;
            coin++;
        }
    }
    return coin;
}

int ch(char l, char m)
{
    char s;
    ifstream  in("table.txt");
    while(s!= l && s!=m)
        in >> s;
    if (s==l && s==m)
        return 0;
    if (s==l)
        return 1;
    if (s==m)
        return -1;
    return 0;
}

bool sort (struct WinString* a, struct WinString* b)
{
    int grea;
    int min_len = a->len;
    if(min_len>b->len)  min_len=b->len;
    for(int i=0;i<min_len;i++)
    {
        grea = ch(a->data[i],b->data[i]);
        if (grea==1) return true;
        if (grea==-1) return false;
    }
    if (a->len < b->len)
        return true;
    return false;
}

int main()
{
    cout << "Put two words: " << endl;
    struct WinString* s_hello = absolutely_str(); //Создаю строку из того, что я ввожу
    struct WinString* s_fun_story = absolutely_str();
    struct WinString* s_talking = united_str(s_hello,s_fun_story); //Соединили строчки
    cout << "OK, " << s_talking->data << endl;
    cout << "Write part of word: ";
    struct WinString* word = absolutely_str();
    int parties = search(s_talking, word->data, 0); //Нахожу в строчке кусок
    cout << parties << endl;
    cout << "A>B?" << endl;
    cout << "A: ";
    struct WinString* s_A = absolutely_str();
    cout << "B: ";
    struct WinString* s_B = absolutely_str();
    bool b = sort(s_A, s_B);
    if (b){
        cout << s_A->data << endl;
        cout << s_B->data << endl;
    }
    if (!b)
    {
        cout << s_B->data << endl;
        cout << s_A->data << endl;
    }
    return 0;
}
