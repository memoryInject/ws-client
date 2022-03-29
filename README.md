
# WS Client

WebSocket command line client for testing WebSocket server.  
This command line tools based on https://github.com/dhbaird/easywsclient,   
JSON Parsing using https://github.com/nlohmann/json,   
Console color and formatting using https://github.com/memoryInject/color-console.  

#### **This application still in an early development phase, only tested with Linux. (It might works on macOS)
## Screenshots

![App Screenshot](https://res.cloudinary.com/memoryinject/image/upload/h_458,c_scale/v1648024478/ws-client-01_lldds8.png)

![App Screenshot](https://res.cloudinary.com/memoryinject/image/upload/h_458,c_scale/v1648024485/ws-client-02_sl3b32.png)



## Features
- Light weight 🍂
- Blazing fast ⚡
- Portable 💼
- JSON formatted output to console.
- JSON support for sending message.
- Auto reconnect, wait for 5 seconds.
- Keywords for manual reconnect (`r`, `reset`, `reconnect`, `restart`).
- Keywords for exit/closing connection (`q`, `quit`, `exit`).
- Keywords for send message to server (`res`, `send`, `response`).
- Keywords for help (`h`, `help`).


## Build

To build the executable download this repo first, then use `make` to create the executable.

```bash
git clone https://github.com/memoryInject/ws-client
cd ws-client
make
```

After running the code above it will create executable at `bin/ws`

#### Optional:
If you want to move the `ws` executable to `~/bin`, run `make move` after running `make`, 
this will make `ws` added to `path`. 
## Usage/Examples
Make sure you build the `ws` executable and added to system `path`.   
To connect to a WebSocket server pass the WebSocket address as first argument.

```bash
❯ ws 'ws://localhost:8000/ws'
Connected: ws://localhost:8000/ws

>>> JSON Data:
{
    "message": "connected"
}

>>> JSON Data:
{
    "message": "Hello World!",
    "type": "notification"
}
```

To exit the program type `q` and `enter`
```bash
~
❯ ws 'ws://localhost:8000/ws'
Connected: ws://localhost:8000/ws

>>> JSON Data:
{
    "message": "connected"
}

>>> JSON Data:
{
    "message": "Hello World!",
    "type": "notification"
}
q
Exit
Connection closed: ws://localhost:8126/ws
~
❯ 
```
   
To restart the program type `r` and `enter`
```bash
~                                                                                  
❯ ws 'ws://localhost:8126/ws'
Connected: ws://localhost:8126/ws

>>> JSON Data:
{
    "message": "connected"
}

r
Reset connection
Connection closed: ws://localhost:8126/ws
Connected: ws://localhost:8126/ws

>>> JSON Data:
{
    "message": "connected"
}
```

To send message to server type `res` and `enter` then type message as `string` or `json` then `enter` to send.
```bash
~                                                                                     
❯ ws 'ws://localhost:8126/ws'
Connected: ws://localhost:8126/ws

>>> JSON Data:
{
    "message": "connected"
}

>>> JSON Data:
{
    "message": "hello from server",
    "type": "notification"
}
res
Message: hello

<<< hello

>>> world
res
Message: { "msg": "world", "type": "info" }

<<< JSON Data:
{
    "msg": "world",
    "type": "info"
}

```
## License

[MIT](https://choosealicense.com/licenses/mit/)

