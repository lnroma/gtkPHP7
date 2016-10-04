#include "previewWindow.h"

/**
 * callback upload button
 */
void GtkPhpPreviewWindow::uploadClick() {
    statusUpload = 1;
}

/**
 * callback cancel button
 */
void GtkPhpPreviewWindow::cancelClick() {
    statusCancel = 1;
}

/**
 * run preview window
 */
void GtkPhpPreviewWindow::preview(char *fileSrc, std::string fileGlade) {
   int argc = 0;
   char **argv = NULL;
   
   auto app = Gtk::Application::create(argc,argv,"org.gtkmm.gtkphp7");
   auto refBuilder = Gtk::Builder::create();

   try {
      refBuilder->add_from_file("picview.glade");
   } catch(const Glib::FileError& ex) {
      std::cerr << "FileError: " << ex.what() << std::endl;
   } catch(const Gtk::BuilderError& ex) {
      std::cerr << "BuilderError: " << ex.what() << std::endl;
   }

   Gtk::Image *image = nullptr;
   refBuilder->get_widget("preview",image);
   
   Gtk::Button *buttonUpload = nullptr;
   refBuilder->get_widget("upload",buttonUpload);
   
   buttonUpload->signal_clicked().connect(
       sigc::mem_fun(*this,&GtkPhpPreviewWindow::uploadClick)
   );

   Gtk::Button *buttonCancel = nullptr;
   refBuilder->get_widget("cancel",buttonCancel);
   
   buttonCancel->signal_clicked().connect(
       sigc::mem_fun(*this,&GtkPhpPreviewWindow::cancelClick)
   );
   
   image->set(fileSrc);
 //  app->run(*mainWindow);
}

Gtk::Window GtkPhpPreviewWindow::getMainWindow() {
   return *mainWindow;
}

std::string GtkPhpPreviewWindow::getStatusUpload() {
   if(statusUpload == 1) {
      return "upload";
   } else {
      return "cancel";
   }
}
