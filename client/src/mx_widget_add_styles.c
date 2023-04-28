#include "../inc/client.h"

void mx_widget_add_styles(GtkWidget *widget) {

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, STYLES_FILE_PATH, NULL);
    GtkStyleContext *widget_ctx = gtk_widget_get_style_context(widget);

    gtk_style_context_add_provider(widget_ctx, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref(provider);
}
