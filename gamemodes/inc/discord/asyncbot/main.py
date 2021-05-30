import discord
from discord.ext import commands
import asyncio
import server.server as server
import settings
import json

#Request types
import requests

description = """
Discord-SARP Connector\n
Author: dy1zan
"""
bot = commands.Bot(command_prefix='!', description=description)
s = None

@bot.event
async def on_ready():
    print('Connected to Discord')
    global s
    s = server.AServer(settings.SERVER_PORT, bot)
    s.start()

#These need moving to their own files where Discord commands are specified.
@bot.command(pass_context=True)
async def a(ctx, *, msg : str):
    out = json.dumps({
        "type":"asay",
        "sender":str(ctx.message.author),
        "message":msg
    })
    global s
    await s.write(out)

@bot.command(pass_context=True)
async def admins(ctx):
    channel = ctx.message.channel
    out = json.dumps({
        "type":"admins",
        "channel":str(channel.id)
    })
    global s
    await s.write(out)


bot.run(settings.BOT_TOKEN)