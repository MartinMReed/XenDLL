Computron (Computron) [Built In]



Memory Addresses Used:

     progressXP
     progressMP
     progressHP
     locationX
     locationY



Config File Options:

   1. "MenuItemIO" = Options: true, false
                     Default: false
                     Whether the extension is active or not. This will be set automatically everytime the game exits

   2. "pauseRestartKey" = Options: *most* of the DIK keys (ie DIK_Q, DIK_ESCAPE, etc)
                          Default: 0x00 (nothing)
                          For a list of keys, view: XenDLL Files/DIK.html
                          Start from the beginning of the script.

   3. "pauseResumeKey" = Options: *most* of the DIK keys (ie DIK_Q, DIK_ESCAPE, etc)
                         Default: 0x00 (nothing)
                         For a list of keys, view: XenDLL Files/DIK.html
                         Start from where the script was stopped/paused.

   4. "scriptFilename" = Options: Filename of the script to download
                         Default: 
                         Requires logging into the Computron system.

   5. "scriptHandle" = Options: User handle registered on the Computron system.
                       Default: 

   5. "scriptPassword" = Options: User password belonging to the handle registered on the Computron system.
                         Default: 



Description / Notes:

   1. clean.bat will clear out Computron.xml, warnLog.txt, and any script files that were downloaded.