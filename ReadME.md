## Introduction: 

In this repo, we implemented a parallel ensemble matching algorithm based on subscription partition for
content-based publish/subscribe systems. 

## Quick Start: 

```
g++ -std=c++11 chrono_time.h data_structure.h generator.h generator.cpp main.cpp printer.h printer.cpp rein.h rein.cpp tama.h tama.cpp enmatp.h enmatp.cpp enmatp_fg.h enmatp_fg.cpp util.h util.cpp -o fgPEM
ulimit -s 819200
./fgPEM
```

Please note that this project is based on C++11 standards, and requires a GNU C++ compiler no older than version 4.8.


## Input: 
The input parameters are read from paras_1.txt


## Output: 

We need to create new folders called result and data. The folder data will create new subscriptions and publications if you don't use any dataset. The folder result will record the result of PEM, fgPEM, REIN and TAMA respectively.