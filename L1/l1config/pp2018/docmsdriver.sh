#!/bin/bash

cmsDriver.py l1Ntuple -s RAW2DIGI --python_filename=l1Ntuple_RAW2DIGI.py -n 10 --no_output --no_exec --era=Run2_2018 \
    --data --conditions=101X_dataRun2_Prompt_v9 \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleAODRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_2 \
    --filein=file:/afs/cern.ch/work/w/wangj/public/ppZB2018/raw/507C6921-2684-E811-BA3A-02163E01A8DF.root

edmConfigDump l1Ntuple_RAW2DIGI.py >& l1Ntuple_RAW2DIGI_DUMP.py

