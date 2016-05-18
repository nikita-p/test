#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
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

using namespace std;
static float ForcePower; //Степень при расстоянии в силе

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
    }
    CData(int x, int y, float m, float vy)    {
        X = x;
        Y = y;
        Vx = 0;
        Vy = vy;
        M = m;
    }
    CData(const CData& t)  {
        this->X = t.X;
        this->Y = t.Y;
        this->Vx = t.Vx;
        this->Vy = t.Vy;
        this->M = t.M;
    }
    CData& operator = (const CData& t){
        this->X = t.X;
        this->Y = t.Y;
        this->Vx = t.Vx;
        this->Vy = t.Vy;
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
        return 20*M; }
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
    void changeCoordinats(float dt)
    {
        X = X + Vx * dt;
        Y = Y + Vy * dt;
    }
    friend class Circle;
};

class Circle : public Fl_Double_Window
{
    vector<CData>* data;
    float dt, G;
    float zoom; //Масштаб
    void draw()
    {
        Fl_Double_Window::draw();
        fl_color(FL_YELLOW);
        changeCoordinatsAndDraw();
    }
    virtual int handle(int ev)
        {
          switch(ev) {
            case FL_KEYUP:
              if (Fl::event_key() == FL_Right) {
                for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++)
                    (*i).X -= 10/zoom;
                return 1;
              }
              if (Fl::event_key() == FL_Left) {
                for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++)
                    (*i).X += 10/zoom;
                return 1;
              }
              if (Fl::event_key() == FL_Up) {
                for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++)
                    (*i).Y += 10/zoom;
                return 1;
              }
              if (Fl::event_key() == FL_Down) {
                for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++)
                    (*i).Y -= 10/zoom;
                return 1;
              }else {
                return 0;
              }
            case FL_PUSH:
              if(Fl::event_x() >0 && Fl::event_y() > 0){
                    data->insert(data->begin(),CData(Fl::event_x()/zoom,Fl::event_y()/zoom,2));
                    return 1;}
          case FL_MOUSEWHEEL:{
              float zoomOld = zoom;
              if(Fl::event_dy() < 0){
                  zoom = zoom*1.2;}
              if(Fl::event_dy() > 0){
                    zoom = zoom/1.2;  }
              for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++){
                  (*i).X = (*i).X - Fl::event_x() * (1 - (zoomOld/zoom));
                  (*i).Y = (*i).Y - Fl::event_y() * (1 - (zoomOld/zoom));}
              return 0;
          }
            default:
              return Fl_Double_Window::handle(ev);
          }
        }
