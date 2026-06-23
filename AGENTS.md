# Winston's Burrow Project Guide

This repo is `pokeemerald-expansion`, not vanilla `pokeemerald`. It already includes modern battle-engine support for Stealth Rock, so Winston's Burrow can reuse the existing hazard queue instead of porting Stealth Rock from scratch.

## Current v0.1 Slice

The first playable loop is intentionally small:

1. New games start in `MAP_INSIDE_OF_TRUCK`, which is currently reused as the Burrow start room.
2. Step onto the trigger tiles near the right side of the room.
3. `VAR_WINSTONS_BURROW_FLOOR` tracks floors 1-10.
4. Floors 1-9 run trainer battles.
5. After floors 3, 6, and 9, the script heals the party.
6. Floor 10 runs the Winston boss battle.
7. Beating Winston sets the floor to 11, disables the trigger, heals the party, and shows the victory message.

The run state variables are in `include/constants/vars.h`:

- `VAR_WINSTONS_BURROW_FLOOR` (`0x40DB`): current floor, with `11` meaning cleared.
- `VAR_WINSTONS_BURROW_STATE` (`0x40DC`): `0` keeps the trigger active, `1` marks the run cleared.

## Important Files

- `include/constants/species.h`: species IDs. `SPECIES_WINSTERRIER` lives in the custom species area.
- `src/data/pokemon/species_info.h`: Winsterrier stats, typing, height, weight, category, Pokedex text, placeholder graphics, and learnset pointers.
- `src/data/graphics/pokemon.h`: declarations that point the engine at sprite, icon, palette, and footprint files.
- `graphics/pokemon/winsterrier/`: placeholder graphics copied from Poochyena.
- `include/constants/moves.h`: move IDs. `MOVE_TUNNEL_AMBUSH` is the custom move.
- `src/data/moves_info.h`: Tunnel Ambush power, type, accuracy, PP, category, and animation.
- `src/battle_main.c`: Tunnel Ambush gets +1 priority during sandstorm in `GetBattleMovePriority`.
- `include/constants/abilities.h`: ability IDs. `ABILITY_TUNNEL_GUIDE` is the custom ability.
- `src/data/abilities.h`: Tunnel Guide name, short description, and AI rating.
- `src/battle_util.c`: Tunnel Guide switch-in behavior.
- `data/battle_scripts_1.s`: battle scripts for Tunnel Guide messages, ability popup, and healing animation.
- `include/battle_scripts.h`: C extern declarations for the new battle scripts.
- `include/constants/battle_string_ids.h`: IDs for Tunnel Guide battle messages.
- `src/battle_message.c`: visible Tunnel Guide message text.
- `include/constants/opponents.h`: trainer IDs and trainer count.
- `src/data/trainers.party`: Winston's team and the Burrow floor trainer parties.
- `data/maps/InsideOfTruck/scripts.inc`: the minimal Burrow run script.
- `data/maps/InsideOfTruck/map.json`: coordinate triggers for starting/continuing the run.

## Sprite Replacement Requirements

The current Winsterrier art is placeholder art. Replace these files later:

- `graphics/pokemon/winsterrier/anim_front.png`: front battle sprite sheet, 64x128 indexed PNG, 4bpp, max 16 colors. This is two 64x64 frames stacked vertically. Palette index 0 should be transparent.
- `graphics/pokemon/winsterrier/back.png`: back battle sprite, 64x64 indexed PNG, 4bpp, max 16 colors. Palette index 0 should be transparent.
- `graphics/pokemon/winsterrier/normal.pal`: normal 16-color JASC/PSP palette for the battle sprites.
- `graphics/pokemon/winsterrier/shiny.pal`: shiny 16-color JASC/PSP palette for the battle sprites.
- `graphics/pokemon/winsterrier/icon.png`: party icon sheet, 32x64 indexed PNG, 4bpp. This normally stores two 32x32 animation frames stacked vertically.
- `graphics/pokemon/winsterrier/footprint.png`: footprint, 16x16 indexed PNG, 1bpp.

If you change sprite dimensions, also update `.frontPicSize`, `.backPicSize`, `.frontPicYOffset`, and `.backPicYOffset` in `src/data/pokemon/species_info.h`.

## Build Notes

The baseline source did not build in this local environment because required toolchain pieces are missing:

- `arm-none-eabi-gcc`
- `arm-none-eabi-as`
- `pkg-config`
- libpng headers used by the PNG conversion tools

After installing the decomp toolchain and dependencies, run:

```sh
make -j4
```

For v0.1 testing, start a new game, step onto the right-side trigger tiles in the start room, and clear floors 1-10. Tunnel Guide should set Stealth Rock on the opponent's side when Winsterrier switches in; if rocks are already present and Winsterrier is damaged, it should heal by 1/4 max HP instead.
