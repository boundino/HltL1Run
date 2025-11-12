
- The collection to be used (HCAL digis) is by default only in RAW data if no special `keep` was used.
<pre>
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"), # use ("simHcalUnsuppressedDigis", "HFQIE10DigiCollection") for MC since 2025
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False) # only turn this on if there is or you know how to produce "towerMaker"
)
process.hfadc = cms.Path(process.HFAdcana)
</pre>
