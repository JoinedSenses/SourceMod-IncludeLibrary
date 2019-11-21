/**
 * vim: set ai et ts=4 sw=4 syntax=sourcepawn :
 * File: particle.inc
 * Description: Functions to show TF2 particles
 * Author(s): L. Duke
 */

#if defined _particle_included
#endinput
#endif
#define _particle_included

#include <timers>
#include <sdktools>

int gParticleCount = 0;

// Particle Attachment Types  -------------------------------------------------
enum ParticleAttachmentType
{
    NoAttach = 0,
    Attach,
    AttachMaintainOffset
};

// Particles ------------------------------------------------------------------

/* CreateParticle()
**
** Creates a particle at an entity's position. Attach determines the attachment
** type (0 = not attached, 1 = normal attachment, 2 = head attachment). Allows
** offsets from the entity's position.
** ------------------------------------------------------------------------- */
stock int CreateParticle(const char[] particleType, float time = 5.0, int entity = -1,
                     ParticleAttachmentType attach = Attach,
                     const char[] attachToBone = "head",
                     const float offsetPos[3] = NULL_VECTOR,
                     const float offsetAng[3] = NULL_VECTOR,
                     int target = -1, const char[] targetBone = "flag",
                     int target_particle = 0,
                     Timer deleteFunc = view_as<Timer>(0),
                     Handle timerHandle = null)
{
    int particle = CreateEntityByName("info_particle_system");
    if (particle > 0 && IsValidEdict(particle))
    {
        float pos[3];
        float ang[3];
        if (entity > 0)
        {
            GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
            AddVectors(pos, offsetPos, pos);

            GetEntPropVector(entity, Prop_Send, "m_angRotation", ang);
            AddVectors(ang, offsetAng, ang);
        }
        else
        {
            pos = offsetPos;

            ang = offsetAng;
        }

        TeleportEntity(particle, pos, ang, NULL_VECTOR);

        DispatchKeyValue(particle, "effect_name", particleType);

        if (entity > 0)
        {
            if (attach > NoAttach)
            {
                char eName[32];
                Format(eName, sizeof(eName), "target%i", entity);
                DispatchKeyValue(entity, "targetname", eName);
                DispatchKeyValue(particle, "parentname", eName);

                //SetVariantString("!activator");
                //AcceptEntityInput(particle, "SetParent", entity, particle, 0);
                SetVariantString(eName);
                AcceptEntityInput(particle, "SetParent");

                if (attachToBone[0] != '\0')
                {
                    SetVariantString(attachToBone);
                    AcceptEntityInput(particle, (attach >= AttachMaintainOffset)
                                                ? "SetParentAttachmentMaintainOffset"
                                                : "SetParentAttachment");
                }
            }
        }

        if (target > 0)
        {
            target_particle = CreateEntityByName("info_particle_system");
            if (target_particle > 0 && IsValidEdict(target_particle))
            {
                char pName[32];
                Format(pName, sizeof(pName), "TF2particle%i", particle);
                DispatchKeyValue(particle, "targetname", pName);

                char tName[32];
                Format(tName, sizeof(tName), "target%i", target);
                DispatchKeyValue(target, "targetname", tName);
                DispatchKeyValue(target_particle, "parentname", tName);

                char origin[3];
                GetEntPropVector(target, Prop_Send, "m_vecOrigin", origin);
                DispatchKeyValueVector(target_particle, "origin", origin);

                SetVariantString(pName);
                AcceptEntityInput(target_particle, "SetParent");
                //SetVariantString("!activator");
                //AcceptEntityInput(target_particle, "SetParent", particle, target_particle, 0);

                //Join the source particle to the destination particle
                char tpName[128];
                Format(tpName, sizeof(tpName), "TF2particle%i", target_particle);
                DispatchKeyValue(target_particle, "targetname", tpName);
                DispatchKeyValue(particle, "cpoint1", tpName);

                if (targetBone[0] != '\0')
                {
                    SetVariantString(targetBone);
                    AcceptEntityInput(target_particle, "SetParentAttachment");
                }
            }
            else
            {
                LogError("CreateParticle: could not create target info_particle_system");
            }
        }

        DispatchSpawn(particle);
        ActivateEntity(particle);
        AcceptEntityInput(particle, "start");
        gParticleCount++;

        if (time > 0.0)
        {
            if (deleteFunc)
            {
                timerHandle = CreateTimer(time, deleteFunc, EntIndexToEntRef(particle));
            }
            else
            {
                timerHandle = CreateTimer(time, RemoveParticle, EntIndexToEntRef(particle));
            }
        }
    }
    else
    {
        LogError("CreateParticle: could not create info_particle_system");
    }

    return particle;
}

