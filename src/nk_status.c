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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n", e, d);
}

int show_status_window(char ***all_files) {

  if (all_files) {
    for (size_t i = 0; i < vector_size(all_files); i++) {
      for (size_t j = 0; j < vector_size(all_files[i]); j++) {
        printf("unstaged[%lu][%lu] = %s\n", i, j, all_files[i][j]);
      }
    }
  }

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
  static int minimizable = nk_true;
  static nk_flags window_flags = 0;

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
