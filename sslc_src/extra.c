#include "parse.h"
#include "parselib.h"
#include "extra.h"

void parseExtraExpression(Procedure *p, int i)
{
	p = p;

	switch(i) {
		PARSEEXP(HAS_SKILL, 2);
		PARSEEXP(USING_SKILL, 2);
		PARSEEXP(ROLL_VS_SKILL, 3);
		PARSEEXP(SKILL_CONTEST, 3);
		PARSEEXP(DO_CHECK, 3);
		PARSEEXP(IS_SUCCESS, 1);
		PARSEEXP(IS_CRITICAL, 1);
		PARSEEXP(HOW_MUCH, 0);
		PARSEEXP(REACTION_INFLUENCE, 3);
		PARSEEXP(RANDOM, 2);
		PARSEEXP(ROLL_DICE, 2);
		PARSEEXP(MOVE_TO, 3);
		PARSEEXP(CREATE_OBJECT_SID, 4);
		PARSEEXP(OBJ_IS_CARRYING_OBJ_PID, 2);
		PARSEEXP(TILE_CONTAINS_OBJ_PID, 3);

#if (FALLOUT_VERSION == 2)
		PARSEEXP(TILE_IN_TILE_RECT, 5);
#endif

		PARSEEXP(SELF_OBJ, 0);
		PARSEEXP(SOURCE_OBJ, 0);
		PARSEEXP(TARGET_OBJ, 0);
		PARSEEXP(DUDE_OBJ, 0);
		PARSEEXP(OBJ_BEING_USED_WITH, 0);
		PARSEEXP(LOCAL_VAR, 1);
		PARSEEXP(MAP_VAR, 1);
		PARSEEXP(GLOBAL_VAR, 1);
		PARSEEXP(SCRIPT_ACTION, 0);
		PARSEEXP(OBJ_TYPE, 1);
		PARSEEXP(OBJ_ITEM_SUBTYPE, 1);
		PARSEEXP(GET_CRITTER_STAT, 2);
		PARSEEXP(SET_CRITTER_STAT, 3);
		PARSEEXP(TILE_DISTANCE, 2);
		PARSEEXP(TILE_DISTANCE_OBJS, 2);
		PARSEEXP(TILE_NUM, 1);
		PARSEEXP(TILE_NUM_IN_DIRECTION, 3);
		PARSEEXP(OBJ_CAN_SEE_OBJ, 2);
		PARSEEXP(ANIM_BUSY, 1);
		PARSEEXP(CRITTER_HEAL, 2);
		PARSEEXP(GAME_TIME, 0);
		PARSEEXP(GAME_TIME_IN_SECONDS, 0);
		PARSEEXP(ELEVATION, 1);
		PARSEEXP(GAME_TICKS, 1);
		PARSEEXP(HAS_TRAIT, 3);
		PARSEEXP(OBJ_CAN_HEAR_OBJ, 2);
		PARSEEXP(GAME_TIME_HOUR, 0);
		PARSEEXP(FIXED_PARAM, 0);
		PARSEEXP(TILE_IS_VISIBLE, 1);
		PARSEEXP(ACTION_BEING_USED, 0);
		PARSEEXP(CRITTER_STATE, 1);
		PARSEEXP(CRITTER_ATTEMPT_PLACEMENT, 3);
		PARSEEXP(OBJ_PID, 1);
		PARSEEXP(CUR_MAP_INDEX, 0);
		PARSEEXP(CRITTER_ADD_TRAIT, 4);
		PARSEEXP(CRITTER_RM_TRAIT, 4);
		PARSEEXP(PROTO_DATA, 2);
		PARSEEXP(MESSAGE_STR, 2);
		PARSEEXP(CRITTER_INVEN_OBJ, 2);

#if (FALLOUT_VERSION == 2)
		PARSEEXP(INVEN_CMDS, 3);
#endif

		PARSEEXP(METARULE, 2);

#if (FALLOUT_VERSION == 2)
		PARSEEXP(METARULE3, 4);
#endif

		PARSEEXP(OBJ_CARRYING_PID_OBJ, 2);
//		PARSEEXP(ADD_MULT_OBJS_TO_INVEN, 3);		// !!! It's statement only!!! Not expression!!!
		PARSEEXP(RM_MULT_OBJS_FROM_INVEN, 3);
		PARSEEXP(GET_MONTH, 0);
		PARSEEXP(GET_DAY, 0);
		PARSEEXP(DAYS_SINCE_VISITED, 0);
		PARSEEXP(GET_POISON, 1);
		PARSEEXP(COMBAT_IS_INITIALIZED, 0);
		PARSEEXP(DIFFICULTY_LEVEL, 0);
		PARSEEXP(RUNNING_BURNING_GUY, 0);
		PARSEEXP(OBJ_IS_LOCKED, 1);
		PARSEEXP(OBJ_IS_OPEN, 1);
		PARSEEXP(GAME_UI_IS_DISABLED, 0);
		PARSEEXP(ITEM_CAPS_TOTAL, 1);
		PARSEEXP(ITEM_CAPS_ADJUST, 2);
		PARSEEXP(ANIM_ACTION_FRAME, 2);
		PARSEEXP(DESTROY_MULT_OBJS, 2);
		PARSEEXP(OBJ_ART_FID, 1);
		PARSEEXP(ART_ANIM, 1);
		PARSEEXP(PARTY_MEMBER_OBJ, 1);
		PARSEEXP(ROTATION_TO_TILE, 2);
		PARSEEXP(SFX_BUILD_CHAR_NAME, 3);
		PARSEEXP(SFX_BUILD_AMBIENT_NAME, 1);
		PARSEEXP(SFX_BUILD_INTERFACE_NAME, 1);
		PARSEEXP(SFX_BUILD_ITEM_NAME, 1);
		PARSEEXP(SFX_BUILD_WEAPON_NAME, 4);
		PARSEEXP(SFX_BUILD_SCENERY_NAME, 3);
		PARSEEXP(SFX_BUILD_OPEN_NAME, 2);
		PARSEEXP(GET_PC_STAT, 1);
		PARSEEXP(COMBAT_DIFFICULTY, 0);
		PARSEEXP(OBJ_ON_SCREEN, 1);
		PARSEEXP(CRITTER_IS_FLEEING, 1);
//		PARSEEXP(TERMINATE_COMBAT, 0);			// !!! It's statement!!! Not Expression!!!
		PARSEEXP(TILE_CONTAINS_PID_OBJ, 3);
		PARSEEXP(OBJ_NAME, 1);

#if (FALLOUT_VERSION != 2)
		PARSEEXP(REACTION, 3);					// 426
#endif

	}
}


