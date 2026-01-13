#include "ConferenceList.h"
#include "stdio.h"
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>

ConferenceList::ConferenceList(wxWindow *parent, wxString title) : wxFrame(parent, wxID_ANY, title)
{
    wxPanel *panel = new wxPanel(this);
    list_ctrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

    list_ctrl->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 50);
    list_ctrl->InsertColumn(1, "Name", wxLIST_FORMAT_LEFT, 150);
    list_ctrl->InsertColumn(2, "Total", wxLIST_FORMAT_LEFT, 50);
    list_ctrl->InsertColumn(3, "Unread", wxLIST_FORMAT_LEFT, 50);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(list_ctrl, wxID_ANY, wxEXPAND | wxALL, 20);
    panel->SetSizer(vbox);

    // self.Bind(wxEVT_LIST_ITEM_SELECTED, self.OnClick, self.list_ctrl);
}