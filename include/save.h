#ifndef SAVE_H
#define SAVE_H

int read_object(char *object_type, char *object_name, void *object, int catalog_size);

int save_object(char *object_type, void *object);

#endif