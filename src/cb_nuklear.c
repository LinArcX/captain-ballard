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
#include "nuklear_glfw_gl2.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static void error_callback(int e, const char *d) {
    printf("Error %d: %s\n", e, d);
}

int show_nuklear_window(char ***projects) {

    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    /* window flags */
    static int show_menu = nk_true;
    static int titlebar = nk_true;
    static int border = nk_true;
    static int resize = nk_true;
    static int movable = nk_true;
    static int no_scrollbar = nk_false;
    static int scale_left = nk_false;
    static nk_flags window_flags = 0;
    static int minimizable = nk_true;

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

        enum { EASY, HARD };
        static int op = EASY;
        static float value = 0.6f;
        static int i = 20;

        if (nk_begin(ctx, "Show", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                    NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE)) {
            if (projects) {
                for (size_t i = 0; i < vector_size(projects); i++) {
                    if (nk_tree_push_id(ctx, NK_TREE_TAB, projects[i][0], NK_MINIMIZED,
                                i)) {
                        nk_layout_row_dynamic(ctx, 30, 3);
                        if (vector_size(projects[i]) > 1) {
                            for (size_t j = 1; j < vector_size(projects[i]); j++) {
                                nk_checkbox_label(ctx, projects[i][j], &titlebar);
                                // printf("v[%lu][%lu] = %s\n", i, j, projects[i][j]);
                            }

                            nk_layout_row_static(ctx, 30, 100, 3);
                            if (nk_button_label(ctx, "Commit")) {
                                for (size_t j = 1; j < vector_size(projects[i]); j++) {
                                    fprintf(stdout, "Files commited!\n");
                                    git_add(projects[i][0], projects[i][j]);
                                }
                            }
                            // if (nk_button_label(ctx, "Add")){
                            //    fprintf(stdout, "Files added! %s\n", projects[i][0]);
                            //}
                            // nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);

                        } else {
                            nk_label(ctx, "This project is clean :)", NK_TEXT_LEFT);
                        }
                        nk_tree_pop(ctx);
                    }
                }
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Notebook", NK_MINIMIZED)) {
                static int current_tab = 0;
                struct nk_rect bounds;
                float step = (2 * 3.141592654f) / 32;
                enum chart_type { CHART_LINE, CHART_HISTO, CHART_MIXED };
                const char *names[] = {"Lines", "Columns", "Mixed"};
                float id = 0;
                int i;

                /* Header */
                nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0, 0));
                nk_style_push_float(ctx, &ctx->style.button.rounding, 0);
                nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                for (i = 0; i < 3; ++i) {
                    /* make sure button perfectly fits text */
                    const struct nk_user_font *f = ctx->style.font;
                    float text_width = f->width(f->userdata, f->height, names[i], nk_strlen(names[i]));
                    float widget_width = text_width + 3 * ctx->style.button.padding.x;
                    nk_layout_row_push(ctx, widget_width);
                    if (current_tab == i) {
                        /* active tab gets highlighted */
                        struct nk_style_item button_color = ctx->style.button.normal;
                        ctx->style.button.normal = ctx->style.button.active;
                        current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
                        ctx->style.button.normal = button_color;
                    } else
                        current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
                }
                nk_style_pop_float(ctx);

                /* Body */
                nk_layout_row_dynamic(ctx, 140, 1);
                if (nk_group_begin(ctx, "Notebook", NK_WINDOW_BORDER)) {
                    nk_style_pop_vec2(ctx);
                    switch (current_tab) {
                        default:
                            break;
                        case CHART_LINE:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            nk_label(ctx, "Files ready to commit:", NK_TEXT_LEFT);
                            //bounds = nk_widget_bounds(ctx);
                            //if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0),
                            //            nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                            //    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255),
                            //            nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
                            //    for (i = 0, id = 0; i < 32; ++i) {
                            //        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                            //        nk_chart_push_slot(ctx, (float)cos(id), 1);
                            //        id += step;
                            //    }
                            //}
                            nk_chart_end(ctx);
                            break;
                        case CHART_HISTO:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            nk_label(ctx, "Files ready to commit:", NK_TEXT_LEFT);
                            //bounds = nk_widget_bounds(ctx);
                            //if (nk_chart_begin_colored(ctx, NK_CHART_COLUMN, nk_rgb(255, 0, 0),
                            //            nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                            //    for (i = 0, id = 0; i < 32; ++i) {
                            //        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                            //        id += step;
                            //    }
                            //}
                            nk_chart_end(ctx);
                            break;
                        case CHART_MIXED:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            nk_label(ctx, "Files ready to commit:", NK_TEXT_LEFT);
                            //bounds = nk_widget_bounds(ctx);
                            //if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0),
                            //            nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                            //    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255),
                            //            nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
                            //    nk_chart_add_slot_colored(ctx, NK_CHART_COLUMN, nk_rgb(0, 255, 0),
                            //            nk_rgb(0, 150, 0), 32, 0.0f, 1.0f);
                            //    for (i = 0, id = 0; i < 32; ++i) {
                            //        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                            //        nk_chart_push_slot(ctx, (float)fabs(cos(id)), 1);
                            //        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 2);
                            //        id += step;
                            //    }
                            //}
                            nk_chart_end(ctx);
                            break;
                    }
                    nk_group_end(ctx);
                } else
                    nk_style_pop_vec2(ctx);
                nk_tree_pop(ctx);
            }

            // for(int i=0; i<5; i++){
            //    if(nk_tree_push_id(ctx, NK_TREE_NODE, "Window", NK_MINIMIZED, i)) {
            //        nk_layout_row_dynamic(ctx, 25, 1);
            //        nk_label(ctx, "Files ready to commit:", NK_TEXT_LEFT);
            //        for (int j = 0; j < 5; j++) {
            //            nk_checkbox_label(ctx, "test", &titlebar);
            //        }
            //        nk_tree_pop(ctx);
            //    }
            //}
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

//    nk_layout_row_dynamic(ctx, 30, 3);
//    nk_checkbox_label(ctx, "Titlebar", &titlebar);
//    nk_checkbox_label(ctx, "Menu", &show_menu);
//    nk_checkbox_label(ctx, "Border", &border);
//    nk_checkbox_label(ctx, "Resizable", &resize);
//    nk_checkbox_label(ctx, "Movable", &movable);
//    nk_checkbox_label(ctx, "No Scrollbar", &no_scrollbar);
//    nk_checkbox_label(ctx, "Minimizable", &minimizable);
//    nk_checkbox_label(ctx, "Scale Left", &scale_left);

//    nk_layout_row_static(ctx, 30, 100, 3);
//    if (nk_button_label(ctx, "Add")){
//        fprintf(stdout, "Files added!\n");
//    }
//    nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
//    if (nk_button_label(ctx, "Commit")){
//        fprintf(stdout, "Files commited!\n");
//    }

///* fixed widget pixel width */
// nk_layout_row_static(ctx, 30, 80, 1);
// if (nk_button_label(ctx, "button")) {
//    /* event handling */
//    printf("HI");
//}

///* fixed widget window ratio width */
// nk_layout_row_dynamic(ctx, 30, 2);
// if (nk_option_label(ctx, "easy", op == EASY))
//    op = EASY;
// if (nk_option_label(ctx, "hard", op == HARD))
//    op = HARD;

///* custom widget pixel width */
// nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
//{
//    nk_layout_row_push(ctx, 50);
//    nk_label(ctx, "Volume:", NK_TEXT_LEFT);
//    nk_layout_row_push(ctx, 110);
//    nk_slider_float(ctx, 0, &value, 1.0f, 0.1f);
//}
// nk_layout_row_end(ctx);
