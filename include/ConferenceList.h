#include "QWK.h"
#include <wx/frame.h>
#include <wx/listctrl.h>

class ConferenceList : public wxFrame
{
public:
    ConferenceList(wxWindow *parent, wxString title);

    void setConferences(char *filename, std::list<Conference *> conferences);

private:
    wxListCtrl *list_ctrl;
    char *filename;
};
