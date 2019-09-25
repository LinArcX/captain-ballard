#include <gtk/gtk.h>
#include <libappindicator3-0.1/libappindicator/app-indicator.h>

static void activate_action(GtkAction* action)
{
    const gchar* name = gtk_action_get_name(action);
    GtkWidget* dialog;

    dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_CLOSE,
        "You activated action: \"%s\"",
        name);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
    gtk_widget_show(dialog);
}

static GtkActionEntry entries[] = {
    { "FileMenu", NULL, "_File" },
    { "Settings", "document-save", "_Settings", "<control>S",
        "Open Settings", G_CALLBACK(activate_action) },
    { "About", "document-open", "_About Us", "<control>O",
        "About us", G_CALLBACK(activate_action) },
    { "Quit", "application-exit", "_Quit", "<control>Q",
        "Exit the application", G_CALLBACK(gtk_main_quit) }, //gtk_widget_destroy
};

static guint n_entries = G_N_ELEMENTS(entries);
static const gchar* ui_info = "<ui>"
                              "  <popup name='IndicatorPopup'>"
                              "    <menuitem action='Settings' />"
                              "    <menuitem action='About' />"
                              "    <separator/>"
                              "    <menuitem action='Quit' />"
                              "  </popup>"
                              "</ui>";

void show_system_tray_window(int argc, char** argv)
{
    GtkWidget* window;
    GtkWidget* indicator_menu;
    GtkActionGroup* action_group;
    GtkUIManager* uim;
    AppIndicator* indicator;
    GError* error = NULL;

    gtk_init(&argc, &argv);

    ///* create a new window, and set its title */
    //window = gtk_application_window_new(app);
    //gtk_window_set_title(GTK_WINDOW(window), "Add new path");
    //gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    //gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    //gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    /* main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Tray");
    gtk_window_set_icon_name(GTK_WINDOW(window), "indicator-messages-new");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Menus */
    action_group = gtk_action_group_new("AppActions");
    gtk_action_group_add_actions(action_group, entries, n_entries, window);

    uim = gtk_ui_manager_new();
    g_object_set_data_full(G_OBJECT(window), "ui-manager", uim, g_object_unref);
    gtk_ui_manager_insert_action_group(uim, action_group, 0);
    gtk_window_add_accel_group(GTK_WINDOW(window), gtk_ui_manager_get_accel_group(uim));

    if (!gtk_ui_manager_add_ui_from_string(uim, ui_info, -1, &error)) {
        g_message("Failed to build menus: %s\n", error->message);
        g_error_free(error);
        error = NULL;
    }

    /* Indicator */
    indicator = app_indicator_new("example-simple-client", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    indicator_menu = gtk_ui_manager_get_widget(uim, "/ui/IndicatorPopup");

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon(indicator, "indicator-messages-new");
    app_indicator_set_menu(indicator, GTK_MENU(indicator_menu));

    gtk_main();
    //return 0;
}

void show_simple_system_tray_window()
{
    gtk_init(NULL, NULL);

    GtkWidget* menu;
    GtkWidget* menu_item;
    AppIndicator* indicator;

    menu = gtk_menu_new();
    menu_item = gtk_menu_item_new_with_label("Hello world!");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    indicator = app_indicator_new("example-simple-client", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon(indicator, "indicator-messages-new");

    app_indicator_set_menu(indicator, GTK_MENU(menu));
    gtk_widget_show_all(menu);
    gtk_main();
}

//int show_system_tray_window(int argc, char** argv)
//{
//    GtkApplication* app;
//    int status;
//
//    app = gtk_application_new("com.github.linarcx", G_APPLICATION_FLAGS_NONE);
//    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//    status = g_application_run(G_APPLICATION(app), 0, 0);
//    g_object_unref(app);
//    return status;
//}

//GtkWidget *table;
//GtkWidget *sw;
//GtkWidget *contents;
//GtkWidget *statusbar;

//table = gtk_table_new (1, 5, FALSE);
//gtk_container_add (GTK_CONTAINER (window), table);

//menubar = gtk_ui_manager_get_widget (uim, "/ui/MenuBar");
//gtk_widget_show (menubar);
//gtk_table_attach (GTK_TABLE (table),
//        menubar,
//        0, 1,                    0, 1,
//        GTK_EXPAND | GTK_FILL,   0,
//        0,                       0);

///* Document */
//sw = gtk_scrolled_window_new (NULL, NULL);
//gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_IN);
//gtk_table_attach (GTK_TABLE (table),
//        sw,
//        /* X direction */       /* Y direction */
//        0, 1,                   3, 4,
//        GTK_EXPAND | GTK_FILL,  GTK_EXPAND | GTK_FILL,
//        0,                      0);

//gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
//contents = gtk_text_view_new ();
//gtk_widget_grab_focus (contents);
//gtk_container_add (GTK_CONTAINER (sw), contents);

///* Create statusbar */
//statusbar = gtk_statusbar_new ();
//gtk_table_attach (GTK_TABLE (table),
//        statusbar,
//        /* X direction */       /* Y direction */
//        0, 1,                   4, 5,
//        GTK_EXPAND | GTK_FILL,  0,
//        0,                      0);

/* Show the window */
//gtk_widget_show_all (window);

//    { "New", "document-new", "_New", "<control>N",
//        "Create a new file", G_CALLBACK(activate_action) },
//
//"  <menubar name='MenuBar'>"
//                              "    <menu action='FileMenu'>"
//                              "      <menuitem action='New'/>"
//                              "      <menuitem action='Open'/>"
//                              "      <menuitem action='Save'/>"
//                              "      <separator/>"
//                              "      <menuitem action='Quit'/>"
//                              "    </menu>"
//                              "  </menubar>"

//static void activate(GtkApplication* app, gpointer user_data)
