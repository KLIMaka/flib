#ifndef _EXTRA_H_
#define _EXTRA_H_

#ifndef FALLOUT_VERSION
#define FALLOUT_VERSION 2
#endif

#include "parse.h"
#include "parselib.h"

//
// Parse library functions.  Given the procedure list and
// what token was just parsed.  Parse everything up to but
// not including the ending ; for this statement.
void parseExtraStatement(Procedure *, int);
void parseExtraExpression(Procedure *p, int i);

//
// code generation function for extra opcodes.
// receives the parsed list of things, the position in that
// list, and the stream to write to.  Generate code, and return
// the position in the last immediately *after* the the statement
// written.
int writeExtraStatement(NodeList *n, int i, OutputStream *);

// write out an expression (library functions that return values)
// These are kind of annoying, since the statement versions have
// to realize they are in a statement instead of in an expression.
// For the statement version, you have to generate the call to the
// interpreter function, and follow it with a pop_stack opcode.
// This means that you have to have code generators in both the
// extra statement and extra expression functions (or, if you're
// clever, you can recursively call writeStatement from
// writeExtraExpression, and let writeExtraExpression conditionally
// write out the pop if it can figure out it's in a statement).
int writeExtraExpression(NodeList *n, int i, OutputStream *f);


// all extra tokens and their meanings
void initLexExtras(void);

#include "opextra.h"

enum {
	C_END_CONSTANT = C_END_LIB,
};

enum {
	// define any user-specific tokens here, starting with T_END_LIB,
	// ending with T_END_TOKEN
	T_GIVE_EXP_POINTS = T_END_LIB,
	T_SCR_RETURN,
	T_PLAY_SFX,
	T_OBJ_NAME,
	T_SFX_BUILD_OPEN_NAME,
	T_GET_PC_STAT,
	T_TILE_CONTAINS_PID_OBJ,
	T_SET_MAP_START,
	T_OVERRIDE_MAP_START,
	T_HAS_SKILL,
	T_USING_SKILL,
	T_ROLL_VS_SKILL,
	T_SKILL_CONTEST,
	T_DO_CHECK,
	T_IS_SUCCESS,
	T_IS_CRITICAL,
	T_HOW_MUCH,

#if (FALLOUT_VERSION == 2)
	T_MARK_AREA_KNOWN,
#else
	T_REACTION,
#endif

	T_REACTION_INFLUENCE,
	T_RANDOM,
	T_ROLL_DICE,
	T_MOVE_TO,
	T_CREATE_OBJECT_SID,
	T_DISPLAY_MSG,
	T_SCRIPT_OVERRIDES,
	T_OBJ_IS_CARRYING_OBJ_PID,
	T_TILE_CONTAINS_OBJ_PID,
	T_SELF_OBJ,
	T_SOURCE_OBJ,
	T_TARGET_OBJ,
	T_DUDE_OBJ,
	T_OBJ_BEING_USED_WITH,
	T_LOCAL_VAR,
	T_SET_LOCAL_VAR,
	T_MAP_VAR,
	T_SET_MAP_VAR,
	T_GLOBAL_VAR,
	T_SET_GLOBAL_VAR,
	T_SCRIPT_ACTION,
	T_OBJ_TYPE,
	T_OBJ_ITEM_SUBTYPE,
	T_GET_CRITTER_STAT,
	T_SET_CRITTER_STAT,
	T_ANIMATE_STAND_OBJ,
	T_ANIMATE_STAND_REVERSE_OBJ,
	T_ANIMATE_MOVE_OBJ_TO_TILE,

#if (FALLOUT_VERSION == 2)
	T_TILE_IN_TILE_RECT,
#else
	T_ANIMATE_JUMP,
#endif

	T_ATTACK_COMPLEX,
	T_MAKE_DAYTIME,
	T_TILE_DISTANCE,
	T_TILE_DISTANCE_OBJS,
	T_TILE_NUM,
	T_TILE_NUM_IN_DIRECTION,
	T_PICKUP_OBJ,
	T_DROP_OBJ,
	T_ADD_OBJ_TO_INVEN,
	T_RM_OBJ_FROM_INVEN,
	T_WIELD_OBJ_CRITTER,
	T_USE_OBJ,
	T_OBJ_CAN_SEE_OBJ,
	T_ATTACK,
	T_START_GDIALOG,
	T_END_DIALOGUE,
	T_DIALOGUE_REACTION,

#if (FALLOUT_VERSION == 2)
	T_METARULE3,
	T_SET_MAP_MUSIC,
#else
	T_TURN_OFF_OBJS_IN_AREA,
	T_TURN_ON_OBJS_IN_AREA,
#endif

	T_SET_OBJ_VISIBILITY,
	T_LOAD_MAP,

#if (FALLOUT_VERSION == 2)
	T_WM_AREA_SET_POS,
	T_SET_EXIT_GRIDS,
#else
	T_BARTER_OFFER,
	T_BARTER_ASKING,
#endif

