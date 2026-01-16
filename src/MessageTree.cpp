#include "MessageTree.h"
#include "QWK.h"
#include <wx/wx.h>

MessageTree::MessageTree(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style,
                         const wxString &name)
    : wxTreeListCtrl(parent, id, pos, size, style, name) {
  AppendColumn("#", 70);
  AppendColumn("Subject", 140);
  AppendColumn("To", 130);
  AppendColumn("From", 130);
  AppendColumn("Date", 150);
}

wxTreeListItem MessageTree::add_message_to_tree(Message *message, wxTreeListItem parentItem) {

  char *messageNumberText = new char[50];
  sprintf(messageNumberText, "%d", message->message_no);

  wxTreeListItem item = InsertItem(parentItem, wxTLI_FIRST, messageNumberText);
  SetItemText(item, 0, messageNumberText);
  SetItemText(item, 1, message->subject);
  SetItemText(item, 2, message->to_name);
  SetItemText(item, 3, message->from_name);
  SetItemText(item, 4, message->date_str);

  return item;
}

void MessageTree::populate_tree_for_conference(QWK *qwk, unsigned int conference_id) {
  std::unordered_map<unsigned int, std::list<Message *>> parent_dict;

  DeleteAllItems();

  // Get the root messages for the conference
  const auto &conference_message_map = qwk->getConferenceMessageMap();
  auto conference_messages_it = conference_message_map.find(conference_id);
  if (conference_messages_it == conference_message_map.end()) {
    std::cerr << "Conference " << conference_id << " not found " << std::endl;
    return;
  }

  const auto &conference_messages = conference_messages_it->second;
  if (conference_messages.empty()) {
    std::cerr << "Conference has no messages" << std::endl;
    return;
  }

  const auto &message_map = qwk->getMessageMap();
  // Build parent-child relationships
  for (auto it = conference_messages.begin(); it != conference_messages.end(); ++it) {
    unsigned int message_no = *it;
    auto message_it = message_map.find(message_no);
    Message *message = message_it->second;

    unsigned int parent_id = message->in_reply_to;
    if (parent_id != 0) // Only process messages that have a parent (reply)
    {
      parent_dict[parent_id].push_back(message);
    }
  }

  // Add root messages (messages with no parent or parent_id = 0)
  for (auto it = conference_messages.begin(); it != conference_messages.end(); ++it) {
    unsigned int message_no = *it;
    auto message_it = message_map.find(message_no);
    Message *message = message_it->second;
    if (message->in_reply_to == 0) {
      wxTreeListItem item = add_message_to_tree(message, GetRootItem());
      add_children_of_id(message->message_no, parent_dict, item);
    }
  }
}

void MessageTree::add_children_of_id(unsigned int parent_id,
                                     std::unordered_map<unsigned int, std::list<Message *>> parent_dict,
                                     wxTreeListItem parentItem) {
  auto children = parent_dict.find(parent_id);
  if (children == parent_dict.end()) {
    return;
  }

  auto messages = &children->second;

  for (auto it = messages->begin(); it != messages->end(); ++it) {
    wxTreeListItem item = add_message_to_tree(*it, parentItem);
    add_children_of_id((*it)->message_no, parent_dict, item);
  }
}