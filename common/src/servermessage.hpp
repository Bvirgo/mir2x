/*
 * =====================================================================================
 *
 *       Filename: servermessage.hpp
 *        Created: 01/24/2016 19:30:45
 *    Description: net message used by client and mono-server
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include <cstdint>
#include <unordered_map>
#include "messagebase.hpp"

enum SMType: uint8_t
{
    SM_NONE = 0,
    SM_PING,
    SM_LOGINOK,
    SM_LOGINFAIL,
    SM_ACTION,
    SM_CORECORD,
    SM_UPDATEHP,
    SM_NOTIFYDEAD,
    SM_DEADFADEOUT,
    SM_EXP,
    SM_SHOWDROPITEM,
    SM_FIREMAGIC,
    SM_SPACEMOVE,
    SM_OFFLINE,
    SM_REMOVEGROUNDITEM,
    SM_PICKUPOK,
    SM_GOLD,
    SM_MAX,
};

#pragma pack(push, 1)
struct SMPing
{
    uint32_t Tick;
};

struct SMAccount
{
    // register operation for the account
    // 0 : validate this account
    // 1 : create account
    // 2 : login
    uint8_t Operation;
    uint8_t Respond;

    char ID[64];
    char Password[128];
};

struct SMLoginOK
{
    uint64_t UID;
    uint32_t DBID;
    uint32_t MapID;
    uint16_t X;
    uint16_t Y;

    uint8_t Male;
    uint8_t Direction;

    uint32_t JobID;
    uint32_t Level;
};

struct SMLoginFail
{
    uint32_t FailID;
};

struct SMAction
{
    uint64_t UID;
    uint32_t MapID;

    uint8_t Action;
    uint8_t Speed;
    uint8_t Direction;

    uint16_t X;
    uint16_t Y;
    uint16_t AimX;
    uint16_t AimY;

    uint64_t AimUID;
    uint64_t ActionParam;
};

struct SMCORecord
{
    uint8_t COType;

    struct _Action
    {
        uint64_t UID;
        uint32_t MapID;

        uint8_t Action;
        uint8_t Speed;
        uint8_t Direction;

        uint16_t X;
        uint16_t Y;
        uint16_t AimX;
        uint16_t AimY;

        uint64_t AimUID;
        uint64_t ActionParam;
    }Action;

    struct _SMCORecord_Monster
    {
        uint32_t MonsterID;
    };

    struct _SMCORecord_Player
    {
        uint32_t DBID;
        uint32_t JobID;
        uint32_t Level;
    };

    struct _SMCORecord_NPC
    {
        uint32_t NPCID;
    };

    union
    {
        _SMCORecord_Monster Monster;
        _SMCORecord_Player  Player;
        _SMCORecord_NPC     NPC;
    };
};

struct SMUpdateHP
{
    uint64_t UID;
    uint32_t MapID;

    uint32_t HP;
    uint32_t HPMax;
};

struct SMDeadFadeOut
{
    uint64_t UID;
    uint32_t MapID;

    uint32_t X;
    uint32_t Y;
};

struct SMNotifyDead
{
    uint64_t UID;
};

struct SMExp
{
    uint32_t Exp;
};

struct SMShowDropItem
{
    struct _CommonItem
    {
        uint32_t ID;
        uint32_t DBID;
    }IDList[16];

    uint16_t X;
    uint16_t Y;
};

struct SMFireMagic
{
    uint64_t UID;
    uint32_t MapID;

    uint8_t Magic;
    uint8_t MagicParam;
    uint8_t Speed;
    uint8_t Direction;

    uint16_t X;
    uint16_t Y;
    uint16_t AimX;
    uint16_t AimY;
    uint64_t AimUID;
};

struct SMOffline
{
    uint64_t UID;
    uint32_t MapID;
};

struct SMRemoveGroundItem
{
    uint16_t X;
    uint16_t Y;
    uint32_t ID;
    uint32_t DBID;
};

struct SMPickUpOK
{
    uint16_t X;
    uint16_t Y;
    uint32_t ID;
    uint32_t DBID;
};

struct SMGold
{
    uint32_t Gold;
};

#pragma pack(pop)

class SMSGParam: public MessageBase
{
    public:
        SMSGParam(uint8_t nHC)
            : MessageBase(nHC)
        {}

    private:
        const MessageAttribute &GetAttribute(uint8_t nHC) const
        {
            static const std::unordered_map<uint8_t, MessageAttribute> s_AttributeTable
            {
                //  0    :     empty
                //  1    : not empty,     fixed size,     compressed
                //  2    : not empty,     fixed size, not compressed
                //  3    : not empty, not fixed size, not compressed

                {SM_NONE,             {0, 0,                               "SM_NONE"            }},
                {SM_PING,             {2, sizeof(SMPing),                  "SM_PING"            }},
                {SM_LOGINOK,          {1, sizeof(SMLoginOK),               "SM_LOGINOK"         }},
                {SM_LOGINFAIL,        {2, sizeof(SMLoginFail),             "SM_LOGINFAIL"       }},
                {SM_ACTION,           {1, sizeof(SMAction),                "SM_ACTION"          }},
                {SM_CORECORD,         {1, sizeof(SMCORecord),              "SM_CORECORD"        }},
                {SM_UPDATEHP,         {1, sizeof(SMUpdateHP),              "SM_UPDATEHP"        }},
                {SM_NOTIFYDEAD,       {1, sizeof(SMNotifyDead),            "SM_NOTIFYDEAD"      }},
                {SM_DEADFADEOUT,      {1, sizeof(SMDeadFadeOut),           "SM_DEADFADEOUT"     }},
                {SM_EXP,              {1, sizeof(SMExp),                   "SM_EXP"             }},
                {SM_SHOWDROPITEM,     {1, sizeof(SMShowDropItem),          "SM_SHOWDROPITEM"    }},
                {SM_FIREMAGIC,        {1, sizeof(SMFireMagic),             "SM_FIREMAGIC"       }},
                {SM_OFFLINE,          {1, sizeof(SMOffline),               "SM_OFFLINE"         }},
                {SM_PICKUPOK,         {1, sizeof(SMPickUpOK),              "SM_PICKUPOK"        }},
                {SM_REMOVEGROUNDITEM, {1, sizeof(SMRemoveGroundItem),      "SM_REMOVEGROUNDITEM"}},
                {SM_GOLD,             {1, sizeof(SMGold),                  "SM_GOLD"            }},
            };

            return s_AttributeTable.at((s_AttributeTable.find(nHC) == s_AttributeTable.end()) ? (uint8_t)(SM_NONE) : nHC);
        }
};
