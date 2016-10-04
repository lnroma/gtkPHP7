#include <iostream>
#include <gtkmm.h>
#include <gtk/gtk.h>
#include <phpcpp.h>

class GtkAlert {
private:
    Gtk::Window *mainWindow = nullptr;
public:
    void createAlert(std::string text);
    void clickCallback();
};