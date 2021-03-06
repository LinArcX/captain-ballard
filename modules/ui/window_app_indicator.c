#include <gtk/gtk.h>
#include <libappindicator3-0.1/libappindicator/app-indicator.h>

#include <cb_file.h>
#include "window_settings.h"

void m_item_exit_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    // Exit from the app
    gtk_main_quit();
    //gtk_widget_destroy(window);
}

void m_item_settings_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    char* db_path = user_data;
    show_settings_window(db_path);
}

void m_item_about_us_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    GdkPixbuf *captain_ballard_logo = NULL;
    GInputStream *log_stream = g_resources_open_stream ("/io/linarcx/captain_ballard/captain-cap.png", 0, NULL);
    if (log_stream != NULL) {
        captain_ballard_logo = gdk_pixbuf_new_from_stream (log_stream, NULL, NULL);
        g_object_unref (log_stream);
    }

    gtk_show_about_dialog(NULL,
            "title", ("About Captain Ballard"),
            "logo", captain_ballard_logo,
            "website", "https://github.com/LinArcX/captain-ballard",
            "website-label", "Captain Ballard",
            "version", "version: 0.1.0",
            "comments", "App that protect your .git projects.",
            "license-type", GTK_LICENSE_GPL_3_0,
            "wrap-license", 1,
            NULL);
}

void show_tray_window(char* db_path)
{
    GtkWidget* menu;
    AppIndicator* indicator;

    GtkWidget* m_item_settings;
    GtkWidget* m_item_about_us;
    GtkWidget* m_item_exit;
    GtkWidget* m_item_separator;

    menu = gtk_menu_new();

    // Settings
    GtkWidget* box_settings = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* icon_settings = gtk_image_new_from_resource("/io/linarcx/captain_ballard/settings.png"); //, GTK_ICON_SIZE_MENU); //folder-music-symbolic
    gtk_image_set_pixel_size(icon_settings, 10);
    GtkWidget* label_settings = gtk_label_new("Settings");
    m_item_settings = gtk_menu_item_new();
    gtk_container_add(GTK_CONTAINER(box_settings), icon_settings);
    gtk_container_add(GTK_CONTAINER(box_settings), label_settings);
    gtk_container_add(GTK_CONTAINER(m_item_settings), box_settings);

    // About Us
    GtkWidget* box_about_us = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* icon_about_us = gtk_image_new_from_resource("/io/linarcx/captain_ballard/about_us.png"); //, GTK_ICON_SIZE_MENU); //folder-music-symbolic
    gtk_image_set_pixel_size(icon_about_us, 10);
    GtkWidget* label_about_us = gtk_label_new("About Us");
    m_item_about_us = gtk_menu_item_new();
    gtk_container_add(GTK_CONTAINER(box_about_us), icon_about_us);
    gtk_container_add(GTK_CONTAINER(box_about_us), label_about_us);
    gtk_container_add(GTK_CONTAINER(m_item_about_us), box_about_us);

    // Exit
    GtkWidget* box_exit = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* icon_exit = gtk_image_new_from_resource("/io/linarcx/captain_ballard/exit.png"); //, GTK_ICON_SIZE_MENU); //folder-music-symbolic
    gtk_image_set_pixel_size(icon_exit, 10);
    GtkWidget* label_exit = gtk_label_new("Exit");
    m_item_exit = gtk_menu_item_new();
    gtk_container_add(GTK_CONTAINER(box_exit), icon_exit);
    gtk_container_add(GTK_CONTAINER(box_exit), label_exit);
    gtk_container_add(GTK_CONTAINER(m_item_exit), box_exit);

    //m_item_close = gtk_menu_item_new_with_label("Close");
    m_item_separator = gtk_separator_menu_item_new();

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_settings);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_about_us);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_separator);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_exit);

    g_signal_connect(m_item_settings, "activate", G_CALLBACK(m_item_settings_selected), db_path);
    g_signal_connect(m_item_about_us, "activate", G_CALLBACK(m_item_about_us_selected), NULL);
    g_signal_connect(m_item_exit, "activate", G_CALLBACK(m_item_exit_selected), NULL);

    indicator = app_indicator_new("example-simple-client", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon(indicator, "indicator-messages-new");
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    gtk_widget_show_all(menu);
}

//gchar* icon_name = "/io/linarcx/captain_ballard/captain-cap.png"; //"indicator-messages"
//indicator = app_indicator_new("example-simple-client", "resource:///io/linarcx/captain_ballard/exit.png", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
//indicator = app_indicator_new("example-simple-client", "../util/icons/captain-cap.png", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
//app_indicator_set_icon_full(indicator, LOCAL_ICON, "Local Icon");

//    char* m_license = read_file_return_content("../LICENSE");

//    const char* authors[1];
//    authors[0] = "LinArcX";
//
//    const char* artists[1];
//    artists[0] = "Morpheous";
//
//    const char* documenters[1];
//    documenters[0] = "Neo";
//    GdkPixbuf* captain_ballard_logo = gdk_pixbuf_new_from_file("../util/icons/captain-cap.png", NULL);

//            "authors", authors,
//            "translator-credits", "Trinity",
//            "documenters", documenters,
//            "artists", artists,
//            "copyright", "Copyright © 2019-2020 LinArcX",
//            "program-name", "Captain Ballard",
//            "license", license,
