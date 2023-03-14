#! /usr/bin/env python

import os
from optparse import OptionParser

from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

def getOptions():
    """
    Parse and return the arguments provided by the user.
    """
    usage = ("Usage: %prog --crabCmd CMD [--workArea WAD --crabCmdOpts OPTS]"
             "\nThe multicrab command executes 'crab CMD OPTS' for each project directory contained in WAD"
             "\nUse multicrab -h for help")

    parser = OptionParser(usage=usage)

    parser.add_option('-c', '--crabCmd',
                      dest = 'crabCmd',
                      default = 'submit',
                      help = "crab command",
                      metavar = 'CMD')

    parser.add_option('-w', '--workArea',
                      dest = 'workArea',
                      default = 'myWorkSpace',
                      help = "work area directory (only if CMD != 'submit')",
                      metavar = 'WAD')

    parser.add_option('-o', '--crabCmdOpts',
                      dest = 'crabCmdOpts',
                      default = '',
                      help = "options for crab command CMD",
                      metavar = 'OPTS')


    parser.add_option('-d', '--dataset',
                      dest = 'dataset',
                      default = 'GJets',
                      help = "dataset to process",
                      choices = ['GJets','GMSB','QCD','H2LLP24b','data'],
		      )
    parser.add_option('-l','--label',
		       dest = 'label',
		       default = '',
		       help = 'label for output directory')

    (options, arguments) = parser.parse_args()

    if arguments:
        parser.error("Found positional argument(s): %s." % (arguments))
    if not options.crabCmd:
        parser.error("(-c CMD, --crabCmd=CMD) option not provided.")
    if options.crabCmd != 'submit':
        if not options.workArea:
            parser.error("(-w WAR, --workArea=WAR) option not provided.")
        if not os.path.isdir(options.workArea):
            parser.error("'%s' is not a valid directory." % (options.workArea))

    return options


def docrab( dataset ):

    options = getOptions()

    # The submit command needs special treatment.
    if options.crabCmd == 'submit':

        # External files needed by CRAB
        #inputJSON    = 'golden2016.json'
        #inputJSON    = 'golden2017.json'
        #inputJSON    = 'Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt'
        inptCfgEB    = 'fullinfo_detids_EB.txt'
        inptCfgEE    = 'fullinfo_detids_EE.txt'

        #--------------------------------------------------------
        # This is the base config:
        #--------------------------------------------------------
        from CRABClient.UserUtilities import config
        config = config()

        config.General.workArea    = options.workArea
        config.General.requestName = None

        config.JobType.pluginName  = 'Analysis'
        config.JobType.psetName    = 'llpgana_mc_aod.py'
        config.JobType.pyCfgParams = None

        config.Data.inputDataset   = None
        #config.Data.lumiMask       = inputJSON    # Comment out for MC only
        #config.Data.splitting     = 'Automatic'
        config.Data.splitting    = 'EventAwareLumiBased' # MC
        #for choosing # of files to run over
	#config.Data.splitting    = 'FileBased' # MC
        #config.Data.totalUnits    = '5' # MC
        if options.dataset == 'GMSB':
        	config.Data.unitsPerJob  =  1500 # MC GMSB
        elif options.dataset == 'GJets':
        	config.Data.unitsPerJob  =  5000 # MC GJet
        else:
        	config.Data.unitsPerJob   = 2000
        if options.dataset == 'data':
        	config.Data.runRange  =  '316241-316245' # MC GMSB

        config.JobType.allowUndistributedCMSSW = True
        config.JobType.inputFiles  = [ inptCfgEB, inptCfgEE ]
        config.Data.publication    = False
        #config.Site.storageSite    = 'T2_US_Nebraska'
        config.Site.storageSite    = 'T3_US_FNALLPC'
        #config.Data.outLFNDirBase  = '/store/user/jaking/LLPGamma/GMSB_v40/'
        #--------------------------------------------------------

        # Will submit one task for each of these input datasets.
        inputDataAndOpts = [[dataset[0]]]

        for inDO in inputDataAndOpts:
            print '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>' 
            print 'Input dataset for Crab Job : ' 
            #print( inDO )
            # inDO[0] is of the form /A/B/C. Since A+B is unique for each inDS, use this in the CRAB request name.
            primaryDataset = (inDO[0].split('/')[1]).split('_T')[0]
            print primaryDataset 
            runEra         = ((inDO[0].split('/')[2]).split('_')[0]+'_'+(inDO[0].split('/')[2]).split('_')[1]).split('AODSIM')[0]
            print runEra 
            dataset        = inDO[0].split('/')[3]
            print dataset 

            trial           = "llpga_"+primaryDataset+"_"+runEra+"_"+dataset
	    if options.label is not '':
		trial += "_"+options.label

            #config.Data.outLFNDirBase  = "/store/user/jaking/LLPGamma/"+trial+"/"
            config.Data.outLFNDirBase  = "/store/user/malazaro/LLPGamma/crabOutput/"+trial+"/"#"/store/group/lpcsusylep/jaking/LLPGamma/"+trial+"/"
            config.General.requestName   = trial+"_"+primaryDataset+"_"+dataset+"_"+runEra+"_request"
            config.Data.outputDatasetTag = trial+"_"+primaryDataset+"_"+dataset+"_"+runEra


	    print "Writing files to:", config.Data.outLFNDirBase
