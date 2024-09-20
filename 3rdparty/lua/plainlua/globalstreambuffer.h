//
// Created by eduwhan on 2024/05/10.
//

#ifndef PP_LEVELEDITOR_GLOBALSTREAMBUFFER_H
#define PP_LEVELEDITOR_GLOBALSTREAMBUFFER_H

#include <stddef.h>

#define STREAM_BUFFER_SIZE 4096

typedef struct {
    char buffer[STREAM_BUFFER_SIZE];
    size_t bufferOffset;
} CustomBuffer;

// Declare the variable with extern
extern CustomBuffer outBuffer;
extern CustomBuffer errBuffer;


#endif //PP_LEVELEDITOR_GLOBALSTREAMBUFFER_H
