# SpacedRepetitionTraining

> **Work in progress.** This plugin is an unfinished prototype: the scheduling logic is only partly written and it
> does not do anything useful in game yet.

*Add spaced repetition to training packs.*

A [BakkesMod](https://bakkesmod.com) plugin for Rocket League. The goal is to replay the shots of a custom
training pack on a spaced-repetition schedule: missed shots come back sooner, made shots come back later. The
plugin jumps between shots with `sv_training_set N`.

## Status

Done:

- A `Shot` record per shot in the pack (attempts, level, counter, success).
- Hooks for loading a training round and for goals and ball explosions (hit/miss detection).
- Batch building and shot selection (`initShotsArray`, `populateCurrBatch`, `getNextShot`, `setNextShot`).
- A settings page (F2 → Plugins) with an enable checkbox.

Still to do:

- `handleRestart`, `shotSucceed` and `shotFailed` are empty, so results don't update the schedule yet.
- `EventPostLoadMap` starts at shot index 3, a leftover from testing.
- `randomizeCurrBatch` shuffles with a sort comparator that isn't a valid ordering; it should use `std::shuffle`.

## Building

Requirements:

- Windows with Visual Studio 2019 or newer and the **v142** C++ toolset (newer Visual Studio versions can retarget the project).
- [BakkesMod](https://bakkesmod.com) installed. `BakkesMod.props` finds the SDK through the registry key
  `HKCU\Software\BakkesMod\AppPath\BakkesModPath`, which the BakkesMod installer creates.

Open `SpacedRepetitionTraining.sln`, choose **Release | x64** and build. The DLL lands in `plugins\`; the post-build step patches it
with `bakkesmod-patch.exe`. The pre-build step runs `update_version.ps1`, which bumps the build number in `version.h`.

To install a build by hand, copy `SpacedRepetitionTraining.dll` into `%APPDATA%\bakkesmod\bakkesmod\plugins` and run
`plugin load SpacedRepetitionTraining` in the BakkesMod console (F6).

## Easy Anti-Cheat

Since April 2026 Rocket League on PC runs Easy Anti-Cheat. BakkesMod only loads when the game is started
**without** anti-cheat, which limits it to offline play: freeplay, custom training, LAN and replays.

## Third-party code

- `fmt/` – [{fmt}](https://github.com/fmtlib/fmt) 7.0.3, MIT licence (see `fmt/LICENSE.rst`).
- `IMGUI/` – [Dear ImGui](https://github.com/ocornut/imgui) (MIT) with BakkesMod's additions, as shipped by the
  BakkesMod plugin template.

## Licence

MIT – see [LICENSE](LICENSE).
