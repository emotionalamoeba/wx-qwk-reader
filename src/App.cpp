#include <wx/artprov.h>
#include <wx/filename.h>
#include <wx/process.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>

#include "ConferenceList.h"
#include "MessageTree.h"
#include "QWK.h"

class Frame1 : public wxFrame
{
public:
  Frame1()
      : wxFrame{
            nullptr,
            wxID_ANY,
            "QWK Reader",
            wxDefaultPosition,
            wxSize(1280, 768),
        }
  {
    buildMenuBar();
    doLayout();
  }

private:
  void doLayout()
  {

    // Create a splitter window
    wxSplitterWindow *splitter = new wxSplitterWindow(
        this, -1, wxPoint(0, 0), wxSize(400, 400), wxSP_3D);

    // Create the left panel
    wxPanel *left_panel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer *left_sizer = new wxBoxSizer(wxVERTICAL);
    // panel1Sizer->Add(textCtrl1, 1, wxEXPAND);
    left_panel->SetSizer(left_sizer);

    wxPanel *right_panel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer *right_sizer = new wxBoxSizer(wxVERTICAL);
    // panel1Sizer->Add(textCtrl1, 1, wxEXPAND);
    right_panel->SetSizer(right_sizer);

    // Split the window vertically and set the left and right panes
    splitter->SplitVertically(left_panel, right_panel);

    MessageTree *tree =
        new MessageTree(left_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                        wxTL_MULTIPLE | wxBORDER_THEME);

    left_sizer->Add(tree, 1, wxEXPAND | wxALL, 5);

    conference_list = new ConferenceList(this, "Conference List");
    conference_list->Show();
  }

  void OnOpen(wxCommandEvent &event)
  {
    wxFileDialog *openFileDialog = new wxFileDialog(
        this, _("Open QWK file"), "", "", "QWK files (*.qwk)|*.qwk",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog->ShowModal() == wxID_OK)
    {
      wxString fileName = openFileDialog->GetPath();
      qwk = new QWK(fileName.mb_str());
      conference_list->setConferences(qwk->getConferenceList());
    }
  }

  void OnQuit(wxCommandEvent &WXUNUSED(event)) { Close(true); }

  void buildMenuBar()
  {
    auto menuFile = new wxMenu;
    auto menuItemFileOpen = menuFile->Append(wxID_OPEN);
    menuItemFileOpen->SetBitmap(
        wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menuFile->AppendSeparator();
    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(
        wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    auto menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    mainMenu->Append(menuFile, "&File");
    mainMenu->Append(menuHelp, "&Help");

    SetMenuBar(mainMenu);

    Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame1::OnOpen));

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame1::OnQuit));
  }

  QWK *qwk;
  wxMenuBar *mainMenu = new wxMenuBar;
  ConferenceList *conference_list;
};

class Application : public wxApp
{
  bool OnInit() override { return (new Frame1)->Show(); }
};

wxIMPLEMENT_APP(Application);