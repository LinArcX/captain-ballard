#ifndef SYSTEM_TRAY_H
#define SYSTEM_TRAY_H
#include <gtk/gtk.h>

void activate_action(GtkAction* action);
void show_system_tray_window();
void show_simple_system_tray_window();
#endif
