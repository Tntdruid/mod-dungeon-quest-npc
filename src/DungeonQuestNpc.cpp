/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Chat.h"
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

        if (Group* group = GetLfgGroup(player))
        {
            // Leaving before the last boss is dead can give the dungeon finder deserter debuff
            if (sLFGMgr->GetState(group->GetGUID()) == lfg::LFG_STATE_FINISHED_DUNGEON)
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Teleport me back and leave the dungeon finder group.",
                    GOSSIP_SENDER_MAIN, ACTION_LEAVE_GROUP);
            else
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Teleport me back and leave the dungeon finder group.",
                    GOSSIP_SENDER_MAIN, ACTION_LEAVE_GROUP,
                    "The dungeon is not finished yet, leaving can give you the Deserter debuff. Are you sure?", 0, false);

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Teleport me back, but stay in the group.", GOSSIP_SENDER_MAIN,
                ACTION_TELEPORT_OUT);
        }
        else
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Teleport me back to where I came from.", GOSSIP_SENDER_MAIN,
                ACTION_TELEPORT_OUT);

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);

        if (player->IsInCombat())
        {
            ChatHandler(player->GetSession()).SendSysMessage("You can't teleport while in combat.");
            return true;
        }

        switch (action)
        {
            case ACTION_TELEPORT_OUT:
                TeleportOut(player);
                break;
            case ACTION_LEAVE_GROUP:
                LeaveLfgGroup(player);
                break;
            default:
                break;
        }

        return true;
    }

private:
    static Group* GetLfgGroup(Player* player)
    {
        Group* group = player->GetGroup();
        if (group && group->isLFGGroup() &&
            sLFGMgr->inLfgDungeonMap(group->GetGUID(), player->GetMapId(), player->GetMap()->GetDifficulty()))
            return group;

        return nullptr;
    }

    static void TeleportOut(Player* player)
    {
        // Dungeon finder: back to where the player was when the dungeon started
        if (GetLfgGroup(player))
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

    // Teleports first, so the player lands where the dungeon finder picked them up, then leaves the group.
    // Leaving clears the dungeon finder state, so the player can queue again right away.
    static void LeaveLfgGroup(Player* player)
    {
        Group* group = GetLfgGroup(player);
        if (!group)
        {
            TeleportOut(player);
            return;
        }

        sLFGMgr->TeleportPlayer(player, true);
        Player::RemoveFromGroup(group, player->GetGUID(), GROUP_REMOVEMETHOD_LEAVE);

        if (!player->IsBeingTeleported() && player->GetMap()->IsDungeon())
            TeleportOut(player);
    }
};

void AddDungeonQuestNpcScripts()
{
    new npc_dungeon_quest_turnin();
}
