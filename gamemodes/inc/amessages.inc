#define MAX_LINES 20 // the number of messages that can be logged/displayed

new LINES_AMOUNT;

new aMessageInfo[MAX_LINES][128]; //20 strings to store the amessages

forward SaveAdminMessages();
public SaveAdminMessages() //if experiencing extra new lines make sure .cfg file is UNIX format
{
	new File:hFile;
	hFile = fopen("amessages.cfg", io_write);
	fclose(hFile);
	hFile = fopen("amessages.cfg", io_append);
	new newstr[128];
	new write[128];
	if(hFile)
	{
		for(new i = 0; i < sizeof(aMessageInfo); i++) {
			
			strmid(newstr, aMessageInfo[i], 0, strlen(aMessageInfo[i]));
			format(write, sizeof(write), "%s\n", newstr);
			fwrite(hFile, write);
			printf("saveAdminMessages: %s", aMessageInfo[i]);
		}
		fclose(hFile);
	} else {
		print("amessages.cfg not found");
    }
}

stock shiftArray() {
	new curr, old;
	for(new i = 0; i < sizeof(aMessageInfo); i++) { //loop through
		curr = i;
		aMessageInfo[old] = aMessageInfo[curr];
		old = curr;
	}
}

stock LoadAdminMessages() {
    new File:file = fopen("amessages.cfg",io_read);
	new line[128];
    if(file) {
        while(fread(file,line)) {
			if( MAX_LINES == LINES_AMOUNT ) break;
			strmid(aMessageInfo[LINES_AMOUNT], line, 0, strlen(line)-1); //linux problem here i bet
			LINES_AMOUNT++;
        }
		printf("LoadAdminMessages: LINES_AMOUNT: %d", LINES_AMOUNT);
        fclose(file);
    } else {
		print("amessages.cfg not found");
    }
}
