#include <iostream>
#include <gtkmm.h>
#include <gtk/gtk.h>
#include <phpcpp.h>

class GtkPhpPreviewWindow  {

private:
  Gtk::Window *mainWindow = nullptr;
  
protected:
  int statusUpload = 0;
  int statusCancel = 0;

public:
  void preview(char *fileSrc, std::string fileGlade);
  void uploadClick();
  void cancelClick();
  Gtk::Window getMainWindow();
  std::string getStatusUpload();

};
