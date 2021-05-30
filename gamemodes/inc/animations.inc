/*

	file:			animations.inc

	description:	All SARP animation commands in a seperate file
					.. using ZCMD instead of strcmp.

	author:			Jay Cortez

	started:		14 november 2017

	ended:			/

*/

CMD:animations(playerid, params[])
	return cmd_animhelp(playerid, params);

CMD:animlist(playerid, params[])
	return cmd_animhelp(playerid, params);

CMD:animhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_WHITE, "Available Animations:");
	SendClientMessage(playerid, COLOR_GRAD1, "/shakehand, /handsup, /drunk, /cry, /bomb, /wank, /lookout, /hitch, /crabs, /rap /showoff, /dance");
	SendClientMessage(playerid, COLOR_GRAD1, "/dj, /what, /lean, /vomit, /getjiggy, /robman, /greet, /fuckme, /cheer, /cpr, /kiss, /gift, /stand");
	SendClientMessage(playerid, COLOR_GRAD2, "/fallover, /taichi, /pedmove, /slapass, /laugh, /bj,  /piss, /stop, /crack, /nobreath, /signal, /followme");
	SendClientMessage(playerid, COLOR_GRAD3, "/gesture 	/hide, /rob, /wave, /hurt, /smoke, /washhands, /salute, /crossarms, /sitdown, /relax, /sit, /dive");
	SendClientMessage(playerid, COLOR_GRAD3, "/deal, /sleep, /bitchslap, /reload, /checktime, /siteat, /eatfood, /chat, /drinkwater, /bat, /opendoor");
	SendClientMessage(playerid, COLOR_GRAD4, "/wavedown, /box, /tag, /goggles, /throw, /robbed, /bar, /idle, /carchat, /slapass, /sunbathe, /snatch");
	SendClientMessage(playerid, COLOR_GRAD5, "/sneak, /copa, /strip, /holdcue, /misc, /trunkput, /lowbarge, /airkick /elbow, /runcuffed");
	SendClientMessage(playerid, COLOR_GRAD6, "/sitonchair");
	SendClientMessage(playerid, COLOR_WHITE, "Type /stopani to force-stop an animation.");
	return 1;
}

CMD:trunkput(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "GANGS", "shake_cara", 4.1, 0, 0, 0, 0, 0);
	return 1;
}

CMD:lowbarge(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "GANGS", "shake_carSH", 4.1, 0, 0, 0, 0, 0);
	return 1;
}

CMD:piss(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	SetPlayerSpecialAction(playerid, 68);
	return 1;
}

CMD:throw(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "GRENADE", "WEAPON_throw", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:robbed(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "SHOP", "SHP_Rob_GiveCash", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:sneak(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "PED", "Player_Sneak", 4.1, 1, 1, 1, 1, 1);
	return 1;
}

CMD:runcuffed(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "SWORD", "sword_block", 4.1, 0, 1, 1, 1, 1);
	return 1;
}

CMD:airkick(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "FIGHT_C", "FightC_M", 4.1, 0, 1, 1, 0, 0);
	return 1;
}

CMD:elbow(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "FIGHT_D", "FightD_3", 4.1, 0, 1, 1, 0, 0);
	return 1;
}

