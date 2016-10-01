<?php
  $gtk = new Gtk;
  $buttonId = $gtk
    ->setTitle("testing windows for habr")
    ->setButtonTittle("testing")
    ->createWindow()
    ->setButton()
    ->render();
  
var_dump($buttonId);
