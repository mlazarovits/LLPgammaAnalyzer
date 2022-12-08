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


## To make Ntuples from AOD
```
cmsRun config/llpgana_mc_aod.py 
```
make sure you are running over the files you want and the output file name is updated

## To run primary vertex time skimmer
```
pvTimeSkimmer
```


## To run Jack's AOD skimmer/histogram maker
```
cd macros
make
./hist_maker_aodsim.obj
```
