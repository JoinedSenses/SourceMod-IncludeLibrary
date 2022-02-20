/**
 * vim: set ts=4 :
 * =============================================================================
 * TF2 Wearables
 * API for dealing with TF2 Wearables.  May be submitted for inclusion with
 * the TF2 extension
 *
 * TF2 Wearable Tools (C)2013-2014 Powerlord (Ross Bemrose).
 * All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */
#if defined _tf2wearables_included_
  #endinput
#endif
#define _tf2wearables_included_

#include <sourcemod>
#include <tf2_stocks>

// numbered 0 to TF2_LOADOUT_SLOT_COUNT-1
#define TF2_LOADOUT_SLOT_COUNT 19

// This order is taken from g_szLoadoutStringsForDisplay in server_srv.so
// Frankly, it makes a lot more sense than the version that contains aGrenade
// since grenades have never existed in TF2
enum TF2LoadoutSlot
{
	TF2LoadoutSlot_Primary,
	TF2LoadoutSlot_Secondary,
	TF2LoadoutSlot_Melee,
	TF2LoadoutSlot_Utility,
	TF2LoadoutSlot_Unknown1 = 3,
	TF2LoadoutSlot_Building,
	TF2LoadoutSlot_Unknown2,
	TF2LoadoutSlot_PDA2,
	TF2LoadoutSlot_Misc1,
	TF2LoadoutSlot_Misc2,
	TF2LoadoutSlot_Action,
	TF2LoadoutSlot_Misc3,
	TF2LoadoutSlot_Taunt1,
	TF2LoadoutSlot_Taunt2,
	TF2LoadoutSlot_Taunt3,
	TF2LoadoutSlot_Taunt4,
	TF2LoadoutSlot_Taunt5,
	TF2LoadoutSlot_Taunt6,
	TF2LoadoutSlot_Taunt7,
	TF2LoadoutSlot_Taunt8,
}

/**
 * Equip a wearable item on a player
 * The wearable equivalent of EquipPlayerItem.
 *
 * NOTE: Due to game restrictions, items will be invisible on non-bot players
 *
 * @param client    Client who should equip this
 * @param wearable  The entity index of the wearable
 * @return          True if it was equipped, false otherwise
 */
native bool TF2_EquipPlayerWearable(int client, int wearable);

/**
 * stock that replaces old version of this function, which is now included
 * in the TF2 extension
 *
 * @param client    Client who should remove this
 * @param wearable  The entity index of the wearable, use 
 * @return          true if it was equipped, false otherwise
 */
stock bool TF2_RemovePlayerWearable(int client, int wearable)
{
	TF2_RemoveWearable(client, wearable);
}

/**
 * Is an entity a wearable?
 *
 * @param entity    The entity to check
 * @return          True if it is a wearable, false if not
 * @error           If the entity index is an invalid entity
 */
native bool TF2_IsWearable(int entity);

/**
 * Get the entity equipped in a loadout slot
 * 
 * @param client    Client whose item we want
 * @param slot      The slot whose item we want
 * @return          Entity index or -1 if no item found
 */
native int TF2_GetPlayerLoadoutSlot(int client, TF2LoadoutSlot slot);

/**
 * Removes all non-Taunt wearables from a client
 *
 * @param client    Player's index.
 */
stock void TF2_RemoveAllWearables(int client)
{
	for (TF2LoadoutSlot i = TF2LoadoutSlot_Misc1; i <= TF2LoadoutSlot_Misc3; ++i)
	{
		int item = TF2_GetPlayerLoadoutSlot(client, i);
		if (IsValidEntity(item))
		{
			// IsWearable check is necessary because of spellbooks, which aren't wearables but appear in the Action slot
			if (TF2_IsWearable(item))
			{
				TF2_RemoveWearable(client, item);
			}
			else
			{
				TF2_RemoveWeapon(client, item);
			}
			
			AcceptEntityInput(item, "Kill");
		}
	}
}

/**
 * Removes all Taunt wearables from a client
 *
 * @param client		Player's index.
 */
stock void TF2_RemoveAllTaunts(int client)
{
	for (TF2LoadoutSlot i = TF2LoadoutSlot_Taunt1; i <= TF2LoadoutSlot_Taunt8; ++i)
	{
		int item = TF2_GetPlayerLoadoutSlot(client, i);
		if (IsValidEntity(item))
		{
			TF2_RemovePlayerWearable(client, item);
			AcceptEntityInput(item, "Kill");
		}
	}
}

/**
 * Removes a weapon from a client
 * Note: You still need to kill the entity afterwards
 *
 * @param client		Player's index.
 * @param weaponIndex	entity index of a weapon
 */
stock void TF2_RemoveWeapon(int client, int weaponIndex)
{
	if (!IsValidEntity(weaponIndex))
	{
		return;
	}
	
	// bug #6206
	// papering over a valve bug where a weapon's extra wearables aren't properly removed from the weapon's owner
	int extraWearable = GetEntPropEnt(weaponIndex, Prop_Send, "m_hExtraWearable");
	if (extraWearable != -1)
	{
		TF2_RemoveWearable(client, extraWearable);
	}

	extraWearable = GetEntPropEnt(weaponIndex, Prop_Send, "m_hExtraWearableViewModel");
	if (extraWearable != -1)
	{
		TF2_RemoveWearable(client, extraWearable);
	}
	
	RemovePlayerItem(client, weaponIndex);
}

public SharedPlugin __pl_tf2wearables =
{
	name = "tf2wearables",
	file = "tf2wearables.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

public void __pl_tf2wearables_SetNTVOptional()
{
	MarkNativeAsOptional("EquipPlayerWearable");
	MarkNativeAsOptional("RemovePlayerWearable");
	MarkNativeAsOptional("IsWearable");
	//MarkNativeAsOptional("GetPlayerWearableSlot");
}
