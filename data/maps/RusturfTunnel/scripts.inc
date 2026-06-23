RusturfTunnel_MapScripts::
	.byte 0

RusturfTunnel_EventScript_RunEntrance::
	lockall
	setvar VAR_WINSTONS_BURROW_PENDING_CAVE_IN, 0
	delay 8
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 0, RusturfTunnel_EventScript_InitBurrow
	call RusturfTunnel_EventScript_CaveIn
	releaseall
	end

RusturfTunnel_EventScript_InitBurrow::
	call RusturfTunnel_EventScript_CaveInWithMessage
	call RusturfTunnel_EventScript_ResetTrainerFlags
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_WINSTONS_BURROW_FLOOR, 1
	setvar VAR_WINSTONS_BURROW_STATE, 0
	setvar VAR_WINSTONS_BURROW_TRAINERS, 0
	msgbox RusturfTunnel_Text_BurrowWelcome, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, RusturfTunnel_EventScript_BeginRun
	setflag FLAG_SYS_POKEMON_GET
	msgbox RusturfTunnel_Text_ChoosePartner, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BURROW_STARTER, FALSE
	switch VAR_RESULT
	case 0, RusturfTunnel_EventScript_GiveGrowlithe
	case 1, RusturfTunnel_EventScript_GiveElectrike
	case 2, RusturfTunnel_EventScript_GiveGreavard
	goto RusturfTunnel_EventScript_GiveGrowlithe

RusturfTunnel_EventScript_GiveGrowlithe::
	setvar VAR_STARTER_MON, 0
	bufferspeciesname STR_VAR_1, SPECIES_GROWLITHE
	givemon SPECIES_GROWLITHE, 5
	goto RusturfTunnel_EventScript_AfterPartnerChosen

RusturfTunnel_EventScript_GiveElectrike::
	setvar VAR_STARTER_MON, 1
	bufferspeciesname STR_VAR_1, SPECIES_ELECTRIKE
	givemon SPECIES_ELECTRIKE, 5
	goto RusturfTunnel_EventScript_AfterPartnerChosen

RusturfTunnel_EventScript_GiveGreavard::
	setvar VAR_STARTER_MON, 2
	bufferspeciesname STR_VAR_1, SPECIES_GREAVARD
	givemon SPECIES_GREAVARD, 5
	goto RusturfTunnel_EventScript_AfterPartnerChosen

RusturfTunnel_EventScript_AfterPartnerChosen::
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RusturfTunnel_EventScript_BeginRun
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto RusturfTunnel_EventScript_BeginRun

RusturfTunnel_EventScript_BeginRun::
	special HealPlayerParty
	msgbox RusturfTunnel_Text_BurrowPartnerReady, MSGBOX_DEFAULT
	releaseall
	end

RusturfTunnel_EventScript_CaveInWithMessage::
	call RusturfTunnel_EventScript_CaveIn
	msgbox RusturfTunnel_Text_CaveIn, MSGBOX_DEFAULT
	return

RusturfTunnel_EventScript_CaveIn::
	setvar VAR_0x8004, 3  @ vertical pan
	setvar VAR_0x8005, 3  @ horizontal pan
	setvar VAR_0x8006, 24 @ num shakes
	setvar VAR_0x8007, 2  @ shake delay
	playse SE_BANG
	delay 6
	playse SE_M_ROCK_THROW
	special ShakeCamera
	waitstate
	playse SE_BANG
	delay 8
	playse SE_WALL_HIT
	call RusturfTunnel_EventScript_SetCaveInTiles
	special DrawWholeMapView
	delay 24
	waitse
	return

RusturfTunnel_EventScript_SetCaveInTiles::
	setmetatile 1, 21, 0x21B, TRUE
	setmetatile 2, 21, 0x211, TRUE
	setmetatile 3, 21, 0x21C, TRUE
	setmetatile 4, 21, 0x205, TRUE
	setmetatile 5, 21, 0x206, TRUE
	setmetatile 1, 22, 0x205, TRUE
	setmetatile 2, 22, 0x211, TRUE
	setmetatile 3, 22, 0x206, TRUE
	setmetatile 4, 22, 0x21B, TRUE
	setmetatile 5, 22, 0x21C, TRUE
	return

