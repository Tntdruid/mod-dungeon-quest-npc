# mod-dungeon-quest-npc

An [AzerothCore](https://www.azerothcore.org/) module for WotLK 3.3.5a. It puts a dungeon's quests at the dungeon
itself, so players can stop running all over the world to collect them before a run.

Every supported dungeon gets two NPCs:

- **Dungeon Quest** stands at the entrance and offers the dungeon's quests.
- **Dungeon Quest Turn-in** stands at the last boss. It accepts every quest that belongs to the dungeon and can
  teleport the player back out.

This covers 48 dungeons across Classic, TBC and WotLK, with 474 quests offered at the entrances.

## What players see

1. Enter the dungeon. The quest giver stands just inside the entrance. Dungeons with more than one entrance, like
   Dire Maul, Scarlet Monastery, Stratholme and Maraudon, have one at each.
2. Pick up the quests. The giver only offers quests that player can take, so it follows the same rules as the
   original quest NPCs (see [Quest requirements](#quest-requirements)).
3. Clear the dungeon. Turn in everything at the NPC next to the last boss. Dungeons with several wings or end
   bosses, like Scarlet Monastery, Dire Maul, Blackrock Spire, Stratholme and Coilfang Reservoir, have one at each.
4. Teleport out from the turn-in NPC.

The original quest NPCs keep working as before, so players can still pick up or turn in quests the normal way.

## Teleporting out

The turn-in NPC's menu shows the quests and these options.

In a dungeon finder group:

- **Teleport me back and leave the dungeon finder group.** Sends the player back to where they were when the
  dungeon started and takes them out of the group, so they can queue again right away. If the dungeon is not
  finished yet, it asks first, because leaving early can give the Deserter debuff.
- **Teleport me back, but stay in the group.** Same teleport, but the player stays in the group.

Otherwise:

- **Teleport me back to where I came from.** Sends the player outside the dungeon portal.

Teleporting is not possible in combat.

## Quest requirements

The giver is a normal quest NPC. Its menu only lists quests that pass the core's own `CanTakeQuest` check, which
is the same check the original NPCs use. That covers:

- previous quests and quest chains
- breadcrumb quests
- race, class, level and reputation
- exclusive groups (quests where you can only pick one)

So a player cannot skip a chain. For example, Leaders of the Fang in Wailing Caverns is only offered after Hamuul
Runetotem and Nara Wildmane are done.

Breadcrumbs work like they do at the original NPCs, which means they are optional. A player can take the main
quest without doing its breadcrumb first. While the breadcrumb is in the quest log, the main quest is not offered,
and once the main quest is started or done, the breadcrumb is no longer offered.

A few quests in the AzerothCore database are only linked to the quest before them with `RewardNextQuest`, which
does not make the earlier quest required. Those can be picked up on their own here, just like at the original NPC.

## Which quests are included

The quest lists are generated from the world database. A quest belongs to a dungeon when any of these point to
the dungeon:

- its zone in the quest log
- the creatures it asks you to kill
- the items it asks for, by where they drop
- the area it asks you to explore
- the NPCs that start or end it

Left out:

- holiday, daily and weekly quests
- disabled quests
- quests that start from an item drop. These still drop as normal and can be turned in at the turn-in NPC.
- quests that start from a scripted NPC inside the dungeon, such as escort quests. Those have to be taken from
  that NPC, or the escort would never start. They can still be turned in at the turn-in NPC.

Trial of the Champion only has daily quests, so it has no NPCs. Halls of Stone only has a turn-in NPC.

### Faction

Many Classic quests have no race limit in the database. Only one faction can do them, because the original NPC
stands in a Horde or Alliance town. The dungeon NPCs are neutral, so the SQL sets `AllowableRaces` on 95 of these
quests to keep them on the right side, for example Necklace Recovery in Uldaman for the Horde. This also applies
at the original NPCs, but only to players who could not reach them anyway.

## Looks

Each NPC copies the model and weapons of an NPC from that dungeon's story. Names stay "Dungeon Quest" and
"Dungeon Quest Turn-in", with the dungeon name as subtitle, so players recognise them everywhere.

| Dungeon | Quest giver looks like | Turn-in NPC looks like |
|---|---|---|
| Ragefire Chasm | Rahauro | Maur Grimtotem |
| The Deadmines | Gryan Stoutmantle | Wilder Thistlenettle |
| Wailing Caverns | Nara Wildmane | Disciple of Naralex |
| Shadowfang Keep | Keeper Bel'dugur | Deathstalker Adamant |
| Blackfathom Deeps | Je'neu Sancrea | Argent Guard Thaelrid |
| The Stockade | Warden Thelwater | Councilman Millstipe |
| Gnomeregan | Ozzie Togglevolt | Scooty |
| Razorfen Kraul | Falfindel Waywarder | Willix the Importer |
| Scarlet Monastery | Sage Truthseeker | Librarian Mae Paledust |
| Razorfen Downs | Andrew Brownell | Belnistrasz |
| Uldaman | Talvash del Kissel | Baelog |
| Zul'Farrak | Chief Engineer Bilgewhizzle | Sergeant Bly |
| Maraudon | Keeper Marandis | Celebras the Redeemed |
| Sunken Temple | Marvon Rivetseeker | Atal'ai Exile |
| Blackrock Depths | Lokhtos Darkbargainer | Kharan Mighthammer |
| Blackrock Spire | Scarshield Quartermaster | Finkle Einhorn |
| Dire Maul | Falrin Treeshaper | Lorekeeper Lydros |
| Scholomance | Alexi Barov | Eva Sarkhoff |
| Stratholme | Aurius | Ysida Harmon |
| Hellfire Ramparts | Stone Guard Stok'ton | Gunny |
| The Blood Furnace | Gunny | Caza'rez |
| The Shattered Halls | Force Commander Danath Trollbane | Nazgrel |
| Coilfang Reservoir (Slave Pens, Underbog, Steamvault) | Watcher Jhang | Ysiel Windsinger |
| Mana-Tombs | Nexus-Prince Haramad | Artificer Morphalius |
| Auchenai Crypts | Mother Kashur | Mother Kashur |
| Sethekk Halls | Isfar | Kalynna Lathred |
| Shadow Labyrinth | Spy To'gun | Spymistress Mehlisah Highcrown |
| Old Hillsbrad Foothills | Taretha | Thrall |
| The Black Morass | Sa'at | Andormu |
| The Mechanar | David Wayne | David Wayne |
| The Botanica | Archmage Vargoth | Lauranna Thar'well |
| The Arcatraz | Akama | Udalo |
| Magisters' Terrace | Exarch Larethor | Kalecgos |
| Utgarde Keep | Defender Mordun | High Executor Anselm |
| The Nexus | Archmage Berinand | Librarian Serrah |
| Azjol-Nerub | Kilix the Unraveler | Kilix the Unraveler |
| Ahn'kahet | Kilix the Unraveler | Kilix the Unraveler |
| Drak'Tharon Keep | Mack Fearsen | Kurzel |
| The Violet Hold | Lieutenant Sinclari | Rhonin |
| Gundrak | Chronicler Bah'Kini | Tol'mar |
| Halls of Stone | (no giver) | Brann Bronzebeard |
| Halls of Lightning | Brann Bronzebeard | Brann Bronzebeard |
| The Oculus | Belgaristrasz | Eternos |
| Utgarde Pinnacle | Brigg Smallshanks | Brigg Smallshanks |
| The Culling of Stratholme | Chromie | Arthas |
| The Forge of Souls | Lady Jaina Proudmoore | Dark Ranger Loralen |
| Pit of Saron | Caladis Brightspear | Myralion Sunblaze |
| Halls of Reflection | Archmage Koreln | Dark Ranger Loralen |

To change a look, edit the entry ID in the `tmp_dungeon_quest_theme` list near the end of the SQL. If a theme NPC
is missing from your database, the NPC falls back to the model in `@MODEL` at the top of the file.

## Install

1. Clone the module into your AzerothCore `modules` folder:

   ```bash
   cd azerothcore-wotlk/modules
   git clone https://github.com/Tntdruid/mod-dungeon-quest-npc.git
   ```

2. Re-run CMake and build the worldserver (for example `./acore.sh compiler build`).
3. Run `sql/dungeon_quest_npcs_standalone.sql` against the world database (`acore_world`).
4. Restart the worldserver.

The SQL lives in `sql/` instead of `data/sql/` on purpose, so it is not applied automatically. It deletes and
re-inserts only its own rows, so it is safe to run again after an update. If you already added NPCs like these
some other way, remove them first, or there will be two in each spot.

Without the module built, the NPCs still work for quests but show no teleport options, and the worldserver logs
that `npc_dungeon_quest_turnin` has no code.

## IDs used

| What | Range |
|---|---|
| Quest givers (`creature_template`) | 9200002 - 9200050 |
| Turn-in NPCs (`creature_template`) | 9300002 - 9300050 |
| Spawns (`creature.guid`) | 9200021 - 9200509 |

The SQL also changes `AllowableRaces` in `quest_template` for the 95 one-faction quests described above.

## Known limitations

- Some last bosses are only spawned by an event (for example Mal'Ganis, Tyrannus and Cyanigosa). Their turn-in
  NPCs stand at a fixed point nearby. These spawns have "check position in-game" in `creature.Comment`. If one
  stands in an odd place, move it with `.npc move` and save the new position in your database.
- Positions are checked in-game for some dungeons only. Reports and fixes are welcome.

## License

GNU AGPL v3, see `LICENSE`.
