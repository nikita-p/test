#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Dial.H>
#include <Fl/Fl_Slider.H>
#include <Fl/Fl_Input.H>
#include <vector>
#include <utility>
#include <math.h>
#include <iostream>

#define MAX_W 1000
#define MAX_H 600
#define G 100

using namespace std;
static int ForcePower; //Степень потенциала
static float dt;

class CData
{
    float X, Y, Vx, Vy, Vr, Vt;
    float M; //Радиус зависит от массы
public:
    CData(int x, float y)    {
        X = x;
        Y = y;
        Vx = 0;
        Vy = 0;
        M = 1;
    }
    CData(int x, int y, float m)    {
        X = x;
        Y = y;
        Vx = 0;
        Vy = 0;
        M = m;
        M = m;
    }
    CData(const CData& t)  {
        this->X = t.X;
        this->Y = t.Y;
        this->Vx = 0;
        this->Vy = 0;
        this->M = t.M;
    }
    CData& operator = (const CData& t){
        this->X = t.X;
        this->Y = t.Y;
        this->Vx = 0;
        this->Vy = 0;
        this->M = t.M;
        return *this;
    }
    float getX()    {
        return X; }
    float getY()    {
        return Y; }
    float getVx()    {
        return Vx; }
    float getVy()    {
        return Vy; }
    float getR()    {
        return 10*M; }
    float getM()    {
        return M; }
    void setX(float m) {
        X = m;  }
    void setY(float m) {
        Y = m;  }
    void setM(float m) {
        M = m;  }
    void plusVx(float vx){
        Vx += vx;
    }
    void plusVy(float vy){
        Vy += vy;
    }
    void changeCoordinats()
    {
        X = X + Vx * dt;
        Y = Y + Vy * dt;
    }
    friend class Circle;
};

class Circle : public Fl_Window
{
    vector<CData>* data;
    void draw()
    {
        Fl_Window::draw();
        fl_color(FL_WHITE);
        changeCoordinatsAndDraw();
    }

public:
    Circle() : Fl_Window(MAX_W,MAX_H, "Tt") {Fl_Window::color(FL_BLACK);}
    Circle(vector<CData>* v) : Fl_Window(MAX_W,MAX_H, "Tt")
    {
        data = v;
        v->insert(v->end(),CData(1000*MAX_W, 1000*MAX_H,0)); // Так решена проблема, когда две последних налетают друг на друга
        Fl_Window::color(FL_BLACK);
    }
    vector<CData>* getVector()
    {
        return data;
    }
    float dist (CData a, CData b)
    {
        return ((b.getY() - a.getY())*(b.getY() - a.getY() ) + (b.getX() - a.getX())*(b.getX() - a.getX()));
    }
    int sgn(float x, float y)
    {
        return ((y > x) - (y < x));
    }
    float TheCriticalDistance(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        /*if((*i).getR()>(*j).getR())
            return (*i).getR();
        return (*j).getR();*/
        return (*j).getR() + (*i).getR();
    }
    void impulse(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        float M1 = (*i).getM();
        float M2 = (*j).getM();
        float a = - atan(((*i).getY() - (*j).getY())/((*i).getX() - (*j).getX()));
        (*i).Vr = (*i).Vx * cos(a) - (*i).Vy * sin(a);
        (*i).Vt = (*i).Vx * sin(a) + (*i).Vy * cos(a);
        (*j).Vr = (*j).Vx * cos(a) - (*j).Vy * sin(a);
        (*j).Vt = (*j).Vx * sin(a) + (*j).Vy * cos(a);
        float V1 = (*i).Vr;
        float V2 = (*j).Vr;
        (*i).Vr = ((M1 - M2)*V1 + 2 * M2 * V2)/(M1+M2);
        (*j).Vr = ((M2 - M1)*V2 + 2 * M1 * V1)/(M1+M2);
        (*i).Vx = (*i).Vr * cos(a) + (*i).Vt * sin(a);
        (*i).Vy = - (*i).Vr * sin(a) + (*i).Vt * cos(a);
        (*j).Vx = (*j).Vr * cos(a) + (*j).Vt * sin(a);
        (*j).Vy = - (*j).Vr * sin(a) + (*j).Vt * cos(a);
    }

