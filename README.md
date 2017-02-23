# bananagram

## \*NIX
```
$ cd Bananagram
$ make
$ ./banana -h
$ ./banana  -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P
$ ./banana -F ../TestData/shuffledWords -n 21
```

## MacOS
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
$ ./banana -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P -o 5 
debug=false; dim=256; tile_count=0; output_options=5
dictionary=; initial_letters=SAITHAT
There are 144 initial tiles.

### __HAT:__A__:SIT__:
__HAT
__A__
SIT__

### __S:__I:HAT:A__:T__:
__S
__I
HAT
A__
T__

### __HAT:__I__:SAT__:
__HAT
__I__
SAT__

...
$ 
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