CMD:taichi(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "PARK", "Tai_Chi_Loop", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:fucku(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "PED", "fucku", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:crack(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:vomit(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:hide(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:robman(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:lookout(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:laugh(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:rob(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "ped", "ARRESTgun", 4.0, 0, 1, 1, 1, 0);
	return 1;
}

CMD:bomb(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:drunk(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "PED", "WALK_DRUNK", 4.1, 1, 1, 1, 1, 1);
	return 1;
}

CMD:slapass(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:gift(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "KISSING", "gift_give", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:cpr(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "MEDIC", "CPR", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:bitchslap(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "MISC", "bitchslap", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:hitch(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "MISC", "Hiker_Pose", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:stand(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "WUZI", "Wuzi_stand_loop", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:greet(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "WUZI", "Wuzi_Greet_Wuzi", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:followme(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "WUZI", "Wuzi_follow", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:what(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "RIOT", "RIOT_ANGRY", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:cry(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "GRAVEYARD", "mrnF_loop", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:washhands(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "BD_FIRE", "wash_up", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:stop(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "PED", "endchat_01", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:salute(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "ON_LOOKERS", "Pointup_loop", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:crabs(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "MISC", "Scratchballs_01", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:showoff(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "Freeweights", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:dive(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "DODGE", "Crush_Jump", 4.0, 0, 1, 1, 1, 0);
	return 1;
}

CMD:relax(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "CRACK", "crckidle1", 4.0, 0, 1, 1, 1, -1);
	return 1;
}

CMD:checktime(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "COP_AMBIENT", "Coplook_watch", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:eatfood(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "FOOD", "EAT_Burger", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:drinkwater(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "BAR", "dnk_stndF_loop", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:opendoor(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "AIRPORT", "thrw_barl_thrw", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:box(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	LoopingAnim(playerid, "GYMNASIUM", "GYMshadowbox", 4.0, 1, 0, 0, 0, 0);
	return 1;
}

CMD:wavedown(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "BD_FIRE", "BD_Panic_01", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:goggles(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	OnePlayAnim(playerid, "goggles", "goggles_put_on", 4.0, 0, 0, 0, 0, 0);
	return 1;
}

CMD:snatch(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /snatch [1-2]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "PED", "BIKE_elbowL", 4.1, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "PED", "BIKE_elbowR", 4.1, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:copa(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 9)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /copa [1-9]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "POLICE", "CopTraf_Away", 4.1, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "POLICE", "CopTraf_Come", 4.1, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "POLICE", "CopTraf_Left", 4.1, 0, 0, 0, 0, 0);
		case 4: OnePlayAnim(playerid, "POLICE", "CopTraf_Stop", 4.1, 0, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "POLICE", "Cop_move_FWD", 4.1, 1, 1, 1, 1, 1);
		case 6: LoopingAnim(playerid, "POLICE", "crm_drgbst_01", 4.1, 0, 0, 0, 1, 5000);
		case 7: OnePlayAnim(playerid, "POLICE", "Door_Kick", 4.1, 0, 1, 1, 1, 1);
		case 8: OnePlayAnim(playerid, "POLICE", "plc_drgbst_01", 4.1, 0, 0, 0, 0, 5000);
		case 9: OnePlayAnim(playerid, "POLICE", "plc_drgbst_02", 4.1, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:strip(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 6)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /strip [1-6]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "STRIP", "strip_E", 4.1, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "STRIP", "strip_G", 4.1, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "STRIP", "STR_A2B", 4.1, 1, 0, 0, 0, 0);
		case 4: LoopingAnim(playerid, "STRIP", "STR_Loop_A", 4.1, 1, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "STRIP", "STR_Loop_B", 4.1, 1, 0, 0, 0, 0);
		case 6: LoopingAnim(playerid, "STRIP", "STR_Loop_C", 4.1, 1, 0, 0, 0, 0);

	}
	return 1;
}

CMD:holdcue(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /holdcue [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "POOL", "POOL_ChalkCue", 4.1, 0, 1, 1, 1, 1);
		case 2: LoopingAnim(playerid, "POOL", "POOL_Idle_Stance", 4.1, 0, 1, 1, 1, 1);

	}
	return 1;
}

CMD:misc(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /misc [1-3]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "CAR", "Fixn_Car_Loop", 4.1, 1, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "CAR", "flag_drop", 4.1, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "PED", "bomber", 4.1, 0, 0, 0, 0, 0);

	}
	return 1;
}

CMD:idle(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 15)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /idle [1-15]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "PLAYIDLES", "shift", 4.1, 1, 1, 1, 1, 0);
		case 2: OnePlayAnim(playerid, "PLAYIDLES", "shldr", 4.1, 1, 1, 1, 1, 1);
		case 3: OnePlayAnim(playerid, "PLAYIDLES", "stretch", 4.1, 1, 1, 1, 1, 1);
		case 4: OnePlayAnim(playerid, "PLAYIDLES", "strleg", 4.1, 1, 1, 1, 1, 1);
		case 5: OnePlayAnim(playerid, "PLAYIDLES", "time", 4.1, 1, 1, 1, 1, 1);
		case 6: LoopingAnim(playerid, "COP_AMBIENT", "Copbrowse_loop", 4.1, 1, 0, 0, 0, 0);
		case 7: LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 0, 0);
		case 8: OnePlayAnim(playerid, "COP_AMBIENT", "Coplook_shake", 4.1, 1, 0, 0, 0, 0);
		case 9: OnePlayAnim(playerid, "COP_AMBIENT", "Coplook_think", 4.1, 1, 0, 0, 0, 0);
		case 10: OnePlayAnim(playerid, "COP_AMBIENT", "Coplook_watch", 4.1, 1, 0, 0, 0, 0);
		case 11: OnePlayAnim(playerid, "PED", "roadcross", 4.1, 1, 0, 0, 0, 0);
		case 12: OnePlayAnim(playerid, "PED", "roadcross_female", 4.1, 1, 0, 0, 0, 0);
		case 13: OnePlayAnim(playerid, "PED", "roadcross_gang", 4.1, 1, 0, 0, 0, 0);
		case 14: OnePlayAnim(playerid, "PED", "roadcross_old", 4.1, 1, 0, 0, 0, 0);
		case 15: OnePlayAnim(playerid, "PED", "woman_idlestance", 4.1, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:bar(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 5)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /bar [1-5]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "BAR", "Barcustom_get", 4.0, 0, 1, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "BAR", "Barserve_bottle", 4.0, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "BAR", "Barserve_give", 4.0, 0, 0, 0, 0, 0);
		case 4: OnePlayAnim(playerid, "BAR", "dnk_stndM_loop", 4.0, 0, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "BAR", "BARman_idle", 4.0, 1, 0, 0, 0, 0);

	}
	return 1;
}

CMD:tag(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /tag [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "GRAFFITI", "graffiti_Chkout", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "GRAFFITI", "spraycan_fire", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:bat(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /bat [1-3]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "BASEBALL", "Bat_IDLE", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "CRACK", "Bbalbat_Idle_01", 4.0, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "CRACK", "Bbalbat_Idle_02", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:hurt(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /hurt [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "SWAT", "gnstwall_injurd", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:smoke(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /smoke [1-2]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "SMOKING", "M_smk_in", 4.0, 0, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:gesture(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 15)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /gesture [1-15]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "GHANDS", "gsign1", 4.0, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "GHANDS", "gsign1LH", 4.0, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "GHANDS", "gsign2", 4.0, 0, 0, 0, 0, 0);
		case 4: OnePlayAnim(playerid, "GHANDS", "gsign2LH", 4.0, 0, 0, 0, 0, 0);
		case 5: OnePlayAnim(playerid, "GHANDS", "gsign3", 4.0, 0, 0, 0, 0, 0);
		case 6: OnePlayAnim(playerid, "GHANDS", "gsign3LH", 4.0, 0, 0, 0, 0, 0);
		case 7: OnePlayAnim(playerid, "GHANDS", "gsign4", 4.0, 0, 0, 0, 0, 0);
		case 8: OnePlayAnim(playerid, "GHANDS", "gsign4LH", 4.0, 0, 0, 0, 0, 0);
		case 9: OnePlayAnim(playerid, "GHANDS", "gsign5", 4.0, 0, 0, 0, 0, 0);
		case 10: OnePlayAnim(playerid, "GHANDS", "gsign5", 4.0, 0, 0, 0, 0, 0);
		case 11: OnePlayAnim(playerid, "GHANDS", "gsign5LH", 4.0, 0, 0, 0, 0, 0);
		case 12: OnePlayAnim(playerid, "GANGS", "Invite_No", 4.0, 0, 0, 0, 0, 0);
		case 13: OnePlayAnim(playerid, "GANGS", "Invite_Yes", 4.0, 0, 0, 0, 0, 0);
		case 14: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkD", 4.0, 0, 0, 0, 0, 0);
		case 15: OnePlayAnim(playerid, "GANGS", "smkcig_prtl", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:chat(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 7)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /chat [1-7]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "PED", "IDLE_CHAT", 4.0, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkA", 4.0, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkB", 4.0, 0, 0, 0, 0, 0);
		case 4: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkE", 4.0, 0, 0, 0, 0, 0);
		case 5: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkF", 4.0, 0, 0, 0, 0, 0);
		case 6: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkG", 4.0, 0, 0, 0, 0, 0);
		case 7: OnePlayAnim(playerid, "GANGS", "prtial_gngtlkH", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:deal(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /deal [1-2]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "DEALER", "shop_pay", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:cheer(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 8)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /cheer [1-8]");

	switch(anumber)
	{
		case 1: OnePlayAnim(playerid, "ON_LOOKERS", "shout_01", 4.0, 0, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "ON_LOOKERS", "shout_02", 4.0, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "ON_LOOKERS", "shout_in", 4.0, 0, 0, 0, 0, 0);
		case 4: LoopingAnim(playerid, "RIOT", "RIOT_ANGRY_B", 4.0, 1, 0, 0, 0, 0);
		case 5: OnePlayAnim(playerid, "RIOT", "RIOT_CHANT", 4.0, 0, 0, 0, 0, 0);
		case 6: OnePlayAnim(playerid, "RIOT", "RIOT_shout", 4.0, 0, 0, 0, 0, 0);
		case 7: OnePlayAnim(playerid, "STRIP", "PUN_HOLLER", 4.0, 0, 0, 0, 0, 0);
		case 8: OnePlayAnim(playerid, "OTB", "wtchrace_win", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:wave(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /wave [1-3]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, 1, 0, 0, 0, 0);
		case 2: OnePlayAnim(playerid, "KISSING", "gfwave2", 4.0, 0, 0, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "PED", "endchat_03", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:fallover(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 4)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /fallover [1-4]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "KNIFE", "KILL_Knife_Ped_Die", 4.0, 0, 1, 1, 1, 0);
		case 2: LoopingAnim(playerid, "PED", "KO_shot_face", 4.0, 0, 1, 1, 1, 0);
		case 3: LoopingAnim(playerid, "PED", "KO_shot_stom", 4.0, 0, 1, 1, 1, 0);
		case 4: LoopingAnim(playerid, "PED", "BIKE_fallR", 4.0, 0, 1, 1, 0, 0);
		//case 5: LoopingAnim(playerid, "PED", "FLOOR_hit_f", 4.0, 0, 1, 1, 1, 0, 1);
		
	}
	return 1;
}

CMD:sleep(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /sleep [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "CRACK", "crckdeth4", 4.0, 0, 0, 0, 1, 0);
		case 2: LoopingAnim(playerid, "CRACK", "crckidle2", 4.0, 0, 0, 0, 1, 0);
	}
	return 1;
}

CMD:lean(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /lean [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "GANGS", "leanIDLE", 4.1, 0, 0, 0, 1, 0);
		case 2: LoopingAnim(playerid, "MISC", "Plyrlean_loop", 4.1, 0, 0, 0, 1, 0);
	}
	return 1;
}

CMD:signal(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /signal [1-2]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "POLICE", "CopTraf_Come", 4.0, 0, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "POLICE", "CopTraf_Stop", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:dj(playerid, params[]) {
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 4)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /dj [1-4]");

	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "SCRATCHING", "scdldlp", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "SCRATCHING", "scdlulp", 4.0, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "SCRATCHING", "scdrdlp", 4.0, 1, 0, 0, 0, 0);
		case 4:LoopingAnim(playerid, "SCRATCHING", "scdrulp", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:carchat(playerid, params[])
{
	if(!AnimationCheck(playerid, true))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	
	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 19)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /carchat [1-19]");

	switch(anumber)
	{
		case 1: ApplyAnimation(playerid, "CAR_CHAT", "carfone_in", 4.1, 0, 1, 1, 1, 1);
		case 2: ApplyAnimation(playerid, "CAR_CHAT", "carfone_loopA", 4.1, 0, 1, 1, 1, 1);
		case 3: ApplyAnimation(playerid, "CAR_CHAT", "carfone_loopA_to_B", 4.1, 0, 1, 1, 1, 1);
		case 4: ApplyAnimation(playerid, "CAR_CHAT", "carfone_loopB", 4.1, 0, 1, 1, 1, 1);
		case 5: ApplyAnimation(playerid, "CAR_CHAT", "carfone_loopB_to_A", 4.1, 0, 1, 1, 1, 1);
		case 6: ApplyAnimation(playerid, "CAR_CHAT", "carfone_out", 4.1, 0, 1, 1, 1, 1);
		case 7: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc1_BL", 4.1, 0, 1, 1, 1, 1);
		case 8: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc1_BR", 4.1, 0, 1, 1, 1, 1);
		case 9: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc1_FL", 4.1, 0, 1, 1, 1, 1);
		case 10: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc1_FR", 4.1, 0, 1, 1, 1, 1);
		case 11: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc2_FL", 4.1, 0, 1, 1, 1, 1);
		case 12: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc3_BR", 4.1, 0, 1, 1, 1, 1);
		case 13: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc3_FL", 4.1, 0, 1, 1, 1, 1);
		case 14: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc3_FR", 4.1, 0, 1, 1, 1, 1);
		case 15: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc4_BL", 4.1, 0, 1, 1, 1, 1);
		case 16: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc4_BR", 4.1, 0, 1, 1, 1, 1);
		case 17: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc4_FL", 4.1, 0, 1, 1, 1, 1);
		case 18: ApplyAnimation(playerid, "CAR_CHAT", "CAR_Sc4_FR", 4.1, 0, 1, 1, 1, 1);
		case 19: ApplyAnimation(playerid, "CAR", "Sit_relaxed", 4.1, 0, 1, 1, 1, 1);
	}
	return 1;
}

CMD:sunbathe(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 18)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /sunbathe [1-18]");
	
	switch(anumber)
	{
		case 1: LoopingAnim(playerid, "SUNBATHE", "batherdown", 4.1, 0, 1, 1, 1, 1);
		case 2: LoopingAnim(playerid, "SUNBATHE", "batherup", 4.1, 0, 1, 1, 1, 1);
		case 3: LoopingAnim(playerid, "SUNBATHE", "Lay_Bac_in", 4.1, 0, 1, 1, 1, 1);
		case 4: LoopingAnim(playerid, "SUNBATHE", "Lay_Bac_out", 4.1, 0, 1, 1, 1, 1);
		case 5: LoopingAnim(playerid, "SUNBATHE", "ParkSit_M_IdleA", 4.1, 0, 1, 1, 1, 1);
		case 6: LoopingAnim(playerid, "SUNBATHE", "ParkSit_M_IdleB", 4.1, 0, 1, 1, 1, 1);
		case 7: LoopingAnim(playerid, "SUNBATHE", "ParkSit_M_IdleC", 4.1, 0, 1, 1, 1, 1);
		case 8: LoopingAnim(playerid, "SUNBATHE", "ParkSit_M_in", 4.1, 0, 1, 1, 1, 1);
		case 9: LoopingAnim(playerid, "SUNBATHE", "ParkSit_M_out", 4.1, 0, 1, 1, 1, 1);
		case 10: LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_idleA", 4.1, 0, 1, 1, 1, 1);
		case 11: LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_idleB", 4.1, 0, 1, 1, 1, 1);
		case 12: LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_idleC", 4.1, 0, 1, 1, 1, 1);
		case 13: LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_in", 4.1, 0, 1, 1, 1, 1);
		case 14: LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_out", 4.1, 0, 1, 1, 1, 1);
		case 15: LoopingAnim(playerid, "SUNBATHE", "SBATHE_F_LieB2Sit", 4.1, 0, 1, 1, 1, 1);
		case 16: LoopingAnim(playerid, "SUNBATHE", "SBATHE_F_Out", 4.1, 0, 1, 1, 1, 1);
		case 17: LoopingAnim(playerid, "SUNBATHE", "SitnWait_in_W", 4.1, 0, 1, 1, 1, 1);
		case 18: LoopingAnim(playerid, "SUNBATHE", "SitnWait_out_W", 4.1, 0, 1, 1, 1, 1);
	}
	return 1;
}

CMD:handsup(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
	return 1;
}

CMD:dance(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 4)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /dance [1-4]");

	switch(anumber) {
		case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
		case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
		case 3: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
		case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
	}
	return 1;
}

CMD:crossarms(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 5)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /crossarms [1-5]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1);
		case 2: LoopingAnim(playerid, "DEALER", "DEALER_IDLE", 4.0, 0, 1, 1, 1, -1);
		case 3: LoopingAnim(playerid, "DEALER", "DEALER_IDLE_01", 4.0, 0, 1, 1, 1, -1);
		case 4: LoopingAnim(playerid, "GRAVEYARD", "mrnM_loop", 4.0, 1, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "GRAVEYARD", "prst_loopa", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:getjiggy(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 10)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /getjiggy [1-10]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "DANCING", "DAN_Down_A", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "DANCING", "DAN_Left_A", 4.0, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "DANCING", "DAN_Loop_A", 4.0, 1, 0, 0, 0, 0);
		case 4: LoopingAnim(playerid, "DANCING", "DAN_Right_A", 4.0, 1, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "DANCING", "DAN_Up_A", 4.0, 1, 0, 0, 0, 0);
		case 6: LoopingAnim(playerid, "DANCING", "dnce_M_a", 4.0, 1, 0, 0, 0, 0);
		case 7: LoopingAnim(playerid, "DANCING", "dnce_M_b", 4.0, 1, 0, 0, 0, 0);
		case 8: LoopingAnim(playerid, "DANCING", "dnce_M_c", 4.0, 1, 0, 0, 0, 0);
		case 9: LoopingAnim(playerid, "DANCING", "dnce_M_c", 4.0, 1, 0, 0, 0, 0);
		case 10: LoopingAnim(playerid, "DANCING", "dnce_M_d", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:siteat(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /siteat [1-2]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "FOOD", "FF_Sit_Eat3", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "FOOD", "FF_Sit_Eat2", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:nobreath(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 2)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /nobreath [1-2]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "PED", "IDLE_tired", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "FAT", "IDLE_tired", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:sit(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 5)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /sit [1-5]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
		case 2: LoopingAnim(playerid, "BEACH", "Lay_Bac_Loop", 4.0, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "BEACH", "ParkSit_W_loop", 4.0, 1, 0, 0, 0, 0);
		case 4: LoopingAnim(playerid, "BEACH", "SitnWait_loop_W", 4.0, 1, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
	}
	return 1;
}

CMD:sitonchair(playerid, params[])
	return cmd_sitdown(playerid, params);

CMD:sitdown(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 7)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /sitdown [1-7]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "Attractors", "Stepsit_in", 4.0, 0, 0, 0, 1, 0);
		case 2: LoopingAnim(playerid, "CRIB", "PED_Console_Loop", 4.0, 1, 0, 0, 0, 0);
		case 3: LoopingAnim(playerid, "INT_HOUSE", "LOU_In", 4.0, 0, 0, 0, 1, 0);
		case 4: LoopingAnim(playerid, "MISC", "SEAT_LR", 4.0, 1, 0, 0, 0, 0);
		case 5: LoopingAnim(playerid, "MISC", "Seat_talk_01", 4.0, 1, 0, 0, 0, 0);
		case 6: LoopingAnim(playerid, "MISC", "Seat_talk_02", 4.0, 1, 0, 0, 0, 0);
		case 7: LoopingAnim(playerid, "ped", "SEAT_down", 4.0, 0, 0, 0, 1, 0);
	}
	return 1;
}

CMD:pedmove(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 26)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /pedmove [1-26]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "PED", "JOG_femaleA", 4.0, 1, 1, 1, 1, 1);
		case 2: LoopingAnim(playerid, "PED", "JOG_maleA", 4.0, 1, 1, 1, 1, 1);
		case 3: LoopingAnim(playerid, "PED", "WOMAN_walkfatold", 4.0, 1, 1, 1, 1, 1);
		case 4: LoopingAnim(playerid, "PED", "run_fat", 4.0, 1, 1, 1, 1, 1);
		case 5: LoopingAnim(playerid, "PED", "run_fatold", 4.0, 1, 1, 1, 1, 1);
		case 6: LoopingAnim(playerid, "PED", "run_old", 4.0, 1, 1, 1, 1, 1);
		case 7: LoopingAnim(playerid, "PED", "Run_Wuzi", 4.0, 1, 1, 1, 1, 1);
		case 8: LoopingAnim(playerid, "PED", "swat_run", 4.0, 1, 1, 1, 1, 1);
		case 9: LoopingAnim(playerid, "PED", "WALK_fat", 4.0, 1, 1, 1, 1, 1);
		case 10: LoopingAnim(playerid, "PED", "WALK_fatold", 4.0, 1, 1, 1, 1, 1);
		case 11: LoopingAnim(playerid, "PED", "WALK_gang1", 4.0, 1, 1, 1, 1, 1);
		case 12: LoopingAnim(playerid, "PED", "WALK_gang2", 4.0, 1, 1, 1, 1, 1);
		case 13: LoopingAnim(playerid, "PED", "WALK_old", 4.0, 1, 1, 1, 1, 1);
		case 14: LoopingAnim(playerid, "PED", "WALK_shuffle", 4.0, 1, 1, 1, 1, 1);
		case 15: LoopingAnim(playerid, "PED", "woman_run", 4.0, 1, 1, 1, 1, 1);
		case 16: LoopingAnim(playerid, "PED", "WOMAN_runbusy", 4.0, 1, 1, 1, 1, 1);
		case 17: LoopingAnim(playerid, "PED", "WOMAN_runfatold", 4.0, 1, 1, 1, 1, 1);
		case 18: LoopingAnim(playerid, "PED", "woman_runpanic", 4.0, 1, 1, 1, 1, 1);
		case 19: LoopingAnim(playerid, "PED", "WOMAN_runsexy", 4.0, 1, 1, 1, 1, 1);
		case 20: LoopingAnim(playerid, "PED", "WOMAN_walkbusy", 4.0, 1, 1, 1, 1, 1);
		case 21: LoopingAnim(playerid, "PED", "WOMAN_walkfatold", 4.0, 1, 1, 1, 1, 1);
		case 22: LoopingAnim(playerid, "PED", "WOMAN_walknorm", 4.0, 1, 1, 1, 1, 1);
		case 23: LoopingAnim(playerid, "PED", "WOMAN_walkold", 4.0, 1, 1, 1, 1, 1);
		case 24: LoopingAnim(playerid, "PED", "WOMAN_walkpro", 4.0, 1, 1, 1, 1, 1);
		case 25: LoopingAnim(playerid, "PED", "WOMAN_walksexy", 4.0, 1, 1, 1, 1, 1);
		case 26: LoopingAnim(playerid, "PED", "WOMAN_walkshop", 4.0, 1, 1, 1, 1, 1);
	}
	return 1;
}

CMD:rap(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /rap [1-3]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "RAPPING", "RAP_A_Loop", 4.0, 1, 1, 1, 1, 0);
		case 2: LoopingAnim(playerid, "RAPPING", "RAP_B_Loop", 4.0, 1, 1, 1, 1, 0);
		case 3: LoopingAnim(playerid, "RAPPING", "RAP_C_Loop", 4.0, 1, 1, 1, 1, 0);
	}
	return 1;
}

CMD:reload(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 5)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /reload [1-5]");

	switch(anumber) {
		case 1: ApplyAnimationEx(playerid, "COLT45", "colt45_reload", 4.0, 0, 0, 0, 0, 0);
		case 2: ApplyAnimationEx(playerid, "COLT45", "sawnoff_reload", 4.0, 0, 0, 0, 0, 0);
		case 3: ApplyAnimationEx(playerid, "BUDDY", "buddy_reload", 4.0, 0, 0, 0, 0, 0);
		case 4: ApplyAnimationEx(playerid, "RIFLE", "RIFLE_load", 4.0, 0, 0, 0, 0, 0);
		case 5: ApplyAnimationEx(playerid, "PYTHON", "python_reload", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

/*CMD:skate(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /skate [1-3]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "SKATE", "skate_idle", 4.0, 1, 1, 1, 1, 1);
		case 2: LoopingAnim(playerid, "SKATE", "skate_run", 4.0, 1, 1, 1, 1, 1);
		case 3: LoopingAnim(playerid, "SKATE", "skate_sprint", 4.0, 1, 1, 1, 1, 1);
	}
	return 1;
}*/

CMD:wank(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 3)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /wank [1-3]");

	switch(anumber) {
		case 1: OnePlayAnim(playerid, "PAULNMAC", "wank_in", 4.0, 0, 1, 1, 0, 0);
		case 2: LoopingAnim(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 1, 0, 0, 0);
		case 3: OnePlayAnim(playerid, "PAULNMAC", "wank_out", 4.0, 0, 0, 0, 0, 0);
	}
	return 1;
}

CMD:fuckme(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 8)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /fuckme [1-8]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "SNM", "SPANKING_IDLEW", 4.1, 0, 1, 1, 1, 0);
		case 2: LoopingAnim(playerid, "SNM", "SPANKING_IDLEP", 4.1, 0, 1, 1, 1, 0);
		case 3: LoopingAnim(playerid, "SNM", "SPANKINGW", 4.1, 0, 1, 1, 1, 0);
		case 4: LoopingAnim(playerid, "SNM", "SPANKINGP", 4.1, 0, 1, 1, 1, 0);
		case 5: LoopingAnim(playerid, "SNM", "SPANKEDW", 4.1, 0, 1, 1, 1, 0);
		case 6: LoopingAnim(playerid, "SNM", "SPANKEDP", 4.1, 0, 1, 1, 1, 0);
		case 7: LoopingAnim(playerid, "SNM", "SPANKING_ENDW", 4.1, 0, 1, 1, 1, 0);
		case 8: LoopingAnim(playerid, "SNM", "SPANKING_ENDP", 4.1, 0, 1, 1, 1, 0);
	}
	return 1;
}

CMD:bj(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	if(sscanf(params, "i", anumber) || anumber < 1 || anumber > 12)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /bj [1-12]");

	switch(anumber) {
		case 1: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 0);
		case 2: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_START_W", 4.1, 0, 1, 1, 1, 0);
		case 3: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.1, 0, 1, 1, 1, 0);
		case 4: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_W", 4.1, 0, 1, 1, 1, 0);
		case 5: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_END_P", 4.1, 0, 1, 1, 1, 0);
		case 6: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_END_W", 4.1, 0, 1, 1, 1, 0);
		case 7: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_START_P", 4.1, 0, 1, 1, 1, 0);
		case 8: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_START_W", 4.1, 0, 1, 1, 1, 0);
		case 9: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_LOOP_P", 4.1, 0, 1, 1, 1, 0);
		case 10: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_LOOP_W", 4.1, 0, 1, 1, 1, 0);
		case 11: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_END_P", 4.1, 0, 1, 1, 1, 0);
		case 12: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_END_W", 4.1, 0, 1, 1, 1, 0);
	}
	return 1;
}

CMD:shakehand(playerid, params[])
{
	if(!AnimationCheck(playerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");

	new anumber = -1;
	new giveplayerid;
	if(sscanf(params, "ui", giveplayerid, anumber) || anumber < 1 || anumber > 6)
		return SendClientMessage(playerid, 0x00BFFFFF, "USAGE:{FFFFFF} /shakehand [playerid/PartOfName] [1-6]");

	if(!IsPlayerConnected(giveplayerid))
		return SendClientMessage(playerid, COLOR_GREY, "That player is offline.");

	if(giveplayerid == playerid)
		return cmd_me(playerid, "shakes their own hand.");

	if(!IsPlayerInRangeOfPlayer(2.0, playerid, giveplayerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not near this player.");

	if(!AnimationCheck(giveplayerid, false))
		return SendClientMessage(playerid, COLOR_GREY, "That player cannot shake your hand right now.");

	new shakeStr[128];
	format(shakeStr, sizeof(shakeStr), "* You offered to shake %s's hand.", PlayerICName(giveplayerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, shakeStr);
	format(shakeStr, sizeof(shakeStr), "* %s has offered to shake your hand (type /accept handshake), to accept it.", PlayerICName(playerid));
	SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, shakeStr);
	HSHAKEOFFER[giveplayerid] = playerid;
	HShakeType[giveplayerid] = anumber;
	return 1;
}

////////////////////////////////////////////////////

stock AnimationCheck(playerid, bool:vehicleAllowed) {
	if(	GetPlayerAnimationIndex(playerid) == 1461 ||
		GetPlayerAnimationIndex(playerid) == 1463 ||
		pDeathState[playerid] != PLAYER_STATE_NORMAL || 
		PlayerTied[playerid] != 0 ||
		PlayerCuffed[playerid] != 0 ||
		PlayerTazed[playerid] != 0 || 
		PlayerFrozen[playerid] != 0 ||
		IsFishing[playerid] != 0 ||
		(PlayerInfo[playerid][pJailed] != 0 && PlayerInfo[playerid][pJailed] != 2)
	) return 0;

	if(!vehicleAllowed && IsPlayerInAnyVehicle(playerid))
		return 0;
	if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return 0;
	return 1;
}