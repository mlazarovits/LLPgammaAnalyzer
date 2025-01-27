import subprocess
import sys
import os

def bash( bashCommand ):
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
	#process = subprocess.Popen(bashCommand.split())
	output, error = process.communicate()
	return output ,error

def bashout( command ):
	output = subprocess.check_output( command, shell=True)
	return output	

def doCommand( command ):
	output = os.system( command )
	return output

mspc = '/store/user/jaking/'
eosll = 'eos root://cmseos.fnal.gov ls '
#srcdir = 'LLPGamma/'
srcdir = 'LLPGamma/GMSB/'
command = eosll+mspc+srcdir
version = ''
date = ''
#command = 'ls'

targdirs = []
subdirlist1 = []
subdirlist2 = []
theFileList = ''
dirls = bashout( command ).splitlines()
#print( dirls )
#print('0000-------------------------------------------------')
#dirselect = 'HTo2LongLivedTo4b'
dirselect = 'GMSB_L'
for line in dirls:
	#print( line )
	if dirselect in line :
		targdirs.append( line )
		#print( line )
#print( targdirs )

#print('1111-------------------------------------------------')
for mydir in targdirs:
	command1 = command+mydir+'/'
	subdir1 = bashout( command1 ).rstrip().splitlines()
	#print( subdir1 )
	#print( mydir+'/'+subdir1+'/' )
	for line in subdir1 : 
		#print( line )
		if version in line : 
			#print( mydir+'/'+line+'/' )
			subdirlist1.append( mydir+'/'+line+'/' )

#print('2222------------------------------------------------' )
for thesubdir in subdirlist1 :
	command2 = command+thesubdir
	subdir2m = bashout( command2 ).rstrip().splitlines()
	for subdir2 in subdir2m :
		if date in subdir2 :
			#print( thesubdir+subdir2+'/0000/' )
			subdirlist2.append(thesubdir+subdir2+'/0000/')
		#for subdir2 in subdirlist2:
		#	command3 = command+subdir2
		#	files = bashout( command3 ).splitlines()
		#	for thefile in files : 
		#		#print( subdir2+thefile )
		#		theFileList +=(subdir2+thefile)

#print('33333------------------------------------------------' )
num = 0
for subdir2 in subdirlist2:
	filename = 'tmp_'+subdir2.split('/')[1]+'.root '
	#print( filename )
	#lists = bashout( "eosls "+mspc+"LLPGamma/"+subdir2 ).rstrip()
	#print( subdir2 )
	haddcommand = "hadd -f "+filename+"`xrdfs root://cmseos.fnal.gov ls -u "+mspc+srcdir+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+mspc+"LLPGamma/"+subdir2+" | grep '\.root'`"
	#haddcommand = "`xrdfs root://cmseos.fnal.gov ls -u "+mspc+"LLPGamma/"+subdir2+"`"
	#haddcommand = "hadd "+filename+lists
	num = num + 1
	preface = str(num)+": "
	print( '---------------------------------------------------' )
	print( preface+haddcommand )
	doCommand( haddcommand )
	print( '---------------------------------------------------' )



	
#print( bashout( 'hadd llpgana_HTo2LongLivedTo4b_t37MC_noele_005_jetht_emf00bc3rh2e_id2pt200nrh5eta15rhe2.root ' + theFileList ) )	