void parseExtraStatement(Procedure *p, int i) {
	switch(i) {
		PARSE(GIVE_EXP_POINTS, 1);				// 409
		PARSE(SCR_RETURN, 1);					// 410
		PARSE(PLAY_SFX, 1);						// 411

#if (FALLOUT_VERSION == 2)
		PARSE(MARK_AREA_KNOWN, 3);				// 426
#endif
		PARSE(SET_MAP_START, 4);				// 416
		PARSE(OVERRIDE_MAP_START, 4);			// 417
		PARSE(HAS_SKILL, 2);					// 418
		PARSE(USING_SKILL, 2);					// 419
		PARSE(ROLL_VS_SKILL, 3);				// 420
		PARSE(SKILL_CONTEST, 3);				// 421
		PARSE(DO_CHECK, 3);						// 422
		PARSE(ROLL_DICE, 2);					// 429
		PARSE(MOVE_TO, 3);						// 430
		PARSE(CREATE_OBJECT_SID, 4);			// 431
		PARSE(DISPLAY_MSG, 1);					// 432
		PARSE(SCRIPT_OVERRIDES, 0);				// 433
		PARSE(SET_LOCAL_VAR, 2);				// 442
		PARSE(SET_MAP_VAR, 2);					// 444
		PARSE(SET_GLOBAL_VAR, 2);				// 446
		PARSE(SET_CRITTER_STAT, 3);				// 451
		PARSE(ANIMATE_STAND_OBJ, 1);			// 452
		PARSE(ANIMATE_STAND_REVERSE_OBJ, 1);	// 453
		PARSE(ANIMATE_MOVE_OBJ_TO_TILE, 3);		// 454
		PARSE(ATTACK_COMPLEX, 8);				// 456
		PARSE(MAKE_DAYTIME, 0);					// 457
		PARSE(PICKUP_OBJ, 1);					// 462
		PARSE(DROP_OBJ, 1);						// 463
		PARSE(ADD_OBJ_TO_INVEN, 2);				// 464
		PARSE(RM_OBJ_FROM_INVEN, 2);			// 465
		PARSE(WIELD_OBJ_CRITTER, 2);			// 466
		PARSE(USE_OBJ, 1);						// 467
		PARSE(ATTACK, 8);						// 469
		PARSE(START_GDIALOG, 5);				// 470
		PARSE(END_DIALOGUE, 0);					// 471
		PARSE(DIALOGUE_REACTION, 1);			// 472

#if (FALLOUT_VERSION == 2)
		PARSE(SET_MAP_MUSIC, 2);				// 474
#endif

		PARSE(SET_OBJ_VISIBILITY, 2);			// 475
		PARSE(LOAD_MAP, 2);						// 476

#if (FALLOUT_VERSION == 2)
		PARSE(WM_AREA_SET_POS, 3);				// 477
		PARSE(SET_EXIT_GRIDS, 5);				// 478
#endif

		PARSE(CRITTER_HEAL, 2);					// 480
		PARSE(SET_LIGHT_LEVEL, 1);				// 481
		PARSE(KILL_CRITTER, 2);					// 485
		PARSE(KILL_CRITTER_TYPE, 2);			// 486
		PARSE(CRITTER_DMG, 3);					// 487
		PARSE(ADD_TIMER_EVENT, 3);				// 488
		PARSE(RM_TIMER_EVENT, 1);				// 489
		PARSE(DESTROY_OBJECT, 1);				// 492
		PARSE(DIALOGUE_SYSTEM_ENTER, 0);		// 497
		PARSE(GAME_TIME_ADVANCE, 1);			// 500
		PARSE(RADIATION_INC, 2);				// 501
		PARSE(RADIATION_DEC, 2);				// 502
		PARSE(CRITTER_ATTEMPT_PLACEMENT, 3);	// 503
		PARSE(CRITTER_ADD_TRAIT, 4);			// 506
		PARSE(CRITTER_RM_TRAIT, 4);				// 507
		PARSE(OBJ_SET_LIGHT_LEVEL, 3);			// 511
		PARSE(WORLD_MAP, 0);					// 512
		PARSE(FLOAT_MSG, 3);					// 514
		PARSE(METARULE, 2);						// 515

#if (FALLOUT_VERSION == 2)
		PARSE(METARULE3, 4);					// 473
#endif
		PARSE(ANIM, 3);							// 516
		PARSE(REG_ANIM_FUNC, 2);				// 518
		PARSE(REG_ANIM_ANIMATE, 3);				// 519
		PARSE(REG_ANIM_ANIMATE_REVERSE, 3);		// 520
		PARSE(REG_ANIM_OBJ_MOVE_TO_OBJ, 3);		// 521
		PARSE(REG_ANIM_OBJ_RUN_TO_OBJ, 3);		// 522
		PARSE(REG_ANIM_OBJ_MOVE_TO_TILE, 3);	// 523
		PARSE(REG_ANIM_OBJ_RUN_TO_TILE, 3);		// 524
		PARSE(PLAY_GMOVIE, 1);					// 525
		PARSE(ADD_MULT_OBJS_TO_INVEN, 3);		// 526
		PARSE(EXPLOSION, 3);					// 530
		PARSE(GSAY_START, 0);					// 532
		PARSE(GSAY_END, 0);						// 533
		PARSE(GSAY_REPLY, 2);					// 534
		PARSE(GSAY_OPTION, 4);					// 535
		PARSE(GSAY_MESSAGE, 3);					// 536
		PARSE(GIQ_OPTION, 5);					// 537
		PARSE(POISON, 2);						// 538
		PARSE(PARTY_ADD, 1);					// 540
		PARSE(PARTY_REMOVE, 1);					// 541
		PARSE(REG_ANIM_ANIMATE_FOREVER, 2);		// 542
		PARSE(CRITTER_INJURE, 2);				// 543
		PARSE(GDIALOG_MOD_BARTER, 1);			// 545
		PARSE(INVEN_UNWIELD, 0);				// 548
		PARSE(OBJ_LOCK, 1);						// 550
		PARSE(OBJ_UNLOCK, 1);					// 551
		PARSE(OBJ_OPEN, 1);						// 553
		PARSE(OBJ_CLOSE, 1);					// 554
		PARSE(GAME_UI_DISABLE, 0);				// 555
		PARSE(GAME_UI_ENABLE, 0);				// 556
		PARSE(GFADE_OUT, 1);					// 558
		PARSE(GFADE_IN, 1);						// 559
		PARSE(ITEM_CAPS_ADJUST, 2);				// 561
		PARSE(REG_ANIM_PLAY_SFX, 3);			// 563
		PARSE(CRITTER_MOD_SKILL, 3);			// 564
		PARSE(ATTACK_SETUP, 2);					// 571
		PARSE(USE_OBJ_ON_OBJ, 2);				// 573
		PARSE(ENDGAME_SLIDESHOW, 0);			// 574
		PARSE(MOVE_OBJ_INVEN_TO_OBJ, 2);		// 575
		PARSE(ENDGAME_MOVIE, 0);				// 576
		PARSE(JAM_LOCK, 1);						// 581
		PARSE(GDIALOG_SET_BARTER_MOD, 1);		// 582
		PARSE(CRITTER_SET_FLEE_STATE, 2);		// 586
		PARSE(TERMINATE_COMBAT, 0);				// 587 !!! Must be here
		PARSE(DEBUG_MSG, 1);					// 588
		PARSE(CRITTER_STOP_ATTACKING, 1);		// 589

#if (FALLOUT_VERSION != 2)
		PARSE(ANIMATE_JUMP, 0);					// 455
		PARSE(TURN_OFF_OBJS_IN_AREA, 4);		// 473
		PARSE(TURN_ON_OBJS_IN_AREA, 4);			// 474
		PARSE(BARTER_OFFER, 3);					// 477
		PARSE(BARTER_ASKING, 3);				// 478
		PARSE(TOWN_MAP, 0);						// 513
#endif
	}
}

