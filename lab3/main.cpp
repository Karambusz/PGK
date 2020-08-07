#include <wx/wxprec.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/fontenum.h>


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    enum figure { star = 0, moon = 1, sun = 2 };
    wxBoxSizer* sizer1; 
    wxBoxSizer* sizer2; 
    wxBoxSizer* sizer3; 
    wxPanel* panel; 
    wxButton* saveFile; 
    wxFileDialog* saveDialog; 
    wxCheckBox* checkBox;
    wxScrollBar* scrollBar;
    wxGauge* gauge; 
    wxButton* colorButton;
    wxColourDialog* colorDialog;
    wxTextCtrl* textCtrl;
    wxChoice* choice;
    wxString textToDraw;
    bool bananaDraw; 
    wxImage* bananaImage;  
    wxBitmap* banana; 
    wxImage imageToSave;
    wxBitmap bitMapToSave;
    wxColor* starColor; 
    figure choosenFigure; 
    int bananaPosition;

    void buttonSaveClick(wxCommandEvent& e);
    void checkBoxClick(wxCommandEvent& e);
    void scrollBarScroll(wxScrollEvent& e);
    void buttonColorClick(wxCommandEvent& e);
    void textCtrlUpdated(wxCommandEvent& e);
    void choiceSelected(wxCommandEvent& e);

    void formPaint(wxPaintEvent& e);
    void formUpdate(wxUpdateUIEvent& e);
    void draw();
};


bool MyApp::OnInit()
{
	wxFrame* frame = new MyFrame();
	frame->Show(true);
	return true;
}
MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "LAB 03")
{   
    SetTitle("GFK Lab 03");
    SetIcon(wxNullIcon);
    SetSize(1, 1, 600, 400);

    sizer1 = new wxBoxSizer(wxHORIZONTAL);
    sizer2 = new wxBoxSizer(wxHORIZONTAL);
    sizer3 = new wxBoxSizer(wxVERTICAL);

    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1,1), wxTAB_TRAVERSAL);
    panel->SetBackgroundColour(wxColour(255, 255, 255));
    panel->SetMinSize(wxSize(500, 410));

    sizer1->Add(panel, 1, wxEXPAND | wxALL, 5);
    sizer2->Add(sizer1, 1, wxEXPAND, 5);

    sizer3->SetMinSize(wxSize(200, 350));
    sizer2->Add(sizer3, 0, wxEXPAND, 5);

    saveFile = new wxButton(this, wxID_ANY, "Zapis do pliku", wxDefaultPosition, wxDefaultSize, 0);
    sizer3->Add(saveFile, 0, wxALIGN_CENTER | wxALL, 5);

    saveDialog = new wxFileDialog(this, _("Choose a file"), _(""), _(""), _("JPG files (*.jpg)|*.jpg"), wxFD_SAVE);

    checkBox = new wxCheckBox(this, wxID_ANY, _("banan"), wxDefaultPosition, wxDefaultSize, 0);
    sizer3->Add(checkBox, 0, wxALIGN_CENTER | wxALL, 5);

    scrollBar = new wxScrollBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
    scrollBar->SetScrollbar(0, 1, 51, 1);
    scrollBar->Disable();
    sizer3->Add(scrollBar, 0, wxEXPAND | wxALL, 5);

    gauge = new wxGauge(this, wxID_ANY, 50, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
    gauge->SetRange(50);
    gauge->SetValue(0);
    sizer3->Add(gauge, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    colorButton = new wxButton(this, wxID_ANY, _("kolor gwiazdki"), wxDefaultPosition, wxDefaultSize, 0);
    colorDialog = new wxColourDialog(this);
    sizer3->Add(colorButton, 0,  wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    textCtrl = new wxTextCtrl(this, wxID_ANY, _("tekst"), wxDefaultPosition, wxDefaultSize, 0);
    sizer3->Add(textCtrl, 0, wxALIGN_CENTER | wxALL, 5);

    wxString choiceTab[3] = { _("Gwiazdka"), _("Ksiezyc"), _("Slonko") };
    choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choiceTab, 0);
    choice->SetSelection(0);
    sizer3->Add(choice, 0, wxALIGN_CENTER | wxALL, 5);

    textToDraw = _("tekst");
    bananaDraw = false;
    bananaPosition = 0;

    bananaImage = new wxImage();
    bananaImage->AddHandler(new wxPNGHandler);
    bananaImage->LoadFile("banan.png", wxBITMAP_TYPE_PNG);

    imageToSave.AddHandler(new wxJPEGHandler);

    banana = new wxBitmap(*bananaImage);
    choosenFigure = star;
    starColor = new wxColour(*wxBLACK);

    saveFile->Bind(wxEVT_BUTTON, &MyFrame::buttonSaveClick, this);
    checkBox->Bind(wxEVT_CHECKBOX, &MyFrame::checkBoxClick, this);

    scrollBar->Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::scrollBarScroll, this);
    scrollBar->Bind(wxEVT_SCROLL_LINEUP, &MyFrame::scrollBarScroll, this);
    scrollBar->Bind(wxEVT_SCROLL_LINEDOWN, &MyFrame::scrollBarScroll, this);
    scrollBar->Bind(wxEVT_SCROLL_CHANGED, &MyFrame::scrollBarScroll, this);
    scrollBar->Bind(wxEVT_SCROLL_PAGEUP, &MyFrame::scrollBarScroll, this);
    scrollBar->Bind(wxEVT_SCROLL_PAGEDOWN, &MyFrame::scrollBarScroll, this);

    colorButton->Bind(wxEVT_BUTTON, &MyFrame::buttonColorClick, this);
    textCtrl->Bind(wxEVT_TEXT, &MyFrame::textCtrlUpdated, this);
    choice->Bind(wxEVT_CHOICE, &MyFrame::choiceSelected, this);
    Bind(wxEVT_PAINT, &MyFrame::formPaint, this);
    Bind(wxEVT_UPDATE_UI, &MyFrame::formUpdate, this);
    

    this->SetSizer(sizer2);
    this->Layout();
    Center();
}

