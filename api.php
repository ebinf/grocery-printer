<?php

  $pretty = isset($_GET["pretty"]);
  $ascii = array(
    "Ä" => "\xA0",
    "Ö" => "\xA1",
    "Ü" => "\xA2",
    "ß" => "\xA3",
    "Ã" => "\xAE",
    "ë" => "\xAF",
    "é" => "\xB0",
    "è" => "\xB1",
    "ê" => "\xB3",
    "ï" => "\xB4",
    "í" => "\xB5",
    "ì" => "\xB6",
    "î" => "\xB8",
    "ö" => "\xB9",
    "ó" => "\xBA",
    "ò" => "\xBB",
    "ô" => "\xBD",
    "ü" => "\xBE",
    "ú" => "\xBF",
    "ù" => "\xC0",
    "û" => "\xC2",
    "Ñ" => "\xC5",
    "ñ" => "\xC6",
    "Å" => "\xCA",
    "ä" => "\xCD",
    "á" => "\xCD",
    "à" => "\xCF",
    "â" => "\xD1",	
  );

  if ($pretty) {
    echo "\x1B\x1D\x62\x01";
    echo "\x1B\x30";
    echo "\x1B\x1E\x64\x03";
    echo "\x1B\x1E\x72\x00";
    echo "\x1B\x1D\x61\x01";
    echo "\x1B\x69\x01\x01";
    echo "Einkaufszettel\n";
    echo "\x1B\x69\x00\x00";
    echo "Datum: " . date("d.m.Y") . "        Zeit: " . date("H:i:s") . " Uhr\n\n";
    echo "\x1B\x1D\x61\x00";
    echo " Menge |                 Produkt\n";
    echo "-------+----------------------------------------\n";
  } else {
    header("Content-type: application/json");
  }

  $db = file_get_contents("db/000000000.data");
  $matches = [];
  $tasks = [];
  if (preg_match_all("({.+?})", $db, $matches) == 0) {
    if ($pretty) {
      echo "\x1B\x1D\x61\x01";
      echo "Alles erledigt :)\n";
      echo "\x1B\x61\x08";
      die();
    }
    die("[]");
  }
  foreach ($matches[0] as $entry) {
    $jentry = json_decode($entry, true);
    $tasks[$jentry["ID"]] = $jentry["Title"];
    if ($jentry["Done"] || $jentry["Title"] == NULL) {
      unset($tasks[$jentry["ID"]]);
    }
  }
  if (!$pretty) {
    echo json_encode(array_values($tasks));
    die();
  }
  if (sizeof($tasks) == 0) {
    echo "\x1B\x1D\x61\x01";
    echo "Alles erledigt :)\n";
    echo "\x1B\x61\x08";
    die();
  }
  foreach ($tasks as $task) {
    $amount = 1;
    if (strpos($task, " ") !== false) {
      $split = explode(" ", $task, 2);
      if (ctype_digit($split[0][0])) {
        $amount = $split[0];
        $task = $split[1];
      }
    }
    echo strtr(str_pad($amount, 7, " ", STR_PAD_BOTH), $ascii);
    echo "| \x1B\x45";
    echo strtr($task, $ascii);
    echo "\x1B\x46\n";
  }
  echo "\x1B\x61\x08";

?>