stock int AttachParticle(int ent, char[] particleType, Timer func = view_as<Timer>(0), float time = 5.0, const float offsetVec[3] = NULL_VECTOR)
{
    return CreateParticle(particleType, time, ent, Attach, "head", offsetVec);
}

stock int AttachParticleEx(int ent, char[] particleType, float time, float addPos[3] = NULL_VECTOR, float addAngle[3] = NULL_VECTOR)
{
    return CreateParticle(particleType, time, ent, Attach, "", addPos, addAngle);
}

stock int AttachParticleBone(int ent, char[] particleType, char[] attachBone, float time,
                         float addPos[3] = NULL_VECTOR, float addAngle[3] = NULL_VECTOR)
{
    return CreateParticle(particleType, time, ent, Attach, attachBone, addPos, addAngle);
}

stock int ShowParticle(float pos[3], char[] particleType, float time, Timer func)
{
    return CreateParticle(particleType, time, 0, NoAttach, "", pos, NULL_VECTOR, NULL_VECTOR, func);
}

stock int ShowParticleEx(char[] particlename, float time, float pos[3], float ang[3] = NULL_VECTOR)
{
    return CreateParticle(particleType, time, 0, NoAttach, "", pos, ang);
}

stock int ShowParticleEntity(int ent, char[] particleType, float time, float addPos[3] = NULL_VECTOR, float addAngle[3] = NULL_VECTOR)
{
    return CreateParticle(particleType, time, ent, Attach, "", pos, ang);
}

stock int GetParticleCount()
{
    return gParticleCount;
}

stock void DeleteParticle(int particleRef)
{
    if (particleRef != INVALID_ENT_REFERENCE)
    {
        int particle = EntRefToEntIndex(particleRef);
        if (particle > 0 && IsValidEntity(particle))
        {
            AcceptEntityInput(particle, "kill");
        }

        particleRef = INVALID_ENT_REFERENCE;
    }
    gParticleCount--;
}

public Action RemoveParticle(Handle timer, any particleRef)
{
    DeleteParticle(particleRef);
    return Plugin_Stop;
}

stock void TE_SetupParticle(const char[] Name, const float origin[3] = NULL_VECTOR,
                       const float start[3] = NULL_VECTOR, const float angles[3] = NULL_VECTOR,
                       int entindex = -1, int attachtype = -1, int attachpoint = -1, bool resetParticles = true)
{
    // find string table
    int tblidx = FindStringTable("ParticleEffectNames");
    if (tblidx == INVALID_STRING_TABLE) 
    {
        LogError("Could not find string table: ParticleEffectNames");
        return;
    }
    
    // find particle index
    char tmp[256];
    int count = GetStringTableNumStrings(tblidx);
    int stridx = INVALID_STRING_INDEX;
    for (int i = 0; i < count; i++)
    {
        ReadStringTable(tblidx, i, tmp, sizeof(tmp));
        if (StrEqual(tmp, Name, false))
        {
            stridx = i;
            break;
        }
    }
    if (stridx == INVALID_STRING_INDEX)
    {
        LogError("Could not find particle: %s", Name);
        return;
    }
    
    TE_Start("TFParticleEffect");
    TE_WriteFloat("m_vecOrigin[0]", origin[0]);
    TE_WriteFloat("m_vecOrigin[1]", origin[1]);
    TE_WriteFloat("m_vecOrigin[2]", origin[2]);
    TE_WriteFloat("m_vecStart[0]", start[0]);
    TE_WriteFloat("m_vecStart[1]", start[1]);
    TE_WriteFloat("m_vecStart[2]", start[2]);
    TE_WriteVector("m_vecAngles", angles);
    TE_WriteNum("m_iParticleSystemIndex", stridx);

    if (entindex != -1)
    {
        TE_WriteNum("entindex", entindex);
    }

    if (attachtype != -1)
    {
        TE_WriteNum("m_iAttachType", attachtype);
    }

    if (attachpoint != -1)
    {
        TE_WriteNum("m_iAttachmentPointIndex", attachpoint);
    }

    TE_WriteNum("m_bResetParticles", resetParticles);    
}

stock void TE_Particle(const char Name, const float origin[3] = NULL_VECTOR,
                  const float start[3] = NULL_VECTOR, const float angles[3] = NULL_VECTOR,
                  int entindex = -1, int attachtype = -1, int attachpoint = -1, bool resetParticles = true,
                  float delay = 0.0)
{
    TE_SetupParticle(Name, origin, start, angles, entindex,
                     attachtype, attachpoint, resetParticles);

    TE_SendToAll(delay);
}

