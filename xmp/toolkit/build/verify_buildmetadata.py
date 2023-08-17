#!/usr/bin/python

# This script is intended to provide a way for developers to easily 
# validate their *.buildmetadata.xml files after they have made changes 
# to any of these files.

# To use this script, one must have a copy of Python installed.
# Mac OSX machines come with Python pre-installed.
# For windows machines the current release of ActivePython can be downloaded 
# from the Downloads area on http://activestate.com 

# Usage.
# You can run verify_buildmetadata.py on the command-line (or double-click it on a windows machine).
# By default this script will verify files ending in .buildmetadata.xml which exist in the same directory as this script.
# Alternatively, you can provide a list of other directories to scan on the command line.

import sys, os, re
import xml.parsers
import xml.dom.minidom

BUILD_METADATA_FILE_SUFFIX = 'buildmetadata.xml'
SUPPORTED_PLATFORM_NAMES = ['win','mac','linux','solaris','hpux','aix']
SUPPORTED_COMPILER_NAMES = ['msvc8','msvc8x64','msvc8ib','msvc8ibx64',
				'msvc9','msvc9x64','msvc9ib','msvc9ibx64',
				'xcode 2.4.1','xcode 3.1','xcode 3.1.1',
				'gcc',
				'ant','ant_1.6.2','ant_1.7.1']

SCRIPT_DIR = os.path.dirname( __file__ )