void MyFrame::buttonSaveClick(wxCommandEvent& e) {
    if (saveDialog->ShowModal() == wxID_OK) {
        imageToSave = bitMapToSave.ConvertToImage();
        imageToSave.SaveFile(saveDialog->GetPath());
    }
}

void MyFrame::checkBoxClick(wxCommandEvent& e) {
    if (e.IsChecked()) {
        bananaDraw = true;
        scrollBar->Enable(bananaDraw);
    }
    else {
        bananaDraw = false;
        scrollBar->Enable(bananaDraw);
    }
}

void MyFrame::scrollBarScroll(wxScrollEvent& e) {
    gauge->SetValue(e.GetPosition());
    bananaPosition = e.GetPosition();
    draw();
}

void MyFrame::buttonColorClick(wxCommandEvent& e) { 
    if (colorDialog->ShowModal() == wxID_OK) {
        *starColor = colorDialog->GetColourData().GetColour();
        Refresh();
    }
}

void MyFrame::textCtrlUpdated(wxCommandEvent& e) {
    textToDraw = e.GetString();
    Refresh();
}

void MyFrame::choiceSelected(wxCommandEvent& e) {
    choosenFigure =static_cast<figure>(e.GetSelection());
}

void MyFrame::formPaint(wxPaintEvent& e) {
    draw();
}

void MyFrame::formUpdate(wxUpdateUIEvent& e) {
   draw();
}

void MyFrame::draw() {
    wxClientDC myDC(panel);
    bitMapToSave = wxBitmap(panel->GetSize().x, panel->GetSize().y);
    wxBufferedDC MyDC(&myDC, bitMapToSave);

    wxSize stayCenter = panel->GetSize();
    int x = stayCenter.GetWidth() / 2;
    int y = stayCenter.GetHeight() / 2;

    MyDC.SetDeviceOrigin(0, 0);
    MyDC.Clear();

    if (bananaDraw) {
        MyDC.DrawBitmap(*banana, x - 60, y + 5 - bananaPosition / 1.25);
    }

    MyDC.DrawLine(x, y + 60, x, y - 10); // body
    MyDC.DrawLine(x - 38, y + 22 - bananaPosition / 1.25, x, y); // left hand
    MyDC.DrawLine(x + 38, y + 22, x, y); // right hand
    MyDC.DrawLine(x - 30, y + 89, x, y + 59); // left leg
    MyDC.DrawLine(x + 30, y + 89, x, y + 59); // right leg
    MyDC.DrawCircle(x, y - 30, 20); // head 
    MyDC.DrawEllipse(x - 12, y - 38, 9, 6); // left eye


    if (bananaDraw) {
        MyDC.DrawEllipticArc(x - 13, y - 33, 26, 15, 180, 360); 
        MyDC.DrawEllipse(x + 3, y - 38, 9, 6); // right eye
    } else {
       MyDC.DrawEllipticArc(x - 13, y - 25, 26, 15, 0, 180);
       MyDC.DrawEllipse(x + 3, y - 40, 6, 9); // right eye
    }

    wxPoint pointsOfStar[5];
    pointsOfStar[0] = wxPoint(x - 60, y - 85);
    pointsOfStar[1] = wxPoint(x - 45, y - 35);
    pointsOfStar[2] = wxPoint(x - 85, y - 60);
    pointsOfStar[3] = wxPoint(x - 35, y - 60);
    pointsOfStar[4] = wxPoint(x - 75, y - 35);

    switch(choosenFigure) {
    case star:
        MyDC.SetPen(wxPen(wxColor(*starColor)));
        MyDC.SetBrush(wxBrush(wxColor(*starColor)));
        MyDC.DrawPolygon(5, pointsOfStar, wxWINDING_RULE);
        break;
    case moon:
        MyDC.SetPen(*wxLIGHT_GREY_PEN);
        MyDC.SetBrush(*wxLIGHT_GREY_BRUSH);
        MyDC.DrawCircle(x - 60, y - 60, 20);
        MyDC.SetPen(*wxWHITE_PEN);
        MyDC.SetBrush(*wxWHITE_BRUSH);
        MyDC.DrawCircle(x - 70, y - 70, 20);
        MyDC.SetPen(*wxBLACK_PEN);
        MyDC.SetBrush(*wxBLACK_BRUSH);
        MyDC.DrawLine(x - 55, y - 46, x - 62, y - 53);
        MyDC.DrawCircle(x - 48, y - 63, 2);
        break;
    case sun:
        MyDC.SetPen(*wxYELLOW_PEN);
        MyDC.SetBrush(*wxYELLOW_BRUSH);
        MyDC.DrawCircle(x - 60, y - 60, 20);
        break;
    }  

    MyDC.DrawText(textToDraw, x - 90, y + 80);
    MyDC.SetFont(wxFont(30, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, _("Times New Roman")));
    MyDC.DrawRotatedText(textToDraw, x + 70, y + 80, 90.0);
} 

wxIMPLEMENT_APP(MyApp);


