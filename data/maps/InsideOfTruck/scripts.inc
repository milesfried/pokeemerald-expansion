InsideOfTruck_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, InsideOfTruck_OnLoad
	map_script MAP_SCRIPT_ON_RESUME, InsideOfTruck_OnResume
	.byte 0

InsideOfTruck_OnLoad:
	setmetatile 4, 1, METATILE_InsideOfTruck_ExitLight_Top, FALSE
	setmetatile 4, 2, METATILE_InsideOfTruck_ExitLight_Mid, FALSE
	setmetatile 4, 3, METATILE_InsideOfTruck_ExitLight_Bottom, FALSE
	end

InsideOfTruck_OnResume:
	end

InsideOfTruck_EventScript_StartBurrow::
	lockall
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 0, InsideOfTruck_EventScript_InitBurrow
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 1, InsideOfTruck_EventScript_Floor1
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 2, InsideOfTruck_EventScript_Floor2
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 3, InsideOfTruck_EventScript_Floor3
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 4, InsideOfTruck_EventScript_Floor4
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 5, InsideOfTruck_EventScript_Floor5
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 6, InsideOfTruck_EventScript_Floor6
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 7, InsideOfTruck_EventScript_Floor7
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 8, InsideOfTruck_EventScript_Floor8
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 9, InsideOfTruck_EventScript_Floor9
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 10, InsideOfTruck_EventScript_Winston
	msgbox InsideOfTruck_Text_BurrowVictory, MSGBOX_DEFAULT
	releaseall
	end

InsideOfTruck_EventScript_InitBurrow::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setflag FLAG_SYS_POKEMON_GET
	setvar VAR_WINSTONS_BURROW_FLOOR, 1
	setvar VAR_WINSTONS_BURROW_STATE, 0
	msgbox InsideOfTruck_Text_BurrowWelcome, MSGBOX_DEFAULT
	msgbox InsideOfTruck_Text_ChoosePartner, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BURROW_STARTER, FALSE
	switch VAR_RESULT
	case 0, InsideOfTruck_EventScript_GiveGrowlithe
	case 1, InsideOfTruck_EventScript_GiveElectrike
	case 2, InsideOfTruck_EventScript_GiveGreavard
	goto InsideOfTruck_EventScript_GiveGrowlithe

InsideOfTruck_EventScript_GiveGrowlithe::
	setvar VAR_STARTER_MON, 0
	bufferspeciesname STR_VAR_1, SPECIES_GROWLITHE
	givemon SPECIES_GROWLITHE, 5
	goto InsideOfTruck_EventScript_AfterPartnerChosen

InsideOfTruck_EventScript_GiveElectrike::
	setvar VAR_STARTER_MON, 1
	bufferspeciesname STR_VAR_1, SPECIES_ELECTRIKE
	givemon SPECIES_ELECTRIKE, 5
	goto InsideOfTruck_EventScript_AfterPartnerChosen

InsideOfTruck_EventScript_GiveGreavard::
	setvar VAR_STARTER_MON, 2
	bufferspeciesname STR_VAR_1, SPECIES_GREAVARD
	givemon SPECIES_GREAVARD, 5
	goto InsideOfTruck_EventScript_AfterPartnerChosen

InsideOfTruck_EventScript_AfterPartnerChosen::
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InsideOfTruck_EventScript_AfterStarterNickname
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon

InsideOfTruck_EventScript_AfterStarterNickname::
	special HealPlayerParty
	msgbox InsideOfTruck_Text_BurrowPartnerGranted, MSGBOX_DEFAULT
	goto InsideOfTruck_EventScript_Floor1

InsideOfTruck_EventScript_Floor1::
	trainerbattle_single TRAINER_RED, InsideOfTruck_Text_Floor1Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor1
	releaseall
	end

InsideOfTruck_EventScript_Floor2::
	trainerbattle_single TRAINER_LEAF, InsideOfTruck_Text_Floor2Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor2
	releaseall
	end

InsideOfTruck_EventScript_Floor3::
	trainerbattle_single TRAINER_BRENDAN_PLACEHOLDER, InsideOfTruck_Text_Floor3Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor3
	releaseall
	end

InsideOfTruck_EventScript_Floor4::
	trainerbattle_single TRAINER_MAY_PLACEHOLDER, InsideOfTruck_Text_Floor4Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor4
	releaseall
	end

InsideOfTruck_EventScript_Floor5::
	trainerbattle_single TRAINER_BURROW_FLOOR_5, InsideOfTruck_Text_Floor5Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor5
	releaseall
	end

InsideOfTruck_EventScript_Floor6::
	trainerbattle_single TRAINER_BURROW_FLOOR_6, InsideOfTruck_Text_Floor6Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor6
	releaseall
	end

InsideOfTruck_EventScript_Floor7::
	trainerbattle_single TRAINER_BURROW_FLOOR_7, InsideOfTruck_Text_Floor7Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor7
	releaseall
	end

InsideOfTruck_EventScript_Floor8::
	trainerbattle_single TRAINER_BURROW_FLOOR_8, InsideOfTruck_Text_Floor8Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor8
	releaseall
	end

