#pragma once

#include <unordered_map>
#include <list>
#include <zip.h>

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

class QWK
{
public:
  QWK(const char *filename);
  const std::unordered_map<unsigned int, std::list<Message *>> &getMessageMap() const { return message_map; };

private:
  void readMessagesFile(zip *archive);
  void process_file_header_chunk();
  Message *process_message_header_chunk(const char *buffer);
  char *bytesToString(const char *buffer, unsigned int startIndex,
                      unsigned int size);
  unsigned int bytesToNumber(const char *buffer, unsigned int startIndex,
                             unsigned int size);

  std::unordered_map<unsigned int, std::list<unsigned int>> conference_message_map = {};
  std::unordered_map<unsigned int, std::list<Message *>> message_map = {};
};
