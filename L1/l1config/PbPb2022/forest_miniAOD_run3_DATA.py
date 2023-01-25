### HiForest Configuration
# Input: miniAOD
# Type: data

import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('HiForest',Run3)

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 125X, data")

# import subprocess, os
# version = subprocess.check_output(
#     ['git', '-C', os.path.expandvars('$CMSSW_BASE/src'), 'describe', '--tags'])
# if version == '':
#     version = 'no git info'
# process.HiForestInfo.HiForestVersion = cms.string(version)

###############################################################################

# input files
process.source = cms.Source("PoolSource",
							duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
							fileNames = cms.untracked.vstring(
								# '/store/group/phys_heavyions/mnguyen/HIRun2022A/RecoPatfromRaw_RAW2DIGI_L1Reco_RECO_PAT_inMINIAOD.root',
								'/store/group/phys_heavyions/mnguyen/HIRun2022A/RecoPatfromRawPrime_RAW2DIGI_L1Reco_RECO_PAT_inMINIAOD_100k.root'
							),
							secondaryFileNames = cms.untracked.vstring(
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/29fe85b5-9ab6-4640-8fa4-4d110c8c9bdb.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/3b4858f8-deea-4ffa-8d53-3e2db6f57f9f.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/f9f56912-4b9e-4da3-995b-bfd25f64256b.root',
								# '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/fee04e5c-a62c-46c0-a177-5e0a2a4dc02d.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/03ff59ee-4b5f-4726-8f12-b1bacec1667d.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/0ea79907-85a4-415b-915a-58ab2771a224.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/31fee8ef-acfd-438d-a40b-9e99ef1ff906.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/41a0deed-b66e-482b-84e2-0b8443b31140.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/76b61dd4-f4be-4bc6-afbb-956bb31b0f72.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/8012f4ef-02a6-44fa-9264-72039a97bc5c.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/91d3a6e0-703b-408b-81b1-6792d727e16e.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/a2bcc43f-f722-440a-bf82-804afbbc31f1.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/aef86815-4230-497c-bdc3-3f5313e61248.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/bfe72ab5-2eb8-496f-b2ca-f89ec8d1153c.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/e3c86ea3-61f8-4c41-9f58-13315a12efa1.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/e6d89c69-5016-4e0c-93f7-dcf7f30ea912.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/f9096b63-d8cf-42d6-b047-716090d3ec3f.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fbaef8bc-f55d-4445-a0f4-f6e66ae8323e.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fc283f2b-100e-45e3-a853-761711004eed.root',
								'/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/294/00000/fc2f7f9c-d67a-4fd8-aba3-49e47cffa191.root'
							),
                            skipEvents = cms.untracked.uint32(0)
)

# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
)

###############################################################################

# load Global Tag, geometry, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
# process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '124X_dataRun3_Prompt_v9', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

#centralityTag = "CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1031x02_offline"
#process.HiForestInfo.info.append(centralityTag)
#
#print('\n')
#print('\033[31m~*~ CENTRALITY TABLE FOR 2018 PBPB DATA ~*~\033[0m')
#print('\033[36m~*~ TAG: ' + centralityTag + ' ~*~\033[0m')
#print('\n')
#process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
#process.GlobalTag.toGet.extend([
#    cms.PSet(
#        record = cms.string("HeavyIonRcd"),
#        tag = cms.string(centralityTag),
#        label = cms.untracked.string("HFtowers"),
#        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
#        ),
#    ])
#
#process.GlobalTag.toGet.extend([
#    cms.PSet(
#        record = cms.string("BTagTrackProbability3DRcd"),
#        tag = cms.string("JPcalib_Data103X_2018PbPb_v1"),
#        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
#        )
#    ])
#
###############################################################################

# root output
process.TFileService = cms.Service("TFileService",
								   fileName = cms.string("/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2022/HiForestMiniAOD_362294_100k_ZB.root"))

# # edm output for debugging purposes
# process.output = cms.OutputModule(
#     "PoolOutputModule",
#     fileName = cms.untracked.string('HiForestEDM.root'),
#     outputCommands = cms.untracked.vstring(
#         'keep *',
#         )
#     )

# process.output_path = cms.EndPath(process.output)

###############################################################################

# event analysis
# process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
#process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
#process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
#process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

#from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_mc
#process.hltobject.triggerNames = trigger_list_mc

process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
################################
# electrons, photons, muons
#process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
################################
# jet reco sequence
#process.load('HeavyIonsAnalysis.JetAnalysis.akCs4PFJetSequence_pponPbPb_data_cff')
################################
# tracks
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")
###############################################################################



###############################################################################
# main forest sequence
process.forest = cms.Path(
	process.HiForestInfo +
	# process.hltanalysis +
	# process.trackSequencePbPb +
	# process.particleFlowAnalyser +
	process.hiEvtAnalyzer #+
	#process.ggHiNtuplizer #+
	#   process.akCs4PFJetAnalyzer
)

#customisation



#########################
# Event Selection -> add the needed filters here
#########################

process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.pAna = cms.EndPath(process.skimanalysis)

process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load("Configuration.StandardSequences.Reconstruction_Data_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.raw2digi_step = cms.Path(process.RawToDigi)
# process.endjob_step = cms.EndPath(process.endOfProcess)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
								  digiLabel = cms.untracked.InputTag("hcalDigis"),
								  minimized = cms.untracked.bool(True),
								  fillhf = cms.bool(True),
)
process.hfadc = cms.Path(process.HFAdcana)

process.recotowers = cms.Path(
	process.bunchSpacingProducer *
	process.calolocalreco *
	process.hcalGlobalRecoSequence *
	process.caloTowersRec
)

# Schedule definition
# process.schedule = cms.Schedule(process.forest,process.raw2digi_step,process.recotowers,process.hfadc,process.endjob_step)
# process.schedule = cms.Schedule(process.forest,process.raw2digi_step,process.recotowers,process.hfadc,process.pAna)

# from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
# associatePatAlgosToolsTask(process)


import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
process.hltfilter = hlt.hltHighLevel.clone(
    HLTPaths = ['HLT_HIZeroBias_v4']
    # HLTPaths = ['HLT_HIMinimumBias_v2']
)
process.filterSequence = cms.Sequence(process.hltfilter)
process.superFilterPath = cms.Path(process.filterSequence)
process.skimanalysis.superFilters = cms.vstring("superFilterPath")
for path in process.paths:
    getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
# MassReplaceInputTag(process, old="rawDataCollector", new="rawPrimeDataRepacker")
MassReplaceInputTag(process, old="rawDataCollector", new="rawDataRepacker")
