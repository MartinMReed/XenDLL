Interactive Map (Eye_Map) +Source: (Updated: 01/08/2007)

Memory Addresses Used:

   1. "Location X"
   2. "Location Y"
   3. "Chatmode"
   4. "Screen"



Config File Options:

   1. "MenuItemIO" = extension is active or not
   2. "Hide/Show Using DIK_[Key]" = hide / show the map display
   3. "DisplayMoveAlways" = (map display) always moveable when true, only moveable when using the move control in the menu if false
   4. "DisplaySizeScale" = (map display) scale larger or smaller
   5. "MapButtonText[0-2]" = text to put on the button
   6. "MapFile[0-2] Filename" = name of the map file
   7. "Map[0-2] File2Game X" = variable used to convert map coordinates to game coordinates (fileX * ____ = gameX)
   8. "Map[0-2] File2Game Y" = variable used to convert map coordinates to game coordinates (fileY * ____ = gameY)
   9. "DisplayX" = x position of the display on the screen
  10. "DisplayY" = y position of the display on the screen



Description / Notes:

   1. can use up to three maps (0,1,2)
   2. map-to-game coordinate conversion must be set in the config file
   3. maps are not provided, the default map is EJ's
   4. map display only shows up when in game
   5. hotkey to hide/show the display only works when not typing in chatmode