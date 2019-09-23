#include "../libs/sqlite/sqlite_util.h"
#include "../util/cb_vector.h"
#include <gtk/gtk.h>

enum {
    COL_PATH = 0,
    COL_ID,
    NUM_COLS
};

enum {
    COL_PATH_STATUS = 0,
    NUM_COLS_STATUS
};

static int i = 0;
gchar* full_address;

GtkWidget* view;
GtkTreeIter iter;
GtkTreeModel* model;
GtkListStore* store;

GtkWidget* view_status;
GtkTreeIter iter_status;
GtkTreeModel* model_status;
GtkListStore* store_status;

gboolean list_store_remove_nth_row(GtkListStore* store, gint n)
{
    GtkTreeIter iter;
    g_return_val_if_fail(GTK_IS_LIST_STORE(store), FALSE);
    /* NULL means the parent is the virtual root node, so the
     n-th top-level element is returned in iter, which is
     the n-th row in a list store (as a list store only has
     top-level elements, and no children) */
    if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, n)) {
        gtk_list_store_remove(store, &iter);
        return TRUE;
    }
    return FALSE;
}

GtkWidget* create_filechooser_dialog(char* init_path,
    GtkFileChooserAction action)
{
    GtkWidget* wdg = NULL;
    switch (action) {
    case GTK_FILE_CHOOSER_ACTION_SAVE:
        wdg = gtk_file_chooser_dialog_new("Save file", NULL, action, "Cancel",
            GTK_RESPONSE_CANCEL, "Save",
            GTK_RESPONSE_OK, NULL);
        break;

    case GTK_FILE_CHOOSER_ACTION_OPEN:
        wdg = gtk_file_chooser_dialog_new("Open file", NULL, action, "Cancel",
            GTK_RESPONSE_CANCEL, "Open",
            GTK_RESPONSE_OK, NULL);
        break;

    case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER:
        wdg = gtk_file_chooser_dialog_new("Select folder", NULL, action, "Cancel",
            GTK_RESPONSE_CANCEL, "Open",
            GTK_RESPONSE_OK, NULL);
        break;

    case GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER:
        break;
    }
    return wdg;
}

gboolean foreach_func(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer user_data)
{
    gchar *c_path, *tree_path_str;
    /* Note: here we use ’iter’ and not ’&iter’, because we did not allocate
     the iter on the stack and are already getting the pointer to a tree iter */

    /* gtk_tree_model_get made copies of the strings for us when retrieving them */
    gtk_tree_model_get(model, iter, COL_PATH, &c_path, -1);
    tree_path_str = gtk_tree_path_to_string(path);

    add_project_path(full_address, c_path);
    //g_print("Row %s: %s %s, born %u\n", tree_path_str, first_name, last_name, year_of_birth);

    g_free(tree_path_str);
    g_free(c_path);
    return FALSE; /* do not stop walking the store, call us with next row */
}

static GtkTreeModel* create_and_fill_model(void)
{
    store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

    ///* Append a row and fill in some data */
    //gtk_list_store_append(store, &iter);
    //gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, "Heinz El-Mann", -1);

    //gtk_list_store_append(store, &iter);
    //gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, "Jane Doe", -1);

    //gtk_list_store_append(store, &iter);
    //gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, "Joe Bungop", -1);

    return GTK_TREE_MODEL(store);
}

static GtkWidget* create_view_and_model(void)
{
    GtkCellRenderer* renderer;
    view = gtk_tree_view_new();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "ID",
        renderer, "text", COL_ID, NULL);

    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "PATH",
        renderer, "text", COL_PATH, NULL);

    model = create_and_fill_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    /* The tree view has acquired its own reference to the
     model, so we can drop ours. That way the model will
     be freed automatically when the tree view is destroyed */
    g_object_unref(model);
    return view;
}

void add_new_path(GtkWidget* widget, gpointer data)
{
    GtkWidget* wdg;
    char* dir_name = "";

    wdg = create_filechooser_dialog(dir_name,
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
    if (gtk_dialog_run(GTK_DIALOG(wdg)) == GTK_RESPONSE_OK) {
        gchar* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(wdg));
        printf("%s", path);
        /* Append a row and fill in some data */
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, COL_ID, i++, COL_PATH, path, -1);
        g_print("Item added!\n");
    }
    g_object_unref(wdg);
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

void save_path(GtkWidget* widget, gpointer data)
{
    full_address = data;
    g_print("data is: %s\n", full_address);
    create_settings_table(full_address);
    wipe_out_table(full_address);
    gtk_tree_model_foreach(GTK_TREE_MODEL(store), foreach_func, NULL);
}

