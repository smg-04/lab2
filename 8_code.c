// gtk_hello.c (GTK3)
#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *w, gpointer d) {
    g_print("Button clicked!\n");
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "GTK Hello");
    gtk_container_set_border_width(GTK_CONTAINER(win), 10);

    GtkWidget *btn = gtk_button_new_with_label("Click me");
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(win), btn);
    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