InsideOfTruck_EventScript_Floor9::
	trainerbattle_single TRAINER_BURROW_FLOOR_9, InsideOfTruck_Text_Floor9Intro, InsideOfTruck_Text_FloorDefeat, InsideOfTruck_EventScript_AfterFloor9
	releaseall
	end

InsideOfTruck_EventScript_Winston::
	trainerbattle_single TRAINER_WINSTON_5, InsideOfTruck_Text_WinstonIntro, InsideOfTruck_Text_WinstonDefeat, InsideOfTruck_EventScript_AfterWinston
	releaseall
	end

InsideOfTruck_EventScript_AfterFloor1::
	setvar VAR_WINSTONS_BURROW_FLOOR, 2
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor2::
	setvar VAR_WINSTONS_BURROW_FLOOR, 3
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor3::
	setvar VAR_WINSTONS_BURROW_FLOOR, 4
	goto InsideOfTruck_EventScript_HealAndDescend

InsideOfTruck_EventScript_AfterFloor4::
	setvar VAR_WINSTONS_BURROW_FLOOR, 5
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor5::
	setvar VAR_WINSTONS_BURROW_FLOOR, 6
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor6::
	setvar VAR_WINSTONS_BURROW_FLOOR, 7
	goto InsideOfTruck_EventScript_HealAndDescend

InsideOfTruck_EventScript_AfterFloor7::
	setvar VAR_WINSTONS_BURROW_FLOOR, 8
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor8::
	setvar VAR_WINSTONS_BURROW_FLOOR, 9
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_AfterFloor9::
	setvar VAR_WINSTONS_BURROW_FLOOR, 10
	goto InsideOfTruck_EventScript_HealAndDescend

InsideOfTruck_EventScript_HealAndDescend::
	special HealPlayerParty
	msgbox InsideOfTruck_Text_BurrowHealed, MSGBOX_DEFAULT
	goto InsideOfTruck_EventScript_Descend

InsideOfTruck_EventScript_Descend::
	msgbox InsideOfTruck_Text_BurrowDescend, MSGBOX_DEFAULT
	warp MAP_INSIDE_OF_TRUCK, 2, 2
	waitstate
	releaseall
	end

InsideOfTruck_EventScript_AfterWinston::
	setvar VAR_WINSTONS_BURROW_FLOOR, 11
	setvar VAR_WINSTONS_BURROW_STATE, 1
	special HealPlayerParty
	msgbox InsideOfTruck_Text_BurrowVictory, MSGBOX_DEFAULT
	releaseall
	end

InsideOfTruck_EventScript_MovingBox::
	msgbox InsideOfTruck_Text_BurrowSign, MSGBOX_SIGN
	end

InsideOfTruck_Text_BurrowWelcome:
	.string "A narrow tunnel opens beneath\n"
	.string "Verdanturf Town.\p"
	.string "This is Winston's Burrow.\n"
	.string "Win each floor battle, descend,\n"
	.string "and survive through floor 10.$"

InsideOfTruck_Text_ChoosePartner:
	.string "Choose one level 5 partner for\n"
	.string "this test run.$"

InsideOfTruck_Text_BurrowPartnerGranted:
	.string "Your partner is ready.\p"
	.string "Floor 1 begins now.$"

InsideOfTruck_Text_Floor1Intro:
	.string "Floor 1.\n"
	.string "Loose stones rattle underfoot!$"

InsideOfTruck_Text_Floor2Intro:
	.string "Floor 2.\n"
	.string "Roots push through the tunnel wall!$"

InsideOfTruck_Text_Floor3Intro:
	.string "Floor 3.\n"
	.string "A miner's marker points downward!$"

InsideOfTruck_Text_Floor4Intro:
	.string "Floor 4.\n"
	.string "The air smells dry and sharp!$"

InsideOfTruck_Text_Floor5Intro:
	.string "Floor 5.\n"
	.string "Claws have carved the floor here!$"

InsideOfTruck_Text_Floor6Intro:
	.string "Floor 6.\n"
	.string "The tunnel bends like a maze!$"

InsideOfTruck_Text_Floor7Intro:
	.string "Floor 7.\n"
	.string "Steel echoes in the dark!$"

InsideOfTruck_Text_Floor8Intro:
	.string "Floor 8.\n"
	.string "Water seeps through the stone!$"

InsideOfTruck_Text_Floor9Intro:
	.string "Floor 9.\n"
	.string "The whole burrow trembles!$"

InsideOfTruck_Text_WinstonIntro:
	.string "Floor 10.\p"
	.string "WINSTON: You found the old path.\n"
	.string "Now prove you can leave it.$"

InsideOfTruck_Text_FloorDefeat:
	.string "The way down opens.$"

InsideOfTruck_Text_WinstonDefeat:
	.string "WINSTON: The burrow accepts you.$"

InsideOfTruck_Text_BurrowHealed:
	.string "The tunnel grows quiet.\p"
	.string "Your party was fully healed.$"

InsideOfTruck_Text_BurrowDescend:
	.string "You descend to the next floor.$"

InsideOfTruck_Text_BurrowVictory:
	.string "The lanterns flare to life.\p"
	.string "You cleared Winston's Burrow!$"

InsideOfTruck_Text_BurrowSign:
	.string "A rough wooden marker reads:\p"
	.string "WINSTON'S BURROW\n"
	.string "Floors cleared by courage only.$"