public:
    Circle(vector<CData>* v) : Fl_Double_Window(MAX_W,MAX_H, "Tt")
    {
        zoom = 1;
        data = v;
        dt = 0.05;
        G = 2*pow(10,-2*ForcePower-1)/dt;
        v->insert(v->end(),CData(1000*MAX_W, 1000*MAX_H,0)); // Так решена проблема, когда две последних налетают друг на друга
        Fl_Double_Window::color(FL_BLACK);
    }
    vector<CData>* getVector()    {
        return data;    }
    void addData(CData a){
        data->insert(data->begin(),a);  }
    float getdt()    {
        return dt;    }
    float getG()    {
        return G;    }
    float dist (CData a, CData b)    {
        return sqrt((b.getY() - a.getY())*(b.getY() - a.getY() ) + (b.getX() - a.getX())*(b.getX() - a.getX()));    }
    int sgn(float x, float y)    {
        return ((y > x) - (y < x));    }
    float TheCriticalDistance(vector<CData>::iterator i, vector<CData>::iterator j)    {
        return (*j).getR() + (*i).getR();    }
    void impulse(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        float M1 = (*i).getM();
        float M2 = (*j).getM();        
        float Energy = M1*(pow((*i).Vx,2) + pow((*i).Vy,2)) + M2*(pow((*j).Vx,2) + pow((*j).Vy,2));
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
/*
        float k = sqrt((M1*(pow((*i).Vx,2) + pow((*i).Vy,2)) + M2*(pow((*j).Vx,2) + pow((*j).Vy,2)))/Energy);
        cout << k <<endl;
        (*i).Vx = (*i).Vx / k;
        (*i).Vy = (*i).Vy / k;
        (*j).Vx = (*j).Vx / k;
        (*j).Vy = (*j).Vy / k;
*/
    }
    int sgn(float i)
    {
        if(i == 0)
            return 0;
        return 2*(i>0) - 1;
    }
    void changeVelocity(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        if((dist(*i, *j)) > TheCriticalDistance(i,j)){
            float Force = G * (*i).getM() * (*j).getM() * pow(dist(*i, *j),ForcePower); //Force
            (*i).plusVx( - sgn(ForcePower)*sgn((*i).getX(), (*j).getX())*Force*dt/(*i).getM() );
            (*j).plusVx( sgn(ForcePower)*sgn((*i).getX(), (*j).getX())*Force*dt/(*j).getM() );
            (*i).plusVy( - sgn(ForcePower)*sgn((*i).getY(), (*j).getY())*Force*dt/(*i).getM() );
            (*j).plusVy( sgn(ForcePower)*sgn((*i).getY(), (*j).getY())*Force*dt/(*j).getM() );
        }
        else
            impulse(i, j);
    }
    void cintoc(vector<CData>::iterator i, vector<CData>::iterator j)
    {
        if(dist(*i, *j) < TheCriticalDistance(i,j))
        {
            float normalX = (*i).getX() - (*j).getX();
            float normalY = (*i).getY() - (*j).getY();
            float sub = TheCriticalDistance(i,j) - dist(*i, *j);
            float hipotenuza = sqrt(pow(normalX, 2) + pow(normalY,2));
            normalX = normalX*sub/(2*hipotenuza);
            normalY = normalY*sub/(2*hipotenuza);
            (*i).X += normalX;
            (*j).X -= normalX;
            (*i).Y += normalY;
            (*j).Y -= normalY;
        }
        return;
    }
    void changeCoordinatsAndDraw()
    {
        for(vector<CData>::iterator i = data->begin(); i != data->end(); i++)
            for(vector<CData>::iterator j = i+1; j != data->end(); j++)
            {
                changeVelocity(i,j);
                (*i).changeCoordinats(dt);
                cintoc(i, j); //Круг внутри круга
                fl_circle((*i).getX()*zoom, (*i).getY()*zoom, (*i).getR()*zoom);
            }
    }
};


void callback(void* winp)
{
    Circle* win =(Circle*)winp;
    win->redraw();
    Fl::repeat_timeout(win->getdt(),callback, win);
}

vector<CData> createCircles()
{
    vector<CData> a;
    a.reserve(10);
    a.insert(a.end(),CData(300,600,2, -10));
    a.insert(a.end(),CData(300,300,2, 10));
    //a.insert(a.end(),CData(200,400,4));
    return a;
}

void b_call(Fl_Widget*, void* d)
{
    pair<vector<CData>*, Fl_Slider** >* p = (pair<vector<CData>*, Fl_Slider** >*)d;
    Fl_Slider** s = p->second;
    p->first->insert(p->first->begin(),CData(s[0]->value(),s[1]->value(),s[2]->value()));
}
void newCircle(vector<CData>* a)
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
     }
     else
     {
         b->label("@||");
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
    //ForcePower = sl->value();
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
    Fl_Double_Window *controlPanel = new Fl_Double_Window(0,0,w,MAX_H);
    controlPanel->color(FL_LIGHT2);
    newCircle(a);
    play();
    force();
}

void cc(Fl_Widget*, void* circle)
{
    Circle* c = (Circle*)circle;
    c->addData(CData(300,100,2));
}


class ControlPanel
{
    Fl_Double_Window* win;
    Fl_Button* addCircle;
    Circle* cir;
public:
    ControlPanel(int x, int y, int W, int H, Circle* C)
    {
        win = new Fl_Double_Window(x,y,W,H);
        win->color(FL_CYAN);
        cir = C;
    }
    void createCircleButton(int x, int y, int W, int H)
    {
        Fl_Button* b = new Fl_Button(x,y,W,H, "+");
        b->callback(cc, cir);
    }
};

void setup(Circle* a)
{
    ControlPanel Contr(0,0,MAX_W/5,MAX_H, a);
    Contr.createCircleButton(30,70,50,50);
}

int main()
{
    ForcePower = -2;
    vector<CData> a = createCircles();
    Circle c(&a);
    //setup(&c);
    Fl::add_timeout(0.0, callback, &c);
    c.show();
    return(Fl::run());
};
