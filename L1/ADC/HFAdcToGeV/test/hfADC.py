import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_pp_on_PbPb_approxSiStripClusters_2025_cff import Run3_pp_on_PbPb_approxSiStripClusters_2025

process = cms.Process('HFADC', Run3_pp_on_PbPb_approxSiStripClusters_2025)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1), 
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
        'file:/eos/cms/store/t0streamer/Data/PhysicsHIPhysicsRawPrime0/000/399/002/run399002_ls0289_streamPhysicsHIPhysicsRawPrime0_StorageManager.dat'
    )
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("HFadc.root"))

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '151X_dataRun3_Express_v1', '')

process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False)
)
process.hfadc = cms.Path(process.HFAdcana)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.hfadc,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.Trigger = cms.EDFilter("TriggerResultsFilter",
      triggerConditions = cms.vstring(
        "HLT_HIZeroBias_HighRate_v8"
      ),
      hltResults = cms.InputTag("TriggerResults", "", "HLT"),
      l1tResults = cms.InputTag("gtStage2Digis"),
      l1tIgnoreMask = cms.bool(False ),
      l1techIgnorePrescales = cms.bool(True ),
      daqPartitions = cms.uint32(1),
      throw = cms.bool(True)
)
for path in process.paths:
    getattr(process,path)._seq = process.Trigger * getattr(process,path)._seq

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")
    
# import FWCore.ParameterSet.VarParsing as VarParsing
# ivars = VarParsing.VarParsing('analysis')
# PD = "PhysicsIonPhysics0"
# ivars.register('streamer',
#                 '',
#                 VarParsing.VarParsing.multiplicity.singleton,
#                 VarParsing.VarParsing.varType.string,
#                 "txt")
# ivars.streamer = "/afs/cern.ch/work/w/wangj/RECO2025/CMSSW_15_0_9/src/recoforrun/run2025/list/" + PD + "_393844.txt"
# ivars.outputFile = "adc_" + PD + "_393844_LS151-170_ZB.root"
# # ivars.inputFiles = "file:/eos/cms/store/t0streamer/Data/PhysicsHIPhysicsRawPrime0/000/399/002/run399002_ls0289_streamPhysicsHIPhysicsRawPrime0_StorageManager.dat"
# ivars.parseArguments() # get and parse the command line arguments
# process.source.fileNames = open(ivars.streamer, "r").read().splitlines()
# # process.source.fileNames = ivars.inputFiles
# process.TFileService.fileName = ivars.outputFile
