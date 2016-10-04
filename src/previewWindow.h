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
  std::string preview(char *fileSrc, std::string fileGlade);
  void uploadClick();
  void cancelClick();
  std::string getStatusUpload();
};
