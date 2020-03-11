Ref: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions

```
cmsrel CMSSW_10_3_1
cd CMSSW_10_3_1/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_10_3_1
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v102.2
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TMuon
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TMuon.git L1Trigger/L1TMuon/data
git cms-addpkg L1Trigger/L1TCalorimeter
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

scram b -j 8

git clone https://github.com/boundino/HltL1Run2021.git
cp -r HltL1Run2021/L1/ADC .
cp HltL1Run2021/L1/l1config/PbPb2018/docmsDriver.sh .

scram b -j 4

./docmsDriver.sh

cmsRun l1Ntuple_data2018_DUMP.py
```
