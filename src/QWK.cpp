#include "QWK.h"

#include <iostream>
#include <zip.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <string>

QWK::QWK(const char *filename)
{
  zip *archive = zip_open(filename, 0, NULL);

  if (!archive)
  {
    std::cerr << "Failed to open the zip file." << std::endl;
  }

  readControlFile(archive);
  readMessagesFile(archive);

  constructConferenceList();
}

int QWK::readControlFile(zip *archive)
{
  unsigned int MAX_LINE_LENGTH = 255;

  zip_stat_t status;
  zip_stat(archive, "CONTROL.DAT", 0, &status);
  size_t size = status.size;

  // Read into vector<char> (byte buffer)
  std::vector<char> buffer(size);
  zip_file *file = zip_fopen(archive, "CONTROL.DAT", 0);
  if (file && zip_fread(file, buffer.data(), size))
  {
    // Create stringstream from buffer
    std::stringstream ss(std::string(buffer.begin(), buffer.end()));

    // Read lines from the stream
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(ss, line))
    {
      lines.push_back(line);
    }

    for (unsigned int line_index = 11; line_index < lines.size() - 3; line_index += 2)
    {
      std::cout << "ID " << lines[line_index] << " title " << lines[line_index + 1] << std::endl;

      Conference* conference = new Conference();
      conference->id = atoi(lines[line_index].c_str());
      conference->title = lines[line_index + 1];
      conference_list.push_back(conference);
    }
  }

  return 0;
}

void QWK::readMessagesFile(zip *archive)
{
  zip_file *file = zip_fopen(archive, "MESSAGES.DAT", 0);
  if (file)
  {
    char buffer[128];
    zip_fread(file, buffer, sizeof(buffer));
    process_file_header_chunk();
    // Process message information

    std::cout << "Messages" << std::endl;

    while (zip_fread(file, buffer, sizeof(buffer)) > 0)
    {
      Message *message = process_message_header_chunk(buffer);
      zip_fread(file, message->text, (message->chunk_count - 1) * 128);

      message_map[message->message_no].push_back(message);
      conference_message_map[message->conference].push_back(message->message_no);
    }
    zip_fclose(file);
  }
}

void QWK::constructConferenceList()
{
}

unsigned int QWK::bytesToNumber(const char *buffer, unsigned int startIndex,
                                unsigned int size)
{
  char *b = new char[size]{};
  std::copy(&buffer[startIndex], &buffer[startIndex + size], b);
  return std::stoi(b);
}

char *QWK::bytesToString(const char *buffer, unsigned int startIndex,
                         unsigned int size)
{
  char *b = new char[size]{};
  std::copy(&buffer[startIndex], &buffer[startIndex + size], b);
  return b;
}

void QWK::process_file_header_chunk() {}

Message *QWK::process_message_header_chunk(const char *chunk)
{
  Message *message = new Message;
  message->status = chunk[0];
  message->chunk_count = bytesToNumber(chunk, 116, 6);
  message->message_no = bytesToNumber(chunk, 1, 7);
  message->subject = bytesToString(chunk, 71, 25);
  message->in_reply_to = bytesToNumber(chunk, 108, 8);
  message->to_name = bytesToString(chunk, 21, 25);
  message->from_name = bytesToString(chunk, 46, 25);
  message->conference = (chunk[123] << 8) | chunk[124];
  message->text = new char[(message->chunk_count - 1) * 128]{};

  // std::cout
  //     << "Message number " << message->message_no << " subject "
  //     << message->subject << std::endl;
  return message;
}