Hook:r_OnGameModeInit()
{
    SetTimer("AutoMessageSystem", 60000 *15, true);
}


forward AutoMessageSystem();
public AutoMessageSystem()
{
    new message = random(7);
    if(message == 0)
    {
      SendClientMessageToAll(0x3D97E0FF, "Searching for most paying job?! /jobhelp, the green marked jobs are most paying ones.");
    }
    else if(message == 1)
    {
      SendClientMessageToAll(0x3D97E0FF, "Bored? Did you know that you can rob ATMs? (/robatm) near it.");   
    }
    else if(message == 2)
    {
      SendClientMessageToAll(0x3D97E0FF, "Did you know that you can find many type of things inside garbage cans by doing (/lookin) near them?");   
    }
    else if(message == 3)
    {
      SendClientMessageToAll(0x3D97E0FF, "You can buy a lot of things to design your house, even VIP in game without donation. Use (/prices)!");   
    }
    else if(message == 4)
    {
      SendClientMessageToAll(0x3D97E0FF, "Did you know that you can now rob truckers and make a lot? Try it out!");   
    }
    else if(message == 5)
    {
      SendClientMessageToAll(0x3D97E0FF, "Did you know if you're active enough and have (40+ hours) you can have Diamond VIP in reward? Do /myactivity.");   
    }
    else if(message == 5)
    {
      SendClientMessageToAll(0x3D97E0FF, "Did you know that businesses now pay a lot and it worths buying them? Got 80m? Buy one!");   
    }
    return 1;
}