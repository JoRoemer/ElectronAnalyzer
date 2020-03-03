import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoAnalyzer")
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = '94X_dataRun2_ReReco_EOY17_v6'

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:/user/roemer/for_wiedenbeck/wprime/reco/CMSSW_10_2_5_patch1/src/reco.root'),
                            #fileNames = cms.untracked.vstring('file:/user/roemer/for_wiedenbeck/wprime/CMSSW_10_2_5_patch1/src/reco.root')
                            )
  
from FWCore.MessageLogger.MessageLogger_cfi import *

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) # These are the number of events to cycle through in the input root file
)

# Load everything here instead of using ISpy_Producer so that we can
# explicitly see what is running and change parameters easily in one
# place if needed

#process.ISpyCSCRecHit2D.iSpyCSCRecHit2DTag = cms.InputTag("csc2DRecHits")
#
#process.ISpyCSCSegment.iSpyCSCSegmentTag = cms.InputTag("cscSegments")
#process.ISpyCSCStripDigi.iSpyCSCStripDigiTag = cms.InputTag('muonCSCDigis:MuonCSCStripDigi')
#process.ISpyCSCStripDigi.thresholdOffset = cms.int32(9)
#process.ISpyCSCWireDigi.iSpyCSCWireDigiTag = cms.InputTag('muonCSCDigis:MuonCSCWireDigi')
#
#process.ISpyCSCCorrelatedLCTDigi.iSpyCSCCorrelatedLCTDigiTag = cms.InputTag('muonCSCDigis:MuonCSCCorrelatedLCTDigi')
#
#process.ISpyDTDigi.iSpyDTDigiTag = cms.InputTag('muonDTDigis')
#process.ISpyDTRecHit.iSpyDTRecHitTag = cms.InputTag('dt1DRecHits')
#process.ISpyDTRecSegment4D.iSpyDTRecSegment4DTag = cms.InputTag('dt4DSegments')
#
##process.ISpyEBRecHit.iSpyEBRecHitTag = cms.InputTag('ecalRecHit:EcalRecHitsEB')
##process.ISpyEERecHit.iSpyEERecHitTag = cms.InputTag('ecalRecHit:EcalRecHitsEE')
##process.ISpyESRecHit.iSpyESRecHitTag = cms.InputTag('ecalPreshowerRecHit:EcalRecHitsES')
#process.ISpyEBRecHit.iSpyEBRecHitTag = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
#process.ISpyEERecHit.iSpyEERecHitTag = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#process.ISpyESRecHit.iSpyESRecHitTag = cms.InputTag("reducedEcalRecHitsES", "", "RECO")

process.RecoAnalyzer = cms.EDAnalyzer("RecoAnalyzer")

process.RecoAnalyzer.electronTag = cms.InputTag('gedGsfElectrons')
process.RecoAnalyzer.ecalEbRecHitTag = cms.InputTag('reducedEcalRecHitsEB')

#process.ISpyHBRecHit.iSpyHBRecHitTag = cms.InputTag("reducedHcalRecHits", "hbhereco", "RECO")
#process.ISpyHERecHit.iSpyHERecHitTag = cms.InputTag("reducedHcalRecHits", "hbhereco", "RECO")
#process.ISpyHFRecHit.iSpyHFRecHitTag = cms.InputTag("reducedHcalRecHits", "hfreco", "RECO")
#process.ISpyHORecHit.iSpyHORecHitTag = cms.InputTag("reducedHcalRecHits", "horeco", "RECO")
#
#process.ISpyJet.iSpyJetTag = cms.InputTag("iterativeCone5CaloJets")
#
#process.ISpyL1GlobalTriggerReadoutRecord.iSpyL1GlobalTriggerReadoutRecordTag = cms.InputTag("gtDigis")
#
#process.ISpyMET.iSpyMETTag = cms.InputTag("htMetIC5")
#
#process.ISpyMuon.iSpyMuonTag = cms.InputTag("muons")
#
#process.ISpyPFJet.iSpyPFJetTag = cms.InputTag('ak4PFJetsCHS', '', 'RECO')
#
#process.ISpyPhoton.iSpyPhotonTag = cms.InputTag('photons')
#
#process.ISpyPixelDigi.iSpyPixelDigiTag = cms.InputTag("siPixelDigis")
#
#process.ISpyPreshowerCluster.iSpyPreshowerClusterTags = cms.VInputTag(cms.InputTag('multi5x5SuperClustersWithPreshower:preshowerXClusters'), cms.InputTag('multi5x5SuperClustersWithPreshower:preshowerYClusters'))
#
#process.ISpyRPCRecHit.iSpyRPCRecHitTag = cms.InputTag("rpcRecHits")
#
#process.ISpySiPixelCluster.iSpySiPixelClusterTag = cms.InputTag("siPixelClusters", "", "RECO")
#process.ISpySiPixelRecHit.iSpySiPixelRecHitTag = cms.InputTag("siPixelRecHits", "", "RECO")
#process.ISpySiStripCluster.iSpySiStripClusterTag = cms.InputTag("siStripClusters")
#process.ISpySiStripDigi.iSpySiStripDigiTag = cms.InputTag("siStripDigis:ZeroSuppressed")
#
#process.ISpySuperCluster.iSpySuperClusterTag = cms.InputTag('hybridSuperClusters')
#
#process.ISpyTrack.iSpyTrackTag = cms.InputTag("generalTracks")
#process.ISpyTrackingRecHit.iSpyTrackingRecHitTag = cms.InputTag("generalTracks")
#
#process.ISpyTriggerEvent.triggerEventTag = cms.InputTag('hltTriggerSummaryAOD')
#process.ISpyTriggerEvent.triggerResultsTag = cms.InputTag('TriggerResults')
#process.ISpyTriggerEvent.processName = cms.string('HLT')
#
#process.ISpyVertex.iSpyVertexTag = cms.InputTag('offlinePrimaryVertices')

process.Analyzer = cms.Path(process.RecoAnalyzer)
#                        process.ISpyCSCRecHit2D*
#                        process.ISpyCSCSegment*
#                        process.ISpyCSCStripDigi*
#                        process.ISpyCSCWireDigi*
#                        process.ISpyCSCCorrelatedLCTDigi*
#                        process.ISpyDTDigi*
#                        process.ISpyDTRecHit*
#                        process.ISpyDTRecSegment4D*
#                        process.ISpyEBRecHit*
#                        process.ISpyEERecHit*
#                        process.ISpyESRecHit*
#                        process.ISpyGsfElectron*
#                        process.ISpyHBRecHit*
#                        process.ISpyHERecHit*
#                        process.ISpyHFRecHit*
#                        process.ISpyHORecHit*
#                        process.ISpyJet*
#                        process.ISpyMET*
#                        process.ISpyMuon*
#                        process.ISpyPFJet*
#                        process.ISpyPhoton*
#                        process.ISpyPixelDigi*
#                        process.ISpySiPixelCluster*
#                        process.ISpySiPixelRecHit*
#                        process.ISpySiStripCluster*
#                        process.ISpySiStripDigi*
#                        process.ISpySuperCluster*
#                        process.ISpyTrack*
#                        process.ISpyTrackingRecHit*
#                        process.ISpyVertex)

process.schedule = cms.Schedule(process.Analyzer)

