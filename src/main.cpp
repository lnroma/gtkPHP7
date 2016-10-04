#include <phpcpp.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gtkmm.h>
#include "previewWindow.h"

//int statusUpload = 0;
//int statusCancel = 0;
//Gtk::Window *mainWindow = nullptr;

class GtkPhp : public Php::Base {
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
    Php::Value preview(Php::Parameters &param);
    static void callback(GtkButton *button, gpointer data);
    char *strToChar(std::string str);
    Php::Value render();
};

/**
 * converting string to char array
 */
char *strToChar(std::string str) {
    char *charStr;
    charStr = new char[str.size()+1];
    std::copy(str.begin(),str.end(),charStr);
    charStr[str.size()] = '\0';
    return charStr;
}

/**
 * run preview window
 * @param param
 * @return string "upload" | "cancel"
 */
Php::Value GtkPhp::preview(Php::Parameters &param) {
    std::string srcPic = param[0];
    std::string fileGlade = param[1];

    char *file;
    file = new char[srcPic.size() + 1];
    std::copy(srcPic.begin(), srcPic.end(),file);
    file[srcPic.size()] = '\0';

    GtkPhpPreviewWindow *gtkPhpPreview = new GtkPhpPreviewWindow();
    Gtk::Application app = gtkPhpPreview->preview(file,fileGlade);
    app->run(gtkPhpPreview->getMainWindow());
    return getStatusUpload();
}

/**
 * set title to window
 * @param params
 * @return Gtk
 */
Php::Value GtkPhp::setTitle(Php::Parameters &params) {
    std::string title = params[0];
    _titleWindow = new char[title.size() + 1];
    std::copy(title.begin(), title.end(), _titleWindow);
    _titleWindow[title.size()] = '\0';
    return this;
}

/**
 * set button title
 * @param paramss
 * @return GtkPhp
 */
Php::Value GtkPhp::setButtonTitle(Php::Parameters &params) {
    std::string title = params[0];
    _buttonTitle = new char[title.size() + 1];
    std::copy(title.begin(), title.end(), _buttonTitle);
    _buttonTitle[title.size()] = '\0';
    return this;
}

/**
 * create window gtk
 * @return GtkPhp
 */
Php::Value GtkPhp::createWindow() {
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
 * @return GtkPhp
 */
Php::Value GtkPhp::setButton() {
    _button = gtk_button_new_with_label(_buttonTitle);
    gtk_container_add(GTK_CONTAINER(_window), _button);
    std::cout << callB << std::endl;
    g_signal_connect(G_OBJECT(_button), "clicked", G_CALLBACK(&GtkPhp::callback), G_OBJECT(_window));
    return this;
}

/**
 * callback click
 * @param button
 * @param window
 */
void GtkPhp::callback(GtkButton *button, gpointer window) {
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_main_quit();
}

/**
 * render alert
 * @return
 */
Php::Value GtkPhp::render() {
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
    Php::Class<GtkPhp> gtk("GtkPhp");
    gtk.method<&GtkPhp::setTitle>("setTitle");
    gtk.method<&GtkPhp::setButtonTitle>("setButtonTittle");
    gtk.method<&GtkPhp::setButton>("setButton");
    gtk.method<&GtkPhp::createWindow>("createWindow");
    gtk.method<&GtkPhp::render>("render");
    gtk.method<&GtkPhp::preview>("preview");
    extension.add(std::move(gtk));
    // return the extension
    return extension;
}
}

