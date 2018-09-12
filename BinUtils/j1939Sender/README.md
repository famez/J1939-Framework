A CLI utility to create frames, configure them and send them through the CAN bus. For example:
    
```bash
    ./j1939Sender 
    Version: 1.0
    Detected SocketCan framework... Using SocketCan backend
    Executing commands...
    j1939Sender> create frame name: wheel_speed title: CCVS
    Frame correctly created
    j1939Sender> create frame name: engine_speed title: EEC1
    Frame correctly created
    j1939Sender> set frame wheel_speed period: 100 priority: 2 source: 20 spn: 84 value: 50
    Spn set to value 50
    j1939Sender> set frame engine_speed period: 100 priority: 2 source: 20 spn: 190 value: 200
    Spn set to value 200
    j1939Sender> send frame wheel_speed interface: can0
    j1939Sender> send frame engine_speed interface: can0
    j1939Sender> list frames
    engine_speed: 08f00420 00 00 00 40 06 00 00 00  Period: 100 ms Sent from: can0 
    wheel_speed: 08fef120 00 00 32 00 00 00 00 00  Period: 100 ms Sent from: can0 

```
