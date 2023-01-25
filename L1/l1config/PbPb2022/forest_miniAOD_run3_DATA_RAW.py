# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: recoFromRAW --conditions auto:run3_data_prompt -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent RECO --process reRECO --scenario pp -n 10 --repacked --nThreads 8 --no_exec --filein /store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/260/00000/6bbafa49-d777-4d0e-b1fe-0985fcdab96f.root --era Run3_pp_on_PbPb
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_pp_on_PbPb_cff import Run3_pp_on_PbPb

process = cms.Process('reRECO',Run3_pp_on_PbPb)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/3b4858f8-deea-4ffa-8d53-3e2db6f57f9f.root',
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('recoFromRAW nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)


from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
# process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_HIMinimumBias_v2"])
process.hltfilter = hltHighLevel.clone(HLTPaths = ["HLT_HIZeroBias_v4"])                                                                                                                             
process.hltfilter.throw = False

process.primaryVertexFilter = cms.EDFilter("VertexSelector",              
										   src = cms.InputTag("offlinePrimaryVertices"),
										   cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
										   filter = cms.bool(True),   # otherwise it won't filter the events
)
                       
# process.superFilterPath = cms.Path(process.hltfilter*process.primaryVertexFilter)      
process.filterSequence = cms.Sequence(process.hltfilter*process.primaryVertexFilter)

# Output definition
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("adc_recoFromRAW_RAW2DIGI.root"))
                                   # fileName = cms.string("/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2022/HiForestMiniAOD_362294_100k_ZB.root"))

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_prompt', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
                                  digiLabel = cms.untracked.InputTag("hcalDigis"),
                                  minimized = cms.untracked.bool(True),
                                  fillhf = cms.bool(True),
)
process.hfadc = cms.Path(process.HFAdcana)

process.endjob_step = cms.EndPath(process.endOfProcess)
# process.RAWoutput_step = cms.EndPath(process.RAWoutput)

# Schedule definition
# process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.superFilterPath,process.hfadc,process.endjob_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.hfadc,process.endjob_step)

for path in process.paths:
    getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
# process.options.numberOfThreads = 8
# process.options.numberOfStreams = 0

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")

# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
