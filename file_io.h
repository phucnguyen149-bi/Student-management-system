#ifndef FILE_IO_H
#define FILE_IO_H

#include "user.h"
#include "unit.h"

int load_users(const char* filename);
int load_units(const char* filename);
int save_users(const char* filename);
int save_units(const char* filename);
#endif