RusturfTunnel_EventScript_ResetTrainerFlags::
	cleartrainerflag TRAINER_RED
	cleartrainerflag TRAINER_LEAF
	cleartrainerflag TRAINER_BRENDAN_PLACEHOLDER
	cleartrainerflag TRAINER_MAY_PLACEHOLDER
	cleartrainerflag TRAINER_BURROW_FLOOR_5
	cleartrainerflag TRAINER_BURROW_FLOOR_6
	cleartrainerflag TRAINER_BURROW_FLOOR_7
	cleartrainerflag TRAINER_BURROW_FLOOR_8
	cleartrainerflag TRAINER_BURROW_FLOOR_9
	cleartrainerflag TRAINER_WINSTON_5
	return

RusturfTunnel_EventScript_RequiredTrainer1::
	lock
	faceplayer
	goto_if_ge VAR_WINSTONS_BURROW_TRAINERS, 1, RusturfTunnel_EventScript_RemoveSeenTrainer
	msgbox RusturfTunnel_Text_RequiredTrainer1, MSGBOX_DEFAULT
	call RusturfTunnel_EventScript_BurrowTrainerBattle
	setvar VAR_WINSTONS_BURROW_TRAINERS, 1
	removeobject VAR_LAST_TALKED
	release
	end

RusturfTunnel_EventScript_RequiredTrainer2::
	lock
	faceplayer
	goto_if_ge VAR_WINSTONS_BURROW_TRAINERS, 2, RusturfTunnel_EventScript_RemoveSeenTrainer
	msgbox RusturfTunnel_Text_RequiredTrainer2, MSGBOX_DEFAULT
	call RusturfTunnel_EventScript_BurrowTrainerBattle
	setvar VAR_WINSTONS_BURROW_TRAINERS, 2
	removeobject VAR_LAST_TALKED
	release
	end

RusturfTunnel_EventScript_FalsePathTrainer::
	lock
	faceplayer
	msgbox RusturfTunnel_Text_FalsePathTrainer, MSGBOX_DEFAULT
	call RusturfTunnel_EventScript_BurrowTrainerBattle
	removeobject VAR_LAST_TALKED
	release
	end

RusturfTunnel_EventScript_RemoveSeenTrainer::
	removeobject VAR_LAST_TALKED
	release
	end

RusturfTunnel_EventScript_BurrowTrainerBattle::
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 1, RusturfTunnel_EventScript_BattleFloor1Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 2, RusturfTunnel_EventScript_BattleFloor2Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 3, RusturfTunnel_EventScript_BattleFloor3Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 4, RusturfTunnel_EventScript_BattleFloor4Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 5, RusturfTunnel_EventScript_BattleFloor5Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 6, RusturfTunnel_EventScript_BattleFloor6Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 7, RusturfTunnel_EventScript_BattleFloor7Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 8, RusturfTunnel_EventScript_BattleFloor8Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 9, RusturfTunnel_EventScript_BattleFloor9Trainer
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 10, RusturfTunnel_EventScript_BattleWinstonTrainer
	return

RusturfTunnel_EventScript_BattleFloor1Trainer::
	cleartrainerflag TRAINER_RED
	trainerbattle_no_intro TRAINER_RED, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor2Trainer::
	cleartrainerflag TRAINER_LEAF
	trainerbattle_no_intro TRAINER_LEAF, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor3Trainer::
	cleartrainerflag TRAINER_BRENDAN_PLACEHOLDER
	trainerbattle_no_intro TRAINER_BRENDAN_PLACEHOLDER, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor4Trainer::
	cleartrainerflag TRAINER_MAY_PLACEHOLDER
	trainerbattle_no_intro TRAINER_MAY_PLACEHOLDER, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor5Trainer::
	cleartrainerflag TRAINER_BURROW_FLOOR_5
	trainerbattle_no_intro TRAINER_BURROW_FLOOR_5, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor6Trainer::
	cleartrainerflag TRAINER_BURROW_FLOOR_6
	trainerbattle_no_intro TRAINER_BURROW_FLOOR_6, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor7Trainer::
	cleartrainerflag TRAINER_BURROW_FLOOR_7
	trainerbattle_no_intro TRAINER_BURROW_FLOOR_7, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor8Trainer::
	cleartrainerflag TRAINER_BURROW_FLOOR_8
	trainerbattle_no_intro TRAINER_BURROW_FLOOR_8, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleFloor9Trainer::
	cleartrainerflag TRAINER_BURROW_FLOOR_9
	trainerbattle_no_intro TRAINER_BURROW_FLOOR_9, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_BattleWinstonTrainer::
	cleartrainerflag TRAINER_WINSTON_5
	trainerbattle_no_intro TRAINER_WINSTON_5, RusturfTunnel_Text_TrainerDefeat
	return

