import discord
from discord.ext import commands
import asyncio
import importlib

#external files belonging to this project
from output import *


description = '''I'm the Sarpian bot designed for San Andreas Roleplay. Treat me like your assistant.'''
bot = commands.Bot(command_prefix='!', description=description)
channel = discord.Object(id='420986449932976129')

serverobj = None

adminRoles = ["199936764935143425", "199937309737484289", "199937425672241153", "365936281298927628", "397886565713444875"]

"""
        EVENTS
"""

@bot.event
async def on_ready():
    #await bot.send_message(channel, "Hi there! I'm the Sarpian bot designed for San Andreas Roleplay.")
    #await bot.send_message(channel, "I will be your assistant. So far though, all I can do is forward you messages from the SA:MP server.")
    print("------Connected...")
    loop = asyncio.get_event_loop()
    coro = asyncio.start_server(readwrite, "0.0.0.0", 5000, loop = loop)
    server = asyncio.run_coroutine_threadsafe(coro, loop)
    print('Socket is up')


class AServer:
    def __init__(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter, bot, loop=None):
        self.loop = loop or asyncio.get_event_loop()


        self.reader = reader
        self.writer = writer
        self.alive = True
        #self.hostname = socket.get_hostname()

        self.__ready_condition = asyncio.Condition()
        self.ready = False
        self.bot = bot


        asyncio.run_coroutine_threadsafe(self.GetData(), self.loop)
        

    async def GetData(self):
        while True:
            #await this function till there is data available
            data = await self.reader.read(300)
            message = data.decode('ascii')
            print(message)

            embed = GetEmbededMessage(message)
            print(embed)
            if not embed: 
                print("There was a blank message!")
            global bot
            global channel
            await bot.send_message(channel, embed=embed)
            

    async def SendData(self, data):
        #if sock not ready, AWAIT until it is
        """ if not self.ready:
            print('waiting..')
            await self.wait_for_ready()
        """
        #send some data
        self.writer.write(data.encode())
        print("SENDING: " + data)
        await self.writer.drain()


async def readwrite(reader, writer):
    global serverobj
    global bot
    global channel
    serverobj = AServer(reader, writer, bot)
    print(serverobj)
    await bot.send_message(channel, "I have connected to the server.")

def is_admin(user):
    for urole in user.roles:
        for role in adminRoles:
            if urole.id == role:
                return True
    return False

@bot.command(pass_context=True)
async def hi(ctx):
    if not is_admin(ctx.message.author):
        await bot.say("You're not an administrator.")
        return

    if serverobj:
        await bot.say("The server is connected.")
    else:
        await bot.say("Uhoh...the server is not connected.")


@bot.command()
async def admins():
    if serverobj:
        await serverobj.SendData("!ADMINS!")

@bot.command()
async def servertime():
    if serverobj:
        await serverobj.SendData("!SERVERTIME!")

@bot.command(pass_context=True)
async def gmx(ctx):
    if not is_admin(ctx.message.author):
        await bot.say("You're not an administrator.")
        return
    if serverobj:
        await serverobj.SendData("!GMX!")

@bot.command(pass_context = True)
async def a(ctx, *, msg):
    if not is_admin(ctx.message.author):
        await bot.say("You're not an administrator.")
        return

    if serverobj:
        nick = ctx.message.author
        await serverobj.SendData("!AMSG!&" + str(nick) + "&" + msg)


"""
        COMMANDS
"""





"""
        IMPORTANT STUFF...
"""



bot.run('MzE2ODY1MDYyMTU5NjQ2NzIy.DYF_ig.cQRn1lz_HqqqNUUR72mH_Kkc9Bw')


try:
    loop.run_forever()
except KeyboardInterrupt:
    pass


# Close the server
loop.run_until_complete(server.wait_closed())
loop.close()


bot.close()