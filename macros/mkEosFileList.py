import subprocess
import sys
import os
import argparse

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
parser.add_argument('--version','-v',help='version of ntuples to pull',required=True)
args = parser.parse_args()


mspc = '/store/user/jaking/'
mdis = '/store/user/lpcsusylep/jaking/'
eosll = 'eos root://cmseos.fnal.gov ls '
#command = eosll+mspc+'LLPGamma/llpga_GMSB_AOD_v48/'
#command = eosll+mspc+'A/'
command = eosll+mdis+'LLPGamma/llpga_GMSB_AOD_v'+args.version+'/'
version = ''
rootfile = '.root'
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
	#print( line )
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

if debug : print( subdirlist1 )
for thesubdir in subdirlist1 :
	command2 = command+thesubdir+'/'
	subdir2 = bashout( command2 ).rstrip().splitlines()
	#print( thesubdir+subdir2+'/0000/' )
	for subdir in subdir2 : 
		command3 = command+thesubdir+subdir+'/'
		subdir3 = bashout( command3 ).rstrip().splitlines()
		for subsubdir in subdir3 : 
			subdirlist2.append(thesubdir+subdir+'/'+subsubdir+'/0000/')


if debug : print( subdirlist2 )
for subdir2 in subdirlist2:
	lists = bashout( command+subdir2 ).rstrip().splitlines()
	for line in lists :
		if rootfile in line : filelist.append(subdir2+line)

for thefile in filelist:
	print( thefile )

	#filename = 'tmp_'+subdir2.split('/')[1]+'.root '
	#print( filename )
	#lists = bashout( "eosls "+mspc+"LLPGamma/"+subdir2 ).rstrip()
	#print( subdir2 )
	#haddcommand = "hadd -f "+filename+"`xrdfs root://cmseos.fnal.gov ls -u "+mspc+"LLPGamma/"+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+mspc+"LLPGamma/"+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+mspc+"LLPGamma/"+subdir2+"`"
	#haddcommand = "hadd "+filename+lists
	#print( mspc+"LLPGamma/"+subdir2 )
	#print( haddcommand )
	#doCommand( haddcommand )
	#print( '---------------------------------------------------' )

	
#print( bashout( 'hadd llpgana_HTo2LongLivedTo4b_t37MC_noele_005_jetht_emf00bc3rh2e_id2pt200nrh5eta15rhe2.root ' + theFileList ) )	
