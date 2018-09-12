To decode frames. For example:
    
```bash
    ./j1939Decoder --id 00febffe --data "00 c4 00 00 00 00 00 00"
    Name	PGN	Source Address	PDU format	Priority	
    EBC2	FEBF	FE		2		0	
    SPN 904: Front Axe Speed -> Value: 196 kph
    SPN 905: Relative Speed; Front Axle, Left Wheel -> Value: -7 kph
    SPN 906: Relative Speed; Front Axle, Right Wheel -> Value: -7 kph
    SPN 907: Relative Speed; Rear Axle 1, Left Wheel -> Value: -7 kph
    SPN 908: Relative Speed; Rear Axle 1, Right Wheel -> Value: -7 kph
    SPN 909: Relative Speed; Rear Axle 2, Left Wheel -> Value: -7 kph
    SPN 910: Relative Speed; Rear Axle 2, Right Wheel -> Value: -7 kph
```
