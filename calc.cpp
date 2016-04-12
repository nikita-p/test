#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Input.H>
#include <Fl/Fl_Widget.H>
#include <Fl/Fl_Text_Display.H>
#include <iostream>
#include <sstream>

#define W 640
#define H 480

using namespace std;

enum op {noth, sum, mult, divis, subtr, sumProg, multProg};
op state; // Состояние калькулятора
float a, b;  //Числа в памяти
bool cl; //Очистить строку ввода?

string FloatToStr(float n)
{
    if(n>-0.00001 && n < 0.00001)
        n = 0;
    stringstream s;
    s << n;
    return s.str();
}

float StrToFloat(string s)
{
    float c;
    c = atoi(s.c_str());
    float k;
    string s_double;
    bool n = false;
    for(string::iterator i=s.begin(); i!=s.end(); i++)
    {
        if(n)
            s_double = s_double + *i;
        if(*i == '.')
          n = true;
    }
    int len = s_double.length();
    k = atoi(s_double.c_str());
    for(int i=0; i<len; i++)
        k = k/10;
    c = c+k;
    return c;
}

class Auto
{
    void sum_foo(string& s)
    {
        if(s=="")
        {
            s = FloatToStr(a);
            b = a;
            cl = true;
            return;
        }
        b = StrToFloat(s);
        a = a + b;
        cl = true;
        s = FloatToStr(a);
    }
    void noth_foo(string& s)
    {
        if(s=="")
        {
            s = FloatToStr(a);
            cl = true;
            return;
        }
        a = StrToFloat(s);
        cl = true;
        s = FloatToStr(a);
    }
    void mult_foo(string& s)
    {
        if(s=="")
        {
            s = FloatToStr(a);
            b = a;
            cl = true;
            return;
        }
        b = StrToFloat(s);
        a = a*b;
        cl = true;
        s = FloatToStr(a);
    }
    void divis_foo(string& s)
    {
        if(s=="")
        {
            s = FloatToStr(a);
            b = a;
            cl = true;
            return;
        }
        b = StrToFloat(s);
        a = a/b;
        cl = true;
        s = FloatToStr(a);
    }
    void subtr_foo(string& s)
    {
        if(s=="")
        {
            s = FloatToStr(a);
            b = -a;
            cl = true;
            return;
        }
        b = - StrToFloat(s);
        a = a + b;
        cl = true;
        s = FloatToStr(a);
    }
    void sumProg_foo(string& s)
    {
        a = a + b;
        cl = true;
        s = FloatToStr(a);
    }
    void multProg_foo(string& s)
    {
        a = a * b;
        cl = true;
        s = FloatToStr(a);
    }

    void state_foo(string& s)
    {
        switch (state) {
        case sum:
            sum_foo(s);
            break;
        case noth:
            noth_foo(s);
            break;
        case mult:
            mult_foo(s);
            break;
        case divis:
            divis_foo(s);
            break;
        case subtr:
            subtr_foo(s);
            break;
        case sumProg:
            sumProg_foo(s);
            break;
        case multProg:
            multProg_foo(s);
            break;
        default:
            s = FloatToStr(a);
            break;
        }
    }
    void clear(string& s)
    {
        if(s=="")
        {
            a = 0;
            state = noth;
        }
        s = "";
        cl = false;
    }
    void switch_state(char c)
    {
        switch (c) {
        case '=':
            if(state == sum || state == sumProg || state == subtr)
            {
                state = sumProg;
                break;
            }
            if(state == mult || state == multProg)
            {
                state = multProg;
                break;
            }
            state = noth;
            break;
        default:
            break;
        }
    }

public:
    void try_clear(string& s)
    {
        if(cl)
            clear(s);
    }
    void use(string& s, char c)
    {
        switch (c) {
        case '+':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s);
            state = sum;
            break;
        case '-':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s);
            state = subtr;
            break;
        case '*':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s);
            state = mult;
            break;
        case '/':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s);
            state = divis;
            break;

        case '=':
            state_foo(s);
            switch_state(c);
            break;
        case 'C':
            clear(s);
            break;
        default:
            s = s + c;
            break;
        }
    }
};

void button_callback(Fl_Widget* widget, void* data)
{
    Auto c;
    Fl_Input* input = (Fl_Input*) data;
    Fl_Button* button = (Fl_Button*) widget;
    string s = string(input->value());
    char b = button->label()[0];
    c.try_clear(s);
    c.use(s, b);
    input->value(s.c_str());
    input->redraw();
}

class Calc
{
    Fl_Input* input;
public:    
    void createButtons()
    {
        Fl_Input* input = new Fl_Input( W/6.0, H/7.0, 4*W/6.0, H/7.0);
        input->textsize(45);
        int n = 1, count = 0;
        char** s1 = new char* [7];
        for(int i=0; i<7; i++)
        {
            s1[i] = new char[2];
            s1[i][1] = '\0';
        }
        s1[0][0] = '+';
        s1[1][0] = '-';
        s1[2][0] = '*';
        s1[6][0] = '/';
        s1[4][0] = '=';
        s1[5][0] = '0';
        s1[3][0] = 'C';
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
            {
                char* s = new char [2];
                s[1]= '\0'; s[0] = char('0' + n);
                if(i!=3 && j!=3)
                {
                    Fl_Button *button = new Fl_Button( j*W/6.0 + W/6.0, i*H/7.0 + 2*H/7.0, W/6.0, H/7.0, s);
                    button->callback(button_callback, input);
                    button->labelsize(36);
                    n++;
                }
                if(i==3 || j==3)
                {
                    Fl_Button *button = new Fl_Button( j*W/6.0 + W/6.0, i*H/7.0 + 2*H/7.0, W/6.0, H/7.0, s1[count]);
                    count++;
                    button->callback(button_callback, input);
                    button->labelfont(FL_BOLD);
                    button->labelsize(36);
                }
            }
        Fl_Button *Point = new Fl_Button( 4*W/6.0 + W/6.0, 4*H/7.0 + 2*H/7.0, W/6.0, H/7.0, ".");
        Point->callback(button_callback, input);
        Point->labelfont(FL_BOLD);
        Point->labelsize(36);
        delete s1;
    }
};

int main(int argc, char **argv)
{
    state = noth;
    Fl_Window *window = new Fl_Window(W, H, "Calculation");
    Calc c;
    c.createButtons();
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
