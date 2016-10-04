#include <phpcpp.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gtkmm.h>
#include "previewWindow.h"
#include "alert.h"

class GtkPhp : public Php::Base {
public:
    Php::Value preview(Php::Parameters &param);
    Php::Value alert(Php::Parameters &param);
};

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
    std::string result = gtkPhpPreview->preview(file,fileGlade);
    return result;
}

/**
 * create alert for call in php
 * @param param
 * @return
 */
Php::Value GtkPhp::alert(Php::Parameters &param) {
    std::string text = param[0];
    GtkAlert *gtkAlert = new GtkAlert();
    gtkAlert->createAlert(text);
    return "ok";
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
    gtk.method<&GtkPhp::preview>("preview");
    gtk.method<&GtkPhp::alert>("alert");
    extension.add(std::move(gtk));
    // return the extension
    return extension;
}
}

