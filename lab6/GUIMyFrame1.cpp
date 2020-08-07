#include "GUIMyFrame1.h"
#define PI 3.14159
#include <iostream>

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
 m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
 this->SetBackgroundColour(wxColor(192, 192, 192));
 m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
 m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::m_scrolledWindow_update( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_b_grayscale_click( wxCommandEvent& event )
{
 // TO DO: Konwersja do skali szarosci  
    Img_Cpy = Img_Org.Copy().ConvertToGreyscale();
    m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_blur_click( wxCommandEvent& event )
{
 // TO DO: Rozmywanie obrazu (blur)
    Img_Cpy = Img_Org.Copy().Blur(5);
    m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_mirror_click( wxCommandEvent& event )
{
 // TO DO: Odbicie lustrzane
    Img_Cpy = Img_Org.Copy().Mirror();
    m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_replace_click( wxCommandEvent& event )
{
 // TO DO: Zamiana kolorow 
    Img_Cpy = Img_Org.Copy();
    Img_Cpy.Replace((char)254, (char)0, (char)0, (char)0, (char)0, (char)255);
    m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_rescale_click( wxCommandEvent& event )
{
 // TO DO: Zmiana rozmiarow do 320x240
    Img_Cpy = Img_Org.Copy();
    m_scrolledWindow->ClearBackground();
    Img_Cpy.Rescale(320, 240);
}

void GUIMyFrame1::m_b_rotate_click( wxCommandEvent& event )
{
 // TO DO: Obrot o 30 stopni
    Img_Cpy = Img_Org.Copy().Rotate(30.0 * PI / 180.0, wxPoint(Img_Org.GetSize().x, Img_Org.GetSize().y));
}

void GUIMyFrame1::m_b_rotate_hue_click( wxCommandEvent& event )
{
 // TO DO: Przesuniecie Hue o 180 stopni
    Img_Cpy = Img_Org.Copy();
    Img_Cpy.RotateHue(0.5);
    m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_mask_click( wxCommandEvent& event )
{
 // TO DO: Ustawienie maski obrazu
    Img_Cpy = Img_Org.Copy();
    m_scrolledWindow->ClearBackground();
    Img_Cpy.SetMaskFromImage(Img_Mask,0,0,0);
}

void GUIMyFrame1::m_s_brightness_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
Brightness(m_s_brightness->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
Contrast(m_s_contrast->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_b_prewitt_click( wxCommandEvent& event )
{
 // TO DO: Pionowa maska Prewitta
    Img_Cpy = Img_Org.Copy();
    unsigned char* pixels = Img_Cpy.GetData();
    unsigned char* originalPixels = Img_Org.GetData();
    int width = Img_Cpy.GetWidth();
    int height = Img_Cpy.GetHeight();

    for (unsigned int i = 1; i < height - 1; i++) {
        for (unsigned int j = 1; j < width - 1; j++) {
            for (unsigned int k = 0; k < 3; k++) { // k = 0(R), k = 1(G), k = 2(B)
                int val = 0;
                val += originalPixels[k + (i - 1) * width * 3 + (j + 1) * 3];
                val -= originalPixels[k + (i - 1) * width * 3 + (j - 1) * 3];
                val += originalPixels[k + i * width * 3 + (j + 1) * 3];
                val -= originalPixels[k + i * width * 3 + (j - 1) * 3];
                val += originalPixels[k + (i + 1) * width * 3 + (j + 1) * 3];
                val -= originalPixels[k + (i + 1) * width * 3 + (j - 1) * 3];
                pixels[k + i * width * 3 + j * 3] = fabs(val) / 3.0;
            }
        }
    }
}

void GUIMyFrame1::m_b_threshold_click( wxCommandEvent& event )
{
 // TO DO: Prog o wartosci 128 dla kazdego kanalu niezaleznie
    Img_Cpy = Img_Org.Copy();
    unsigned char* points = Img_Cpy.GetData();
    int dataSize = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;
    for (int i = 0; i < dataSize; i++)
    {
        if (points[i] < 128) {
            points[i] = 0;
        }
        else {
            points[i] = 255;
        }
    }
}


void GUIMyFrame1::Contrast(int value)
{
 // TO DO: Zmiana kontrastu obrazu. value moze przyjmowac wartosci od -100 do 100
    Img_Cpy = Img_Org.Copy();
    unsigned char* pixels = Img_Cpy.GetData();
    int dataSize = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;

    double offset_c = (100.0 + (double)value) / (101.0 - (double)value);
    for (int i = 0; i < dataSize; i++) {
        int tmp = offset_c * (pixels[i] - 127.0) + 127.0;
        if (tmp > 255) {
            pixels[i] = 255;
        }
        else if (tmp < 0) {
            pixels[i] = 0;
        }
        else {
            pixels[i] = tmp;
        }
    }
}

void GUIMyFrame1::Repaint()
{
 wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
 wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
 m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
 dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::Brightness(int value)
{
 // TO DO: Zmiana jasnosci obrazu. value moze przyjmowac wartosci od -100 do 100
    Img_Cpy = Img_Org.Copy();
    unsigned char* pixels = Img_Cpy.GetData();
    int dataSize = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;
    
    for (int i = 0; i < dataSize; i++) {
        if (pixels[i] + value > 255) {
            pixels[i] = 255;
        }
        else if (pixels[i] + value < 0) {
            pixels[i] = 0; 
        }
        else { 
            pixels[i] = pixels[i] + value;
        }
    }

}