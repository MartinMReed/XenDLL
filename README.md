## Project Files
* Computron (macro/bot): located under d3d8.DLL files
 * Keytron: Config editor for Computron
 * Transformer: Script editor for Computron macro/bot
* Download & Installation Guide
* Extensions: Source code for the default extensions shipped with XenDLL
* HG_SDK.DLL: HGooey SDK template project
* HGooey.DLL: Custom DirectX GUI
* Screenshots
* Xenimus: Default installation files
* d3d8.DLL: DirectX DLL hook base code
 * Built-in Extension Files: Built-in extensions, including Computron

## Demo
[YouTube - XenDLL (Computron Edition) Test - Nov 13, 2007](http://www.youtube.com/watch?v=gDUa6F2XrkU)  
[YouTube - ItemImager - Dec 06, 2007](http://www.youtube.com/watch?v=hl5mjyWB49k)

## Extensions

### Computron (Computron) [Built-In]
* Custom macro/bot for leveling.
* Reads script files and executes key presses and mouse clicks. Able to monitor the game environment and execute secondary scripts based on expected variables.
* Ability to look at the current polygon count on the screen to determine if the environment is as expected. Extra characters or monsters on the screen will raise the polygon count. If monsters are missing you may want to move on without casting spells because that can give you away as being a bot.
* Ability to check the current world coordinates. If someone messes with your character and sends you to a different location, you can check the expected world coordinates and execute a fallback script to reset yourself to a safe location (and probably sleep for a while).
* Ability to log your character off and stop execute scripts if you feel the environment has become sketchy or dangerous.
* Demo available on YouTube. Link listed above.

#### Memory Addresses Used
* progressXP
* progressMP
* progressHP
* locationX
* locationY

#### Config File Options
* "MenuItemIO"
 * Options: true, false
 * Default: false
 * Whether the extension is active or not. This will be set automatically everytime the game exits
* "pauseRestartKey"
 * Options: *most* of the DIK keys (ie DIK_Q, DIK_ESCAPE, etc)
 * Default: 0x00 (nothing)
 * For a list of keys, view: XenDLL Files/DIK.html
 * Start from the beginning of the script.
* "pauseResumeKey"
 * Options: *most* of the DIK keys (ie DIK_Q, DIK_ESCAPE, etc)
 * Default: 0x00 (nothing)
 * For a list of keys, view: XenDLL Files/DIK.html
 * Start from where the script was stopped/paused.
* "scriptFilename"
 * Options: Filename of the script to download
 * Default: 
 * Requires logging into the Computron system.
* "scriptHandle"
 * Options: User handle registered on the Computron system.
 * Default: 
* "scriptPassword"
 * Options: User password belonging to the handle registered on the Computron system.
 * Default: 

---

### Item Imager (ItemImager)
* Iterates through every item in the game and captures a screenshot, description and stats.
* Demo available on YouTube. Link listed above.

---

### Auto Click (AutoClick) [Built-In]
* Setting a key for AutoClick will send a left mouse click. The key stroke will be ignored and not repeated while you hold it down.

#### Memory Addresses Used
* none

#### Config File Options
* "MenuItemIO"
 * Options: true, false
 * Default: false
 * Whether the extension is active or not. This will be set automatically everytime the game exits
* "trigger"
 * Options: MButton, LButton, RButton, or *most* of the DIK keys (ie DIK_Q, DIK_ESCAPE, etc)
 * Default: MButton
 * For a list of keys, view: XenDLL Files/DIK.html

---

### Screenshot Saver (Screenshot) [Built-In]
* Screenshots will be saved when using the print screen key.

#### Memory Addresses Used
* none

#### Config File Options
* "filePath"
 * Default: screenshots (in your xenimus directory)
 * The location to save the screenshots.
* "screenshotQuality"
 * Options: 1 - 100
 * Default: 100
 * The quality (percent) of the image taken.
* "includeGUI"
 * Options: true, false
 * Default: true
 * Include the XenDLL GUI objects in the screenshot.

---

### Interactive Map (Eye_Map)
* Display an windowed map and adjust it according to your characters world location.
* Map display only shows up when in game.
* Toggle between three different maps.
* Additional maps are not provided, the default map is EJ's.
* Map-to-Game coordinate conversion must be set in the config file.
* Hotkey to hide/show the display only works when not typing in chatmode.

#### Memory Addresses Used
* "Location X"
* "Location Y"
* "Chatmode"
* "Screen"

#### Config File Options
* "MenuItemIO" = extension is active or not
* "Hide/Show Using DIK_[Key]" = hide / show the map display
* "DisplayMoveAlways" = (map display) always moveable when true, only moveable when using the move control in the menu if false
* "DisplaySizeScale" = (map display) scale larger or smaller
* "MapButtonText[0-2]" = text to put on the button
* "MapFile[0-2] Filename" = name of the map file
* "Map[0-2] File2Game X" = variable used to convert map coordinates to game coordinates (fileX * ____ = gameX)
* "Map[0-2] File2Game Y" = variable used to convert map coordinates to game coordinates (fileY * ____ = gameY)
* "DisplayX" = x position of the display on the screen
* "DisplayY" = y position of the display on the screen

---

### Frames Per Second (Frames_Per_Sec)
* Displays the number of frames drawn per second in the top right corner.

#### Memory Addresses Used
* none

#### Config File Options
* "MenuItemIO" = extension is active or not

---

### Game Crasher (Fire_Exit)
* Crashes the game when the extension's icon in the menu is clicked.

#### Memory Addresses Used
* none

#### Config File Options
* none

---

### Cancel Chat (Safe_Typer)
* When in chatmode, if the left mouse button is pressed, chat will be cancelled.
* Useful for quickly exiting if your character gets attacked.

#### Memory Addresses Used
* "Chatmode"

#### Config File Options
* none

---

### Free Night Vision (Night_Vision)
* Enables constant night vision.
* Slight flicker when Xenimus tries to reset night vision to off... just a side effect of free night vision.

#### Memory Addresses Used
* "Night Vision"

#### Config File Options
* "MenuItemIO" = extension is active or not

---

### Model Changer (Model_Changer)
* Change the models being displayed on your character. This change can only be seen on your screen and not by other players.
* If the model changer loads, and is missing a memory address, the others that have addresses will still work.
* When changing the model numbers, some times you may run into a number or range of numbers that crash the game.
 * To skip over these use the model_changer config options listed above.
 * You can specify as many numbers/ranges for each model type that you want.
  * To do this you must replace the '#' with a number.
  * The first set must be 0 and not 1.
  * Examples of this are created after the first run of the program.
  * Edit at your own risk.
  * The "low" number is the first number skipped, and the "high" number is the last number skipped... they may be the same number.
  * To specify the highest number possible use the Asterisk character \*
* When in-game, after turning off the model changer, to go back to your normal models either switch weapons or log off and back on
* The model ID changer is located under the DLL controller config button (next to the DLL load and unload buttons)

#### Memory Addresses Used
* "Body Model"
* "Helmet Model"
* "Weapon Model"
* "Shield Model"
* "Spell Model"

#### Config File Options
* "MenuItemIO" = extension is active or not
* "Critical Body Low #" = see description / notes
* "Critical Body High #" = see description / notes
* "Critical Helmet Low #" = see description / notes
* "Critical Helmet High #" = see description / notes
* "Critical Weapon Low #" = see description / notes
* "Critical Weapon High #" = see description / notes
* "Critical Shield Low #" = see description / notes
* "Critical Shield High #" = see description / notes
* "Critical Spell Low #" = see description / notes
* "Critical Spell High #" = see description / notes
* "BodyID" = last used body model id
* "HelmetID" = last used helmet model id
* "WeaponID" = last used weapon model id
* "ShieldID" = last used shield model id
* "SpellID" = last used spell model id

## Keytron
* Script editor for Computron.
* Screenshot included int the Screenshot directory.

## Transformer
* Included in version 39... the last version was 41, but it seems to be lost.
* Transformer is the script editor used with Computron.
* The main version is the EXE, while the PHP version was made later but never really used. The Computron extension source may make references to logging in or accessing the Transformer PHP site, however the main use was with the EXE scripts and no login system.
* Screenshot of the EXE version is included in the Screenshot directory.

## Development Setup

### Setup the DirectX8 SDK
* Download the DirectX 8 SDK
* Extract to C:\tools\directx\DX8SDK
* Add the location to your environment variables as $(DX8SDK)

### Setup the DirectX9 SDK
* Download the DirectX 9 SDK (June 2010)
* Extract to C:\tools\directx\DX9SDK
* Add the location to your environment variables as $(DX9SDK)

### Setup the Detours SDK
* Download the Microsoft Detours SDK
* Extract to C:\tools\detours
* Add the location to your environment variables as $(DETOURS_SDK)
