import FWCore.ParameterSet.Config as cms

def customise(process):
    if hasattr(process,'DigiToRaw'):
        process=customise_DigiToRaw(process)
    if hasattr(process,'RawToDigi'):
        process=customise_RawToDigi(process)
    if hasattr(process,'reconstruction'):
        process=customise_Reco(process)
    if hasattr(process,'L1simulation_step'):
       process=customise_L1Emulator(process)
    if hasattr(process,'digitisation_step'):
        process=customise_Digi(process)
    if hasattr(process,'dqmoffline_step'):
        process=customise_DQM(process)
    if hasattr(process,'dqmHarvesting'):
        process=customise_harvesting(process)
    elif hasattr(process,'validation_step') :
        process=customise_Validation(process)
    return process

def customise_DigiToRaw(process):
    return process

def customise_RawToDigi(process):
    return process

def customise_Digi(process):
    process.RandomNumberGeneratorService.simMuonGEMDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('HepJamesRandom')
        )
    
    process.mix.mixObjects.mixSH.crossingFrames.append('MuonGEMHits')
    process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MuonGEMHits"))
    process.mix.mixObjects.mixSH.subdets.append('MuonGEMHits')
    
    process.load('SimMuon.GEMDigitizer.muonGEMDigis_cfi')
    process.load('SimMuon.GEMDigitizer.muonGEMCSCPadDigis_cfi')
    process.muonDigi += process.simMuonGEMDigis
    process.muonDigi += process.simMuonGEMCSCPadDigis

    process=outputCustoms(process)
    return process

def customise_L1Emulator(process):
    process.simCscTriggerPrimitiveDigis.gemPadProducer =  cms.untracked.InputTag("simMuonGEMCSCPadDigis","")
    process.simCscTriggerPrimitiveDigis.clctSLHC.clctPidThreshPretrig = 2
    process.simCscTriggerPrimitiveDigis.clctParam07.clctPidThreshPretrig = 2
    tmb = process.simCscTriggerPrimitiveDigis.tmbSLHC
    tmb.doGemMatching = cms.untracked.bool(True)
    tmb.gemMatchDeltaEta = cms.untracked.double(0.08)
    tmb.gemMatchDeltaBX = cms.untracked.int32(1)
    lct_store_gemdphi = True
    if lct_store_gemdphi:
        tmb.gemClearNomatchLCTs = cms.untracked.bool(False) 
	tmb.gemMatchDeltaPhiOdd = cms.untracked.double(2.)
        tmb.gemMatchDeltaPhiEven = cms.untracked.double(2.)
    return process

def customise_DQM(process):
    return process

def customise_Validation(process):
    print "Loading gem digi validation module from SLHCUpgradeSimulations.Configuration.gemCustoms.py"
    process.load('Validation.MuonGEMHits.MuonGEMHits_cfi')
    process.load('Validation.MuonGEMDigis.MuonGEMDigis_cfi')
    process.genvalid_all += cms.Sequence( process.gemHitsValidation*process.gemDigiValidation)
    return process


def customise_harvesting(process):
    print "Loading gem digi harvesting module from SLHCUpgradeSimulations.Configuration.gemCustoms.py"
    process.load('Validation.MuonGEMHits.PostProcessor_cff')
    process.load('Validation.MuonGEMDigis.PostProcessor_cff')
    process.genHarvesting += process.MuonGEMHitsPostProcessors
    process.genHarvesting += process.MuonGEMDigisPostProcessors
    return (process)        

def customise_Reco(process):
    process.load('RecoLocalMuon.GEMRecHit.gemRecHits_cfi')
    process.gemRecHits.gemDigiLabel = cms.InputTag("simMuonGEMDigis")
    process.muonlocalreco += process.gemRecHits
    process=outputCustoms(process)
    process.standAloneMuons.STATrajBuilderParameters.EnableGEMMeasurement = cms.bool(True)
    process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.EnableGEMMeasurement = cms.bool(True)
    return process


def outputCustoms(process):

    alist=['AODSIM','RECOSIM','FEVTSIM','FEVTDEBUG','FEVTDEBUGHLT','RECODEBUG','RAWRECOSIMHLT','RAWRECODEBUGHLT']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            getattr(process,b).outputCommands.append('keep *_simMuonGEMDigis_*_*')
            getattr(process,b).outputCommands.append('keep *_simMuonGEMCSCPadDigis_*_*')
            getattr(process,b).outputCommands.append('keep *_gemRecHits_*_*')

    return process

