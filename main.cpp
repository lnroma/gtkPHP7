#include <phpcpp.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gtkmm.h>

//int statusUpload = 0;
//int statusCancel = 0;
//Gtk::Window *mainWindow = nullptr;

class GtkPhp : public Php::Base {
private:
    GtkWidget *_window;
    char *_titleWindow;
    char *_buttonTitle;
    GtkWidget *_button;
    GtkWidget *_buttonCancel;
    Php::Value callB;
    Gtk::Window *mainWindow = nullptr;

protected:
    int statusUpload = 0;
    int statusCancel = 0;

public:
    Php::Value setTitle(Php::Parameters &params);
    Php::Value setButtonTitle(Php::Parameters &params);
    Php::Value createWindow();
    Php::Value setButton();
    Php::Value preview(Php::Parameters &param);
    static void callback(GtkButton *button, gpointer data);
    void uploadClick();
    void cancelClick();
    Php::Value render();
};

/**
 * callback upload button
 */
void GtkPhp::uploadClick() {
    statusUpload = 1;
    delete mainWindow;
}

/**
 * callback cancel button
 */
void GtkPhp::cancelClick() {
    statusCancel = 1;
    delete mainWindow;
}

/**
 * run preview window
 * @param param
 * @return string "upload" | "cancel"
 */
Php::Value GtkPhp::preview(Php::Parameters &param) {
    int argc = 0;
    char **argv = NULL;
    char *file = nullptr;

    std::string fileSrc = param[0];
    file = new char[fileSrc.size() + 1];
    std::copy(fileSrc.begin(), fileSrc.end(), file);
    file[fileSrc.size()] = '\0';

    Php::Value srcPic = param[0];

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.gtkphp7");
    auto refBuilder = Gtk::Builder::create();

    try {
        refBuilder->add_from_file("picview.glade");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
    }

    refBuilder->get_widget("window1",mainWindow);

    // preview picture
    Gtk::Image *image = nullptr;
    refBuilder->get_widget("preview",image);
    // button ok click
    Gtk::Button *buttonUpload = nullptr;
    refBuilder->get_widget("upload",buttonUpload);
    //GtkPhp *gtkphp;
    buttonUpload->signal_clicked().connect(sigc::mem_fun(*this,&GtkPhp::uploadClick));
    // button cancel click
    Gtk::Button *buttonCancel = nullptr;
    refBuilder->get_widget("cancel",buttonCancel);
    buttonCancel->signal_clicked().connect(sigc::mem_fun(*this,&GtkPhp::cancelClick));
    image->set(file);
    app->run(*mainWindow);

    if(statusUpload == 1) {
        return "upload";
    } else {
        return "cancel";
    }
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

