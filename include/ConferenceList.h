#include "QWK.h"
#include <wx/frame.h>
#include <wx/listctrl.h>

class ConferenceList : public wxFrame {
public:
  ConferenceList(wxWindow *parent, wxString title, const wxPoint &pos, const wxSize &size);

  void setConferences(std::list<Conference *> conferences);
  wxString GetConferenceIdFromListId(unsigned int item_index);

private:
  void OnClick(wxListEvent &event);

  wxListCtrl *list_ctrl;
  char *filename;
};
