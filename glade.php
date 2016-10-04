<?php
  $gtk = new GtkPhp();
  $chose = $gtk->preview("/tmp/1475520491674_screen.png","test.glade");
  var_dump($gtk->alert("testing thsi is symple text for alert"));
  var_dump($chose);
//$gtk->preview("/");
