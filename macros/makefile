CC=g++
#CFLAGS = -pthread -std=c++1z -m64 -pthread -lm -ldl -rdynamic
CFLAGS = -pthread -std=c++17 -m64
#LIBS = -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc
LIBS = -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
#RTLOC = /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_5/external/slc7_amd64_gcc700/bin/../../../../../../../slc7_amd64_gcc700/lcg/root/
#RTLOC = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/cmssw/CMSSW_11_3_0_pre6/external/slc7_amd64_gcc900/bin/../../../../../../../slc7_amd64_gcc900/lcg/root/
RTLOC = /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_6_20/external/slc7_amd64_gcc700/bin/../../../../../../../slc7_amd64_gcc700/lcg/root/
#RTVRS = 6.12.07-ogkkac4
#RTVRS = 6.22.08-ljfedo
RTVRS = 6.14.09-pafccj6
INCLUDES = -I$(RTLOC)$(RTVRS)/include
LFLAGS = -L$(RTLOC)$(RTVRS)/lib

p1=hist_maker_aodsim_test
p2=hist_maker_aodsim
pv_time=pv-time-skimmer

alltargets: $(p2) $(pv_time)
pv-time: $(pv_time)

$(p1): $(p1).cpp
	$(CC) -o $(p1).obj $(p1).cpp $(CFLAGS) $(INCLUDES) $(LFLAGS) $(LIBS)

$(p2): $(p2).cpp
	$(CC) -o $(p2).obj $(p2).cpp $(CFLAGS) $(INCLUDES) $(LFLAGS) $(LIBS)

$(pv_time): $(pv_time).C
	$(CC) $(CFLAGS) -o $(pv_time).x $(pv_time).C $(INCLUDES) $(LFLAGS) $(LIBS)  
clean:
	rm -f *.obj
	rm -f *.x
	rm -f *.d
