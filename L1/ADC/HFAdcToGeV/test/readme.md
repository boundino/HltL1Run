- Test minimalist config
Save max HF signal ADC on both sides (`mMaxL1HFAdcPlus`, `mMaxL1HFAdcMinus`), with an HLT path filtered.
<pre>
cmsrel CMSSW_15_0_9_patch2
cd CMSSW_15_0_9_patch2/src
cmsenv
git clone https://github.com/boundino/HltL1Run.git
ln -s HltL1Run/L1/ADC/ .
scram b -j4
ln -s HltL1Run/L1/ADC/HFAdcToGeV/test/HFadc.py .
cmsRun HFadc.py
</pre>
