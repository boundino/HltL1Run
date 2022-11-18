
<pre>
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(True)
)
process.hfadc = cms.Path(process.HFAdcana)
</pre>
