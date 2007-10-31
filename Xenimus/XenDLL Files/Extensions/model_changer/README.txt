Model Changer (Model_Changer) +Source

Memory Addresses Used:

   1. "Body Model"
   2. "Helmet Model"
   3. "Weapon Model"
   4. "Shield Model"
   5. "Spell Model"



Config File Options:

   1. "MenuItemIO" = extension is active or not
   2. "Critical Body Low #" = see description / notes
   3. "Critical Body High #" = see description / notes
   4. "Critical Helmet Low #" = see description / notes
   5. "Critical Helmet High #" = see description / notes
   6. "Critical Weapon Low #" = see description / notes
   7. "Critical Weapon High #" = see description / notes
   8. "Critical Shield Low #" = see description / notes
   9. "Critical Shield High #" = see description / notes
  10. "Critical Spell Low #" = see description / notes
  11. "Critical Spell High #" = see description / notes
  12. "BodyID" = last used body model id
  13. "HelmetID" = last used helmet model id
  14. "WeaponID" = last used weapon model id
  15. "ShieldID" = last used shield model id
  16. "SpellID" = last used spell model id



Description / Notes:

   1. if the model changer loads, and is missing a memory address, the others that have addresses will still work
   2. when changing the model numbers, some times you may run into a number or range of numbers that crash the game. to skip over these use the model_changer config options listed above. you can specify as many numbers/ranges for each model type that you want. to do this you must replace the '#' with a number. the first set must be 0 and not 1. examples of this are created after the first run of the program. edit at your own risk. the "low" number is the first number skipped, and the "high" number is the last number skipped... they may be the same number. to specify the highest number possible use the Asterisk character *
   3. when ingame, after turning off the model changer, to go back to your normal models either switch weapons or log off and back on
   4. the model id changer is located under the dll controller config button (next to the dll load and unload buttons)