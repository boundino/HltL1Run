#!/bin/bash

## CMSSW_10_0_3 v97.22 with 100X calibs ##

GLOBTAG="100X_upgrade2018_realistic_v11"
SAMPLE="/store/himc/HINPbPbSpring18DR/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV_2018/GEN-SIM-RAW/NoPU_100X_upgrade2018_realistic_v10-v1/100000/0CAE619A-1527-E811-990A-0242AC1C0504.root"
NEVENT=10

cmsDriver.py l1Ntuple_MC -s RAW2DIGI --era=Run2_2018  \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAWSimHcalTP \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleGEN \
    --custom_conditions=HcalChannelQuality_2018_v3.0_mc,HcalChannelQualityRcd,frontier://FrontierProd/CMS_CONDITIONS \
    --conditions=$GLOBTAG -n $NEVENT --mc --no_exec --no_output \
    --filein=$SAMPLE

echo '
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis")
)
process.ADCanalysisPath = cms.Path(process.HFAdcana)
process.schedule.append(process.ADCanalysisPath)
' >> l1Ntuple_MC_RAW2DIGI.py

edmConfigDump l1Ntuple_MC_RAW2DIGI.py >& l1Ntuple_MC_RAW2DIGI_DUMP.py


    # --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_2 \
