Better Sounds Hack
for GB Studio 2.0.0-beta5 (not tested on earlier versions)

This "hack" replaces the Sound: Play Effect event with one where the Duration set for Beeps and Crashes actually works, instead of being ignored.


=========== INSTALL ===========
To add Better Sounds to your GBS project, copy the contents of this archive into your project directory. You may need to close and reopen your project.
Everything is already sorted into directories, so everything should end up in the correct place automatically.
If you want to copy the files manually (e.g. to resolve conflicts with your existing engine mods), these are the files included:

/assets/engine/engine.json
/assets/engine/include/data_pts.h
/assets/engine/src/core/MusicManager.c
/plugins/BetterSounds/events/eventBetterSounds.js


============ USAGE ============
This is a drop-in replacement for the Sound: Play Effect event, and should not need any changes to your project to take effect.

If you were relying on the fixed lengths of the Beep and Crash effects, you should double-check that you have correct lengths set for those. The fixed length was 0.25 seconds (or, more precisely, 63/256 seconds).
Although GBS allows you to set a sound duration anywhere between 0 and 4.25 seconds, Beeps and Crashes can only have lengths between 0.004s and 0.25s, this is a hardware limitation. The sound duration will be clamped appropriately. However, the full duration will be used for the waiting period if "Wait until finished" is checked.

This hack adds a new Engine Field called "Beep/Crash Time". It's set automatically by each Sound: Play Effect event that plays a Beep or Crash, and you shouldn't ever need to use it manually.


======= KNOWN CONFLICTS =======
If you use the UI interactable hack, the first two files in that list will conflict with it. The conflicts should be easy to resolve manually, however.
Ctrl+F for "Better Sounds" to find the changed parts.
engine.json adds a new engine field in both mods, you'll want to copy the definition for the sound_time field into your existing engine.json.
data_pts.h adds the corresponding C variable for the sound_time field.