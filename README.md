# LLPgammaAnalyzer
Long Lived Particle to Photon Analyzer

## To set up CMSSW Analyzers
Make sure the CMSSW environment is set with
```
cmssw
```
Integrate analyzers with CMSSW
```
scram b 
```

## To run AOD skimmer/histogram maker:
```
cd macros
make
./hist_maker_aodsim.obj
```
