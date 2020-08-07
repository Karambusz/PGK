#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"
#define PI 3.14159


struct Point {
    float x, y, z;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
    int R, G, B;
    Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
    Point begin, end;
    Color color;
    Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    :
    MyFrame1(parent)
{
    m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
    m_staticText25->SetLabel(_("Obr\u00F3t X:"));
    m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
    m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

    WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
    WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
    WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

    WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
    WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
    WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

    WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
    WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
    WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint(wxUpdateUIEvent& event)
{
    Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click(wxCommandEvent& event)
{
    wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (WxOpenFileDialog.ShowModal() == wxID_OK)
    {
        double x1, y1, z1, x2, y2, z2;
        int r, g, b;

        std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
        if (in.is_open())
        {
            data.clear();
            while (!in.eof())
            {
                in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
                data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
            }
            in.close();
        }
    }
}

void GUIMyFrame1::Scrolls_Updated(wxScrollEvent& event)
{
    WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
    WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
    WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

    WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
    WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
    WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

    WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
    WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
    WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


    Repaint();
}


void GUIMyFrame1::Repaint()
{
    wxClientDC *dc = new wxClientDC(WxPanel);
    wxBufferedDC buffer(dc);

    wxSize size = WxPanel->GetSize();
    int x = size.GetWidth();
    int y = size.GetHeight();

    buffer.SetBackground(*wxWHITE_BRUSH);
    buffer.Clear();

    Matrix4 rotation = Set_RotateX(WxSB_RotateX->GetValue()) * Set_RotateY(WxSB_RotateY->GetValue()) * Set_RotateZ(WxSB_RotateZ->GetValue());
    Matrix4 transformation = Set_Translate() * rotation * Set_Scale();

    Matrix4 center;
    center.data[0][0] = center.data[1][1] = center.data[2][2] = 1.0;
    center.data[0][3] = center.data[1][3] = 0.5;

    for (Segment seg : data) {
        Vector4 vector[2];

        vector[0].Set(seg.begin.x, seg.begin.y, seg.begin.z);
        vector[1].Set(seg.end.x, seg.end.y, seg.end.z);

        vector[0] = transformation * vector[0];
        vector[1] = transformation * vector[1];


        if (vector[0].GetZ() < 0)
            vector[0].Set(vector[0].GetX(), vector[0].GetY(), 0.0001);
        if (vector[1].GetZ() < 0)
            vector[1].Set(vector[0].GetX(), vector[0].GetY(), 0.0001);

         if (vector[0].GetZ() > 0 && vector[1].GetZ() > 0) {
              vector[0].Set(vector[0].GetX() / vector[0].GetZ(), vector[0].GetY() / vector[0].GetZ(), vector[0].GetZ());
              vector[1].Set(vector[1].GetX() / vector[1].GetZ(), vector[1].GetY() / vector[1].GetZ(), vector[1].GetZ());
              vector[0] = center * vector[0];
              vector[1] = center * vector[1];
              buffer.SetPen(wxPen(RGB(seg.color.R, seg.color.G, seg.color.B)));
              buffer.DrawLine(vector[0].GetX() * x, vector[0].GetY() * y, vector[1].GetX() * x, vector[1].GetY() * y);
         }
      }
}

    




Matrix4 GUIMyFrame1::Set_RotateX(double alpha) {
    Matrix4 rotateX;

    double cosinus = cos(alpha * PI / 180);
    double sinus = sin(alpha * PI / 180);
    rotateX.data[0][0] = 1.0;
    rotateX.data[1][1] = rotateX.data[2][2] = cosinus;
    rotateX.data[1][2] = -sinus;
    rotateX.data[2][1] = sinus;

    return rotateX;
}

Matrix4 GUIMyFrame1::Set_RotateY(double alpha) {
    Matrix4 rotateY;

    double cosinus = cos(alpha * PI / 180);
    double sinus = sin(alpha * PI / 180);
    rotateY.data[0][0] = rotateY.data[2][2] = cosinus;
    rotateY.data[0][2] = -sinus;
    rotateY.data[1][1] = 1.0;
    rotateY.data[2][0] = sinus;

    return rotateY;
}

Matrix4 GUIMyFrame1::Set_RotateZ(double alpha) {
    Matrix4 rotateZ;

    double cosinus = cos(alpha * PI / 180);
    double sinus = sin(alpha * PI / 180);
    rotateZ.data[0][0] = rotateZ.data[1][1] = cosinus;
    rotateZ.data[0][1] = -sinus;
    rotateZ.data[1][0] = sinus;
    rotateZ.data[2][2] = 1.0;
    

    return rotateZ;
}

Matrix4 GUIMyFrame1::Set_Translate() {
    Matrix4 translationMatrix;

    translationMatrix.data[0][0] = translationMatrix.data[1][1] = translationMatrix.data[2][2] = 1.0;
    translationMatrix.data[0][3] = (WxSB_TranslationX->GetValue() - 100) / 50.0;
    translationMatrix.data[1][3] = -(WxSB_TranslationY->GetValue() - 100) / 50.0;
    translationMatrix.data[2][3] = (WxSB_TranslationZ->GetValue() - 100) / 50.0 + 2.0;

    return translationMatrix;
}

Matrix4 GUIMyFrame1::Set_Scale() {
    Matrix4 scaleMatrix;

    scaleMatrix.data[0][0] = WxSB_ScaleX->GetValue() / 100.0;
    scaleMatrix.data[1][1] = -WxSB_ScaleY->GetValue() / 100.0;
    scaleMatrix.data[2][2] = WxSB_ScaleZ->GetValue() / 100.0;
    

    return scaleMatrix;
}



