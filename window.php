<?php
function alert($title) {
  $gtk = new GtkPhp;
  return $gtk
    ->setTitle($title)
    ->setButtonTittle("Ok")
    ->createWindow()
    ->setButton()
    ->render();
}

if(alert("Hellow habr")) {
   alert("Hellow again");
}
