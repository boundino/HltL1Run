#!/bin/bash

globaltag=103X_dataRun2_Prompt_v3
era=Run2_2018
# filein='file:/afs/cern.ch/work/w/wangj/public/HIZeroBiasReducedFormat/HIRun2018A-v1/FABCF51E-4182-C341-B159-BF5C73482FAA.root' ; rawlabel="rawDataReducedFormat" ;
filein='file:/afs/cern.ch/work/w/wangj/public/HIEmptyBX/HIRun2018A-v1/596BE573-BF9D-6548-A994-46113B0151EB.root' ; rawlabel="rawDataRepacker" ;

config=l1Ntuple_data2018

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec \
    --python_filename=${config}.py -n 10 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_4 \
    --filein=${filein}

# --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleAODRAWEMU \

echo '
process.caloStage1Digis.InputLabel = cms.InputTag("'${rawlabel}'")
process.caloStage2Digis.InputLabel = cms.InputTag("'${rawlabel}'")
process.gmtStage2Digis.InputLabel = cms.InputTag("'${rawlabel}'")
process.gtStage2Digis.InputLabel = cms.InputTag("'${rawlabel}'")
# process.RawToDigi = cms.Sequence(process.L1TRawToDigi+process.siPixelDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis+process.muonCSCDigis+process.muonDTDigis+process.muonRPCDigis+process.castorDigis+process.scalersRawToDigi+process.tcdsDigis+process.onlineMetaDataDigis+process.muonGEMDigis+process.ctppsRawToDigi+process.muonRPCNewDigis)
# process.L1NtupleEMU = cms.Sequence(process.l1EventTree+process.l1CaloTowerEmuTree+process.l1uGTEmuTree)
# process.SimL1EmulatorCore = cms.Sequence(process.SimL1TCalorimeter+process.SimL1TechnicalTriggers+process.SimL1TGlobal)
' >> ${config}.py

echo '
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis")
)
process.hfadc = cms.Path(process.HFAdcana)
' >> ${config}.py

edmConfigDump ${config}.py >& ${config}_DUMP.py

sed -i 's/"rawDataCollector"/"'${rawlabel}'"/g' ${config}_DUMP.py

echo '
import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('"'"'analysis'"'"')

ivars.maxEvents = 10
ivars.outputFile='"'"'L1Ntuple.root'"'"'
ivars.inputFiles='"'"${filein}"'"'
ivars.parseArguments()# get and parse the command line arguments

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(ivars.inputFiles)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(ivars.maxEvents)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(ivars.outputFile)
)' >> ${config}_DUMP.py
