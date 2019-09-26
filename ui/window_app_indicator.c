#include <gtk/gtk.h>
#include <libappindicator3-0.1/libappindicator/app-indicator.h>

#include "window_settings.h"

void m_item_close_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    gtk_main_quit();
    //gtk_widget_destroy(window);
}

void m_item_settings_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    char* full_address = user_data;
    show_settings_window(full_address);
}

void show_tray_window(char* full_address)
{
    GtkWidget* m_item_settings;
    GtkWidget* m_item_close;
    GtkWidget* m_item_separator;
    AppIndicator* indicator;
    GtkWidget* menu;

    menu = gtk_menu_new();

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget* icon = gtk_image_new_from_file("../assets/gears.png"); //, GTK_ICON_SIZE_MENU); //folder-music-symbolic
    gtk_image_set_pixel_size(icon, 8);
    GtkWidget* label = gtk_label_new("Settings");
    m_item_settings = gtk_menu_item_new();
    gtk_container_add(GTK_CONTAINER(box), icon);
    gtk_container_add(GTK_CONTAINER(box), label);
    gtk_container_add(GTK_CONTAINER(m_item_settings), box);

    m_item_close = gtk_menu_item_new_with_label("Close");
    m_item_separator = gtk_separator_menu_item_new();

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_settings);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_separator);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_close);

    g_signal_connect(m_item_settings, "activate", G_CALLBACK(m_item_settings_selected), full_address);
    g_signal_connect(m_item_close, "activate", G_CALLBACK(m_item_close_selected), NULL);

    gchar* icon_name = "../assets/captain-cap.png"; //"indicator-messages"
    indicator = app_indicator_new("example-simple-client", icon_name, APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon(indicator, "indicator-messages-new");
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    gtk_widget_show_all(menu);
}
