# LLPgammaAnalyzer
Long Lived Particle to Photon Analyzer - adapted for primary vertex timing analysis

## To set up CMSSW Analyzers and Executables
Make sure the CMSSW environment is set with
```
cd CMSSW_X_X_X/src
cmsenv
```
Integrate analyzers and create executables with CMSSW
```
scram b 
```
now you are set to run the analyzer/ntuplizer or custom executables! (executable scripts found in ```bin/```)
Every time the analyzer changes, make sure to redo this step to recompile in CMSSW.


## To make Ntuples from AOD
```
cd test/
cmsRun llpgana_mc_aod.py 
```
make sure you are running over the files you want and the output file name is updated


## To run multicrab jobs for ntuplizer
```
cd test/
python2 multicrab_mc_llpgana_cmssw106_aod.py -d [dataset] -l [requestName label] 
```
this script needs to be run with python 2 for certain dependencies. 
Make sure you are running from the ```test/``` directory so the multicrab script and the config script are in the same place.

## To run primary vertex time skimmer
```
cd /uscms/home/mlazarov/nobackup/CMSSW_10_6_5/src/PVTiming/LLPgammaAnalyzer
pvTimeSkimmer
```
## To make file lists to run skimmer over
```
cd macros/
python mkEosFileList.py
```
## To run Jack's code
```
cd ~/nobackup/Jack_LLPCode/CMSSW_X_X_X/src
scram b
```

## To run Jack's AOD skimmer/histogram maker
```
cd LLPgammaAnalyzer/macros
make
./hist_maker_aodsim.obj
```

## Troubleshooting
- if `undefined reference` error pops up during compilation, make sure the necessary source (.cc) files are included in the relevant BuildFile
- instructions are for running on LPC system
- make sure you edit the header files in `interface/` not `include/`
- can find the global tag for the corresponding dataset/sample in [CMS DAS](https://cmsweb.cern.ch/das/)

## Analyzer notes
- primary vertex (PV) coordinates are gotten at the beginning of the each event in event loop
### Steps for adding a new branch
- declare branch object in corresponding ```.hh``` file
- add branch to tree
- fill branch in appropriate spot
- make sure branch gets cleared in an appropriate spot too
