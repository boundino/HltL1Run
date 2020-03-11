#!/bin/bash

globaltag=103X_dataRun2_HLT_v1
era=Run2_2018_pp_on_AA
file='file:/afs/cern.ch/work/w/wangj/public/PbPb2018MCMB/RAW/FE243EFA-6DAB-E811-BFDE-9CB654AD72EC.root'
input='"file:/eos/cms/store/t0streamer/Data/HIPhysicsMinimumBias0/000/326/382/run326382_ls0021_streamHIPhysicsMinimumBias0_StorageManager.dat"'

config=l1Ntuple_RAW2DIGI_RAW

cmsDriver.py l1Ntuple -s RAW2DIGI \
    -n 10 \
    --no_output --no_exec \
    --era=$era \
    --data --conditions=$globaltag \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --filein=$file --python_filename=${config}.py

echo '
process.caloStage1Digis.InputLabel = cms.InputTag("rawDataRepacker")
process.caloStage2Digis.InputLabel = cms.InputTag("rawDataRepacker")
process.gmtStage2Digis.InputLabel = cms.InputTag("rawDataRepacker")
process.gtStage2Digis.InputLabel = cms.InputTag("rawDataRepacker")
process.RawToDigi = cms.Sequence(process.L1TRawToDigi+process.siPixelDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis+process.muonCSCDigis+process.muonDTDigis+process.muonRPCDigis+process.castorDigis+process.scalersRawToDigi+process.tcdsDigis+process.onlineMetaDataDigis+process.muonGEMDigis+process.ctppsRawToDigi+process.muonRPCNewDigis)
process.L1NtupleEMU = cms.Sequence(process.l1EventTree+process.l1CaloTowerEmuTree+process.l1uGTEmuTree)
process.SimL1EmulatorCore = cms.Sequence(process.SimL1TCalorimeter+process.SimL1TechnicalTriggers+process.SimL1TGlobal)
' >> ${config}.py

echo '
process.source = cms.Source(
    "NewEventStreamFileReader",
    fileNames = cms.untracked.vstring('${input}'
    ),
)
' >> ${config}.py

echo '
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis")
)
process.hfadc = cms.Path(process.HFAdcana)
' >> ${config}.py

edmConfigDump ${config}.py >& ${config}_DUMP.py

sed -i 's/"rawDataCollector"/"rawDataRepacker"/g' ${config}_DUMP.py

echo '
import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('"'"'analysis'"'"')

ivars.maxEvents = 10
ivars.outputFile='"'"'L1Ntuple.root'"'"'
ivars.inputFiles='"'"'file:/eos/cms/store/t0streamer/Data/HIPhysicsMinimumBias0/000/326/382/run326382_ls0021_streamHIPhysicsMinimumBias0_StorageManager.dat'"'"'
ivars.parseArguments()# get and parse the command line arguments

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(ivars.inputFiles)
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(ivars.maxEvents)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(ivars.outputFile)
)' >> ${config}_DUMP.py

