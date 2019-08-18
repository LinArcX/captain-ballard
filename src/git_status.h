#ifndef STATUA_H
#define STATUA_H

#include "git_common.h"

#define MAX_PATHSPEC 8

enum {
    FORMAT_DEFAULT   = 0,
    FORMAT_LONG      = 1,
    FORMAT_SHORT     = 2,
    FORMAT_PORCELAIN = 3,
};

struct opts {
    git_status_options statusopt;
    char *repodir;
    char *pathspec[MAX_PATHSPEC];
    int npaths;
    int format;
    int zterm;
    int showbranch;
    int showsubmod;
    int repeat;
};

static void print_short(git_repository *repo, git_status_list *status, char** files);
int show_status_of_git_repo(char** files);

#endif
