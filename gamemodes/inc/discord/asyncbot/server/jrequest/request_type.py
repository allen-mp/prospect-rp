import discord

request_types = {}

class request_type(object):
    """
        ..decorator:: on_response
            Defines a new request callback.
            :inherits: object

            :example:
                @on_request
                def my_request_type_name(data : str):
                    print("I got some data!\n" + data['body'])
    """
    def __init__(self, f):

        # check if the request callback is already defined somewhere
        if(request_types.get(f.__name__)):
            raise NameError("The response function " + f.__name__ + " has already been defined.")

        # add the callback to the list
        request_types[f.__name__] = f

    def __call__(self, f):
        def wrapper(*args):
            f(*args)

        return wrapper

def get_types():
    return request_types