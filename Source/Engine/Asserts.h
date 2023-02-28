#pragma once

#define SASSERT(in)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if(!(in))                                                                                                      \
        {                                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0) // NOLINT
#define SMASSERT(in, msg)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if(!(in))                                                                                                      \
        {                                                                                                              \
            LOG_FATAL(msg);                                                                                            \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0) // NOLINT
#define EASSERT(in, msg)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if(!(in))                                                                                                      \
        {                                                                                                              \
            LOG_FATAL(msg);                                                                                            \
            return;                                                                                                    \
        }                                                                                                              \
    } while(0) // NOLINT
#define SMAASSERT(in, msg, arg)                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        if(!(in))                                                                                                      \
        {                                                                                                              \
            LOG_FATAL(msg, arg);                                                                                       \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0) // NOLINT
