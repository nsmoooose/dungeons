#ifndef __DUNGEONS_ERROR_H__
#define __DUNGEONS_ERROR_H__

#define d_bug(...) d_bug_impl (__FILE__, __LINE__, __VA_ARGS__)

void d_bug_impl (const char *file, int line, const char *msg, ...);

#endif
