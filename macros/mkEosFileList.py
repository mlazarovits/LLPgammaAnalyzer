import subprocess
import sys
import os
import argparse
# uses python3

def bash( bashCommand ):
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
	#process = subprocess.Popen(bashCommand.split())
	output, error = process.communicate()
	return output ,error

def bashout( command ):
	#output = subprocess.check_output( command, shell=True)
	output = subprocess.run( command, shell=True, check=True, stdout=subprocess.PIPE, universal_newlines=True )
	return output.stdout	

def doCommand( command ):
	output = os.system( command )
	return output

parser = argparse.ArgumentParser()
parser.add_argument('-p','--path',help='which eos space to use: user or group',default='user',choices=['user','group'])
parser.add_argument('-s','--sample',help='which sample to use',default='JetHT')
args = parser.parse_args()

#m_user = '/store/user/jaking/'
#m_group = '/store/user/lpcsusylep/jaking/'
m_user = '/store/user/malazaro/LLPGamma/crab_ntuples/'
m_group = '/store/user/lpcsusylep/malazaro/LLPGamma/'

eosll = 'eos root://cmseos.fnal.gov ls '
eospwd = 'root://cmseos.fnal.gov/'
#command = eosll+m_group+'LLPGamma/llpga_GMSB_AOD_v59/'
command = eosll
if args.path == 'user':
	command += m_user
	eospwd += m_user
elif args.path == 'group':
	command += m_group
	eospwd += m_group
else:
	print("Please specify either user or group eos path.")
	exit()

jetHT = "llpga_JetHT_Run2018A-UL2018_MiniAODv2-v1_MINIAOD/"
GMSB = "llpga_GMSB_AOD_v59/"
sample = ''
if args.sample == "JetHT":
	sample = jetHT 
elif args.sample == "GMSB":
	sample = GMSB
else:
	print("Please specify valid sample or add to script.")
	exit()

command += sample
fileName = "/uscms/home/mlazarov/nobackup/CMSSW_10_6_5/src/PVTiming/LLPgammaAnalyzer/fileLists/"+sample[:-1]+"_"+args.path+".txt"


#command = eosll+m_user+'ecalTiming/'
#command = eosll+m_group+'ecalTiming/EGamma/'
#command = eosll+m_user+'EGamma/'
version = ''
#version = '_v11_'
#version = '_noOOTAmp_'
#version = '_wthOOTAmp_'
rootfile = '.root'
#dirselect = 'HTo2LongLivedTo4b'
#dirselect = '_newRtParams4_v26b_'
#dirselect = '_newRtParams3_test_v26_'
#dirselect = 'tt_kurhs_124cc5_cert'
#dirselect = '22eraC_EGamma_MINIAOD_Run2022C-PromptReco-v1_357328-357331'
#dirselect = 'noOOTCC_kustc0_EGamma_MINIAOD_Run2022C-PromptReco-v1_357328-357331'
#dirselect = '22eraC_CCstc0_EGamma_MINIAOD_Run2022C-PromptReco-v1_357328-357331'
#dirselect = 'noOOTCC_kustc0_EGamma_MINIAOD_Run2022C-PromptReco-v1_357101-357268'
#dirselect = 'CCstc0_EGamma_MINIAOD_Run2022C-PromptReco-v1_357101-357268'
dirselect = ''

#debug = True
debug = False

#deep = True
deep = False

targdirs = []
subdirlist1 = []
subdirlist2 = []
subdirlist3 = []
filelist = []
theFileList = ''

dirls = bashout( command ).splitlines()
if debug : print( '-------------------------------------------------')
for line in dirls:
	if dirselect in line : targdirs.append( line )
if debug : print( targdirs )
if deep :
	for mydir in targdirs:
		command1 = command+mydir+'/'
		subdir1 = bashout( command1 ).rstrip().splitlines()
		#print( subdir1 )
		#print( mydir+'/'+subdir1+'/' )
		for line in subdir1 : 
			#print( line )
			if version in line : 
				subdirlist1.append( mydir+'/'+line+'/' )
		#print( subdirlist1 )
else : 
	for mydir in targdirs:
		subdirlist1.append( mydir+'/' )

if debug : print("subdirlist1", subdirlist1 )
for thesubdir in subdirlist1 :
	command2 = command+thesubdir+'/'
	subdir2 = bashout( command2 ).rstrip().splitlines()
	#print( thesubdir+subdir2+'/0000/' )
	for subdir in subdir2 : 
		command3 = command+thesubdir+subdir+'/'
		subdir3 = bashout( command3 ).rstrip().splitlines()
		for subsubdir in subdir3 : 
			subdirlist2.append(thesubdir+subdir+'/'+subsubdir+'/0000/')

if debug : print("subdirlist2", subdirlist2 )
for subdir2 in subdirlist2:
	lists = bashout( command+subdir2 ).rstrip().splitlines()
	for line in lists :
		if rootfile in line : filelist.append(subdir2+line)

f = open(fileName,"x")
for thefile in filelist:
	f.write(eospwd+sample+thefile)	
	f.write("\n")
	#print( thefile )
print("Wrote files to:",fileName)
	#filename = 'tmp_'+subdir2.split('/')[1]+'.root '
	#print( filename )
	#lists = bashout( "eosls "+m_user+"LLPGamma/"+subdir2 ).rstrip()
	#print( subdir2 )
	#haddcommand = "hadd -f "+filename+"`xrdfs root://cmseos.fnal.gov ls -u "+m_user+"LLPGamma/"+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+m_user+"LLPGamma/"+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+m_user+"LLPGamma/"+subdir2+"`"
	#haddcommand = "hadd "+filename+lists
	#print( m_user+"LLPGamma/"+subdir2 )
	#print( haddcommand )
	#doCommand( haddcommand )
	#print( '---------------------------------------------------' )

	
#print( bashout( 'hadd llpgana_HTo2LongLivedTo4b_t37MC_noele_005_jetht_emf00bc3rh2e_id2pt200nrh5eta15rhe2.root ' + theFileList ) )	