def verifyFile( filepath, outputFileObj ):
	if outputFileObj:
		s = '%s : ' % os.path.basename(filepath)
		outputFileObj.write( s )
		outputFileObj.flush()
	
	##############################################################
	# Parse file and verify that xml contents are well-formed.
	##############################################################
	try:
		dom = xml.dom.minidom.parse( filepath )
	except xml.parsers.expat.ExpatError, e:
		if outputFileObj:
			outputFileObj.write( 'XML ERROR: %s\n' % str(e) )
			outputFileObj.flush()
		return False

	##############################################################
	# Verify document element has tagname buildmetadata
	##############################################################
	documentElement = dom.documentElement
	if not documentElement:
		if outputFileObj:
			outputFileObj.write( 'ERROR: No document element found.\n' )
			outputFileObj.flush()
		return False

	if documentElement.tagName != 'buildmetadata':
		if outputFileObj:
			outputFileObj.write( "ERROR: Document element does not have name 'buildmetadata'.\n" )
			outputFileObj.flush()
		return False

	##############################################################
	# Verify there is at least one platform element child of 
	# documentElement and that all such elements have known 
	# platform names. 
	# ALSO verify each platform element has EXACTLY one child element.
	##############################################################
	cnt = 0
	platformErrors = 0
	for c in documentElement.childNodes:
		if c.nodeType == dom.ELEMENT_NODE and c.tagName == 'platform':
			cnt += 1
			platformName = c.getAttribute('name')
			if platformName not in SUPPORTED_PLATFORM_NAMES:
				if outputFileObj:
					outputFileObj.write( 'ERROR: Platform name "%s" is not currently supported.\n' % platformName )
					outputFileObj.flush()
				platformErrors += 1
				
			numChildBuildGroupAndTargetElementsOfPlatformElt = 0
			for c2 in c.childNodes:
				if c2.nodeType == dom.ELEMENT_NODE and c2.tagName in ['buildgroup', 'target']:
					numChildBuildGroupAndTargetElementsOfPlatformElt += 1
			if numChildBuildGroupAndTargetElementsOfPlatformElt != 1:
				if outputFileObj:
					outputFileObj.write( 'ERROR: platform element for "%s" is contains %d buildgroup or target elements (it is required to contain exactly 1).\n' % (platformName, numChildBuildGroupAndTargetElementsOfPlatformElt) )
					outputFileObj.flush()
				platformErrors += 1
				
	if platformErrors:
		return False
				
	if not cnt:
		if outputFileObj:
			outputFileObj.write( 'ERROR: No <platform> elements found as children of document element.\n' )
			outputFileObj.flush()
		return False
	del cnt

	##############################################################################
	# Verify all buildgroup and target elements have unique names and titles.
	##############################################################################
	groupname_dict = {}
	grouptitle_dict = {}
	for e in dom.getElementsByTagName('buildgroup') + dom.getElementsByTagName('target'):
		name = e.getAttribute('name')
		title = e.getAttribute('title')
		if not name.strip():
			if outputFileObj:
				outputFileObj.write( "ERROR: Found buildgroup or target with missing or empty 'name' attribute.\n" )
				outputFileObj.flush()
			return False
		if not title.strip():
			if outputFileObj:
				outputFileObj.write( "ERROR: Found buildgroup or target with missing or empty 'title' attribute.\n" )
				outputFileObj.flush()
			return False
		if name != name.strip():
			if outputFileObj:
				outputFileObj.write( "ERROR: Found buildgroup or target with 'name' attribute containing leading or trailing whitespace.\n" )
				outputFileObj.flush()
			return False
		if title != title.strip():
			if outputFileObj:
				outputFileObj.write( "ERROR: Found buildgroup or target with 'title' attribute containing leading or trailing whitespace.\n" )
				outputFileObj.flush()
			return False
		if re.search( r'\s', name ):
			if outputFileObj:
				outputFileObj.write( "ERROR: Found buildgroup or target with 'name' attribute (\"%s\") containing whitespace. Releng & PK would prefer to have these attributes not contain whitespace." % name )
				outputFileObj.flush()
			return False
		if name in groupname_dict:
			if outputFileObj:
				outputFileObj.write( 'ERROR: Found multiple buildgroup or target elements with the name "%s".\n' % name )
				outputFileObj.flush()
			return False
		groupname_dict[name] = 1
		if title in grouptitle_dict:
			if outputFileObj:
				outputFileObj.write( 'ERROR: Found multiple buildgroup or target elements with the title "%s".\n' % title )
				outputFileObj.flush()
			return False
		grouptitle_dict[title] = 1
	del groupname_dict
	del grouptitle_dict

	###########################################################################################
	# Verify all target elements have a buildtags attribute and that it has either a release 
	# or debug value in its list. If on Windows, make sure it contains either a 32 or 64 value too.
	###########################################################################################
	#for e in dom.getElementsByTagName('target'):
	#	buildtags = e.getAttribute('buildtags')
	#	if not buildtags:
	#		outputFileObj.write( 'ERROR: Found <target> element with no buildtags attribute: name="%s"\n' % e.getAttribute('name') )
	#		outputFileObj.flush()
	#		return False
	#
	#	buildtags_list = buildtags.split(',')
	#	if ('debug' not in buildtags_list) and ('release' not in buildtags_list):
	#		outputFileObj.write( 'ERROR: Found <target> element with buildtags attribute which does not contain "debug" or "release": name="%s"\n' % e.getAttribute('name') )
	#		outputFileObj.flush()
	#		return False
	#
	#	tmp = e
	#	while 1:
	#		if tmp.tagName == 'platform':
	#			break
	#		tmp  = tmp.parentNode
	#		if tmp.nodeType != 1:
	#			outputFileObj.write( 'ERROR: Found <target> element which doees not descend from a <platform> element: name="%s"\n' % e.getAttribute('name') )
	#			outputFileObj.flush()
	#			return False
	#	platform = tmp.getAttribute('name')
	#
	#	if platform == 'win' and ('32' not in buildtags_list) and ('64' not in buildtags_list):
	#		outputFileObj.write( 'ERROR: Found windows <target> element with buildtags attribute which does not contain "32" or "64": name="%s"\n' % e.getAttribute('name') )
	#		outputFileObj.flush()
	#		return False

	###########################################################################################
	# Verify all target elements have a single <compile> and a single <files> child elements.
	###########################################################################################
	for e in dom.getElementsByTagName('target'):
		numCompilerElts = 0
		numFilesElts = 0
		for c in e.childNodes:
			if c.nodeType == dom.ELEMENT_NODE:
				if c.tagName == 'compiler':
					numCompilerElts += 1
					############################################################################
					# <compiler> element verfication
					############################################################################
					name = c.getAttribute('name')
					projectfilelocation = c.getAttribute('projectfilelocation')
					if name not in SUPPORTED_COMPILER_NAMES:
						if outputFileObj:
							outputFileObj.write( 'ERROR: Found <compiler> element with unsupported name attribute value "%s".\n' % name )
							outputFileObj.flush()
						return False
					if not projectfilelocation.strip():
						if outputFileObj:
							outputFileObj.write( "ERROR: Found <compiler> element with missing or empty 'projectfilelocation' attribute.\n" )
							outputFileObj.flush()
						return False
					if projectfilelocation != projectfilelocation.strip():
						if outputFileObj:
							outputFileObj.write( "ERROR: Found <compiler> element with 'projectfilelocation' attribute containing leading or trailing whitespace.\n" )
							outputFileObj.flush()
						return False
					if projectfilelocation[0] != '/':
						if outputFileObj:
							outputFileObj.write( "ERROR: Found <compiler> element with 'projectfilelocation' attribute value which does not being with '/'.\n" )
							outputFileObj.flush()
						return False

				elif c.tagName == 'files':
					numFilesElts += 1
					############################################################################
					# <files> element verfication
					############################################################################
					for f in c.childNodes:
						if f.nodeType == dom.ELEMENT_NODE:
							if f.tagName == 'file':
								location = f.getAttribute('location')
								if not location.strip():
									if outputFileObj:
										outputFileObj.write( "ERROR: Found <file> element with missing or empty 'location' attribute.\n" )
										outputFileObj.flush()
									return False
								if location != location.strip():
									if outputFileObj:
										outputFileObj.write( "ERROR: Found <file> element with 'location' attribute containing leading or trailing whitespace.\n" )
										outputFileObj.flush()
									return False
								if location[0] != '/':
									if outputFileObj:
										outputFileObj.write( "ERROR: Found <file> element with 'location' attribute value which does not being with '/'.\n" )
										outputFileObj.flush()
									return False

		if numCompilerElts == 0:
			if outputFileObj:
				outputFileObj.write( "ERROR: Found <target> element without a <compile> child.\n" )
				outputFileObj.flush()
			return False
		elif numCompilerElts > 1:
			if outputFileObj:
				outputFileObj.write( "ERROR: Found <target> element with multiple <compile> children.\n" )
				outputFileObj.flush()
			return False
		if numFilesElts == 0:
			if outputFileObj:
				outputFileObj.write( "ERROR: Found <target> element without a <files> child.\n" )
				outputFileObj.flush()
			return False
		elif numFilesElts > 1:
			if outputFileObj:
				outputFileObj.write( "ERROR: Found <target> element with multiple <files> children.\n" )
				outputFileObj.flush()
			return False


	if outputFileObj:
		outputFileObj.write( "OK\n" )
		outputFileObj.flush()
	return True