static void activate(GtkApplication* app, gpointer user_data)
{
    gchar* full_address = user_data;

    GtkWidget *window, *view, *scrolled_win, *hbox, *vbox;
    GtkWidget *btn_close, *btn_add, *btn_remove, *btn_save;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Add new path");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    // buttons and their handlers
    btn_close = gtk_button_new_with_label("Close");
    g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_widget_destroy),
        window);

    btn_remove = gtk_button_new_with_label("Remove");
    g_signal_connect_swapped(btn_remove, "clicked", G_CALLBACK(remove_path),
        NULL);

    btn_add = gtk_button_new_with_label("Add");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_new_path), NULL);

    btn_save = gtk_button_new_with_label("Save");
    g_signal_connect(btn_save, "clicked", G_CALLBACK(save_path), full_address);

    // scrolled view
    view = create_view_and_model();
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_win), view);

    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_remove, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_add, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_save, TRUE, TRUE, 1);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(window, "delete_event", gtk_main_quit, NULL);
    gtk_widget_show_all(window);
}

void combo_selected(GtkWidget* widget, gpointer user_data)
{
    gchar*** all_files = user_data;
    gchar* text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX(widget));

    //if (all_files) {
    //    for (size_t i = 0; i < vector_size(all_files); i++) {
    //        gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), all_files[i][0]);
    //    }
    //}

    gtk_list_store_append(store_status, &iter_status);
    gtk_list_store_set(store_status, &iter_status, COL_PATH_STATUS, text, -1);

    //gtk_label_set_text(GTK_LABEL(window), text);
    g_free(text);
}

void add_files(GtkWidget* widget, gpointer data)
{
}

void commit_files(GtkWidget* widget, gpointer data)
{
}

void push_files(GtkWidget* widget, gpointer data)
{
}

static GtkTreeModel* create_fill_model_status(void)
{
    store_status = gtk_list_store_new(NUM_COLS_STATUS, G_TYPE_STRING, G_TYPE_UINT);
    return GTK_TREE_MODEL(store_status);
}

static GtkWidget* create_view_model_status(void)
{
    GtkCellRenderer* renderer;
    view_status = gtk_tree_view_new();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view_status), -1, "PATH",
        renderer, "text", COL_PATH_STATUS, NULL);

    model_status = create_fill_model_status();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view_status), model_status);

    /* The tree view has acquired its own reference to the
     model, so we can drop ours. That way the model will
     be freed automatically when the tree view is destroyed */
    g_object_unref(model_status);
    return view_status;
}

static void show_status_activate(GtkApplication* app, gpointer user_data)
{
    gchar*** all_files = user_data;

    GtkWidget *window, *view, *scrolled_win, *hbox, *vbox;
    GtkWidget* combo;
    GtkWidget *btn_close, *btn_add, *btn_commit, *btn_push;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Captain Ballard");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    // buttons and their handlers
    combo = gtk_combo_box_text_new();
    g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_selected), all_files);

    btn_close = gtk_button_new_with_label("Close");
    g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    btn_add = gtk_button_new_with_label("Add");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_files), NULL);

    btn_commit = gtk_button_new_with_label("Commit");
    g_signal_connect_swapped(btn_commit, "clicked", G_CALLBACK(commit_files), NULL);

    btn_push = gtk_button_new_with_label("Push");
    g_signal_connect(btn_push, "clicked", G_CALLBACK(push_files), full_address);

    if (all_files) {
        for (size_t i = 0; i < vector_size(all_files); i++) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), all_files[i][0]);
        }
    }

    // scrolled view
    view = create_view_model_status();
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_win), view);

    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), combo, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_add, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_commit, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(hbox), btn_push, TRUE, TRUE, 1);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "delete_event", gtk_main_quit, NULL);
    gtk_widget_show_all(window);
}

int show_launcher_window(char* full_address)
{
    GtkApplication* app;
    int status;

    app = gtk_application_new("com.github.linarcx", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), full_address);
    status = g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);
    return status;
}

int show_status_window(char*** all_files)
{
    GtkApplication* app;
    int status;

    app = gtk_application_new("com.github.linarcx", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(show_status_activate), all_files);
    status = g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);

    return status;
}

//for (size_t j = 0; j < vector_size(all_files[i]); j++) {
//printf("unstaged[%lu][%lu] = %s\n", i, j, all_files[i][j]);
//}

//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "Arch");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "Fedora");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "Mint");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "Gentoo");
//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "Debian");

//////////////////////////////////////////////////////

//hbox = gtk_hbox_new(FALSE, 0);
//vbox = gtk_vbox_new(FALSE, 15);

//gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 0);

//label = gtk_label_new("...");
//gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

//gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
//gtk_container_add(GTK_CONTAINER(window), hbox);
