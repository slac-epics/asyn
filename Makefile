#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS += configure
DIRS += makeSupport

DIRS += asyn
asyn_DEPEND_DIRS = configure
DIRS += testApp
testApp_DEPEND_DIRS = asyn
DIRS += testGpibApp
testGpibApp_DEPEND_DIRS = testApp
DIRS += testGpibSerialApp
testGpibSerialApp_DEPEND_DIRS = testApp testGpibApp
DIRS += testEpicsApp
testEpicsApp_DEPEND_DIRS = testApp
DIRS += testManagerApp
testManagerApp_DEPEND_DIRS = asyn
DIRS += testAsynPortDriverApp
testAsynPortDriverApp_DEPEND_DIRS = asyn
DIRS += testErrorsApp
testErrorsApp_DEPEND_DIRS = asyn
DIRS += testUsbtmcApp
testUsbtmcApp_DEPEND_DIRS = asyn

ifdef SNCSEQ
DIRS += testIPServerApp
testIPServerApp_DEPEND_DIRS = asyn
endif

DIRS += iocBoot
iocBoot_DEPEND_DIRS = testApp testGpibApp testGpibSerialApp testEpicsApp testManagerApp
ifdef SNCSEQ
iocBoot_DEPEND_DIRS += testIPServerApp
endif

# We don't try to build asyn documentation here, so we added the
# html distriubution directory to our version control
# Thus, we don't want to try and install or uninstall html
# This keeps the distclean target from removing it.
INSTALL_HTML	=

include $(TOP)/configure/RULES_TOP

