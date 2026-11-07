# mod-dungeon-quest-npc

AzerothCore module that adds two NPCs to 48 dungeons (Classic, TBC and WotLK):

- **Dungeon Quest** at every entrance: gives the dungeon's quests.
- **Dungeon Quest Turn-in** at the last boss: accepts all the dungeon's quests and can teleport the player back out.

Each NPC copies the model and weapons of an NPC from the dungeon's story (for example Je'neu Sancrea and
Argent Guard Thaelrid in Blackfathom Deeps).

## Turn-in NPC menu

In a dungeon finder group:

- **Teleport me back and leave the dungeon finder group.** Back to where the player was when the dungeon
  started, and out of the group, so the player can queue again right away. Asks for confirmation if the
  dungeon is not finished yet, because leaving early can give the Deserter debuff.
- **Teleport me back, but stay in the group.** Same teleport, the player stays in the group.

Otherwise:

- **Teleport me back to where I came from.** Sends the player outside the dungeon portal.

Teleporting is not possible in combat.

## Install

1. Copy the `mod-dungeon-quest-npc` folder into `azerothcore-wotlk/modules/`.
2. Re-run CMake and build the worldserver.
3. Run `sql/dungeon_quest_npcs_standalone.sql` against the world database (`acore_world`).
4. Restart the worldserver.

The SQL is kept in `sql/` on purpose, so it is not applied automatically. Run it by hand, and only on a server
that does not already have these NPCs from `data/sql/custom/db_world/dungeon_quest_npcs*.sql`, or you will get
two NPCs in each spot.

## IDs used

| What | Range |
|---|---|
| Quest givers (`creature_template`) | 9200002 - 9200050 |
| Turn-in NPCs (`creature_template`) | 9300002 - 9300050 |
| Spawns (`creature.guid`) | 9200021 - 9200509 |

The SQL deletes and re-inserts its own rows, so it is safe to run more than once. Change `@MODEL` at the top to
pick the fallback model used when a theme NPC is missing.

## Notes

- Givers do not offer quests that start from an item drop or from a scripted (escort/event) NPC inside the
  dungeon. The turn-in NPC accepts every quest of the dungeon.
- Holiday, daily/weekly and disabled quests are left out. Trial of the Champion has only daily quests, so it has
  no NPCs.
- Turn-in NPCs for bosses that are spawned by an event are placed at a fixed point nearby. Their spawns have
  "check position in-game" in `creature.Comment`. Move them with `.npc move` if needed.
- Without the module built, the NPCs still work for quests but show no teleport options.

## License

GNU AGPL v3, see `LICENSE`.