int writeExtraStatement(NodeList *n, int i, OutputStream *f) {
	switch(n->nodes[i].token) {
		EXP(GIVE_EXP_POINTS, 1);				// 409
		EXP(SCR_RETURN, 1);						// 410
		EXP(PLAY_SFX, 1);						// 411

#if (FALLOUT_VERSION == 2)
		EXP(MARK_AREA_KNOWN, 3);				// 426
#endif

		EXP(SET_MAP_START, 4);					// 416
		EXP(OVERRIDE_MAP_START, 4);				// 417
		EXPST(HAS_SKILL, 2);					// 418
		EXPST(USING_SKILL, 2);					// 419
		EXPST(ROLL_VS_SKILL, 3);				// 420
		EXPST(SKILL_CONTEST, 3);				// 421
		EXPST(DO_CHECK, 3);						// 422
		EXPST(ROLL_DICE, 2);					// 429
		EXPST(MOVE_TO, 3);						// 430
		EXPST(CREATE_OBJECT_SID, 4);			// 431
		EXP(DISPLAY_MSG, 1);					// 432
		EXP(SCRIPT_OVERRIDES, 0);				// 433
		EXP(SET_LOCAL_VAR, 2);					// 442
		EXP(SET_MAP_VAR, 2);					// 444
		EXP(SET_GLOBAL_VAR, 2);					// 446
		EXPST(SET_CRITTER_STAT, 3);				// 451
		EXP(ANIMATE_STAND_OBJ, 1);				// 452
		EXP(ANIMATE_STAND_REVERSE_OBJ, 1);		// 453
		EXP(ANIMATE_MOVE_OBJ_TO_TILE, 3);		// 454
		EXP(ATTACK_COMPLEX, 8);					// 456
		EXP(MAKE_DAYTIME, 0);					// 457
		EXP(PICKUP_OBJ, 1);						// 462
		EXP(DROP_OBJ, 1);						// 463
		EXP(ADD_OBJ_TO_INVEN, 2);				// 464
		EXP(RM_OBJ_FROM_INVEN, 2);				// 465
		EXP(WIELD_OBJ_CRITTER, 2);				// 466
		EXP(USE_OBJ, 1);						// 467
		EXP(ATTACK, 8);							// 469
		EXP(START_GDIALOG, 5);					// 470
		EXP(END_DIALOGUE, 0);					// 471
		EXP(DIALOGUE_REACTION, 1);				// 472

#if (FALLOUT_VERSION == 2)
		EXP(SET_MAP_MUSIC, 2);					// 474
#endif

		EXP(SET_OBJ_VISIBILITY, 2);				// 475
		EXP(LOAD_MAP, 2);						// 476

#if (FALLOUT_VERSION == 2)
		EXP(WM_AREA_SET_POS, 3);				// 477
		EXP(SET_EXIT_GRIDS, 5);					// 478
#endif

		EXPST(CRITTER_HEAL, 2);					// 480
		EXP(SET_LIGHT_LEVEL, 1);				// 481
		EXP(KILL_CRITTER, 2);					// 485
		EXP(KILL_CRITTER_TYPE, 2);				// 486
		EXP(CRITTER_DMG, 3);					// 487
		EXP(ADD_TIMER_EVENT, 3);				// 488
		EXP(RM_TIMER_EVENT, 1);					// 489
		EXP(DESTROY_OBJECT, 1);					// 492
		EXP(DIALOGUE_SYSTEM_ENTER, 0);			// 497
		EXP(GAME_TIME_ADVANCE, 1);				// 500
		EXP(RADIATION_INC, 2);					// 501
		EXP(RADIATION_DEC, 2);					// 502
		EXPST(CRITTER_ATTEMPT_PLACEMENT, 3);	// 503
		EXPST(CRITTER_ADD_TRAIT, 4);			// 506
		EXPST(CRITTER_RM_TRAIT, 4);				// 507
		EXP(OBJ_SET_LIGHT_LEVEL, 3);			// 511
		EXP(WORLD_MAP, 0);						// 512
		EXP(FLOAT_MSG, 3);						// 514
		EXPST(METARULE, 2);						// 515

#if (FALLOUT_VERSION == 2)
		EXPST(METARULE3, 4);					// 473
#endif

		EXP(ANIM, 3);							// 516
		EXP(REG_ANIM_FUNC, 2);					// 518
		EXP(REG_ANIM_ANIMATE, 3);				// 519
		EXP(REG_ANIM_ANIMATE_REVERSE, 3);		// 520
		EXP(REG_ANIM_OBJ_MOVE_TO_OBJ, 3);		// 521
		EXP(REG_ANIM_OBJ_RUN_TO_OBJ, 3);		// 522
		EXP(REG_ANIM_OBJ_MOVE_TO_TILE, 3);		// 523
		EXP(REG_ANIM_OBJ_RUN_TO_TILE, 3);		// 524
		EXP(PLAY_GMOVIE, 1);					// 525
		EXP(ADD_MULT_OBJS_TO_INVEN, 3);			// 526
		EXP(EXPLOSION, 3);						// 530
		ST(GSAY_START);							// 532 !!!
		ST(GSAY_END);							// 533 !!!
		EXP(GSAY_REPLY, 2);						// 534
		EXPPROC(GSAY_OPTION, 4);				// 535 !!!
		EXP(GSAY_MESSAGE, 3);					// 536
		EXPPROC(GIQ_OPTION, 5);					// 537 !!!
		EXP(POISON, 2);							// 538
		EXP(PARTY_ADD, 1);						// 540
		EXP(PARTY_REMOVE, 1);					// 541
		EXP(REG_ANIM_ANIMATE_FOREVER, 2);		// 542
		EXP(CRITTER_INJURE, 2);					// 543
		EXP(GDIALOG_MOD_BARTER, 1);				// 545
		EXP(INVEN_UNWIELD, 0);					// 548
		EXP(OBJ_LOCK, 1);						// 550
		EXP(OBJ_UNLOCK, 1);						// 551
		EXP(OBJ_OPEN, 1);						// 553
		EXP(OBJ_CLOSE, 1);						// 554
		EXP(GAME_UI_DISABLE, 0);				// 555
		EXP(GAME_UI_ENABLE, 0);					// 556
		EXP(GFADE_OUT, 1);						// 558
		EXP(GFADE_IN, 1);						// 559
		EXPST(ITEM_CAPS_ADJUST, 2);				// 561
		EXP(REG_ANIM_PLAY_SFX, 3);				// 563
		EXP(CRITTER_MOD_SKILL, 3);				// 564
		EXP(ATTACK_SETUP, 2);					// 571
		EXP(USE_OBJ_ON_OBJ, 2);					// 573
		EXP(ENDGAME_SLIDESHOW, 0);				// 574
		EXP(MOVE_OBJ_INVEN_TO_OBJ, 2);			// 575
		EXP(ENDGAME_MOVIE, 0);					// 576
		EXP(JAM_LOCK, 1);						// 581
		EXP(GDIALOG_SET_BARTER_MOD, 1);			// 582
		EXP(CRITTER_SET_FLEE_STATE, 2);			// 586
		EXP(TERMINATE_COMBAT, 0);				// 587
		EXP(DEBUG_MSG, 1);						// 588
		EXP(CRITTER_STOP_ATTACKING, 1);			// 589

#if (FALLOUT_VERSION != 2)
		EXP(ANIMATE_JUMP, 0);					// 455
		EXP(TURN_OFF_OBJS_IN_AREA, 4);			// 473
		EXP(TURN_ON_OBJS_IN_AREA, 4);			// 474
		EXP(BARTER_OFFER, 3);					// 477
		EXP(BARTER_ASKING, 3);					// 478
		EXP(TOWN_MAP, 0);						// 513
#endif

		default:
			i++;
			break;
	}

	return i;
}

