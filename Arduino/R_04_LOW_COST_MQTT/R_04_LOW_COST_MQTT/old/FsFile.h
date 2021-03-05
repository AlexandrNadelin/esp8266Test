#ifndef __FS_FILE_H
#define __FS_FILE_H

struct fs_file {
  const char *data;
  int len;
  int index;
  void *pextension;
  uint8_t flags;
};

#endif