def scanDir( d, outputFileObj ):
	"Returns a 2-tuple of the form (numFilesVerified, numFilesFound)"
	numFilesVerified = 0
	numFilesFound = 0
	for f in os.listdir( d ):
		if f.endswith( BUILD_METADATA_FILE_SUFFIX ):
			numFilesFound += 1
			if verifyFile( os.path.join( d, f ), outputFileObj ):
				numFilesVerified += 1
	return (numFilesVerified, numFilesFound)

def main( pathlist, outputFileObj=None ):
	"Returns a 2-tuple of the form (numFilesVerified, numFilesFound)"
	numFilesVerified = 0
	numFilesFound = 0
	if not pathlist:
		# Default to looking for files in SCRIPT_DIR
		pathlist = [ SCRIPT_DIR ]
	
	if outputFileObj:
		outputFileObj.write('\n')

	for p in pathlist:
		if os.path.isdir(p):
			r = scanDir( p, outputFileObj )
			numFilesVerified += r[0]
			numFilesFound += r[1]
		elif os.path.isfile(p):
			r = verifyFile( p, outputFileObj )
			if r:
				numFilesVerified += 1
			numFilesFound += 1
		else:
			raise Exception('Could not find filepath "%s"' % p)

	if outputFileObj:
		outputFileObj.write( '\n' )
		if numFilesFound == 0:
			outputFileObj.write( 'NO FILES FOUND?\n' )
		elif numFilesFound == numFilesVerified:
			outputFileObj.write( 'ALL FILES VERIFIED OK (at least as far as this tool can tell).\n' )
		else:
			numFilesWithProblems = numFilesFound - numFilesVerified
			outputFileObj.write( 'PROBLEMS FOUND IN %d FILE%s!\n' % (numFilesWithProblems, ['S',''][numFilesWithProblems==1]))

		outputFileObj.flush()

	return (numFilesVerified, numFilesFound)

if __name__ == '__main__':
	quiet_mode = False
	args = sys.argv[1:]
	if args and args[0] == '-q':
		quiet_mode = True
		args = args[1:]
	numFilesFound, numFilesVerified = main( args, sys.stdout )

	if not quiet_mode:
		if sys.platform == 'win32':
			print "\nPress Enter To Finish..."
			sys.stdin.readline()
