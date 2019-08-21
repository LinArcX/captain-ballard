#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <GLFW/glfw3.h>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "../libs/nuklear/nuklear.h"
#include "cb_vector.h"
#include "git_common.h"
#include "nk_glfw_gl2.h"
#include "sqlite_util.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

char **settings = NULL;

/* window flags */
static int run_as_daemon = nk_true;
static int show_menu = nk_true;
static int show_app_about = nk_false;

static int border = nk_true;
static int resize = nk_true;
static int movable = nk_true;
static int no_scrollbar = nk_false;
static int scale_left = nk_false;
static int minimizable = nk_true;
static nk_flags window_flags = 0;

/* Platform */
static GLFWwindow *win;
int width = 0, height = 0;
struct nk_context *ctx;
struct nk_colorf bg;

static void error_callback(int e, const char *d) {
    printf("Error %d: %s\n", e, d);
}

int show_launcher_window(char *full_address) {
    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Settings", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* GUI */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        if (nk_begin(ctx, "Main", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                    NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE)) {
            static int inactive = 1;
            static const float ratio[] = {120, 150};
            static char field_buffer[64];
            static char text[9][64];
            static int text_len[9];
            static char box_buffer[512];
            static int field_len;
            static int box_len;
            nk_flags active;

            if (show_menu) {
                /* menubar */
                enum menu_states { MENU_DEFAULT, MENU_WINDOWS };
                static nk_size mprog = 60;
                static int mslider = 10;
                static int mcheck = nk_true;
                nk_menubar_begin(ctx);

                /* menu #1 */
                nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
                nk_layout_row_push(ctx, 45);
                if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200))) {
                    static int check = nk_true;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
                        show_app_about = nk_true;
                    nk_menu_end(ctx);
                }
                nk_layout_row_push(ctx, 200);
                nk_checkbox_label(ctx, "Run as daemon", &run_as_daemon);
                if (!run_as_daemon) {
                    static int current_minute = 30;
                    // printf("buffer: %s", text[0]);
                    nk_layout_row_begin(ctx, NK_STATIC, 22, 3);
                    nk_layout_row_push(ctx, 100);
                    nk_label(ctx, "Period Time:", NK_TEXT_LEFT);
                    nk_layout_row_push(ctx, 180);
                    nk_property_int(ctx, "#Minute:", 10, &current_minute, 200, 5, 1);
                }
                nk_menubar_end(ctx);
            }

            if (show_app_about) {
                /* about popup */
                static struct nk_rect s = {WINDOW_WIDTH / 2 - 160,
                    WINDOW_HEIGHT / 2 - 150, 300, 200};
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE,
                            s)) {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "Captain Ballard", NK_TEXT_LEFT);
                    nk_label(ctx, "By LinArcX", NK_TEXT_LEFT);
                    nk_label(ctx, "nuklear is licensed under GPL3 License.",
                            NK_TEXT_LEFT);
                    nk_popup_end(ctx);
                } else
                    show_app_about = nk_false;
            }

            nk_label(ctx, "List of projects:", NK_TEXT_LEFT);
            nk_layout_row_static(ctx, 180, WINDOW_WIDTH - 50, 1);
            nk_edit_string(ctx, NK_EDIT_BOX, box_buffer, &box_len, 512,
                    nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 3);
            nk_label(ctx, "Project Path:", NK_TEXT_LEFT);
            active = nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, text[7],
                    &text_len[7], 64, nk_filter_ascii);
            if (nk_button_label(ctx, "Add") || (active & NK_EDIT_COMMITED)) {
                text[7][text_len[7]] = '\n';
                text_len[7]++;
                memcpy(&box_buffer[box_len], &text[7], (nk_size)text_len[7]);
                box_len += text_len[7];
                text_len[7] = 0;
            }

            if ((nk_size)box_buffer[0] > 0) {
                inactive = 0;
            } else {
                inactive = 1;
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_layout_row_static(ctx, 30, 80, 1);
            if (inactive) {
                struct nk_style_button button;
                button = ctx->style.button;
                ctx->style.button.normal = nk_style_item_color(nk_rgb(40, 40, 40));
                ctx->style.button.hover = nk_style_item_color(nk_rgb(40, 40, 40));
                ctx->style.button.active = nk_style_item_color(nk_rgb(40, 40, 40));
                ctx->style.button.border_color = nk_rgb(60, 60, 60);
                ctx->style.button.text_background = nk_rgb(60, 60, 60);
                ctx->style.button.text_normal = nk_rgb(60, 60, 60);
                ctx->style.button.text_hover = nk_rgb(60, 60, 60);
                ctx->style.button.text_active = nk_rgb(60, 60, 60);
                nk_button_label(ctx, "Save");
                ctx->style.button = button;
            } else if (nk_button_label(ctx, "Save")) {
                if (run_as_daemon) {
                    // vector_push_back(settings, "sd");
                    // vector_push_back(settings, "ad");

                    create_settings_table(full_address);
                    add_project_path(full_address, box_buffer);

                    // printf("\nbox_buffer: %s", box_buffer);
                    // printf("\nbox_buffer[0]: %d", box_buffer[0]);
                    // printf("\n(nk_size)box_buffer: %lu", (nk_size)box_buffer);
                    // printf("\n(nk_size)box_buffer[0]: %lu", (nk_size)box_buffer[0]);

                    // for(int i=0; i< (nk_size)box_buffer[0]; i++){
                    //    printf("box_buffer[0][i]; %s", box_buffer[i]);
                    //}
                    fprintf(stdout, "Settings saved\n");
                }
            }
        }
        nk_end(ctx);

        if (settings) {
            for (size_t i = 0; i < vector_size(settings); i++)
                printf("\nTitle: %s", settings[i]);
        }

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }

    nk_glfw3_shutdown();
    glfwTerminate();
    vector_free(settings);
    return 0;
}