int writeExtraExpression(NodeList *n, int i, OutputStream *f) {
	f = f;

	switch(n->nodes[i].token) {
		EXP(HAS_SKILL, 2);						// 418
		EXP(USING_SKILL, 2);					// 419
		EXP(ROLL_VS_SKILL, 3);					// 420
		EXP(SKILL_CONTEST, 3);					// 421
		EXP(DO_CHECK, 3);						// 422
		EXP(IS_SUCCESS, 1);						// 423
		EXP(IS_CRITICAL, 1);					// 424
		EXP(HOW_MUCH, 0);						// 425
		EXP(REACTION_INFLUENCE, 3);				// 427
		EXP(RANDOM, 2);							// 428
		EXP(ROLL_DICE, 2);						// 429
		EXP(MOVE_TO, 3);						// 430
		EXP(CREATE_OBJECT_SID, 4);				// 431
		EXP(OBJ_IS_CARRYING_OBJ_PID, 2);		// 434
		EXP(TILE_CONTAINS_OBJ_PID, 3);			// 435

#if (FALLOUT_VERSION == 2)
		EXP(TILE_IN_TILE_RECT, 5);				// 455
#endif

		EXP(SELF_OBJ, 0);						// 436
		EXP(SOURCE_OBJ, 0);						// 437
		EXP(TARGET_OBJ, 0);						// 438
		EXP(DUDE_OBJ, 0);						// 439
		EXP(OBJ_BEING_USED_WITH, 0);			// 440
		EXP(LOCAL_VAR, 1);						// 441
		EXP(MAP_VAR, 1);						// 443
		EXP(GLOBAL_VAR, 1);						// 445
		EXP(SCRIPT_ACTION, 0);					// 447
		EXP(OBJ_TYPE, 1);						// 448
		EXP(OBJ_ITEM_SUBTYPE, 1);				// 449
		EXP(GET_CRITTER_STAT, 2);				// 450
		EXP(SET_CRITTER_STAT, 3);				// 451
		EXP(TILE_DISTANCE, 2);					// 458
		EXP(TILE_DISTANCE_OBJS, 2);				// 459
		EXP(TILE_NUM, 1);						// 460
		EXP(TILE_NUM_IN_DIRECTION, 3);			// 461
		EXP(OBJ_CAN_SEE_OBJ, 2);				// 468
		EXP(ANIM_BUSY, 1);						// 479
		EXP(CRITTER_HEAL, 2);					// 480
		EXP(GAME_TIME, 0);						// 482
		EXP(GAME_TIME_IN_SECONDS, 0);			// 483
		EXP(ELEVATION, 1);						// 484
		EXP(GAME_TICKS, 1);						// 490
		EXP(HAS_TRAIT, 3);						// 491
		EXP(OBJ_CAN_HEAR_OBJ, 2);				// 493
		EXP(GAME_TIME_HOUR, 0);					// 494
		EXP(FIXED_PARAM, 0);					// 495
		EXP(TILE_IS_VISIBLE, 1);				// 496
		EXP(ACTION_BEING_USED, 0);				// 498
		EXP(CRITTER_STATE, 1);					// 499
		EXP(CRITTER_ATTEMPT_PLACEMENT, 3);		// 503
		EXP(OBJ_PID, 1);						// 504
		EXP(CUR_MAP_INDEX, 0);					// 505
		EXP(CRITTER_ADD_TRAIT, 4);				// 506
		EXP(CRITTER_RM_TRAIT, 4);				// 507
		EXP(PROTO_DATA, 2);						// 508
		EXP(MESSAGE_STR, 2);					// 509
		EXP(CRITTER_INVEN_OBJ, 2);				// 510

#if (FALLOUT_VERSION == 2)
		EXP(INVEN_CMDS, 3);						// 513
#endif

		EXP(METARULE, 2);						// 515

#if (FALLOUT_VERSION == 2)
		EXP(METARULE3, 4);						// 473
#endif

		EXP(OBJ_CARRYING_PID_OBJ, 2);			// 517
//		EXP(ADD_MULT_OBJS_TO_INVEN, 3);			// 526
		EXP(RM_MULT_OBJS_FROM_INVEN, 3);		// 527
		EXP(GET_MONTH, 0);						// 528
		EXP(GET_DAY, 0);						// 529
		EXP(DAYS_SINCE_VISITED, 0);				// 531
		EXP(GET_POISON, 1);						// 539
		EXP(COMBAT_IS_INITIALIZED, 0);			// 544
		EXP(DIFFICULTY_LEVEL, 0);				// 546
		EXP(RUNNING_BURNING_GUY, 0);			// 547
		EXP(OBJ_IS_LOCKED, 1);					// 549
		EXP(OBJ_IS_OPEN, 1);					// 552
		EXP(GAME_UI_IS_DISABLED, 0);			// 557
		EXP(ITEM_CAPS_TOTAL, 1);				// 560
		EXP(ITEM_CAPS_ADJUST, 2);				// 561
		EXP(ANIM_ACTION_FRAME, 2);				// 562
		EXP(DESTROY_MULT_OBJS, 2);				// 572
		EXP(ENDGAME_SLIDESHOW, 0);				// 574
		EXP(ENDGAME_MOVIE, 0);					// 576
		EXP(OBJ_ART_FID, 1);					// 577
		EXP(ART_ANIM, 1);						// 578
		EXP(PARTY_MEMBER_OBJ, 1);				// 579
		EXP(ROTATION_TO_TILE, 2);				// 580
		EXP(SFX_BUILD_CHAR_NAME, 3);			// 565
		EXP(SFX_BUILD_AMBIENT_NAME, 1);			// 566
		EXP(SFX_BUILD_INTERFACE_NAME, 1);		// 567
		EXP(SFX_BUILD_ITEM_NAME, 1);			// 568
		EXP(SFX_BUILD_WEAPON_NAME, 4);			// 569
		EXP(SFX_BUILD_SCENERY_NAME, 3);			// 570
		EXP(SFX_BUILD_OPEN_NAME, 2);			// 413
		EXP(GET_PC_STAT, 1);					// 414
		EXP(COMBAT_DIFFICULTY, 0);				// 583
		EXP(OBJ_ON_SCREEN, 1);					// 584
		EXP(CRITTER_IS_FLEEING, 1);				// 585
		EXP(TILE_CONTAINS_PID_OBJ, 3);			// 415
		EXP(OBJ_NAME, 1);						// 412

#if (FALLOUT_VERSION != 2)
		EXP(REACTION, 3);						// 426
#endif
		
		default:
			i++;
			break;
	}

	return i;
}

