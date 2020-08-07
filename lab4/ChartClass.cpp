#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"
#define PI 3.14159

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c)
{
    cfg= std::move(c);
    x_step=200;
}

void ChartClass::Set_Range()
{
    double xmin=9999.9,xmax=-9999.9,ymin=9999.9,ymax=-9999.9;
    double x,y,step;
    int i;

    xmin=cfg->Get_x_start();
    xmax=cfg->Get_x_stop();

    step=(cfg->Get_x_stop()-cfg->Get_x_start())/(double)x_step;
    x=cfg->Get_x_start();

    for (i=0;i<=x_step;i++) {
        y=GetFunctionValue(x);
        if (y>ymax) ymax=y;
        if (y<ymin) ymin=y;
        x=x+step;
    }

    y_min=ymin;
    y_max=ymax;
    x_min=xmin;
    x_max=xmax;
}


double ChartClass::GetFunctionValue(double x)
{
    if (cfg->Get_F_type()==1) return x*x;
    if (cfg->Get_F_type()==2) return 0.5*exp(4*x-3*x*x);
    return x+sin(x*4.0);
}

void ChartClass::Draw(wxDC *dc, int w, int h)
{
    Set_Range();
    dc->SetBackground(wxBrush(RGB(255, 255, 255)));
    dc->Clear();
    dc->SetPen(wxPen(RGB(255, 0, 0)));
    dc->DrawRectangle(10, 10, w - 20, h - 20);
    dc->SetClippingRegion(wxRect(10, 10, w - 20, h - 20));

    Matrix transformationMatrix;
    if (cfg->RotateScreenCenter()) {
        transformationMatrix = Set_Rotate(cfg->Get_Alpha(), w, h) * Set_Translate() * Set_Scale(w, h);
    }
    else {
        transformationMatrix = Set_Translate() * Set_Rotate(cfg->Get_Alpha(), w, h)* Set_Scale(w, h);
    }

    dc->SetPen(*wxBLUE_PEN);
    line2d(dc, transformationMatrix, 0, y_min, 0, y_max, w, h);
    line2d(dc, transformationMatrix, x_min, 0, x_max, 0, w, h);
   
    Vector textVector;
    double dx = (cfg->Get_x1() - cfg->Get_x0()) / 4;
    double dy = ((cfg->Get_y1() - cfg->Get_y0()) / 10) + 2;
    for (double i = x_min; i <= x_max; i += dx)
    {   
        textVector.Set(i,- 0.07);
        line2d(dc, transformationMatrix, i, -0.07, i, 0.07, w, h);
        textVector = transformationMatrix * textVector;
        dc->DrawRotatedText(wxString::Format(wxT("%.2f"), i), textVector.GetX(), h - textVector.GetY(), cfg->Get_Alpha());     
    }
    for (double i = y_min; i <= y_max; i += dy) {
        
        textVector.Set(0.05, i);
        line2d(dc, transformationMatrix, -0.07, i, 0.07, i, w, h);
        textVector = transformationMatrix * textVector;
        if (i < -0.1 || i > 0.1) {
            dc->DrawRotatedText(wxString::Format(wxT("%.2f"), i), textVector.GetX(), h - textVector.GetY(), cfg->Get_Alpha());
        }   
    }
    line2d(dc, transformationMatrix, 0, y_max, 0.05, y_max - 0.08, w, h);
    line2d(dc, transformationMatrix, 0, y_max, -0.05, y_max - 0.08, w, h);
    

    dc->SetPen(*wxGREEN_PEN);
    double step = (cfg->Get_x1() - cfg->Get_x0()) / x_step;
    for (double x = cfg->Get_x_start(); x <= cfg->Get_x_stop(); x += step) {
        line2d(dc, transformationMatrix, x, GetFunctionValue(x), x + step, GetFunctionValue(x + step), w, h);
    }
}

void ChartClass::line2d(wxDC* myDC, Matrix t, double x1, double y1, double x2, double y2, int w, int h)
{
    Vector vector[2];
    vector[0].Set(x1, y1);
    vector[1].Set(x2, y2);

    vector[0] = t * vector[0];
    vector[1] = t * vector[1];

    myDC->DrawLine(vector[0].GetX(), h - vector[0].GetY(), vector[1].GetX(), h - vector[1].GetY());
}


double ChartClass::Get_Y_min()
{
    Set_Range();
    return y_min;
}

double ChartClass::Get_Y_max()
{
    Set_Range();
    return y_max;
}

Matrix ChartClass::Set_Rotate(double alpha, int w, int h) {
    Matrix rotateMatrix;

    rotateMatrix.data[0][0] = cos(alpha * PI / 180);
    rotateMatrix.data[0][1] = -sin(alpha * PI / 180);
    rotateMatrix.data[1][0] = sin(alpha * PI / 180);
    rotateMatrix.data[1][1] = cos(alpha * PI / 180);


    if (cfg->RotateScreenCenter()) {
        rotateMatrix.data[0][2] = (w / 2.0) * (1 - cos(alpha * PI / 180)) + (h / 2.0) * (sin(alpha * PI / 180));
        rotateMatrix.data[1][2] = -(w / 2.0) * sin(alpha * PI / 180) + (h / 2.0) * (1 - cos(alpha * PI / 180));
    }else {
        Matrix tmp = Set_Scale(w, h);
        rotateMatrix.data[0][2] = tmp.data[0][2] * (1 - cos(alpha * PI / 180)) + tmp.data[1][2] * (sin(alpha * PI / 180));
        rotateMatrix.data[1][2] = -tmp.data[0][2] * sin(alpha * PI / 180) + tmp.data[1][2] * (1 - cos(alpha * PI / 180));
    }

    return rotateMatrix;
}

Matrix ChartClass::Set_Translate() {
    Matrix translationMatrix;

    translationMatrix.data[0][0] = 1;
    translationMatrix.data[0][2] = cfg->Get_dX();
    translationMatrix.data[1][1] = 1;
    translationMatrix.data[1][2] = cfg->Get_dY();

    return translationMatrix;
}

Matrix ChartClass::Set_Scale(int  w, int h) {
    Matrix scaleMatrix;
    
    double sx = (w - x_min) / (cfg->Get_x1() - cfg->Get_x0());
    double sy = (h - y_min) / (cfg->Get_y1() - cfg->Get_y0());

    scaleMatrix.data[0][0] = sx;
    scaleMatrix.data[0][2] = x_min - sx * cfg->Get_x0();
    scaleMatrix.data[1][1] = sy;
    scaleMatrix.data[1][2] = y_min - sy * cfg->Get_y0();

    return scaleMatrix;
}



