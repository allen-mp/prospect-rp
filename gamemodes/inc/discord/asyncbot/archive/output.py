import discord
import re

def GetEmbededMessage(data):
    embed = ""

    if data.startswith('!AMSG!'): # admin sent a message to /a
        embed = discord.Embed(title=get_player(data), description=get_msg(data), color=0xFEB918)

    elif data.startswith('!PREPORT!'): # a player has sent a /report
        embed = discord.Embed(title="Report from " + get_player(data), description=get_msg(data), color=0xFFFF91)
        embed.set_footer(text=get_time(data))

    elif data.startswith('!PAYLOG!'): # a player is playing with money
        embed = discord.Embed(title="Logger: Transactions", description=get_msg(data), color=0x46c130)
        embed.set_footer(text=get_time(data))

    elif data.startswith('!ADMINS!'): # Discord user requested admins
        embed = discord.Embed(title="Online Administrators", description=get_msg(data), color=0x58aecb)


    elif data.startswith('!SERVERTIME!'):
        embed = discord.Embed(description="Server time: " + get_msg(data), color=0x58aecb)

    elif data.startswith('!ANN!'):
        embed = discord.Embed(title="Announcement", description=get_msg(data), color=0x58aecb)

    elif data.startswith('!GOVMSG!'):
        embed = discord.Embed(title="LIVE: Public Service Announcement", description=get_msg(data), color=get_color(data))
        embed.set_footer(text=get_time(data))

    return embed


def get_time(content):
    regex = re.search('!TIME!(.+?)!TIME!', content)
    if regex:
        return regex.group(1)
    else:
        return ""

def get_player(content):
    regex = re.search('!PLAYER!(.+?)!PLAYER!', content)
    if regex:
        return regex.group(1)
    else:
        return ""

def get_msg(content):
    regex = re.search('!MSG!(.+?)!MSG!', content)
    if regex:
        return regex.group(1)
    else:
        return ""

def get_color(content):
    regex = re.search('!COLOR!(.+?)!COLOR!', content)
    if regex:
        return regex.group(1)
    else:
        return 0xFFFFFF
