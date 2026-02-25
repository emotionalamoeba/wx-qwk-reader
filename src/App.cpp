#include <wx/artprov.h>
#include <wx/filename.h>
#include <wx/gdicmn.h>
#include <wx/process.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/wx.h>

#include "ConferenceList.h"
#include "MessageTree.h"
#include "QWK.h"

#include <wx/persist.h>
#include <wx/persist/toplevel.h>

class Frame1 : public wxFrame {
public:
  Frame1()
      : wxFrame{nullptr, wxID_ANY, "QWK Reader", wxDefaultPosition, wxSize(1280, 768), wxDEFAULT_FRAME_STYLE, "MAIN"} {

    buildMenuBar();
    doLayout();
  }

private:
  QWK *qwk;
  wxMenuBar *mainMenu = new wxMenuBar;
  ConferenceList *conference_list;
  MessageTree *messageTree;
  wxTextCtrl *messageText;

  void OnConferenceSelected(wxListEvent &event) {
    int item_index = event.GetIndex();
    if (item_index != -1) {
      wxString item_text = conference_list->GetConferenceIdFromListId(item_index);
      messageTree->populate_tree_for_conference(qwk, atoi(item_text));

      return;
    }

    std::cout << "Could not select a conference" << std::endl;
  }

  void OnMessageSelected(wxTreeListEvent &event) {
    unsigned int selectedMessageId = std::atoi(messageTree->GetItemText(event.GetItem()));
    Message *selectedMessage = qwk->getMessageById(selectedMessageId);
    messageText->SetValue(selectedMessage->text);
    messageText->Update();
  }

  void doLayout() {

    // Create a splitter window
    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxPoint(0, 0), wxSize(1280, 400), wxSP_3D);

    // Create the left panel
    wxPanel *left_panel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer *left_sizer = new wxBoxSizer(wxVERTICAL);
    left_panel->SetSizer(left_sizer);

    wxPanel *right_panel = new wxPanel(splitter, wxID_ANY);

    wxBoxSizer *right_sizer = new wxBoxSizer(wxVERTICAL);
    messageText = new wxTextCtrl(right_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                 wxTE_READONLY | wxTE_MULTILINE);
    messageText->SetFont(wxFont(9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    right_sizer->Add(messageText, 1, wxEXPAND | wxALL, 5);

    right_panel->SetSizer(right_sizer);

    // Split the window vertically and set the left and right panes
    splitter->SplitVertically(left_panel, right_panel);

    messageTree =
        new MessageTree(left_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxBORDER_THEME);
    messageTree->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &Frame1::OnMessageSelected, this);

    left_sizer->Add(messageTree, 1, wxEXPAND | wxALL, 5);

    conference_list = new ConferenceList(this, "Conference List", wxPoint(0, 0), wxSize(200, 768));
    conference_list->Bind(wxEVT_LIST_ITEM_SELECTED, &Frame1::OnConferenceSelected, this);
    conference_list->Show();

    CreateStatusBar(3);
    SetStatusText(wxT("Ready"), 0);
  }

  void OnOpen(wxCommandEvent &event) {
    wxFileDialog *openFileDialog =
        new wxFileDialog(this, _("Open QWK file"), "", "", "QWK files (*.qwk)|*.qwk", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog->ShowModal() == wxID_OK) {
      wxString fileName = openFileDialog->GetPath();
      qwk = new QWK(fileName.mb_str());
      conference_list->setConferences(qwk->getConferenceList());
    }
  }

  void OnQuit(wxCommandEvent &WXUNUSED(event)) { Close(true); }

  void buildMenuBar() {
    auto menuFile = new wxMenu;
    auto menuItemFileOpen = menuFile->Append(wxID_OPEN);
    menuItemFileOpen->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menuFile->AppendSeparator();
    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    auto menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    mainMenu->Append(menuFile, "&File");
    mainMenu->Append(menuHelp, "&Help");

    SetMenuBar(mainMenu);

    Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame1::OnOpen));

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame1::OnQuit));
  }
};

class Application : public wxApp {
  bool OnInit() override {
    Frame1 *mainFrame = new Frame1();
    if (!wxPersistenceManager::Get().RegisterAndRestore((wxFrame *)mainFrame))
      return mainFrame->Show(true);
    else
      return mainFrame->Show(true);
  }
};

wxIMPLEMENT_APP(Application);