    void changeVelocity(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        if(sqrt(dist(*i, *j)) > TheCriticalDistance(i,j)){
            float Force = G * (*i).getM() * (*j).getM() * ( pow((dist(*i, *j)),ForcePower/2) ); //Force
            (*i).plusVx( sgn((*i).getX(), (*j).getX())*Force*dt/(*i).getM() );
            (*j).plusVx( - sgn((*i).getX(), (*j).getX())*Force*dt/(*j).getM() );
            (*i).plusVy( sgn((*i).getY(), (*j).getY())*Force*dt/(*i).getM() );
            (*j).plusVy( - sgn((*i).getY(), (*j).getY())*Force*dt/(*j).getM() );
        }
        else
            impulse(i, j);
    }
    void removeFarCirclesAndDraw()
    {
        int len = data->size() - 1; //Предупреждаю удаление фиктивного круга
        vector<CData>::iterator i = data->begin();
        for(int  k = 0; k < len; k++)
        {
            (*i).changeCoordinats();
            fl_circle((*i).getX(), (*i).getY(), (*i).getR());
            vector<CData>::iterator remove = i;
            ++i;
            //if((*remove).getX() > 5*MAX_W || (*remove).getX() < -5*MAX_W
            //        || (*remove).getY() > 5*MAX_H || (*remove).getY() < -5*MAX_H)
            //      data->erase(remove);
        }
    }
    void changeCoordinatsAndDraw()
    {
        for(vector<CData>::iterator i = data->begin(); i != data->end(); i++)
            for(vector<CData>::iterator j = i+1; j != data->end(); j++)
                changeVelocity(i,j);
        removeFarCirclesAndDraw();
    }
};


void callback(void* winp)
{
    Circle* win =(Circle*)winp;
    win->redraw();
    Fl::repeat_timeout(0.05,callback, win);
}

vector<CData> createCircles()
{
    vector<CData> a;
    a.reserve(10);
    a.insert(a.end(),CData(540,300,2));
    a.insert(a.end(),CData(340,100,1));
    a.insert(a.end(),CData(200,400,4));
    return a;
}

void b_call(Fl_Widget*, void* d)
{
    pair<vector<CData>*, Fl_Slider** >* p = (pair<vector<CData>*, Fl_Slider** >*)d;
    Fl_Slider** s = p->second;
    p->first->insert(p->first->begin(),CData(s[0]->value(),s[1]->value(),s[2]->value()));
}
void newCircle(vector<CData>* a) //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
{
    Fl_Slider** sliders = new Fl_Slider* [3];
    sliders[0] = new Fl_Slider(10,10,30,100, "X");
    sliders[0]->bounds(MAX_W, 0);
    sliders[1] = new Fl_Slider(50,10,30,100, "Y");
    sliders[1]->bounds(MAX_H, 0);
    sliders[2] = new Fl_Slider(100,10,30,100, "M");
    sliders[2]->bounds(10,0);
    Fl_Button* b = new Fl_Button(150, 20, 40, 40, "+");
    b->box(FL_FLAT_BOX);
    pair<vector<CData>*, Fl_Slider** >* Pair = new pair<vector<CData>*, Fl_Slider** >(a,sliders);
    b->callback(b_call, Pair);
}

void play_callback(Fl_Widget* but, void*)
{
     Fl_Button* b = (Fl_Button*)but;
     if(string(b->label()) == "@||")
     {
         b->label("@>");
         dt = 0;
     }
     else
     {
         b->label("@||");
         dt = 1;
     }
}

void play()
{
    Fl_Button* b = new Fl_Button(150, 120, 40, 40, "@||");
    b->callback(play_callback);
}

void slid_callback(Fl_Widget* slid, void*)
{
    Fl_Slider* sl = (Fl_Slider*)slid;
    ForcePower = sl->value();
}

void force()
{
    Fl_Slider* pow = new Fl_Slider(20,200,30,100, "R^a");
    pow->bounds(0,-3);
    pow->callback(slid_callback);
}

void drawSetup(vector<CData>* a)
{
    int w = MAX_W/5;
    int h = MAX_H;
    Fl_Window *controlPanel = new Fl_Window(0,0,w,MAX_H);
    controlPanel->color(FL_LIGHT2);
    newCircle(a);
    play();
    force();
}

int main()
{    
    dt = 1;
    ForcePower = -2;
    vector<CData> a = createCircles();
    Circle c(&a);
    drawSetup(&a);
    Fl::add_timeout(0.1, callback, &c);
    c.show();
    return(Fl::run());
};
