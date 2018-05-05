#ifndef __CP_FILTER_H
#define __CP_FILTER_H

#include <stdio.h>

#include "server.h"
#include "sds.h"

// For now only filters GET, PUT commands
// only if CRC16(KEY) % 1024 == 0
inline static int cpFilterRedisCmd(struct redisCommand *cmd,
                                   robj **argv,
                                   int argc)
{
    // HACK: we now that the key for get and put are always 1st arg
    return ((cmd->proc == getCommand || cmd->proc == setCommand) &&
            (crc16(argv[1]->ptr, sdslen(argv[1]->ptr)) % 16384 == 0));
}

inline static void cpPrintRedisCmd(struct redisCommand *cmd, 
                                   robj **argv, 
                                   int argc)
{
    int i;
    printf("=== CP Tracing ===\n");
    printf("name: %s\n"
           "arity: %d\n"
           "sflags: %s\n"
           "firstkey: %d\n"
           "lastkey: %d\n"
           "keystep: %d\n"
           "microseconds: %lld\n"
           "calls: %lld\n",
           cmd->name,
           cmd->arity,
           cmd->sflags,
           cmd->firstkey,
           cmd->lastkey,
           cmd->keystep,
           cmd->microseconds,
           cmd->calls);
    for (i = 0; i < argc; i++) {
        printf("%s\n", (char *)argv[i]->ptr);
    }
        
}

#endif
