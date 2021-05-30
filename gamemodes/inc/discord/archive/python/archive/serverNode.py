import asyncio
from server import Server


class ServerNode:


    def __init__(self, host, port):
        self.loop = asyncio.get_event_loop()
        self.aserver = ""
        self.process = ""
        self.host = host
        self.port = port

        asyncio.run_coroutine_threadsafe(self.start_server(), self.loop)
        
    async def start_server(self):

        #start server & add to async loop
        self.aserver = await asyncio.start_server(self.readwrite, self.host, self.port, loop = loop)
        self.process  = loop.run_until_complete(self.async_server)
        self.loop.run_until_complete()

    def readwrite(self, reader, writer):
        self.server = Server(reader, writer)

    def close(self):
        self.process.close()
        self.async_loop.run_until_complete(server.wait_closed())
        self.async_loop.close()


