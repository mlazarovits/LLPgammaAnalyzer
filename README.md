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


## To run multicrab jobs for ntuplizer
```
cd test/
python2 multicrab_mc_llpgana_cmssw106_aod.py
```
this script needs to be run with python 2 for certain dependencies

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

## Troubleshooting
- if `undefined reference` error pops up during compilation, make sure the necessary source (.cc) files are included in the relevant BuildFile
- instructions are for running on LPC system
- make sure you edit the header files in `interface/` not `include/`

