
- The collection to be used (HCAL digis) is usually in RAW data if no special `keep` was used.
<pre>
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("simHcalUnsuppressedDigis","HFQIE10DigiCollection"), # was "hcalDigis" before 2025
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False) # only turn this on when you have or know how to produce "towerMaker"
)
process.hfadc = cms.Path(process.HFAdcana)
</pre>
