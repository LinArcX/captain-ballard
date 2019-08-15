#include <git2.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "cb_daemon.h"
#include "cb_file.h"
#include "cb_log.h"
#include "cb_util.h"
#include "cb_nuklear.h"

#define LOG_FILE "/home/linarcx/captain_ballard.log"

int main() {
    daemonize();
    while (1) {
        sleep(10);
        show_nuklear_window();
        log_message(LOG_FILE, "Time's out.\n");
    }
    return 0;
}

////const char* path = "/mnt/D/WorkSpace/Other/test";
//
// int status_cb(const char *path,
//        unsigned int status_flags,
//        void *payload)
//{
//    printf("\n\nflag: %d", status_flags);
//    printf(", path: %s", path);
//    return 0;
//}
//
// int main()
//{
//    git_libgit2_init();
//
//    git_repository* rep;
//    git_status_list* statuses;
//
//    int error = 0;
//    size_t count = 0;
//    git_status_options opt = GIT_STATUS_OPTIONS_INIT;
//
//
//    char *home_name = "/home/";
//    char *user_name = get_current_user_name();
//    char *primitive_path = "/.config/CaptainBallard/projects";
//
//    char full_address[100] = "";
//    strcat(full_address, home_name);
//    strcat(full_address, user_name);
//    strcat(full_address, primitive_path);
//
//    read_file_line_by_line(full_address);
//
//
//
//    // git open
//    error = git_repository_open(&rep, path);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        printf("Error: %d : %s",  e->klass, e->message);
//        goto SHUTDOWN;
//    }
//
//    // simple
//    error = git_status_foreach(rep, status_cb, NULL);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        printf("Error: %d : %s",  e->klass, e->message);
//    }
//
// SHUTDOWN:
//    git_repository_free(rep);
//    git_libgit2_shutdown();
//    return 0;
//}
//
// detailed
// opt.flags = GIT_STATUS_OPT_DEFAULTS;
// error = git_status_list_new(&statuses, rep, &opt);
// if (error < 0)
//{
//    const git_error *e = giterr_last();
//    std::cout << "Error: " << error << " / " << e->klass << " : " <<
//    e->message << std::endl;

//    goto SHUTDOWN;
//}

// count = git_status_list_entrycount(statuses);
// std::cout<< "count: " << count << std::endl;
// for (size_t i = 0; i < count; ++i)
//{
//    const git_status_entry* entry = git_status_byindex(statuses, i);
//    std::cout<<"status: " << entry->index_to_workdir->status << "\tpath: "
//    << entry->index_to_workdir->new_file.path << std::endl;
//}

// git_status_list_free(statuses);


//unsigned int x_hours = 0;
//unsigned int x_minutes = 0;
//unsigned int x_seconds = 0;
//unsigned int x_milliseconds = 0;

//unsigned int totaltime = 0;
//unsigned int count_down_time_in_secs = 10; // 1 minute is 60, 1 hour is 3600
//unsigned int time_left = 0;

//clock_t x_startTime;
//clock_t x_countTime;

//x_startTime = clock();                           // start clock
//time_left = count_down_time_in_secs - x_seconds; // update timer

//while (time_left > 0) {
//    x_countTime = clock(); // update timer difference
//    x_milliseconds = x_countTime - x_startTime;
//    x_seconds = (x_milliseconds / (CLOCKS_PER_SEC)) - (x_minutes * 60);
//    x_minutes = (x_milliseconds / (CLOCKS_PER_SEC)) / 60;
//    x_hours = x_minutes / 60;
//    time_left =
//        count_down_time_in_secs - x_seconds; // subtract to get difference
//    // printf( "\nYou have %d seconds left ( %d ) count down timer by
//    // TopCoder",time_left,count_down_time_in_secs);
//}

