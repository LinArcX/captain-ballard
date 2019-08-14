// Created by LinArcX

#include <git2.h>
#include <stdio.h>

const char* path = "/mnt/D/WorkSpace/Other/test";

int status_cb(const char *path,
        unsigned int status_flags,
        void *payload)
{
    printf("path: %s", path);
    printf("\nflag: %d", status_flags);
    return 0;
}

int main()
{
    git_libgit2_init();

    git_repository* rep;
    git_status_list* statuses;

    int error = 0;
    size_t count = 0;
    git_status_options opt = GIT_STATUS_OPTIONS_INIT;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        printf("Error: %d : %s",  e->klass, e->message);
        goto SHUTDOWN;
    }

    // detailed
    //opt.flags = GIT_STATUS_OPT_DEFAULTS;
    //error = git_status_list_new(&statuses, rep, &opt);
    //if (error < 0)
    //{
    //    const git_error *e = giterr_last();
    //    std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

    //    goto SHUTDOWN;
    //}

    //count = git_status_list_entrycount(statuses);
    //std::cout<< "count: " << count << std::endl;
    //for (size_t i = 0; i < count; ++i)
    //{
    //    const git_status_entry* entry = git_status_byindex(statuses, i);
    //    std::cout<<"status: " << entry->index_to_workdir->status << "\tpath: " << entry->index_to_workdir->new_file.path << std::endl;
    //}

    //git_status_list_free(statuses);

    // simple
    error = git_status_foreach(rep, status_cb, NULL);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        printf("Error: %d : %s",  e->klass, e->message);
    }

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}