	T_ANIM_BUSY,
	T_CRITTER_HEAL,
	T_SET_LIGHT_LEVEL,
	T_GAME_TIME,
	T_GAME_TIME_IN_SECONDS,
	T_ELEVATION,
	T_KILL_CRITTER,
	T_KILL_CRITTER_TYPE,
	T_CRITTER_DMG,
	T_ADD_TIMER_EVENT,
	T_RM_TIMER_EVENT,
	T_GAME_TICKS,
	T_HAS_TRAIT,
	T_DESTROY_OBJECT,
	T_OBJ_CAN_HEAR_OBJ,
	T_GAME_TIME_HOUR,
	T_FIXED_PARAM,
	T_TILE_IS_VISIBLE,
	T_DIALOGUE_SYSTEM_ENTER,
	T_ACTION_BEING_USED,
	T_CRITTER_STATE,
	T_GAME_TIME_ADVANCE,
	T_RADIATION_INC,
	T_RADIATION_DEC,
	T_CRITTER_ATTEMPT_PLACEMENT,
	T_OBJ_PID,
	T_CUR_MAP_INDEX,
	T_CRITTER_ADD_TRAIT,
	T_CRITTER_RM_TRAIT,
	T_PROTO_DATA,
	T_MESSAGE_STR,
	T_CRITTER_INVEN_OBJ,
	T_OBJ_SET_LIGHT_LEVEL,
	T_WORLD_MAP,

#if (FALLOUT_VERSION == 2)
	T_INVEN_CMDS,
#else
	T_TOWN_MAP,
#endif

	T_FLOAT_MSG,
	T_METARULE,
	T_ANIM,
	T_OBJ_CARRYING_PID_OBJ,
	T_REG_ANIM_FUNC,
	T_REG_ANIM_ANIMATE,
	T_REG_ANIM_ANIMATE_REVERSE,
	T_REG_ANIM_OBJ_MOVE_TO_OBJ,
	T_REG_ANIM_OBJ_RUN_TO_OBJ,
	T_REG_ANIM_OBJ_MOVE_TO_TILE,
	T_REG_ANIM_OBJ_RUN_TO_TILE,
	T_PLAY_GMOVIE,
	T_ADD_MULT_OBJS_TO_INVEN,
	T_RM_MULT_OBJS_FROM_INVEN,
	T_GET_MONTH,
	T_GET_DAY,
	T_EXPLOSION,
	T_DAYS_SINCE_VISITED,
	T_GSAY_START,
	T_GSAY_END,
	T_GSAY_REPLY,
	T_GSAY_OPTION,
	T_GSAY_MESSAGE,
	T_GIQ_OPTION,
	T_POISON,
	T_GET_POISON,
	T_PARTY_ADD,
	T_PARTY_REMOVE,
	T_REG_ANIM_ANIMATE_FOREVER,
	T_CRITTER_INJURE,
	T_COMBAT_IS_INITIALIZED,
	T_GDIALOG_MOD_BARTER,
	T_DIFFICULTY_LEVEL,
	T_RUNNING_BURNING_GUY,
	T_INVEN_UNWIELD,
	T_OBJ_IS_LOCKED,
	T_OBJ_LOCK,
	T_OBJ_UNLOCK,
	T_OBJ_IS_OPEN,
	T_OBJ_OPEN,
	T_OBJ_CLOSE,
	T_GAME_UI_DISABLE,
	T_GAME_UI_ENABLE,
	T_GAME_UI_IS_DISABLED,
	T_GFADE_OUT,
	T_GFADE_IN,
	T_ITEM_CAPS_TOTAL,
	T_ITEM_CAPS_ADJUST,
	T_ANIM_ACTION_FRAME,
	T_REG_ANIM_PLAY_SFX,
	T_CRITTER_MOD_SKILL,
	T_SFX_BUILD_CHAR_NAME,
	T_SFX_BUILD_AMBIENT_NAME,
	T_SFX_BUILD_INTERFACE_NAME,
	T_SFX_BUILD_ITEM_NAME,
	T_SFX_BUILD_WEAPON_NAME,
	T_SFX_BUILD_SCENERY_NAME,
	T_ATTACK_SETUP,
	T_DESTROY_MULT_OBJS,
	T_USE_OBJ_ON_OBJ,
	T_ENDGAME_SLIDESHOW,
	T_MOVE_OBJ_INVEN_TO_OBJ,
	T_ENDGAME_MOVIE,
	T_OBJ_ART_FID,
	T_ART_ANIM,
	T_PARTY_MEMBER_OBJ,
	T_ROTATION_TO_TILE,
	T_JAM_LOCK,
	T_GDIALOG_SET_BARTER_MOD,
	T_COMBAT_DIFFICULTY,
	T_OBJ_ON_SCREEN,
	T_CRITTER_IS_FLEEING,
	T_CRITTER_SET_FLEE_STATE,
	T_TERMINATE_COMBAT,
	T_DEBUG_MSG,
	T_CRITTER_STOP_ATTACKING,

	T_END_TOKEN
};

#endif
