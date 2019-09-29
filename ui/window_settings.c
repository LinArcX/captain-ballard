#include <dirent.h>
#include <errno.h>
#include <gtk/gtk.h>

#include "../libs/sqlite/sqlite_util.h"
#include "../util/c/cb_vector.h"

#define LOG_FILE "/home/linarcx/captain_ballard.log"

enum {
    COL_PATH = 0,
    COL_ID,
    NUM_COLS
};

static int i = 0;
gchar* db_path;

GtkApplication* app;
GtkWidget* window;
GtkWidget* combo;

GtkWidget* view;
GtkTreeIter iter;
GtkTreeModel* model;
GtkListStore* store;
GtkWidget* settings_tabs;

void close_settings_window(GtkWidget* widget, gpointer data)
{
    gtk_window_close(widget);
}

GtkWidget* create_filechooser_dialog(char* init_path,
        GtkFileChooserAction action)
{
    GtkWidget* file_chooser_dialog = NULL;
    switch (action) {
        case GTK_FILE_CHOOSER_ACTION_SAVE:
            file_chooser_dialog = gtk_file_chooser_dialog_new("Save file", NULL, action, "Cancel",
                    GTK_RESPONSE_CANCEL, "Save",
                    GTK_RESPONSE_OK, NULL);
            break;

        case GTK_FILE_CHOOSER_ACTION_OPEN:
            file_chooser_dialog = gtk_file_chooser_dialog_new("Open file", NULL, action, "Cancel",
                    GTK_RESPONSE_CANCEL, "Open",
                    GTK_RESPONSE_OK, NULL);
            break;

        case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER:
            file_chooser_dialog = gtk_file_chooser_dialog_new("Select folder", NULL, action, "Cancel",
                    GTK_RESPONSE_CANCEL, "Open",
                    GTK_RESPONSE_OK, NULL);
            break;

        case GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER:
            break;
    }
    return file_chooser_dialog;
}

gboolean save_path_func(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer user_data)
{
    gchar *c_path, *tree_path_str;
    /* Note: here we use ’iter’ and not ’&iter’, because we did not allocate the iter on the stack and are already getting the pointer to a tree iter */

    /* gtk_tree_model_get made copies of the strings for us when retrieving them */
    gtk_tree_model_get(model, iter, COL_PATH, &c_path, -1);
    tree_path_str = gtk_tree_path_to_string(path);

    insert_path(db_path, c_path);
    //g_print("Row %s: %s %s, born %u\n", tree_path_str, first_name, last_name, year_of_birth);

    g_free(tree_path_str);
    g_free(c_path);
    return FALSE; /* do not stop walking the store, call us with next row */
}

static GtkTreeModel* create_and_fill_model(void)
{
    store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
    //gtk_list_store_append(store, &iter);
    //gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, "Heinz El-Mann", -1);
    return GTK_TREE_MODEL(store);
}

static GtkWidget* create_view_and_model(void)
{
    GtkCellRenderer* renderer;
    view = gtk_tree_view_new();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "ID", renderer, "text", COL_ID, NULL);

    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "PATH", renderer, "text", COL_PATH, NULL);

    model = create_and_fill_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    g_object_unref(model);
    return view;
}

void add_new_path(GtkWidget* widget, gpointer data)
{
    GtkWidget* file_chooser_dialog;
    char* dir_name = "";

    file_chooser_dialog = create_filechooser_dialog(dir_name, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
    if (gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) == GTK_RESPONSE_OK) {
        gchar* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));

        char git_address[200] = "";
        char* git_dir = "/.git/";

        strcat(git_address, path);
        strcat(git_address, git_dir);

        DIR* dir = opendir(git_address);
        if (dir) {
            /* Directory exists. */
            printf("%s", path);
            /* Append a row and fill in some data */
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, path, -1);
            g_print("Item added!\n");
            closedir(dir);
        } else if (ENOENT == errno) {
            /* Directory does not exist. */
            char* message = "Selected directory isn't a .git dir!\n";

            GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s", message);
            gtk_window_set_title(GTK_WINDOW(dialog), "Warning");

            if (dialog) {
                g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
                gtk_widget_show_all(dialog);
            }

        } else {
            /* opendir() failed for some other reason. */
            g_print("opendir() failed!\n");
        }
    }
    g_object_unref(file_chooser_dialog);
}

