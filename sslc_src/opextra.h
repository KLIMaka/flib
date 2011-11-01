#ifndef _OPEXTRA_H_
#define _OPEXTRA_H_

#ifndef FALLOUT_VERSION
#define FALLOUT_VERSION 2
#endif

#include "oplib.h"

enum {
   // define any user-specific opcodes here, starting at O_END_LIB
   // ending with O_END_OP
	O_GIVE_EXP_POINTS = O_END_LIB,
	O_SCR_RETURN,
	O_PLAY_SFX,
	O_OBJ_NAME,
	O_SFX_BUILD_OPEN_NAME,
	O_GET_PC_STAT,
	O_TILE_CONTAINS_PID_OBJ,
	O_SET_MAP_START,
	O_OVERRIDE_MAP_START,
	O_HAS_SKILL,
	O_USING_SKILL,
	O_ROLL_VS_SKILL,
	O_SKILL_CONTEST,
	O_DO_CHECK,
	O_IS_SUCCESS,
	O_IS_CRITICAL,
	O_HOW_MUCH,

#if (FALLOUT_VERSION == 2)
	O_MARK_AREA_KNOWN,
#else
	O_REACTION,
#endif

	O_REACTION_INFLUENCE,
	O_RANDOM,
	O_ROLL_DICE,
	O_MOVE_TO,
	O_CREATE_OBJECT_SID,
	O_DISPLAY_MSG,
	O_SCRIPT_OVERRIDES,
	O_OBJ_IS_CARRYING_OBJ_PID,
	O_TILE_CONTAINS_OBJ_PID,
	O_SELF_OBJ,
	O_SOURCE_OBJ,
	O_TARGET_OBJ,
	O_DUDE_OBJ,
	O_OBJ_BEING_USED_WITH,
	O_LOCAL_VAR,
	O_SET_LOCAL_VAR,
	O_MAP_VAR,
	O_SET_MAP_VAR,
	O_GLOBAL_VAR,
	O_SET_GLOBAL_VAR,
	O_SCRIPT_ACTION,
	O_OBJ_TYPE,
	O_OBJ_ITEM_SUBTYPE,
	O_GET_CRITTER_STAT,
	O_SET_CRITTER_STAT,
	O_ANIMATE_STAND_OBJ,
	O_ANIMATE_STAND_REVERSE_OBJ,
	O_ANIMATE_MOVE_OBJ_TO_TILE,

#if (FALLOUT_VERSION == 2)
	O_TILE_IN_TILE_RECT,
#else
	O_ANIMATE_JUMP,
#endif

	O_ATTACK_COMPLEX,
	O_MAKE_DAYTIME,
	O_TILE_DISTANCE,
	O_TILE_DISTANCE_OBJS,
	O_TILE_NUM,
	O_TILE_NUM_IN_DIRECTION,
	O_PICKUP_OBJ,
	O_DROP_OBJ,
	O_ADD_OBJ_TO_INVEN,
	O_RM_OBJ_FROM_INVEN,
	O_WIELD_OBJ_CRITTER,
	O_USE_OBJ,
	O_OBJ_CAN_SEE_OBJ,
	O_ATTACK,
	O_START_GDIALOG,
	O_END_DIALOGUE,
	O_DIALOGUE_REACTION,

#if (FALLOUT_VERSION == 2)
	O_METARULE3,
	O_SET_MAP_MUSIC,
#else
	O_TURN_OFF_OBJS_IN_AREA,
	O_TURN_ON_OBJS_IN_AREA,
#endif

	O_SET_OBJ_VISIBILITY,
	O_LOAD_MAP,

#if (FALLOUT_VERSION == 2)
	O_WM_AREA_SET_POS,
	O_SET_EXIT_GRIDS,
#else
	O_BARTER_OFFER,
	O_BARTER_ASKING,
#endif

