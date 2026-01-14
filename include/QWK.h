#pragma once

#include <unordered_map>
#include <list>
#include <zip.h>
#include <string>

struct Message
{
  char status;
  unsigned int message_no;
  unsigned int in_reply_to;
  char *to_name;
  char *from_name;
  char *subject;
  char *date_str;
  unsigned int conference;
  unsigned int chunk_count;
  char *text;
};

struct Conference
{
  unsigned int id;
  std::string *title;
  unsigned int message_count;
  unsigned int unread_message_count;
};

class QWK
{
public:
  QWK(const char *filename);
  const std::unordered_map<unsigned int, std::list<Message *>> &getMessageMap() const { return message_map; };
  const std::list<Conference *> &getConferenceList() const { return conference_list; };

private:
  int readControlFile(zip *archive);
  void readMessagesFile(zip *archive);
  void process_file_header_chunk();
  Message *process_message_header_chunk(const char *buffer);
  char *bytesToString(const char *buffer, unsigned int startIndex,
                      unsigned int size);
  unsigned int bytesToNumber(const char *buffer, unsigned int startIndex,
                             unsigned int size);
  void constructConferenceList();

  std::list<Conference *> conference_list = {};
  std::unordered_map<unsigned int, std::list<unsigned int>> conference_message_map = {};
  std::unordered_map<unsigned int, std::list<Message *>> message_map = {};
};
