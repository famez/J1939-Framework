A CLI utility to create frames, configure them and send them through the CAN bus.
    
```bash
    j1939Sender
    Version: 1.0
    Interfaces: 
    can0 
    Initialized: can0 
    j1939Sender>
    j1939Sender> create frame name: my_frame title: CCVS
    Frame correctly created
    j1939Sender> create frame name: my_frame1 pgn: FEF1
    Frame correctly created
    j1939Sender> list frames
    my_frame: 00fef1fe ff ff ff ff ff ff ff ff 
    my_frame1: 00fef1fe ff ff ff ff ff ff ff ff 
    set frame my_frame period: 100
    set frame my_frame source: 45
    set frame my_frame spn: 84  value: 15
    print frame my_frame
    send frame my_frame interface: can0

```

# Commands

## Create frame


```bash
	create frame name: <name> title: <pgn_name>
	create frame name: <name> pgn: <pgn_number>
```

Example:

```bash
	create frame name: my_frame title: CCVS
	create frame name: my_frame1 pgn: FEF1
```

## List frames


```bash
	list frames
```

## Set frame values

```bash
	set frame <name> [period: <millis>] [source: <src_addr_hex>] [source: <src_addr_hex>] [priority: <prio>] [spn: <spn>  value: <value>] [spn: <spn>  value: <value>]...
```

Example:

```bash
	set frame my_frame period: 100 source: 2C
	set frame my_frame spn: 84 value: 15 priority: 4
```

## Print frame

```bash
	print frame <name>
```

Example:

```bash
	print frame my_frame
```

## List interfaces

```bash
	list interfaces
```

## Send frame

```bash
	send frame <name> interface: <interface>
```

Example:

```bash
	send frame my_frame interface: can0
```

## Unsend frame

```bash
	unsend frame <name> [interface: <interface>]
```

Example:

```bash
	unsend frame my_frame
	unsend frame my_frame interface: can0
```

## List TTS

```bash
	list tts all
	list tts number: <number>
```

Example:

```bash
	list tts number: 18
```


## Set TTS

```bash
	set tts number: <number> status: <status>
```

Example:

```bash
	set tts number: 3 status: 2
```


## Send TTS

```bash
	send tts interface: <interface> period: <millis>
```

Example:

```bash
	send tts interface: can0 period: 4000
```


## Add dtc

```bash
	add dtc <name> oc: <occurrence_code> fmi: <failure_mode_identidier> spn: <suspect_number_parameter>
```

Example:

```bash
	create frame name: dm1 title: DM1
	add dtc dm1 oc: 2 fmi: 6 spn: 84
```

## Set dtc

```bash
	set dtc <name> <pos> oc: <occurrence_code> fmi: <failure_mode_identidier> spn: <suspect_number_parameter>
```

Example:

```bash
	set dtc dm1 0 oc: 3 fmi: 5 spn: 84
```

## Delete dtc

```bash
	delete dtc <name> <pos>
```

Example:

```bash
	delete dtc dm1 0
```


## Quit

```bash
	quit
```
