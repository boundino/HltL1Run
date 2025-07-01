import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY

process = cms.Process('HFADC', Run3_2025_OXY)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring('file:/eos/cms/store/t0streamer/Data/PhysicsIonPhysics0/000/393/767/run393767_ls0021_streamPhysicsIonPhysics0_StorageManager.dat')
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("HFadc.root"))

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '150X_dataRun3_Prompt_v3', '')

process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    # digilabel = cms.untracked.InputTag("simHcalUnsuppressedDigis","HFQIE10DigiCollection"), # MC starting from 2025
    digiLabel = cms.untracked.InputTag("hcalDigis"), # Data and MC before 2025
    minimized = cms.untracked.bool(False),
    fillhf = cms.bool(False) # only turn this on when you know how to produce "towerMaker"
)
process.hfadc = cms.Path(process.HFAdcana)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.hfadc,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# process.Trigger = cms.EDFilter( "TriggerResultsFilter",
#       triggerConditions = cms.vstring(
#         "HLT_HIZeroBias_v8"
#          ),
#       hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
#       l1tResults = cms.InputTag( "gtStage2Digis" ),
#       l1tIgnoreMask = cms.bool( False ),
#       l1techIgnorePrescales = cms.bool( True ),
#       daqPartitions = cms.uint32( 1 ),
#       throw = cms.bool( True )
# )
# for path in process.paths:
#     getattr(process,path)._seq = process.Trigger * getattr(process,path)._seq
    
# from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process, old="rawDataCollector", new="rawDataMapperByLabel")
# MassReplaceInputTag(process, old="rawDataCollector", new="rawPrimeDataRepacker")
# MassReplaceInputTag(process, old="rawDataCollector", new="rawDataRepacker")
# delattr(process, "rawPrimeDataRepacker")

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')
ivars.register('streamer',
                '',
                VarParsing.VarParsing.multiplicity.singleton,
                VarParsing.VarParsing.varType.string,
                "txt")
ivars.streamer = "/afs/cern.ch/work/w/wangj/RECO2023/CMSSW_13_2_4/src/reco2023/list/PhysicsHIPhysicsRawPrime0_374322.txt"
ivars.outputFile = "/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2023/adc/HFadc_r374322_HLT_HIZeroBias_v8.root"
ivars.parseArguments() # get and parse the command line arguments
process.source.fileNames = open(ivars.streamer, "r").read().splitlines()
process.TFileService.fileName = ivars.outputFile