void initLexExtras(void)
{
	lexAddToken(T_GIVE_EXP_POINTS, "give_exp_points");
	lexAddToken(T_SCR_RETURN, "scr_return");
	lexAddToken(T_PLAY_SFX, "play_sfx");
	lexAddToken(T_OBJ_NAME, "obj_name");
	lexAddToken(T_SFX_BUILD_OPEN_NAME, "sfx_build_open_name");
	lexAddToken(T_GET_PC_STAT, "get_pc_stat");
	lexAddToken(T_TILE_CONTAINS_PID_OBJ, "tile_contains_pid_obj");
	lexAddToken(T_SET_MAP_START, "set_map_start");
	lexAddToken(T_OVERRIDE_MAP_START, "override_map_start");
	lexAddToken(T_HAS_SKILL, "has_skill");
	lexAddToken(T_USING_SKILL, "using_skill");
	lexAddToken(T_ROLL_VS_SKILL, "roll_vs_skill");
	lexAddToken(T_SKILL_CONTEST, "skill_contest");
	lexAddToken(T_DO_CHECK, "do_check");
	lexAddToken(T_IS_SUCCESS, "is_success");
	lexAddToken(T_IS_CRITICAL, "is_critical");
	lexAddToken(T_HOW_MUCH, "how_much");

#if (FALLOUT_VERSION == 2)
	lexAddToken(T_MARK_AREA_KNOWN, "mark_area_known");
#else
	lexAddToken(T_REACTION, "reaction");
#endif

	lexAddToken(T_REACTION_INFLUENCE, "reaction_influence");
	lexAddToken(T_RANDOM, "random");
	lexAddToken(T_ROLL_DICE, "roll_dice");
	lexAddToken(T_MOVE_TO, "move_to");
	lexAddToken(T_CREATE_OBJECT_SID, "create_object_sid");
	lexAddToken(T_DISPLAY_MSG, "display_msg");
	lexAddToken(T_SCRIPT_OVERRIDES, "script_overrides");
	lexAddToken(T_OBJ_IS_CARRYING_OBJ_PID, "obj_is_carrying_obj_pid");
	lexAddToken(T_TILE_CONTAINS_OBJ_PID, "tile_contains_obj_pid");
	lexAddToken(T_SELF_OBJ, "self_obj");
	lexAddToken(T_SOURCE_OBJ, "source_obj");
	lexAddToken(T_TARGET_OBJ, "target_obj");
	lexAddToken(T_DUDE_OBJ, "dude_obj");
	lexAddToken(T_OBJ_BEING_USED_WITH, "obj_being_used_with");
	lexAddToken(T_LOCAL_VAR, "local_var");
	lexAddToken(T_SET_LOCAL_VAR, "set_local_var");
	lexAddToken(T_MAP_VAR, "map_var");
	lexAddToken(T_SET_MAP_VAR, "set_map_var");
	lexAddToken(T_GLOBAL_VAR, "global_var");
	lexAddToken(T_SET_GLOBAL_VAR, "set_global_var");
	lexAddToken(T_SCRIPT_ACTION, "script_action");
	lexAddToken(T_OBJ_TYPE, "obj_type");
	lexAddToken(T_OBJ_ITEM_SUBTYPE, "obj_item_subtype");
	lexAddToken(T_GET_CRITTER_STAT, "get_critter_stat");
	lexAddToken(T_SET_CRITTER_STAT, "set_critter_stat");
	lexAddToken(T_ANIMATE_STAND_OBJ, "animate_stand_obj");
	lexAddToken(T_ANIMATE_STAND_REVERSE_OBJ, "animate_stand_reverse_obj");
	lexAddToken(T_ANIMATE_MOVE_OBJ_TO_TILE, "animate_move_obj_to_tile");

#if (FALLOUT_VERSION == 2)
	lexAddToken(T_TILE_IN_TILE_RECT, "tile_in_tile_rect");
#else
	lexAddToken(T_ANIMATE_JUMP, "animate_jump");
#endif

	lexAddToken(T_ATTACK_COMPLEX, "attack_complex");
	lexAddToken(T_MAKE_DAYTIME, "make_daytime");
	lexAddToken(T_TILE_DISTANCE, "tile_distance");
	lexAddToken(T_TILE_DISTANCE_OBJS, "tile_distance_objs");
	lexAddToken(T_TILE_NUM, "tile_num");
	lexAddToken(T_TILE_NUM_IN_DIRECTION, "tile_num_in_direction");
	lexAddToken(T_PICKUP_OBJ, "pickup_obj");
	lexAddToken(T_DROP_OBJ, "drop_obj");
	lexAddToken(T_ADD_OBJ_TO_INVEN, "add_obj_to_inven");
	lexAddToken(T_RM_OBJ_FROM_INVEN, "rm_obj_from_inven");
	lexAddToken(T_WIELD_OBJ_CRITTER, "wield_obj_critter");
	lexAddToken(T_USE_OBJ, "use_obj");
	lexAddToken(T_OBJ_CAN_SEE_OBJ, "obj_can_see_obj");
	lexAddToken(T_ATTACK, "attack");
	lexAddToken(T_START_GDIALOG, "start_gdialog");
	lexAddToken(T_END_DIALOGUE, "end_dialogue");
	lexAddToken(T_DIALOGUE_REACTION, "dialogue_reaction");

#if (FALLOUT_VERSION == 2)
	lexAddToken(T_METARULE3, "metarule3");
	lexAddToken(T_SET_MAP_MUSIC, "set_map_music");
#else
	lexAddToken(T_TURN_OFF_OBJS_IN_AREA, "turn_off_objs_in_area");
	lexAddToken(T_TURN_ON_OBJS_IN_AREA, "turn_on_objs_in_area");
#endif

	lexAddToken(T_SET_OBJ_VISIBILITY, "set_obj_visibility");
	lexAddToken(T_LOAD_MAP, "load_map");

#if (FALLOUT_VERSION == 2)
	lexAddToken(T_WM_AREA_SET_POS, "wm_area_set_pos");
	lexAddToken(T_SET_EXIT_GRIDS, "set_exit_grids");
#else
	lexAddToken(T_BARTER_OFFER, "barter_offer");
	lexAddToken(T_BARTER_ASKING, "barter_asking");
#endif

	lexAddToken(T_ANIM_BUSY, "anim_busy");
	lexAddToken(T_CRITTER_HEAL, "critter_heal");
	lexAddToken(T_SET_LIGHT_LEVEL, "set_light_level");
	lexAddToken(T_GAME_TIME, "game_time");
	lexAddToken(T_GAME_TIME_IN_SECONDS, "game_time_in_seconds");
	lexAddToken(T_ELEVATION, "elevation");
	lexAddToken(T_KILL_CRITTER, "kill_critter");
	lexAddToken(T_KILL_CRITTER_TYPE, "kill_critter_type");
	lexAddToken(T_CRITTER_DMG, "critter_dmg");
	lexAddToken(T_ADD_TIMER_EVENT, "add_timer_event");
	lexAddToken(T_RM_TIMER_EVENT, "rm_timer_event");
	lexAddToken(T_GAME_TICKS, "game_ticks");
	lexAddToken(T_HAS_TRAIT, "has_trait");
	lexAddToken(T_DESTROY_OBJECT, "destroy_object");
	lexAddToken(T_OBJ_CAN_HEAR_OBJ, "obj_can_hear_obj");
	lexAddToken(T_GAME_TIME_HOUR, "game_time_hour");
	lexAddToken(T_FIXED_PARAM, "fixed_param");
	lexAddToken(T_TILE_IS_VISIBLE, "tile_is_visible");
	lexAddToken(T_DIALOGUE_SYSTEM_ENTER, "dialogue_system_enter");
	lexAddToken(T_ACTION_BEING_USED, "action_being_used");
	lexAddToken(T_CRITTER_STATE, "critter_state");
	lexAddToken(T_GAME_TIME_ADVANCE, "game_time_advance");
	lexAddToken(T_RADIATION_INC, "radiation_inc");
	lexAddToken(T_RADIATION_DEC, "radiation_dec");
	lexAddToken(T_CRITTER_ATTEMPT_PLACEMENT, "critter_attempt_placement");
	lexAddToken(T_OBJ_PID, "obj_pid");
	lexAddToken(T_CUR_MAP_INDEX, "cur_map_index");
	lexAddToken(T_CRITTER_ADD_TRAIT, "critter_add_trait");
	lexAddToken(T_CRITTER_RM_TRAIT, "critter_rm_trait");
	lexAddToken(T_PROTO_DATA, "proto_data");
	lexAddToken(T_MESSAGE_STR, "message_str");
	lexAddToken(T_CRITTER_INVEN_OBJ, "critter_inven_obj");
	lexAddToken(T_OBJ_SET_LIGHT_LEVEL, "obj_set_light_level");
	lexAddToken(T_WORLD_MAP, "world_map");

#if (FALLOUT_VERSION == 2)
	lexAddToken(T_INVEN_CMDS, "inven_cmds");
#else
	lexAddToken(T_TOWN_MAP, "town_map");
#endif

	lexAddToken(T_FLOAT_MSG, "float_msg");
	lexAddToken(T_METARULE, "metarule");
	lexAddToken(T_ANIM, "anim");
	lexAddToken(T_OBJ_CARRYING_PID_OBJ, "obj_carrying_pid_obj");
	lexAddToken(T_REG_ANIM_FUNC, "REG_ANIM_FUNC");
	lexAddToken(T_REG_ANIM_ANIMATE, "reg_anim_animate");
	lexAddToken(T_REG_ANIM_ANIMATE_REVERSE, "reg_anim_animate_reverse");
	lexAddToken(T_REG_ANIM_OBJ_MOVE_TO_OBJ, "reg_anim_obj_move_to_obj");
	lexAddToken(T_REG_ANIM_OBJ_RUN_TO_OBJ, "reg_anim_obj_run_to_obj");
	lexAddToken(T_REG_ANIM_OBJ_MOVE_TO_TILE, "reg_anim_obj_move_to_tile");
	lexAddToken(T_REG_ANIM_OBJ_RUN_TO_TILE, "reg_anim_obj_run_to_tile");
	lexAddToken(T_PLAY_GMOVIE, "play_gmovie");
	lexAddToken(T_ADD_MULT_OBJS_TO_INVEN, "add_mult_objs_to_inven");
	lexAddToken(T_RM_MULT_OBJS_FROM_INVEN, "rm_mult_objs_from_inven");
	lexAddToken(T_GET_MONTH, "get_month");
	lexAddToken(T_GET_DAY, "get_day");
	lexAddToken(T_EXPLOSION, "explosion");
	lexAddToken(T_DAYS_SINCE_VISITED, "days_since_visited");
	lexAddToken(T_GSAY_START, "gsay_start");
	lexAddToken(T_GSAY_END, "gsay_end");
	lexAddToken(T_GSAY_REPLY, "gsay_reply");
	lexAddToken(T_GSAY_OPTION, "gsay_option");
	lexAddToken(T_GSAY_MESSAGE, "gsay_message");
	lexAddToken(T_GIQ_OPTION, "giq_option");
	lexAddToken(T_POISON, "poison");
	lexAddToken(T_GET_POISON, "get_poison");
	lexAddToken(T_PARTY_ADD, "party_add");
	lexAddToken(T_PARTY_REMOVE, "party_remove");
	lexAddToken(T_REG_ANIM_ANIMATE_FOREVER, "reg_anim_animate_forever");
	lexAddToken(T_CRITTER_INJURE, "critter_injure");
	lexAddToken(T_COMBAT_IS_INITIALIZED, "combat_is_initialized");
	lexAddToken(T_GDIALOG_MOD_BARTER, "gdialog_mod_barter");
	lexAddToken(T_DIFFICULTY_LEVEL, "difficulty_level");
	lexAddToken(T_RUNNING_BURNING_GUY, "running_burning_guy");
	lexAddToken(T_INVEN_UNWIELD, "inven_unwield");
	lexAddToken(T_OBJ_IS_LOCKED, "obj_is_locked");
	lexAddToken(T_OBJ_LOCK, "obj_lock");
	lexAddToken(T_OBJ_UNLOCK, "obj_unlock");
	lexAddToken(T_OBJ_IS_OPEN, "obj_is_open");
	lexAddToken(T_OBJ_OPEN, "obj_open");
	lexAddToken(T_OBJ_CLOSE, "obj_close");
	lexAddToken(T_GAME_UI_DISABLE, "game_ui_disable");
	lexAddToken(T_GAME_UI_ENABLE, "game_ui_enable");
	lexAddToken(T_GAME_UI_IS_DISABLED, "game_ui_is_disabled");
	lexAddToken(T_GFADE_OUT, "gfade_out");
	lexAddToken(T_GFADE_IN, "gfade_in");
	lexAddToken(T_ITEM_CAPS_TOTAL, "item_caps_total");
	lexAddToken(T_ITEM_CAPS_ADJUST, "item_caps_adjust");
	lexAddToken(T_ANIM_ACTION_FRAME, "anim_action_frame");
	lexAddToken(T_REG_ANIM_PLAY_SFX, "reg_anim_play_sfx");
	lexAddToken(T_CRITTER_MOD_SKILL, "critter_mod_skill");
	lexAddToken(T_SFX_BUILD_CHAR_NAME, "sfx_build_char_name");
	lexAddToken(T_SFX_BUILD_AMBIENT_NAME, "sfx_build_ambient_name");
	lexAddToken(T_SFX_BUILD_INTERFACE_NAME, "sfx_build_interface_name");
	lexAddToken(T_SFX_BUILD_ITEM_NAME, "sfx_build_item_name");
	lexAddToken(T_SFX_BUILD_WEAPON_NAME, "sfx_build_weapon_name");
	lexAddToken(T_SFX_BUILD_SCENERY_NAME, "sfx_build_scenery_name");
	lexAddToken(T_ATTACK_SETUP, "attack_setup");
	lexAddToken(T_DESTROY_MULT_OBJS, "destroy_mult_objs");
	lexAddToken(T_USE_OBJ_ON_OBJ, "use_obj_on_obj");
	lexAddToken(T_ENDGAME_SLIDESHOW, "endgame_slideshow");
	lexAddToken(T_MOVE_OBJ_INVEN_TO_OBJ, "move_obj_inven_to_obj");
	lexAddToken(T_ENDGAME_MOVIE, "endgame_movie");
	lexAddToken(T_OBJ_ART_FID, "obj_art_fid");
	lexAddToken(T_ART_ANIM, "art_anim");
	lexAddToken(T_PARTY_MEMBER_OBJ, "party_member_obj");
	lexAddToken(T_ROTATION_TO_TILE, "rotation_to_tile");
	lexAddToken(T_JAM_LOCK, "jam_lock");
	lexAddToken(T_GDIALOG_SET_BARTER_MOD, "gdialog_set_barter_mod");
	lexAddToken(T_COMBAT_DIFFICULTY, "combat_difficulty");
	lexAddToken(T_OBJ_ON_SCREEN, "obj_on_screen");
	lexAddToken(T_CRITTER_IS_FLEEING, "critter_is_fleeing");
	lexAddToken(T_CRITTER_SET_FLEE_STATE, "critter_set_flee_state");
	lexAddToken(T_TERMINATE_COMBAT, "terminate_combat");
	lexAddToken(T_DEBUG_MSG, "debug_msg");
	lexAddToken(T_CRITTER_STOP_ATTACKING, "critter_stop_attacking");
}