RusturfTunnel_EventScript_FloorExit::
	lockall
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 1, RusturfTunnel_EventScript_AfterFloor1
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 2, RusturfTunnel_EventScript_AfterFloor2
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 3, RusturfTunnel_EventScript_AfterFloor3
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 4, RusturfTunnel_EventScript_AfterFloor4
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 5, RusturfTunnel_EventScript_AfterFloor5
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 6, RusturfTunnel_EventScript_AfterFloor6
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 7, RusturfTunnel_EventScript_AfterFloor7
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 8, RusturfTunnel_EventScript_AfterFloor8
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 9, RusturfTunnel_EventScript_AfterFloor9
	goto_if_eq VAR_WINSTONS_BURROW_FLOOR, 10, RusturfTunnel_EventScript_AfterWinston
	releaseall
	end

RusturfTunnel_EventScript_AfterFloor1::
	setvar VAR_WINSTONS_BURROW_FLOOR, 2
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor2::
	setvar VAR_WINSTONS_BURROW_FLOOR, 3
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor3::
	setvar VAR_WINSTONS_BURROW_FLOOR, 4
	goto RusturfTunnel_EventScript_HealAndDescend

RusturfTunnel_EventScript_AfterFloor4::
	setvar VAR_WINSTONS_BURROW_FLOOR, 5
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor5::
	setvar VAR_WINSTONS_BURROW_FLOOR, 6
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor6::
	setvar VAR_WINSTONS_BURROW_FLOOR, 7
	goto RusturfTunnel_EventScript_HealAndDescend

RusturfTunnel_EventScript_AfterFloor7::
	setvar VAR_WINSTONS_BURROW_FLOOR, 8
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor8::
	setvar VAR_WINSTONS_BURROW_FLOOR, 9
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_AfterFloor9::
	setvar VAR_WINSTONS_BURROW_FLOOR, 10
	goto RusturfTunnel_EventScript_HealAndDescend

RusturfTunnel_EventScript_HealAndDescend::
	special HealPlayerParty
	msgbox RusturfTunnel_Text_BurrowHealed, MSGBOX_DEFAULT
	goto RusturfTunnel_EventScript_Descend

RusturfTunnel_EventScript_Descend::
	setvar VAR_WINSTONS_BURROW_TRAINERS, 0
	msgbox RusturfTunnel_Text_BurrowDescend, MSGBOX_DEFAULT
	warp MAP_RUSTURF_TUNNEL, 2, 21
	waitstate
	releaseall
	end

RusturfTunnel_EventScript_AfterWinston::
	setvar VAR_WINSTONS_BURROW_FLOOR, 0
	setvar VAR_WINSTONS_BURROW_STATE, 0
	setvar VAR_WINSTONS_BURROW_TRAINERS, 0
	setvar VAR_WINSTONS_BURROW_PENDING_CAVE_IN, 0
	special HealPlayerParty
	msgbox RusturfTunnel_Text_BurrowVictory, MSGBOX_DEFAULT
	warp MAP_VERDANTURF_TOWN, 8, 5
	waitstate
	releaseall
	end

RusturfTunnel_Text_CaveIn:
	.string "The tunnel erupts behind you!\p"
	.string "Fallen stone seals the entrance.\p"
	.string "You've been blocked in!$"

RusturfTunnel_Text_BurrowWelcome:
	.string "A narrow tunnel opens beneath\n"
	.string "Verdanturf Town.\p"
	.string "This is Winston's Burrow.\n"
	.string "Find the ladder, descend,\n"
	.string "and survive through floor 10.$"

RusturfTunnel_Text_ChoosePartner:
	.string "Choose one level 5 partner for\n"
	.string "this run.$"

RusturfTunnel_Text_BurrowPartnerReady:
	.string "Your party is ready.\p"
	.string "Floor 1 begins now.$"

RusturfTunnel_Text_RequiredTrainer1:
	.string "No one reaches the ladder\n"
	.string "without a battle.$"

RusturfTunnel_Text_RequiredTrainer2:
	.string "The ladder is close.\n"
	.string "Prove you can reach it.$"

RusturfTunnel_Text_FalsePathTrainer:
	.string "Wrong turn.\n"
	.string "Right battle.$"

RusturfTunnel_Text_TrainerDefeat:
	.string "The trainer steps aside.$"

RusturfTunnel_Text_BurrowHealed:
	.string "The tunnel grows quiet.\p"
	.string "Your party was fully healed.$"

RusturfTunnel_Text_BurrowDescend:
	.string "You climb down the ladder.$"

RusturfTunnel_Text_BurrowVictory:
	.string "The lanterns flare to life.\p"
	.string "You cleared Winston's Burrow!$"
