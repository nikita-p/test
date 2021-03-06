#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Input.H>
#include <Fl/Fl_Output.H>
#include <Fl/Fl_Widget.H>
#include <Fl/Fl_Text_Display.H>
#include <iostream>
#include <sstream>

#define W 620
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
    void sum_foo(string& s, char c)
    {
        if(s=="")
        {
            b = a;
            if(c == '=')
                a = a + a;
            s = FloatToStr(a);
            cl = true;
            return;
        }
        b = StrToFloat(s);
        a = a + b;
        cl = true;
        s = FloatToStr(a);
    }
    void noth_foo(string& s, char)
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
    void mult_foo(string& s, char c)
    {
        if(s=="")
        {
            b = a;
            if(c == '=')
                a = a * a;
            s = FloatToStr(a);
            cl = true;
            return;
        }
        b = StrToFloat(s);
        a = a*b;
        cl = true;
        s = FloatToStr(a);
    }
    void divis_foo(string& s, char c)
    {
        if(s=="")
        {
            b = 1/a;
            if(c == '=')
                a = a/a;
            s = FloatToStr(a);
            cl = true;
            return;
        }
        b = 1/StrToFloat(s);
        a = a*b;
        cl = true;
        s = FloatToStr(a);
    }
    void subtr_foo(string& s, char c)
    {
        if(s=="")
        {
            b = -a;
            if(c == '=')
                a = a - a;
            s = FloatToStr(a);
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

    void state_foo(string& s, char c)
    {
        switch (state) {
        case sum:
            sum_foo(s, c);
            break;
        case noth:
            noth_foo(s, c);
            break;
        case mult:
            mult_foo(s, c);
            break;
        case divis:
            divis_foo(s, c);
            break;
        case subtr:
            subtr_foo(s, c);
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
    void switch_state(char c, string& )
    {
        switch (c) {
        case '=':
            if(state == sumProg || state == sum || state == subtr)
            {
                state = sumProg;
                break;
            }
            if(state == multProg || state == mult || state == divis)
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
            state_foo(s, c);
            state = sum;
            break;
        case '-':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s, c);
            state = subtr;
            break;
        case '*':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s, c);
            state = mult;
            break;
        case '/':
            if(state==sumProg || state == multProg)
                state = noth;
            state_foo(s, c);
            state = divis;
            break;
        case '=':
        {
            state_foo(s, c);
            switch_state(c, s);
            break;
        }
        case 'C':
            clear(s);
            break;
        case '.':
        {
            bool n = false;
            for(string::iterator i = s.begin(); i<s.end(); i++)
                if( *i == '.')
                    n = true;
            if(!n)
                s = s + c;
            break;
        }
        default:
            s = s + c;
            break;
        }
    }
};

void button_callback(Fl_Widget* widget, void* data)
{
    Auto c;
    Fl_Output* input = (Fl_Output*) data;
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
    Fl_Output* input;
    void createOneButtonStyle(Fl_Button* but)
    {
        but->color(FL_WHITE);
        but->color2(FL_BLACK);
        but->box(FL_FLAT_BOX);
        but->labelsize(36);
    }
public:    
    void createButtons()
    {
        Fl_Output* input = new Fl_Output( W/6.0, H/7.0, 4*W/6.0, H/7.0);
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
                    createOneButtonStyle(button);
                    button->callback(button_callback, input);
                    n++;
                }
                if(i==3 || j==3)
                {
                    Fl_Button *button = new Fl_Button( j*W/6.0 + W/6.0, i*H/7.0 + 2*H/7.0, W/6.0, H/7.0, s1[count]);
                    count++;
                    createOneButtonStyle(button);
                    button->callback(button_callback, input);
                    button->labelfont(FL_BOLD);
                }
            }
        Fl_Button *Point = new Fl_Button( 4*W/6.0 + W/6.0, 4*H/7.0 + 2*H/7.0, W/6.0, H/7.0, ".");
        createOneButtonStyle(Point);
        Point->callback(button_callback, input);
        Point->labelfont(FL_BOLD);
        delete s1;
    }
};

int main(int argc, char **argv)
{
    state = noth;
    Fl_Window *window = new Fl_Window(W, H, "Calculation");
    window->color(FL_WHITE);
    Calc c;
    c.createButtons();
    window->end();
    window->show(argc, argv);
    return Fl::run();
}

