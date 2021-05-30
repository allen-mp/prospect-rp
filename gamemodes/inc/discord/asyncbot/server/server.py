import asyncio
import json

from . jrequest import request_type


class AServer(object):
    """
        Initalizes a new asynchronous server object.
        This server invokes methods with request_type decorators,
        when data is read. See requests.py
    """

    def __init__(self, port, bot):

        print(request_type.get_types())
        self.port = port
        self.bot = bot

        # constants
        self.BUFFER_SIZE = 512

        # privates
        self.__writer = None
        self.__alive = False
        self.__loop = None


    def start(self):
        """
            Starts the asynchronous server on the given port.
        """
        self.__loop = asyncio.get_event_loop()
        self.__alive = True
        coro = asyncio.start_server(self.handle_connection, '0.0.0.0', self.port)
        asyncio.ensure_future(coro)

       # try:
       #     self.__loop.run_forever()
       # finally:
       #     self.__loop.close()


    def stop(self):
        """
            Stops the asynchronous server.
        """
        self.__loop.stop()
        self.__alive = False


    async def handle_connection(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
        """
            Handles a new client connection using NIO.
            When reading, it invokes the suitable request callback depending
            on the JSON request.

            :param: reader, the stream reader to read data from (in bytes)
            :param: writer, the stream writer to write data to (in bytes)
        """
        print("Incoming client connection")

        try:
            self.__writer = writer

            while self.__alive:
                #await self.write("hi")
                data = await reader.readuntil(b"\r\n")
                data = data.decode('ascii')
                
                print(data)

                try:
                    # try parse the JSON, then get the type of request
                    data = json.loads(data)

                    # if it's a /terminate/ type, terminate the server
                    if(data['type'] == 'terminate'):
                        self.stop()

                    # invoke a callback belonging to a requset type if it is available
                    funcs = request_type.get_types()
                    if(funcs.get(data['type'])):    
                        await funcs[data['type']](self, data)

                except:
                    pass

        except ConnectionError:
            self._writer = None
            print("Client disconnected")



    async def write(self, message: str):
        """
            Writes data to the stream writer
        """
        if self.__writer == None:
            return

        self.__writer.write(message.encode())
        await self.__writer.drain()


