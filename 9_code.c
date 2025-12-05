// gtk_calc.c (very small calculator)
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

static GtkWidget *entry;

static void on_button(GtkWidget *w, gpointer data) {
    const char *lab = (const char*)data;
    if (strcmp(lab, "=") == 0) {
        const char *expr = gtk_entry_get_text(GTK_ENTRY(entry));
        // WARNING: very crude evaluator: supports single operator expressions like "12+3"
        double a=0,b=0; char op=0;
        sscanf(expr, "%lf%c%lf", &a, &op, &b);
        double res=0;
        if (op=='+') res=a+b;
        else if (op=='-') res=a-b;
        else if (op=='*') res=a*b;
        else if (op=='/') res=(b!=0)?a/b:0;
        char out[64]; snprintf(out, sizeof(out), "%g", res);
        gtk_entry_set_text(GTK_ENTRY(entry), out);
    } else if (strcmp(lab, "C")==0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else {
        const char *cur = gtk_entry_get_text(GTK_ENTRY(entry));
        char tmp[256]; snprintf(tmp, sizeof(tmp), "%s%s", cur, lab);
        gtk_entry_set_text(GTK_ENTRY(entry), tmp);
    }
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Calc");
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    GtkWidget *grid = gtk_grid_new();
    const char *labels[] = {"7","8","9","/",
                            "4","5","6","*",
                            "1","2","3","-",
                            "0",".","C","+" ,
                            "="};
    int idx=0;
    for (int r=0;r<5;r++) {
        for (int c=0;c<4;c++) {
            if (r==4 && c>0) continue;
            if (idx >= (int)(sizeof(labels)/sizeof(labels[0]))) break;
            GtkWidget *btn = gtk_button_new_with_label(labels[idx]);
            g_signal_connect(btn, "clicked", G_CALLBACK(on_button), (gpointer)labels[idx]);
            gtk_grid_attach(GTK_GRID(grid), btn, c, r, 1, 1);
            idx++;
        }
    }
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
