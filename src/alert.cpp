#include "alert.h"

/**
 * callback click to button
 */
void GtkAlert::clickCallback() {
    delete mainWindow;
}

/**
 * create alert
 * @param text
 */
void GtkAlert::createAlert(std::string text) {
    int argc = 0;
    char **argv = NULL;

    auto app = Gtk::Application::create(argc,argv,"org.gtkmm.gtkphp7");
    auto refBuilder = Gtk::Builder::create();

    try {
        refBuilder->add_from_file("alert.glade");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
    }

    refBuilder->get_widget("dialog1",mainWindow);

    Gtk::Button *buttonCancel = nullptr;
    refBuilder->get_widget("ok",buttonCancel);

    buttonCancel->signal_clicked().connect(
            sigc::mem_fun(*this,&GtkAlert::clickCallback)
    );

    Gtk::Label *label = nullptr;
    refBuilder->get_widget("text",label);

    label->set_text(text);

    app->run(*mainWindow);
}