int show_status_window(char ***all_files) {

    if (all_files) {
        for (size_t i = 0; i < vector_size(all_files); i++) {
            for (size_t j = 0; j < vector_size(all_files[i]); j++) {
                printf("unstaged[%lu][%lu] = %s\n", i, j, all_files[i][j]);
            }
        }
    }

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }

    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* GUI */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        if (nk_begin(ctx, "Show", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                    NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE)) {

            if (all_files) {
                for (size_t i = 0; i < vector_size(all_files); i++) {
                    if (nk_tree_push_id(ctx, NK_TREE_TAB, all_files[i][0], NK_MINIMIZED,
                                i)) {
                        nk_layout_row_dynamic(ctx, 30, 3);
                        if (vector_size(all_files[i]) > 1) {
                            for (size_t j = 1; j < vector_size(all_files[i]); j++) {
                                nk_label(ctx, all_files[i][j], NK_TEXT_LEFT);
                                // nk_checkbox_label(ctx, all_files[i][j], &titlebar);
                                // printf("v[%lu][%lu] = %s\n", i, j, projects[i][j]);
                            }
                            // nk_layout_row_static(ctx, 30, 100, 3);
                            // if (nk_button_label(ctx, "Add")){
                            //    fprintf(stdout, "Files added!\n");
                            //}
                            // nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
                            // if (nk_button_label(ctx, "Commit")){
                            //    fprintf(stdout, "Files commited!\n");
                            //}
                        } else {
                            nk_label(ctx, "This project is clean :)", NK_TEXT_LEFT);
                        }
                        nk_tree_pop(ctx);
                    }
                }
            }
        }
        nk_end(ctx);

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}

// nk_edit_string(ctx, NK_EDIT_FIELD, text[0], &text_len[0], 80,
// nk_filter_default); nk_edit_string(ctx, NK_EDIT_FIELD|NK_EDIT_SIG_ENTER,
// text[0], &text_len[0], 80,  nk_filter_default);

// if(nk_strlen(text[0])>0){
//    inactive = 0;
//}else{
//    inactive = 1;
//}
