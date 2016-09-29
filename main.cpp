#include <phpcpp.h>
#include <iostream>
#include <gtk/gtk.h> 
#include <thread>

class getGtk : public Php::Base {

private:
  static Gtk *gtk;

public:   

  static Gtk getClass() {
     if(gtk) {
       return gtk;
     }
     gtk = new Gtk();
     return gtk;
  }
}

class Gtk : public Php::Base
{
private:
    GtkWidget *_window;
    char *_titleWindow;
    char *_buttonTitle;
    GtkWidget *_button;
    Php::Value callB;
    bool buttonPress = false;

public:
   
   Php::Value setTitle(Php::Parameters &params) {
        std::string title = params[0];
	_titleWindow = new char[title.size() + 1];
        std::copy(title.begin(), title.end(), _titleWindow);
        _titleWindow[title.size()] = '\0';
	return this;
   }
   
   Php::Value setButtonTitle(Php::Parameters &params) {
        std::string title = params[0];
        _buttonTitle = new char[title.size() + 1];
        std::copy(title.begin(), title.end(), _buttonTitle);
        _buttonTitle[title.size()] = '\0';
	return this;
   }

   Php::Value createWindow() {
        int argc = 0;
        char **argv = NULL;
        
        gtk_init(&argc, &argv);

	std::cout<<"gtk init!"<<std::endl;

    	_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(_window), _titleWindow);
    	gtk_container_set_border_width (GTK_CONTAINER(_window), 50);
	return this;
   }

   static void asyncTest() {
     std::cout << "async function run" << std::endl;
     //Gtk *gtk = new Gtk();
     Gtk *gtk = getGtk::getClass();
     if (gtk->checkButtonPress()) {
       std::cout << "button pressed" << std::endl;
     }
     //while(true) {
     //   std::cout << "async function" << std::endl;
     //}
   }   

   Php::Value setButton() {
        _button = gtk_button_new_with_label(_buttonTitle);
        gtk_container_add(GTK_CONTAINER(_window), _button);
	std::cout<<callB<<std::endl;
	g_signal_connect(G_OBJECT(_button), "clicked",G_CALLBACK(&Gtk::callback), &callB);
	return this;
   }
   
   Php::Value setCallback(Php::Parameters &params) {
       callB = params[0];   
       return this;
   }
   
   Php::Value checkButtonPress() {
      return buttonPress;
   }
   
   void setButtonPress() {
     buttonPress = true;
   }
//   static void runCallback() {
//  	Gtk::callB(); 
//   }

   static void callback(GtkButton *button, gpointer data) {
	  //Gtk *gtk = new Gtk();
	  Gtk *gtk = getGtk::getClass();
          gtk->setButtonPress();
          if(gtk->checkButtonPress()) {
	    std::cout<<"button pressed"<<std::endl;
          }
	  std::cout<<"callback!"<<std::endl; 
   }
  
   Php::Value render() {
	std::cout << "render function" <<std::endl;
       std::thread func_thread(Gtk::asyncTest);
       gtk_widget_show_all(_window);
       gtk_main();
	func_thread.join();
       return this;
   }
};

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
    PHPCPP_EXPORT void *get_module() 
    {
        static Php::Extension extension("gtkphp7", "1.0");
 	Php::Class<Gtk> gtk("Gtk");
	Php::Class<getGtk> getGtk("getGtk");
 	gtk.method<&Gtk::setTitle> ("setTitle");
        gtk.method<&Gtk::setButtonTitle> ("setButtonTittle");
        gtk.method<&Gtk::setButton> ("setButton");
	gtk.method<&Gtk::createWindow> ("createWindow");
	gtk.method<&Gtk::render> ("render");
	gtk.method<&Gtk::setCallback> ("setCallback");
//	getGtk.method<&getGtk::getClass> ("getClass");
	extension.add(std::move(gtk));
	extension.add(std::move(getGtk));
        // return the extension
        return extension;
    }
}
