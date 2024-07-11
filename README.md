# grocery-printer

A very simple solution to make a thermal printer print a grocery list. For more information, check the [project description](https://ebinf.eu/projekt/einkaufszettel-drucker/).

It requires the thermal printer to be connected to the ESP32 via a serial connection. The printer must be compatible with the ESC/POS protocol. Tested with a Star TSP600.

The ESP32 then connects to a WiFi network and requests the list via a HTTP API. The printer will then print the list.

The API can be found in `api.php`. It reads the list from a database generated by [prologic/todo](https://git.mills.io/prologic/todo/src/commit/e0eca7da54978befe459ba3bb6b46a7185a5883a) of 2020.
