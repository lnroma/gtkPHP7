#include <phpcpp.h>
#include <iostream>
#include <gtk/gtk.h>
#include <thread>

class Gtk : public Php::Base {
private:
    GtkWidget *_window;
    char *_titleWindow;
    char *_buttonTitle;
    GtkWidget *_button;
    Php::Value callB;

public:
    Php::Value setTitle(Php::Parameters &params);
    Php::Value setButtonTitle(Php::Parameters &params);
    Php::Value createWindow();
    Php::Value setButton();
    static void callback(GtkButton *button, gpointer data);
    Php::Value render();
};

/**
 * set title to window
 * @param params
 * @return Gtk
 */
Php::Value Gtk::setTitle(Php::Parameters &params) {
    std::string title = params[0];
    _titleWindow = new char[title.size() + 1];
    std::copy(title.begin(), title.end(), _titleWindow);
    _titleWindow[title.size()] = '\0';
    return this;
}

/**
 * set button title
 * @param params
 * @return Gtk
 */
Php::Value Gtk::setButtonTitle(Php::Parameters &params) {
    std::string title = params[0];
    _buttonTitle = new char[title.size() + 1];
    std::copy(title.begin(), title.end(), _buttonTitle);
    _buttonTitle[title.size()] = '\0';
    return this;
}

/**
 * create window gtk
 * @return Gtk
 */
Php::Value Gtk::createWindow() {
    int argc = 0;
    char **argv = NULL;
    gtk_init(&argc, &argv);
    _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(_window), _titleWindow);
    gtk_container_set_border_width(GTK_CONTAINER(_window), 50);
    return this;
}

/**
 * set button gtk
 * @return Gtk
 */
Php::Value Gtk::setButton() {
    _button = gtk_button_new_with_label(_buttonTitle);
    gtk_container_add(GTK_CONTAINER(_window), _button);
    std::cout << callB << std::endl;
    g_signal_connect(G_OBJECT(_button), "clicked", G_CALLBACK(&Gtk::callback), G_OBJECT(_window));
    return this;
}

/**
 * callback click
 * @param button
 * @param window
 */
void Gtk::callback(GtkButton *button, gpointer window) {
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_main_quit();
}

/**
 * render alert
 * @return
 */
Php::Value Gtk::render() {
    gtk_widget_show_all(_window);
    gtk_main();
    return true;
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
/**
 *  Function that is called by PHP right after the PHP process
 *  has started, and that returns an address of an internal PHP
 *  strucure with all the details and features of your extension
 *
 *  @return void*   a pointer to an address that is understood by PHP
 */
PHPCPP_EXPORT void *get_module() {
    static Php::Extension extension("gtkphp7", "1.0");
    Php::Class<Gtk> gtk("Gtk");
    gtk.method<&Gtk::setTitle>("setTitle");
    gtk.method<&Gtk::setButtonTitle>("setButtonTittle");
    gtk.method<&Gtk::setButton>("setButton");
    gtk.method<&Gtk::createWindow>("createWindow");
    gtk.method<&Gtk::render>("render");
    extension.add(std::move(gtk));
    // return the extension
    return extension;
}
}
