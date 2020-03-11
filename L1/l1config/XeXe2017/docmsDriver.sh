#!/bin/bash

GLOBTAG="94X_dataRun2_v2"
# SAMPLE="file:/afs/cern.ch/work/w/wangj/public/XeXeMB8/B2E50951-92AF-E711-A534-02163E01A2FC.root"
# SAMPLE="root://cms-xrd-global.cern.ch//store/hidata/XeXeRun2017/HIMinimumBias7/RAW/v1/000/304/906/00000/F49A44F8-A4AF-E711-875D-02163E011D08.root"
# SAMPLE="root://cms-xrd-global.cern.ch//store/hidata/XeXeRun2017/HIEmptyBX/RAW/v1/000/304/906/00000/06437993-C0AF-E711-8293-02163E011CB0.root"
SAMPLE="root://cms-xrd-global.cern.ch//store/hidata/XeXeRun2017/HIZeroBias/RAW/v1/000/304/906/00000/4475DC78-B7AF-E711-B624-02163E01423F.root"
OUTPUT="/eos/cms/store/group/phys_heavyions/wangj/temp/L1Ntuple_HIZeroBias_304906.root"
NEVENT=100
TAG=XeXe

##
# rm l1Ntuple_RAW2DIGI*.py*

cmsDriver.py l1Ntuple -s RAW2DIGI --era=Run2_2017_pp_on_XeXe \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAWCalouGT \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU \
    --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo \
    --conditions=$GLOBTAG -n $NEVENT --data --no_exec --no_output \
    --filein=$SAMPLE --fileout=$

edmConfigDump l1Ntuple_RAW2DIGI.py >& l1Ntuple_RAW2DIGI_DUMP_${TAG}.py
sed -ie 's/rawDataCollector/rawDataRepacker/g' l1Ntuple_RAW2DIGI_DUMP_${TAG}.py
# sed -ie '0,/FG_HF_threshold = cms\.uint32(17)/s/FG_HF_threshold = cms\.uint32(17)/FG_HF_threshold = cms.uint32('$TAG')/g' l1Ntuple_RAW2DIGI_DUMP_${TAG}.py
