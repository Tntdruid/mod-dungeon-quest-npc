/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "CreatureScript.h"
#include "Group.h"
#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"

enum DungeonQuestNpcActions
{
    ACTION_TELEPORT_OUT = GOSSIP_ACTION_INFO_DEF + 1,
    ACTION_LEAVE_GROUP  = GOSSIP_ACTION_INFO_DEF + 2
};

// Turn-in NPC at the last boss: accepts the dungeon quests and sends the player back out of the dungeon.
class npc_dungeon_quest_turnin : public CreatureScript
{
public:
    npc_dungeon_quest_turnin() : CreatureScript("npc_dungeon_quest_turnin") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Teleport me back to where I came from.", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_OUT);

        if (IsInLfgDungeon(player))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Teleport me back and leave the dungeon group.", GOSSIP_SENDER_MAIN,
                ACTION_LEAVE_GROUP, "You will leave your group. Are you sure?", 0, false);

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);

        switch (action)
        {
            case ACTION_TELEPORT_OUT:
                TeleportOut(player);
                break;
            case ACTION_LEAVE_GROUP:
                // Leaving a dungeon finder group inside the dungeon already teleports the player to the entry point
                if (Group* group = player->GetGroup())
                    Player::RemoveFromGroup(group, player->GetGUID(), GROUP_REMOVEMETHOD_LEAVE);
                if (!player->IsBeingTeleported() && player->GetMap()->IsDungeon())
                    TeleportOut(player);
                break;
            default:
                break;
        }

        return true;
    }

private:
    static bool IsInLfgDungeon(Player* player)
    {
        Group* group = player->GetGroup();
        return group && group->isLFGGroup() &&
            sLFGMgr->inLfgDungeonMap(group->GetGUID(), player->GetMapId(), player->GetMap()->GetDifficulty());
    }

    static void TeleportOut(Player* player)
    {
        // Dungeon finder: back to where the player was when the dungeon started
        if (IsInLfgDungeon(player))
        {
            sLFGMgr->TeleportPlayer(player, true);
            return;
        }

        // Walked in: back outside the dungeon portal
        if (AreaTriggerTeleport const* at = sObjectMgr->GetGoBackTrigger(player->GetMapId()))
        {
            player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation);
            return;
        }

        player->TeleportToEntryPoint();
    }
};

void AddDungeonQuestNpcScripts()
{
    new npc_dungeon_quest_turnin();
}
