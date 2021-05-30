#define MAX_TRAIN_SPEED		25
#define TRAIN_PAY 			150

enum stationinfoenum{
	Float:Six,
	Float:Siy,
	Float:Siz,
	TrainFee
}
new StationInfo[4][stationinfoenum] = {
	{2853.3149,1539.0455,12.1256,150},
	{-1941.0543,192.4175,26.9654,300},
	{791.0947,-1345.3594,-0.1244,100},
	{1695.2631,-1953.6426,14.8756,50}
};
PassengersMoving(playerid){
	GameTextForPlayer(playerid, "~g~Passengers are moving...", 5000, 5);
	TogglePlayerControllable(playerid, 0);
	return SetTimerEx("PassengersMovingFunction", 5000, false, "i", playerid);
}
function PassengersMovingFunction(playerid){
	return TogglePlayerControllable(playerid,1);
}
Hook:tr_OnPlayerEnterCheckpoint(playerid){
	if(PlayerInfo[playerid][pJob]==JOB_TRAINDRIVER){
        if(GetPVarInt(playerid, "DrivingTrain")==1){
            DisablePlayerCheckpoint(playerid);
            SetPlayerCheckpoint(playerid, StationInfo[0][Six],StationInfo[0][Siy],StationInfo[0][Siz],20);
            SetPVarInt(playerid, "DrivingTrain", 2);
            if(GetVehicleSpeed(GetPlayerVehicleID(playerid))>MAX_TRAIN_SPEED){
            	return SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "** Your speed was too high thats why you missed the station [MAX Speed %i] **",MAX_TRAIN_SPEED);
            }else{
            	SendClientMessage(playerid,COLOR_LIGHTBLUE,"** You earned $150 for transporting peoples **");
            	PassengersMoving(playerid);
            	return GiveMoney(playerid,StationInfo[0][TrainFee]);
            } 
        }
        if(GetPVarInt(playerid, "DrivingTrain")==2){
            DisablePlayerCheckpoint(playerid);
            SetPlayerCheckpoint(playerid, StationInfo[1][Six],StationInfo[1][Siy],StationInfo[1][Siz],20);
            SetPVarInt(playerid, "DrivingTrain", 3);
            if(GetVehicleSpeed(GetPlayerVehicleID(playerid))>MAX_TRAIN_SPEED){
            	return SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "** You speed was too high thats why you missed the station [MAX Speed %i] **",MAX_TRAIN_SPEED);
            }else{
            	SendClientMessage(playerid,COLOR_LIGHTBLUE,"** You earned $150 for transporting peoples **");
            	return GiveMoney(playerid,StationInfo[1][TrainFee]);
            }
        }
        if(GetPVarInt(playerid, "DrivingTrain")==3){
            DisablePlayerCheckpoint(playerid);
            SetPlayerCheckpoint(playerid, StationInfo[2][Six],StationInfo[2][Siy],StationInfo[2][Siz],20);
            SetPVarInt(playerid, "DrivingTrain", 4);
            if(GetVehicleSpeed(GetPlayerVehicleID(playerid))>MAX_TRAIN_SPEED){
            	return SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "** You speed was too high thats why you missed the station [MAX Speed %i] **",MAX_TRAIN_SPEED);
            }else{
            	SendClientMessage(playerid,COLOR_LIGHTBLUE,"** You earned $150 for transporting peoples **");
            	return GiveMoney(playerid,StationInfo[2][TrainFee]);
            }
             
        }
        if(GetPVarInt(playerid, "DrivingTrain")==4){
            DisablePlayerCheckpoint(playerid);
            SetPlayerCheckpoint(playerid, StationInfo[3][Six],StationInfo[3][Siy],StationInfo[3][Siz],20);
            DeletePVar(playerid, "DrivingTrain");
            RemovePlayerFromVehicle(playerid);
            if(GetVehicleSpeed(GetPlayerVehicleID(playerid))>MAX_TRAIN_SPEED){
            	return SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "** You speed was too high thats why you missed the station [MAX Speed %i] **",MAX_TRAIN_SPEED);
            }else{
            	SendClientMessage(playerid,COLOR_LIGHTBLUE,"** You earned $150 for transporting peoples **");
            	return GiveMoney(playerid,StationInfo[3][TrainFee]);
            }
        }
    }
	return 1;
}
Hook:train_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger){
	if(GetVehicleModel(vehicleid) == 538 && PlayerInfo[playerid][pJob] != JOB_TRAINDRIVER) {
		ClearAnimations(playerid, 1);
		SetCameraBehindPlayer(playerid);
		return SendClientMessage(playerid, COLOR_GREY, "You must have train driver job before driving a train");
	}
	SetPVarInt(playerid, "DrivingTrain", 1);
	SetPlayerCheckpoint(playerid, StationInfo[0][Six],StationInfo[0][Siy],StationInfo[0][Siz],20);
	return 1;
}