void remove_path(GtkWidget* widget, gpointer data)
{
    GtkTreeSelection* selection;
    GtkTreeIter iter;

    /* This will only work in single or browse selection mode! */
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar* name;
        gtk_tree_model_get(model, &iter, COL_PATH, &name, -1);
        g_print("selected row is: %s\n", name);
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        g_free(name);
    } else {
        g_print("no row selected.\n");
    }
}

void save_settings(GtkWidget* widget, gpointer data)
{
    gint has_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(store), NULL);
    if (has_rows) {
        char* db_path = data;
        gchar* period;
        period = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX(combo));

        // Settings
        create_table(db_path, "CREATE TABLE settings("
                "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
                "period   TEXT NOT NULL);");
        wipe_table(db_path, "DELETE from settings;");
        insert_settings(db_path, period);

        // Projects
        create_table(db_path, "CREATE TABLE projects("
                "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
                "path TEXT    NOT NULL);");
        wipe_table(db_path, "DELETE from projects;");

        //gtk_list_store
        gtk_tree_model_foreach(GTK_TREE_MODEL(store), save_path_func, NULL);

    } else {
        char* message = "Error! Please select at least one .git directory!";
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s", message);
        gtk_window_set_title(GTK_WINDOW(dialog), "Warning");

        if (dialog) {
            g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
            gtk_widget_show_all(dialog);
        }
    }
}

void create_settings_tab(char* db_path)
{
    int index = 0;
    char* period;
    char** vector_periods = NULL;

    vector_push_back(vector_periods, "1");
    vector_push_back(vector_periods, "5");
    vector_push_back(vector_periods, "10");
    vector_push_back(vector_periods, "15");
    vector_push_back(vector_periods, "20");
    vector_push_back(vector_periods, "30");
    vector_push_back(vector_periods, "45");
    vector_push_back(vector_periods, "60");
    vector_push_back(vector_periods, "45");
    vector_push_back(vector_periods, "90");
    vector_push_back(vector_periods, "100");
    vector_push_back(vector_periods, "120");
    vector_push_back(vector_periods, "150");
    vector_push_back(vector_periods, "180");
    vector_push_back(vector_periods, "210");
    vector_push_back(vector_periods, "240");
    vector_push_back(vector_periods, "300");

    // Read "period" vale from database and update combobox, but before that check if database exits or not!
    FILE* fp;
    if ((fp = fopen(db_path, "r")) != NULL) {
        period = get_period(db_path);
        if (vector_periods) {
            size_t i;
            for (i = 0; i < vector_size(vector_periods); ++i) {
                if (!strcmp(period, vector_periods[i])) {
                    index = i;
                    //printf("v[%d] = %s\n", i, v[i]);
                }
            }
        }
    }

    GtkWidget *scrolled_win, *vbox;
    GtkWidget *hbox_internal, *vbox_internal, *label_period;

    // Title for page settings
    GtkWidget* title_settings_tab = gtk_label_new("Settings");

    hbox_internal = gtk_hbox_new(FALSE, 0);
    vbox_internal = gtk_vbox_new(FALSE, 15);

    // buttons and their handlers
    combo = gtk_combo_box_text_new();

    if (vector_periods) {
        size_t i;
        for (i = 0; i < vector_size(vector_periods); ++i) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), vector_periods[i]);
        }
    }

    vector_free(vector_periods);

    label_period = gtk_label_new("Show status page every: (minute) ");

    gtk_box_pack_start(GTK_BOX(hbox_internal), label_period, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_internal), combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_internal), hbox_internal, FALSE, FALSE, 0);

    // scrolled view
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_win), vbox_internal);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);

    // append page to notebook
    gtk_combo_box_set_active(combo, index);
    gtk_notebook_append_page(GTK_NOTEBOOK(settings_tabs), vbox, title_settings_tab);
}

void create_paths_tab()
{
    GtkWidget *view, *scrolled_win, *hbox, *vbox;
    GtkWidget *btn_add, *btn_remove;

    view = create_view_and_model();

    // Read "period" vale from database and update combobox, but before that check if database exits or not!
    FILE* fp;
    if ((fp = fopen(db_path, "r")) != NULL) {
        char** paths = NULL;
        paths = get_projects(db_path);

        if (paths) {
            size_t i;
            for (i = 0; i < vector_size(paths); ++i) {
                //log_message(LOG_FILE, paths[i]);
                //log_message(LOG_FILE, "\n");

                /* Append a row and fill in some data */
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, COL_ID, 0, COL_PATH, paths[i], -1);
            }
        }
    }

    // Title for page settings
    GtkWidget* title_git_folders_tab = gtk_label_new("Git folders");

    // buttons and their handlers
    btn_add = gtk_button_new_with_label("Add");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_new_path), NULL);

    btn_remove = gtk_button_new_with_label("Remove");
    g_signal_connect_swapped(btn_remove, "clicked", G_CALLBACK(remove_path), NULL);

    // scrolled view
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_win), view);

    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), btn_add, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_remove, TRUE, TRUE, 1);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

    // append page to notebook
    gtk_notebook_append_page(GTK_NOTEBOOK(settings_tabs), vbox, title_git_folders_tab);
}

