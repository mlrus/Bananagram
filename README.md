# bananagram

```
A*3 B*1 E*3 G*1 I*1 L*1 N*3 P*1 Q*1 R*1 T*4 U*1

___E___Q___       ______T__       _G______
PAGAN_BUILT       ____GRANT       _R__B_P_
___T___A___       ______B__       QUAIL_A_
___ENTER___       __PANEL__       _N__E_T_
___N___T___       QUITE____       _T_EATEN
                  __N______       ____T_N_
                  __T______       


B*2 D*1 E*3 G*1 I*2 L*1 N*2 O*4 R*2 S*1 T*2

__________O       ____INGREDIENTS       __________O_
_______B__O       ____L_O__O_____       __________B_
______BORED       ___BE__________       _T__O_____O_
_______O___                             _INGREDIENTS
_______L___                             _L_O________
                                        BE__________
```

## \*NIX
```
$ cd Bananagram
$ make
$ ./banana -h
$ ./banana  -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P
$ ./banana -F ../TestData/shuffledWords -n 21
```

## OS-X
```
$ git clone https://github.com/mlrus/Bananagram.git
$ xcodebuild -project Bananagram.xcodeproj/ -alltargets
$ build/Release/SimpleTest
$ build/Release/bananagram -h
$ build/Release/bananagram -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P
$ build/Release/bananagram -F TestData/shuffledWords -n 21
$ build/Release/bananagram -F TestData/shuffledWords -n 21 -o 4 
```

## Sample
```
build/Release/bananagram -F TestData/words.5letters -s -T -M 1000000
debug=false
dictionary=TestData/words.5letters; dim=256; initial_letters=; max_results=1000000; output_options=1; preserve_order=false; shuffle_tiles=true; shuffle_words=false; tile_count=21
Sorting words
Dedup words
Randomizing unplaced tiles
Sorting words
Dedup words
Enabled 76 of 3372 words
Copied 76 words
A*3 B*1 D*1 E*5 F*1 H*2 M*1 O*2 R*1 W*2 X*1 Z*1
Considering 76 in decreasing size order
Buid from DOZER
Buid from BOOZE
                              total=1 unique=1 (100%)
_____E____
_____M____
_____BOOZE
_____E____
__WHARF___
____H_____
____E_____
____A_____
WAXED_____


                              total=3 unique=2 (67%)
_______A____
_______M____
_______BOOZE
_______E____
____WHARF___
____A_______
____X_______
____E_______
HEWED_______


                              total=4 unique=3 (75%)
_______A____
_______M____
____H__BOOZE
____E__E____
____WHARF___
____E_______
WAXED_______

...
5 : ### _______AMAZE:_______B____:_______H____:____W__O____:____AFORE___:____X_______:____E_______:HEWED_______:
5 : ### _____E____:_____M____:_____BOOZE:_____E____:__WHARF___:____H_____:____E_____:____A_____:WAXED_____:
3 : ### ______M_:______A_:___BOOZE:______E_:__HEWED_:____H___:____A___:WAXER___:____F___:
3 : ### _____W_:_____A_:_____X_:_____E_:__WHARF:____H__:____E__:____A__:EMBED__:__O____:__O____:__Z____:__E____:
2 : ### ______H:______E:____W_W:____A_E:A_FOXED:M___E__:ABHOR__:Z______:E______:
3 : ### _____B_F:_____O_A:_____O_M:___W_Z_E:___AHEAD:___X____:___E____:WHERE___:
#unique=217; #computed=656; #starts=76 for HEZXFAAEABHMODWOEREWE
```

or

```
$ ./banana -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P -o 4 | grep '#' | sort -u
### __H:__A:SAT:I__:T__:
### __H:__A:SIT:A__:T__:
### __H:__I:SAT:A__:T__:
### __HAT:__A__:SIT__:
### __HAT:__I__:SAT__:
### __HIT:__A__:SAT__:
### __S:__A:HAT:I__:T__:
### __S:__A:HIT:A__:T__:
### __S:__I:HAT:A__:T__:
### __SAT:__A__:HIT__:
### __SAT:__I__:HAT__:
### __SIT:__A__:HAT__:
$ 
```


