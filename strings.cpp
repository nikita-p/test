#include <iostream>
#include <fstream>

using namespace std;

const int MAX_STRING=100;

struct MyString
{
    char* word;
    int len;
};

struct MyString* add_from_string(char* inputWord) //Добавление из существущей последовательности символов
{
    int i = 0, len = 0;
    struct MyString* string = new struct MyString;
    string->word = new char [MAX_STRING];
    while(inputWord[i]!='\0')
    {
        string->word[i] = inputWord[i];
        len++; i++;
    }
    string->len = len;
    return string;
}

struct MyString* add_from_cin() //Добавление из cin'a
{
    cout << "Input word: ";
    char* inputWord;
    inputWord = new char [MAX_STRING];
    cin >> inputWord;

    struct MyString* string = new struct MyString;
    string = add_from_string(inputWord);

    return string;
}

void print(struct MyString* string) //Печать слова
{
        cout << "Output word: " << string->word << endl;
}

int char_compare (char one, char two) //Сравнение символов
{
    if(one==two)
        return 0;
    char t = 'a';
    ifstream in("chars_table.txt");
    while (t!='@') //Последний символ
    {
        in >> t;
        if(t==one)
            return 1;
        if(t==two)
            return -1;
    }
    return 0;
}

int string_compare(struct MyString* a, struct MyString* b) //Сравнение строк
{
    for(int i=0; i<a->len || i< b->len;i++)
    {
        if(a->word[i]=='\0')
            return 1;
        if(b->word[i]=='\0')
            return -1;
        int t = char_compare(a->word[i], b->word[i]);
        if(t == 1)
            return 1;
        if(t == -1)
            return -1;
    }
    return 0;
}

void compare(struct MyString* a, struct MyString* b)
{
    int t = string_compare(a,b);
    cout << "It is right: " << endl;
    if(t==1 || t==0)
    {
        print(a);
        print(b);
    }
    if(t==-1)
    {
        print(b);
        print(a);
    }
}

struct MyString* merger(struct MyString* a, struct MyString* b) //Слияние двух строк
{
    struct MyString* un = new struct MyString;
    un->len = a->len + b->len;
    un->word = new char [un->len];
    for(int i=0; i<a->len;i++)
        un->word[i] = a->word[i];
    for(int i=0;i<b->len;i++)
        un->word[a->len + i]=b->word[i];
    return un;
}

int search(struct MyString* string, int position, struct MyString* piece)
{
    int count = 0, j=0;
    if(position<0)
        position = 0;
    for(int i=position; i<string->len; i++)
    {
        if(string->word[i] != piece->word[j])
            j=0;
        if(string->word[i] == piece->word[j])
            j++;
        if(j == piece->len) //Здесь не все варианты проходят, например в слове azazaziz он не найдёт azaziz 
        {
            count++;
            j=0;
        }
    }
    return count;
}

struct MyString* replace(struct MyString* all, struct MyString* piece, int begin, int end) //Замена
{
    if(end>all->len) //условия для внештатных ситуаций
        end = all->len;
    if(begin>all->len)
        begin = all->len;
    if(begin<0)
        begin = 0;
    if(end<0)
        end = 0;
    struct MyString* newAll = new struct MyString;
    newAll->len = all->len + piece->len + (begin - end);
    newAll->word = new char [newAll->len];
    for(int i=0;i<begin;i++)
        newAll->word[i]=all->word[i];
    for(int i=0;i<piece->len;i++)
        newAll->word[begin+i]=piece->word[i];
    for(int i=end; i<all->len; i++)
        newAll->word[begin + piece->len + (i - end)] = all->word[i];
    delete[] all;
    return newAll;
}

int main()
{
    struct MyString* word = add_from_cin();
    struct MyString* wordTwo = add_from_cin();
    compare(word, wordTwo); //Сравнение++
    struct MyString* merge = merger(word,wordTwo); //Слияние++
    delete[] word;
    delete[] wordTwo;
    struct MyString* piece = add_from_cin();
    print (merge);
    int k = search(merge,0,piece); //Поиск++
    cout <<"Number of pieces: "<< k << endl;
    merge = replace(merge, piece, 0, 0); //Замена++
    print (merge);
    delete[] merge;
    return 0;
}
