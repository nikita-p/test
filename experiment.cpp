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
    float X, Y, Vx, Vy, Vrx, Vry, Vtx, Vty;
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
    CData(int x, int y, float m, float vx, float vy)    {
        X = x;
        Y = y;
        Vx = 0;
        Vy = vy;
        Vx = vx;
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
        return 50*M; }
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
    float zoom, cX, cY; //Масштаб
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
              }
              if (Fl::event_key() == 'f') {
                  ForcePower = ForcePower - 0.5;
                  return 1;
                }
              if (Fl::event_key() == 'd') {
                  ForcePower = ForcePower + 0.5;
                  return 1;
                }
              if (Fl::event_key() == 'c') {
                  int len = data->size();
                  for(int i = 0; i<len-1; i++){
                      data->pop_back();
                  }
                  return 1;
                }
              if (Fl::event_key() == 's') {
                  for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++)                  {
                      (*i).Vy = 0; (*i).Vx = 0; }
                  return 1;
                }
              else {
                return 0;
              }
            case FL_PUSH:
              if(Fl::event_x() >0 && Fl::event_y() > 0){
                    //data->insert(data->begin(),CData(Fl::event_x()/zoom,Fl::event_y()/zoom,2));
                    cX = Fl::event_x();
                    cY = Fl::event_y();
                    return 1;}
          case FL_MOUSEWHEEL:{
              float zoomOld = zoom;
              if(Fl::event_dy() < 0){
                  zoom = zoom*1.2;}
              if(Fl::event_dy() > 0){
                    zoom = zoom/1.2;  }
              for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++){
                  (*i).X = (*i).X - Fl::event_x()/zoomOld + Fl::event_x()/zoom; //YAHOO!!!
                  (*i).Y = (*i).Y - Fl::event_y()/zoomOld + Fl::event_y()/zoom;
              }
              return 0;
          }
          case FL_DRAG:{
              if(Fl::event_button() == FL_RIGHT_MOUSE)
                  for(vector<CData>::iterator i = data->begin(); i!= data->end(); i++){
                      (*i).X = (*i).X - (cX - Fl::event_x())/(10*zoom);
                      (*i).Y = (*i).Y - (cY - Fl::event_y())/(10*zoom);}
              return 0;
          }
          case FL_RELEASE:          {
              if(Fl::event_button() == FL_LEFT_MOUSE)
                  data->insert(data->begin(),CData(cX/zoom,cY/zoom,2,(Fl::event_x() - cX)/10,(Fl::event_y() - cY)/10));
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
/*
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

        float M1 = (*i).getM();
        float M2 = (*j).getM();
        float fullM = M1 + M2;
        float VcenX = (M1*(*i).Vx + M2*(*j).Vx)/fullM;
        float VcenY = (M1*(*i).Vy + M2*(*j).Vy)/fullM;
        (*i).Vr = - ((*i).Vx - VcenX);
        (*i).Vt = - ((*i).Vy - VcenY);
        (*j).Vr = - ((*j).Vx - VcenX);
        (*j).Vt = - ((*j).Vy - VcenY);
        (*i).Vx = (*i).Vr + VcenX;
        (*i).Vy = (*i).Vt + VcenY;
        (*j).Vx = (*j).Vr + VcenX;
        (*j).Vy = (*j).Vt + VcenY;
*/
        float M1 = (*i).getM();
        float M2 = (*j).getM();
        float fullM = M1 + M2;
        float lx = ((*j).getX() - (*i).getX())/dist(*i,*j);
        float ly = ((*j).getY() - (*i).getY())/dist(*i,*j);
        (*i).Vrx = ((*i).Vx * lx + (*i).Vy * ly)*lx;
        (*i).Vry = ((*i).Vx * lx + (*i).Vy * ly)*ly;
        (*i).Vtx = (*i).Vx - (*i).Vrx;
        (*i).Vty = (*i).Vy - (*i).Vry;
        (*j).Vrx = ((*j).Vx * lx + (*j).Vy * ly)*lx;
        (*j).Vry = ((*j).Vx * lx + (*j).Vy * ly)*ly;
        (*j).Vtx = (*j).Vx - (*j).Vrx;
        (*j).Vty = (*j).Vy - (*j).Vry;

        float V1x = (*i).Vrx;
        float V2x = (*j).Vrx;
        float V1y = (*i).Vry;
        float V2y = (*j).Vry;
        (*i).Vrx = ((M1 - M2)*V1x + 2 * M2 * V2x)/fullM;
        (*j).Vrx = ((M2 - M1)*V2x + 2 * M1 * V1x)/fullM;
        (*i).Vry = ((M1 - M2)*V1y + 2 * M2 * V2y)/fullM;
        (*j).Vry = ((M2 - M1)*V2y + 2 * M1 * V1y)/fullM;

        (*i).Vx = (*i).Vrx + (*i).Vtx;
        (*i).Vy = (*i).Vry + (*i).Vty;
        (*j).Vx = (*j).Vrx + (*j).Vtx;
        (*j).Vy = (*j).Vry + (*j).Vty;
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
                fl_circle((*i).getX()*zoom, (*i).getY()*zoom, (*i).getR()*zoom); //*zoom
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
    a.reserve(50);
    a.insert(a.end(),CData(300,600,5));
    return a;
}

void cc(Fl_Widget*, void* circle)
{
    Circle* c = (Circle*)circle;
    c->addData(CData(300,100,2));
}

int main()
{
    ForcePower = -1;
    vector<CData> a = createCircles();
    Circle c(&a);
    Fl::add_timeout(0.0, callback, &c);
    c.show();
    return(Fl::run());
};
