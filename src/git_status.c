#include "cb_vector.h"
#include "git_status.h"

int project_status(git_repository *repo, char **files) {
  git_status_list *status;
  struct opts o = {GIT_STATUS_OPTIONS_INIT, "."};

  o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
  o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                      GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                      GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

  if (git_repository_is_bare(repo))
    fatal("Cannot report status on bare repository", git_repository_path(repo));

show_status:
  if (o.repeat)
    printf("\033[H\033[2J");
  check_lg2(git_status_list_new(&status, repo, &o.statusopt),
            "Could not get status", NULL);
  print_short(repo, status, files);
  git_status_list_free(status);
  if (o.repeat) {
    sleep(o.repeat);
    goto show_status;
  }
  return 0;
}

static void print_short(git_repository *repo, git_status_list *status,
                        char **files) {
  char istatus, wstatus;
  size_t i, maxi = git_status_list_entrycount(status);
  const git_status_entry *s;
  const char *c;

  for (i = 0; i < maxi; ++i) {
    s = git_status_byindex(status, i);

    if (s->status == GIT_STATUS_CURRENT)
      continue;

    c = NULL;
    if (s->index_to_workdir) {
      c = s->index_to_workdir->new_file.path;
    }
    vector_push_back(files, (char *)c);
  }

  //  for (i = 0; i < maxi; ++i) {
  //    s = git_status_byindex(status, i);
  //    if (s->status == GIT_STATUS_WT_NEW)
  //      printf("?? %s\n", s->index_to_workdir->old_file.path);
  //  }
}

// int show_status_of_git_repo(char **files) {
//  char *address;
//  int error = 0;
//
//  git_libgit2_init();
//  git_repository *rep;
//  git_status_list *statuses;
//
//  address = files[0];
//
//  error = git_repository_open(&rep, address);
//  if (error < 0) {
//    const git_error *e = giterr_last();
//    printf("Error: %d : %s", e->klass, e->message);
//    goto SHUTDOWN;
//  }
//  lg2_status(rep, files);
//
// SHUTDOWN:
//  git_repository_free(rep);
//  git_libgit2_shutdown();
//  return 0;
//}
