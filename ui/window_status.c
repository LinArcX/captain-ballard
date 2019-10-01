#include <gtk/gtk.h>

#include "../util/c/cb_vector.h"
enum {
    COL_PATH_STATUS = 0,
    NUM_COLS_STATUS
};

GtkListStore* store_status;

gboolean foreach_func_remove_all(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, GList** rowref_list)
{
    g_assert(rowref_list != NULL);
    GtkTreeRowReference* rowref;
    rowref = gtk_tree_row_reference_new(model, path);
    *rowref_list = g_list_append(*rowref_list, rowref);

    // gtk_tree_model_get(model, iter, COL_PATH_STATUS, &year_of_birth, -1);
    // if (year_of_birth > 10) {
    // }
    return FALSE; /* do not stop walking the store, call us with next row */
}

void remove_all_status_data(void)
{
    GList* rr_list = NULL;
    GList* node; /* list of GtkTreeRowReferences to remove */

    gtk_tree_model_foreach(GTK_TREE_MODEL(store_status), (GtkTreeModelForeachFunc)foreach_func_remove_all, &rr_list);
    for (node = rr_list; node != NULL; node = node->next) {
        GtkTreePath* path;
        path = gtk_tree_row_reference_get_path((GtkTreeRowReference*)node->data);
        if (path) {
            GtkTreeIter iter;
            if (gtk_tree_model_get_iter(GTK_TREE_MODEL(store_status), &iter, path)) {
                gtk_list_store_remove(store_status, &iter);
            }
            /* FIXME/CHECK: Do we need to free the path here? */
        }
    }
    g_list_foreach(rr_list, (GFunc)gtk_tree_row_reference_free, NULL);
    g_list_free(rr_list);
}

void close_status_window(GtkWidget* widget, gpointer data)
{
    gtk_window_close(widget);
}

void combo_selected(GtkWidget* widget, gpointer user_data)
{
    GtkTreeIter iter_status;
    remove_all_status_data();
    gchar*** all_files = user_data;
    gchar* text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX(widget));

    if (all_files) {
        for (size_t i = 0; i < vector_size(all_files); i++) {
            if (!strcmp(all_files[i][0], text)) {
                for (size_t j = 0; j < vector_size(all_files[i]); j++) {
                    printf("unstaged[%lu][%lu] = %s\n", i, j, all_files[i][j]);
                    gtk_list_store_append(store_status, &iter_status);
                    gtk_list_store_set(store_status, &iter_status, COL_PATH_STATUS, all_files[i][j], -1);
                }
            }
        }
    }

    if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store_status), &iter_status, NULL, 0)) {
        gtk_list_store_remove(store_status, &iter_status);
    }

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

    GtkWidget* view_status;
    GtkCellRenderer* renderer;
    GtkTreeModel* model_status;

    view_status = gtk_tree_view_new();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view_status), -1, "PATH",
            renderer, "text", COL_PATH_STATUS, NULL);

    model_status = create_fill_model_status();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view_status), model_status);

    /* The tree view has acquired its own reference to the model, so we can drop ours. That way the model will be freed automatically when the tree view is destroyed */
    g_object_unref(model_status);
    return view_status;
}

void show_status_window(char*** user_data) //(GtkApplication* app, gpointer user_data)
{
    gchar*** all_files = user_data;

    GtkWidget *view, *scrolled_win, *hbox, *vbox;
    GtkWidget *combo, *btn_add, *btn_commit, *btn_push;
    GtkWidget* status_window;
    GtkApplication* status_app;

    status_app = gtk_application_new("captain_ballard.status_window", G_APPLICATION_FLAGS_NONE);

    /* create a new window, and set its title */
    status_window = gtk_application_window_new(status_app);
    gtk_window_set_title(GTK_WINDOW(status_window), "Status");
    gtk_container_set_border_width(GTK_CONTAINER(status_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(status_window), 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(status_window), TRUE);
    gtk_window_set_position(status_window, GTK_WIN_POS_CENTER);
    //gtk_image_new_from_resource("/io/linarcx/captain_ballard/about_us.png");

    static GdkPixbuf *window_icon = NULL;
    GInputStream *stream = g_resources_open_stream ("/io/linarcx/captain_ballard/captain-cap.png", 0, NULL);
    if (stream != NULL) {
        window_icon = gdk_pixbuf_new_from_stream (stream, NULL, NULL);
        g_object_unref (stream);
    }
    gtk_window_set_icon(status_window, window_icon);

    //gtk_window_set_icon_from_file(status_window, "/io/linarcx/captain_ballard/about_us.png", NULL);//"../util/images/captain-cap.png", NULL);

    // buttons and their handlers
    combo = gtk_combo_box_text_new();
    g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_selected), all_files);

    btn_add = gtk_button_new_with_label("Add");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_files), NULL);

    btn_commit = gtk_button_new_with_label("Commit");
    g_signal_connect_swapped(btn_commit, "clicked", G_CALLBACK(commit_files), NULL);

    btn_push = gtk_button_new_with_label("Push");
    g_signal_connect(btn_push, "clicked", G_CALLBACK(push_files), NULL);

    //gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "sa");
    if (all_files) {
        for (size_t i = 0; i < vector_size(all_files); i++) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), all_files[i][0]);
            printf("item: %s", all_files[i][0]);
        }
    }

    // scrolled view
    view = create_view_model_status();
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_win), view);

    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), combo, TRUE, TRUE, 1);
    //gtk_box_pack_start(GTK_BOX(hbox), btn_add, TRUE, TRUE, 1);
    //gtk_box_pack_start(GTK_BOX(hbox), btn_commit, TRUE, TRUE, 1);
    //gtk_box_pack_start(GTK_BOX(hbox), btn_push, TRUE, TRUE, 1);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(status_window), vbox);

    gtk_combo_box_set_active(combo, 0);
    gtk_widget_show_all(status_window);
    g_object_unref(status_app);

    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(window, "delete_event", gtk_main_quit, NULL);

    //gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 1);
    //btn_close = gtk_button_new_with_label("Close");
    //g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(close_status_window), status_window); //gtk_widget_destroy
}