void show_settings_window(char* db_path)
{
    GtkWidget *vbox_settings, *btn_save;

    app = gtk_application_new("captaine_ballard.launcher_window", G_APPLICATION_FLAGS_NONE);

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Add new path");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(window, GTK_WIN_POS_CENTER);
    gtk_window_set_icon_from_file(window, "../util/images/captain-cap.png", NULL);

    /* Create settings_tab */
    settings_tabs = gtk_notebook_new();

    btn_save = gtk_button_new_with_label("Save");
    g_signal_connect(btn_save, "clicked", G_CALLBACK(save_settings), db_path);

    vbox_settings = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_settings), settings_tabs, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_settings), btn_save, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox_settings);

    // Create tabs
    create_paths_tab();
    create_settings_tab(db_path);

    // Show Settings window
    gtk_widget_show_all(window);
    g_object_unref(app);
}

//static void activate(GtkApplication* app, gpointer user_data)
//g_signal_connect(window, "delete_event", gtk_main_quit, NULL);

//gboolean list_store_remove_nth_row(GtkListStore* store, gint n)
//{
//    GtkTreeIter iter;
//    g_return_val_if_fail(GTK_IS_LIST_STORE(store), FALSE);
//    /* NULL means the parent is the virtual root node, so the
//       n-th top-level element is returned in iter, which is
//       the n-th row in a list store (as a list store only has
//       top-level elements, and no children) */
//    if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, n)) {
//        gtk_list_store_remove(store, &iter);
//        return TRUE;
//    }
//    return FALSE;
//}

//void save_path(GtkWidget* widget, gpointer data)
//{
//    // char* db_path = data;
//    // g_print("database path is: %s\n", db_path);
//
//    // create_table(db_path, "CREATE TABLE projects("
//    //                       "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
//    //                       "path TEXT    NOT NULL);");
//
//    // wipe_table(db_path, "DELETE from projects;");
//
//    // gtk_tree_model_foreach(GTK_TREE_MODEL(store), save_path_func, NULL);
//}

//gchar* db_path = user_data;
//btn_save = gtk_button_new_with_label("Save");
//g_signal_connect(btn_save, "clicked", G_CALLBACK(save_path), db_path);

//btn_close = gtk_button_new_with_label("Close");
//g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(close_settings_window), window); //gtk_widget_destroy

//gtk_box_pack_start(GTK_BOX(hbox), btn_save, TRUE, TRUE, 1);
//gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 1);
//gtk_container_add(GTK_CONTAINER(window), vbox);

///////////////////
//gtk_container_add(GTK_CONTAINER(window), hbox);
//btn_save = gtk_button_new_with_label("Save");
//g_signal_connect(btn_save, "clicked", G_CALLBACK(save_settings), db_path);
//btn_close = gtk_button_new_with_label("Close");
//g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(close_settings_window), window); //gtk_widget_destroy
//view = create_view_and_model();
//hbox = gtk_hbox_new(FALSE, 5);
//gtk_box_pack_start(GTK_BOX(hbox), btn_save, TRUE, TRUE, 1);
//gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 1);
//gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
//gtk_container_add(GTK_CONTAINER(window), vbox);

//void combo_period_selected(GtkWidget* widget, gpointer user_data)
//{
//    combo_period = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX(widget));
//    g_free(combo_period);
//}

/////////////////////
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "1");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "5");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "10");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "15");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "20");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "30");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "45");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "60");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "75");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "90");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "100");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "120");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "150");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "180");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "210");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "240");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "300");
//g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_period_selected), NULL);

/////////////////////
//g_print("row: %s", number_of_rows);

// GtkTreeIter iter;

// /* This will only work in single or browse selection mode! */
// selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
// if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
//     gchar* name;
//     gtk_tree_model_get(model, &iter, COL_PATH, &name, -1);
//     g_print("selected row is: %s\n", name);
//     gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
//     g_free(name);
// }
// //gboolean i = gtk_tree_model_iter_next(GTK_TREE_MODEL(model), iter);