#>>>>>>>>>>>>>>>>>>>     #2018   #globalTag=106X_dataRun2_v28
            #config.JobType.pyCfgParams   = ['globalTag=106X_dataRun2_v28','outputFileName=output.root']
#>>>>>>>>>>>>>>>>>>>	    #2017   #globalTag=106X_dataRun2_v20
            #config.JobType.pyCfgParams   = ['globalTag=106X_dataRun2_v20', 'outputFileName=output.root']
#>>>>>>>>>>>>>>>>>>>	    #2016  #globalTag=106X_dataRun2_v27
            #config.JobType.pyCfgParams   = ['globalTag=106X_dataRun2_v27', 'outputFileName=output.root']

#>>>>>>>>>>>>>>>>>>>     #MC Run3  #globalTag=112X_mcRun3_2021_realistic_v16  #  <<<<<<<   comment/uncomment lumi mask when using/!using MC  >>>>>>>>>>>>>
            ##config.JobType.pyCfgParams   = ['globalTag=112X_mcRun3_2021_realistic_v16','outputFileName=output.root']

	    if options.dataset == 'GMSB':
#>>>>>>>>>>>>>>>>>>>     #MC GMSB 17  #globalTag=94X_mc2017_realistic_v14  #  <<<<<<<   comment/uncomment lumi mask when using/!using MC  >>>>>>>>>>>>>
            	config.JobType.pyCfgParams   = ['globalTag=94X_mc2017_realistic_v14','outputFileName=output.root']
            ##config.JobType.pyCfgParams   = ['globalTag=106X_dataRun2_v28','outputFileName=output.root','hasGenInfo=True']
