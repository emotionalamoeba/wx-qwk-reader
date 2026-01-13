#include "QWK.h"
#include <wx/treelist.h>

class MessageTree : public wxTreeListCtrl
{
public:
  MessageTree(wxWindow *parent, wxWindowID id,
              const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize,
              long style = wxTL_DEFAULT_STYLE,
              const wxString &name = wxTreeListCtrlNameStr);

  void populate_tree_for_conference(QWK *qwk, unsigned int conference_id);

private:
  wxTreeListItem add_message_to_tree(Message *message,
                                     wxTreeListItem parentItem);

  void
  add_children_of_id(unsigned int parent_id,
                     std::unordered_map<unsigned int, std::list<Message *>> parent_dict,
                     wxTreeListItem parentItem);
};