	O_ANIM_BUSY,
	O_CRITTER_HEAL,
	O_SET_LIGHT_LEVEL,
	O_GAME_TIME,
	O_GAME_TIME_IN_SECONDS,
	O_ELEVATION,
	O_KILL_CRITTER,
	O_KILL_CRITTER_TYPE,
	O_CRITTER_DMG,
	O_ADD_TIMER_EVENT,
	O_RM_TIMER_EVENT,
	O_GAME_TICKS,
	O_HAS_TRAIT,
	O_DESTROY_OBJECT,
	O_OBJ_CAN_HEAR_OBJ,
	O_GAME_TIME_HOUR,
	O_FIXED_PARAM,
	O_TILE_IS_VISIBLE,
	O_DIALOGUE_SYSTEM_ENTER,
	O_ACTION_BEING_USED,
	O_CRITTER_STATE,
	O_GAME_TIME_ADVANCE,
	O_RADIATION_INC,
	O_RADIATION_DEC,
	O_CRITTER_ATTEMPT_PLACEMENT,
	O_OBJ_PID,
	O_CUR_MAP_INDEX,
	O_CRITTER_ADD_TRAIT,
	O_CRITTER_RM_TRAIT,
	O_PROTO_DATA,
	O_MESSAGE_STR,
	O_CRITTER_INVEN_OBJ,
	O_OBJ_SET_LIGHT_LEVEL,
	O_WORLD_MAP,

#if (FALLOUT_VERSION == 2)
	O_INVEN_CMDS,
#else
	O_TOWN_MAP,
#endif

	O_FLOAT_MSG,
	O_METARULE,
	O_ANIM,
	O_OBJ_CARRYING_PID_OBJ,
	O_REG_ANIM_FUNC,
	O_REG_ANIM_ANIMATE,
	O_REG_ANIM_ANIMATE_REVERSE,
	O_REG_ANIM_OBJ_MOVE_TO_OBJ,
	O_REG_ANIM_OBJ_RUN_TO_OBJ,
	O_REG_ANIM_OBJ_MOVE_TO_TILE,
	O_REG_ANIM_OBJ_RUN_TO_TILE,
	O_PLAY_GMOVIE,
	O_ADD_MULT_OBJS_TO_INVEN,
	O_RM_MULT_OBJS_FROM_INVEN,
	O_GET_MONTH,
	O_GET_DAY,
	O_EXPLOSION,
	O_DAYS_SINCE_VISITED,
	O_GSAY_START,
	O_GSAY_END,
	O_GSAY_REPLY,
	O_GSAY_OPTION,
	O_GSAY_MESSAGE,
	O_GIQ_OPTION,
	O_POISON,
	O_GET_POISON,
	O_PARTY_ADD,
	O_PARTY_REMOVE,
	O_REG_ANIM_ANIMATE_FOREVER,
	O_CRITTER_INJURE,
	O_COMBAT_IS_INITIALIZED,
	O_GDIALOG_MOD_BARTER,
	O_DIFFICULTY_LEVEL,
	O_RUNNING_BURNING_GUY,
	O_INVEN_UNWIELD,
	O_OBJ_IS_LOCKED,
	O_OBJ_LOCK,
	O_OBJ_UNLOCK,
	O_OBJ_IS_OPEN,
	O_OBJ_OPEN,
	O_OBJ_CLOSE,
	O_GAME_UI_DISABLE,
	O_GAME_UI_ENABLE,
	O_GAME_UI_IS_DISABLED,
	O_GFADE_OUT,
	O_GFADE_IN,
	O_ITEM_CAPS_TOTAL,
	O_ITEM_CAPS_ADJUST,
	O_ANIM_ACTION_FRAME,
	O_REG_ANIM_PLAY_SFX,
	O_CRITTER_MOD_SKILL,
	O_SFX_BUILD_CHAR_NAME,
	O_SFX_BUILD_AMBIENT_NAME,
	O_SFX_BUILD_INTERFACE_NAME,
	O_SFX_BUILD_ITEM_NAME,
	O_SFX_BUILD_WEAPON_NAME,
	O_SFX_BUILD_SCENERY_NAME,
	O_ATTACK_SETUP,
	O_DESTROY_MULT_OBJS,
	O_USE_OBJ_ON_OBJ,
	O_ENDGAME_SLIDESHOW,
	O_MOVE_OBJ_INVEN_TO_OBJ,
	O_ENDGAME_MOVIE,
	O_OBJ_ART_FID,
	O_ART_ANIM,
	O_PARTY_MEMBER_OBJ,
	O_ROTATION_TO_TILE,
	O_JAM_LOCK,
	O_GDIALOG_SET_BARTER_MOD,
	O_COMBAT_DIFFICULTY,
	O_OBJ_ON_SCREEN,
	O_CRITTER_IS_FLEEING,
	O_CRITTER_SET_FLEE_STATE,
	O_TERMINATE_COMBAT,
	O_DEBUG_MSG,
	O_CRITTER_STOP_ATTACKING,

	O_END_OP,
};

#endif

