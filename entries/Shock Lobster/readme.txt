-------------------------
==    Shock Lobster    ==
-------------------------
Copyright 2021 Dave VanEe

Shock Lobster is a Game Boy ROM designed to run on the original Game Boy handheld
video game system using a flash cartridge. It should also play well on most
emulators.

Venture forth on your quest as a lowly lobster imbued with magical powers, 
determined to inflict lethal damage to whatever stands in your way!

You start out with a handful of skills allowing you to avoid oncoming obstacles 
and deal damage, but will use the pearls you collect on your way to unlock more 
skills, upgrades, and items to push your high score higher and higher.


Loadout Controls:
-----------------
D-Pad: Select item
A: Confirm/unlock/toggle
B: Back
Select: Show/hide details (these are very important, and also included below)
Start: Start game


Battle Controls:
----------------
D-Pad: Select active button pair
A/B: Use the corresponding skill from the current button pair
Start: Pause/Unpause

Items are either usable at the beginning of battle or when defeat is near. The
prompt will only be visible for 2 seconds, and can be hidden early by pressing
START.


Gameplay Details:
-----------------
Press SELECT on the status screen to show detailed information about each 
skill, helping you decide how and when to best use the tools at your disposal 
for maximum output.

The A and B (X and Z) buttons are used to activate skills in combat, with the 
D-Pad used to select which pair of buttons is active. By default the selected 
pair will stick to the top pair when the D-Pad is released, but this can be 
disabled on the status screen. Skills may consume your energy, which is constantly
refilling, or generate charges, which are consumed by finishers which scale 
with the number of charges used.

Several of the skills apply damage-over-time effects to the enemy, time-limited 
buffs to yourself, or have cooldowns before they can be used again, which are 
represented by bars on the bottom-left of the game screen.

The game speed and loadout/items used will be summarized on the game over panel,
allowing you to compete with your friends to get the highest score using a specific
combination of skills and upgrades!


Skills:
-------
Jet:
Release a jet of water, propelling yourself upwards and dealing 3 points of damage.
The damage effect can only occur once every second.

Zap [40 energy]:
Zap the enemy, dealing 40 damage. Generates 1 charge.

Shock [35 energy]:
Shock the enemy for 5 damage and an additional 24 damage every 3 seconds over 9 
seconds. Generates 1 charge.

Discharge [35+30 energy]:
Finisher which deals damage per charge, plus 0.5 additional damage per extra 
point of energy (up to 30 energy).
1 charge: 17 damage
2 charges: 31 damage
3 charges: 45 damage
4 charges: 60 damage
5 charges: 74 damage

Electrify [30 energy]:
Finisher which deals damage over time. Damage is increased per charge:
1 charge: 51 damage over 16 seconds
2 charges: 94 damage over 16 seconds
3 charges: 138 damage over 16 seconds
4 charges: 181 damage over 16 seconds
5 charges: 225 damage over 16 seconds

Empower [25 energy]:
Finisher which increases damage done by 30%. Lasts longer per charge:
1 charge: 14 seconds
2 charges: 19 seconds
3 charges: 24 seconds
4 charges: 29 seconds
5 charges: 34 seconds

Invigorate:
Instantly regain 60 energy. 30 second cooldown.

Focus:
Reduce the energy cost of all skills by 50% for 4 seconds. Invigorate cannot
be used while Focus is active. 80 second cooldown.


Upgrades:
---------
Amplify:
Increase the damage of Zap by 20% when Shock or Electrify are active.

Detonate:
Increase the critical strike chance of Disharge by 30% when Shock or Electrify
are active.

High Pressure:
Jet now launches you further into the air and deals double damage.

Overcharge:
Critical hits from skills which generate charges generate an additional charge.

Residual Charge:
The periodic damage from your Electrify skill can now critically hit.

Expertise:
Double your base critical strike chance from 30% to 60%.

Clarity:
All direct damage has a chance to cause you to enter a state of clarity, which
reduces the base energy cost of the next skill to zero.

Refresh:
Zap will now extend the duration of an active Electrify by 2 seconds (up to 6 
seconds total).

Items:
------
Note: All items are consumable, only providing a single use for each purchase.

First Strike:
Let loose an initial strike of two 100 damage blasts. Each hit may be a critical
hit. [Consumable]

Blitz:
Come out swinging with three 100 damage blasts. Each hit may be a critical hit.
[Consumable]

Final Word:
Unleash an impassioned last-ditch retort, dealing three 100 damage blasts. Each hit may be a critical
hit. [Consumable]

Second Wind:
Bounce back from defeat for a second chance. [Consumable]


Options:
--------
Game Speed:
Adjust the game speed. Hiscores are tracked per game speed.

Music:
Enable game music.

Sticky D-Pad:
Enable sticky button pair selection in-game, which will snap back to the top
pair when the D-pad is released.

Reset Save:
Press A four times to reset the save data. Details panel must be visible to
activate. **WARNING** No final confirmation!


Tools Used for Development:
---------------------------
- RGBDS (https://rgbds.gbdev.io/)
- Emulicious (https://emulicious.net/)
- Aseprite (https://www.aseprite.org/)
- hUGETracker (https://nickfa.ro/index.php/HUGETracker)
- OpenMPT (https://openmpt.org/)
- Visual Studio Code (https://code.visualstudio.com/)


Additional Code/Assets:
-----------------------

- gb-starter-kit by ISSOtm (https://github.com/ISSOtm/gb-starter-kit)
- gb-vwf by ISSOtm (https://github.com/ISSOtm/gb-vwf)
- hUGEDriver by Superdisk (https://github.com/SuperDisk/hUGEDriver)
- Sound Effect Driver and BCD functions by PinoBatch
    (https://github.com/pinobatch/libbet/blob/master/src/audio.z80)
    (https://github.com/pinobatch/libbet/blob/master/src/bcd.z80)
- Lucky Bestiary by LuckyCassette (https://luckycassette.itch.io/lucky-bestiary-gb)
- MinimalPixel Font by Mounir Tohami (https://mounirtohami.itch.io/minimalpixel-font)
- Electrox Font by Dennis Ludlow (https://www.1001fonts.com/electrox-font.html)

Music From GB Studio Community Assets
    (https://github.com/DeerTears/GB-Studio-Community-Assets/tree/master/Music)
- ​"Tape It Together" and ​"Serious Ping Pong Matches" (cropped) by DeerTears
- "FridgeMusic" by Tomas Danko (https://creativecommons.org/licenses/by/4.0/)
    Patterns were reordered to improve overall effect on hardware in game context
- "Darkstone Remix" (cropped) by Tronimal