#>>>>>>>>>>>>>>>>>>>     #MC RunIISummer20UL18RECO
	    if options.dataset == 'GJets':
            	config.JobType.pyCfgParams   = ['globalTag=106X_upgrade2018_realistic_v11_L1v1','outputFileName=output.root','hasGenInfo=True']
	    if options.dataset == 'data':
            	config.JobType.pyCfgParams   = ['globalTag=106X_dataRun2_v28','outputFileName=output.root']
	    else:
		print("Error: global tag not specificed for this dataset. Check the available tags in the script and set accordingly.")
		exit()

            config.Data.inputDataset     = inDO[0]
            # Submit.
            try:
                print "Submitting for input dataset %s" % primaryDataset + '_' + runEra + '_' + dataset
                crabCommand(options.crabCmd, config = config, *options.crabCmdOpts.split())
                os.system("rm -rf %s/crab_%s/inputs" % (config.General.workArea, config.General.requestName))
            except HTTPException as hte:
                print "Submission for input dataset %s failed: %s" % (inDO[0], hte.headers)
            except ClientException as cle:
                print "Submission for input dataset %s failed: %s" % (inDO[0], cle)

    # All other commands can be simply executed.
    elif options.workArea:

        for dir in os.listdir(options.workArea):
            projDir = os.path.join(options.workArea, dir)
            if not os.path.isdir(projDir):
                continue
            # Execute the crab command.
            msg = "Executing (the equivalent of): crab %s --dir %s %s" % (options.crabCmd, projDir, options.crabCmdOpts)
            print "-"*len(msg)
            print msg
            print "-"*len(msg)
            try:
                crabCommand(options.crabCmd, dir = projDir, *options.crabCmdOpts.split())
            except HTTPException as hte:
                print "Failed executing command %s for task %s: %s" % (options.crabCmd, projDir, hte.headers)
            except ClientException as cle:
                print "Failed executing command %s for task %s: %s" % (options.crabCmd, projDir, cle)


#############################################################

def run_multi( ds ):

# DataSet: DATA
 
        dsData = [

			# Dataset: JetHT UL2018

			['/JetHT/Run2018A-UL2018_MiniAODv2-v1/MINIAOD',''],
		#	['/JetHT/Run2018B-UL2018_MiniAODv2-v1/MINIAOD',''],
		#	['/JetHT/Run2018C-UL2018_MiniAODv2-v1/MINIAOD',''],
		#	['/JetHT/Run2018D-UL2018_MiniAODv2-v1/MINIAOD',''],
		#	['/JetHT/Run2018D-UL2018_MiniAODv2-v2/MINIAOD',''],

			# Dataset: /EGamma/Run2018-12Nov2019_UL2018-/MINIAOD

           # ['/EGamma/Run2018A-12Nov2019_UL2018-v2/MINIAOD',''],
           # ['/EGamma/Run2018B-12Nov2019_UL2018-v2/MINIAOD',''],
           # ['/EGamma/Run2018C-12Nov2019_UL2018-v2/MINIAOD',''],
           # ['/EGamma/Run2018D-12Nov2019_UL2018-v4/MINIAOD',''],
    
			# Dataset: /DoubleEG/Run2016-21Feb2020_UL2016-/MINIAOD

            #['/DoubleEG/Run2016B-21Feb2020_ver2_UL2016_HIPM-v1/MINIAOD',''],
            #['/DoubleEG/Run2016C-21Feb2020_UL2016_HIPM-v1/MINIAOD',''],
            #['/DoubleEG/Run2016D-21Feb2020_UL2016_HIPM-v1/MINIAOD',''],
            #['/DoubleEG/Run2016E-21Feb2020_UL2016_HIPM-v1/MINIAOD',''],
            #['/DoubleEG/Run2016F-21Feb2020_UL2016-v1/MINIAOD',''],
            #['/DoubleEG/Run2016G-21Feb2020_UL2016-v1/MINIAOD',''],
            #['/DoubleEG/Run2016H-21Feb2020_UL2016-v1/MINIAOD',''],

			# Dataset: /DoubleEG/Run2017-09Aug2019_UL2017-/MINIAOD

            #['/DoubleEG/Run2017B-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/DoubleEG/Run2017C-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/DoubleEG/Run2017D-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/DoubleEG/Run2017E-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/DoubleEG/Run2017F-09Aug2019_UL2017-v1/MINIAOD',''],

			# Dataset: /SingleElectron/Run2017-09Aug2019_UL2017-/MINIAOD

            #['/SingleElectron/Run2017B-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/SingleElectron/Run2017C-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/SingleElectron/Run2017C-09Aug2019_UL2017_EcalRecovery-v1/MINIAOD',''],
            #['/SingleElectron/Run2017D-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/SingleElectron/Run2017E-09Aug2019_UL2017-v1/MINIAOD',''],
            #['/SingleElectron/Run2017F-09Aug2019_UL2017_rsb-v2/MINIAOD',''],
            #['/SingleElectron/Run2017F-09Aug2019_UL2017_EcalRecovery-v1/MINIAOD',''],

        ]

# Dataset: QCD HT MC 

        run = 'RunIISummer19UL18RECO-106X_upgrade2018_realistic_v11' 
        tune = '_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8' 
        mcdset = 'AODSIM'
        dsQCD = [

            ['/QCD_HT50to100'   +tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT100to200'  +tune+'/'+run+'_L1v1-v2/AODSIM',''],
            ['/QCD_HT200to300'  +tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT200to300'  +tune+'/'+run+'_L1v1_ext1-v1/AODSIM',''], 
            ['/QCD_HT300to500'  +tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT500to700'  +tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT700to1000' +tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT1000to1500'+tune+'/'+run+'_L1v1-v2/AODSIM',''], 
            ['/QCD_HT1500to2000'+tune+'/'+run+'_L1v1-v2/AODSIM',''],
            ['/QCD_HT2000toInf' +tune+'/'+run+'_L1v1-v2/AODSIM',''],

		]

# Dataset: Run3 HTo2LongLivedTo4b

        Ht2LLstring = 'Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2'
        dsHt2LLt4b = [

            ['/HTo2LongLivedTo4b_MH-1000_MFF-450_CTau-100000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-1000_MFF-450_CTau-10000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-125_MFF-12_CTau-9000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-125_MFF-25_CTau-15000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-125_MFF-25_CTau-1500mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-30000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-250_MFF-120_CTau-10000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-250_MFF-120_CTau-1000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-250_MFF-60_CTau-10000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-250_MFF-60_CTau-1000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-250_MFF-60_CTau-500mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-160_CTau-10000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-160_CTau-1000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-160_CTau-500mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-80_CTau-10000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-80_CTau-1000mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM'],
            ['/HTo2LongLivedTo4b_MH-350_MFF-80_CTau-500mm_TuneCP5_14TeV-pythia8/'+Ht2LLstring+'/MINIAODSIM']

		]

# Dataset: Run2 17 GMSB

        dsGMSB = [ 

            ##['/GMSB_L-100TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-100TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v2/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-100TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-150TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-150TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-150TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-200TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-200TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-200TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-250TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-250TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-250TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-300TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-300TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-300TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-350TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-350TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-350TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-400TeV_Ctau-0_1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-400TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-400TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-500TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-500TeV_Ctau-0p1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-500TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-600TeV_Ctau-0p001cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ##['/GMSB_L-600TeV_Ctau-0p1cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-10000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-1000cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-10cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-1200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-200cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-400cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-600cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM'],
            ['/GMSB_L-600TeV_Ctau-800cm_TuneCP5_13TeV-pythia8/RunIIFall17DRPremix-PU2017_94X_mc2017_realistic_v11-v1/AODSIM']

        ]

# Dataset: GJets HT 18
##DEBUG MODE
        dsGJET = [

            ['/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18RECO-4cores5k_106X_upgrade2018_realistic_v11_L1v1-v2/AODSIM']#,
           # ['/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v2/AODSIM'],
           # ['/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v2/AODSIM'],
           # ['/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v2/AODSIM'],
           # ['/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v2/AODSIM']

		]
	if ds == 'GMSB':
        	runDataset = dsGMSB
	elif ds == 'data':
		runDataset = dsData
	elif ds == 'GJets':	
        	runDataset = dsGJET
        elif ds == 'QCD':
		runDataset = dsQCD 
	elif ds == 'H2LLP24b':
		runDataset = dsHt2LLt4b
	for dataset in runDataset:
                docrab(dataset)

options = getOptions()
run_multi( options.dataset )

