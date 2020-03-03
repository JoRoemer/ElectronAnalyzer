// -*- C++ -*-
//
// Package:    RecoAnalyzer/RecoAnalyzer
// Class:      RecoAnalyzer
//
/**\class RecoAnalyzer RecoAnalyzer.cc RecoAnalyzer/RecoAnalyzer/plugins/RecoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jonas Roemer
//         Created:  Wed, 29 Jan 2020 10:30:41 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "TH2D.h"
#include "TFile.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;
using reco::GsfElectronCollection;
//using reco::EcalRecHitCollection;

class RecoAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit RecoAnalyzer(const edm::ParameterSet&);
      ~RecoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::string getName(const long event_num, const double pt, const int cluster);

      // ----------member data ---------------------------
      edm::EDGetTokenT<GsfElectronCollection> electronsToken_;
      edm::EDGetTokenT<EcalRecHitCollection> ecalEbRecHitToken_;

      std::map<std::string, TH2D> hists;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RecoAnalyzer::RecoAnalyzer(const edm::ParameterSet& iConfig)
 :
  electronsToken_(consumes<GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electronTag"))),
  ecalEbRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("ecalEbRecHitTag")))

{
   //now do what ever initialization is needed

}


RecoAnalyzer::~RecoAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<GsfElectronCollection> electrons;
   iEvent.getByToken(electronsToken_, electrons);

   edm::Handle<EcalRecHitCollection> ebRecHitCollection;
   iEvent.getByToken(ecalEbRecHitToken_,ebRecHitCollection);

   edm::ESHandle<CaloGeometry> geom;
   iSetup.get<CaloGeometryRecord>().get(geom);
   auto event_num = iEvent.id().event();
   const auto hist_name = "ecal_" + std::to_string(event_num);
   TH2D hist_ecal = TH2D(hist_name.c_str(), ";iphi;ieta;E_T", 360, 0, 360,
                                                              170, -85, 85);
   for (auto hit = ebRecHitCollection->begin(); hit != ebRecHitCollection->end(); ++hit)
   {
      const auto ieta = EBDetId(hit->detid()).ieta();
      const auto iphi = EBDetId(hit->detid()).iphi();
      const auto cell = geom->getGeometry(hit->detid());
      const auto& pos = cell->getPosition();
      const auto eta = pos.eta();
     
      const auto bin = hist_ecal.FindBin(iphi, ieta);
      hist_ecal.SetBinContent(bin, hit->energy() / cosh(eta));
   }
   hists[hist_name] = hist_ecal;

   for (auto electron = electrons->begin();
        electron != electrons->end();
        ++electron)
   {
      std::cout << electron->pt() << " " << electron->eta() << " " << electron->phi() << std::endl;
      auto sc = *(electron->superCluster());
      int cluster_counter = 0;
      for (const auto cluster : sc.clusters())
      {
         // hist stuff 
         const auto hist_name = getName(event_num, electron->pt(), cluster_counter);
         // Get first hit to estimate eta, phi
         const auto first_hit = cluster->hitsAndFractions()[0];
         const auto ieta_seed = EBDetId(ebRecHitCollection->find(first_hit.first)->detid()).ieta();
         const auto iphi_seed = EBDetId(ebRecHitCollection->find(first_hit.first)->detid()).iphi();

         TH2D hist = TH2D(hist_name.c_str(), ";iphi;ieta;E_T", 100, iphi_seed - 50, iphi_seed + 50,
                                                               100, ieta_seed - 50, ieta_seed + 50);

         for (const auto& hit : cluster->hitsAndFractions())
         {
            const auto it = ebRecHitCollection->find(hit.first);
            const auto cell = geom->getGeometry(it->detid());
            const auto ieta = EBDetId(it->detid()).ieta();
            const auto iphi = EBDetId(it->detid()).iphi();
            const auto& pos = cell->getPosition();
            const auto eta = pos.eta();
            const auto bin = hist.FindBin(iphi, ieta);
            hist.SetBinContent(bin, it->energy() * hit.second / cosh(eta));
         }
         hists[hist_name] = hist;
         cluster_counter++;
      }
   }
}


std::string RecoAnalyzer::getName(const long event_num, const double pt, const int cluster)
{
   return "event_" + std::to_string(event_num) + "_electron_" + std::to_string(static_cast<int>(pt)) + "_cluster_" + std::to_string(cluster);
}

// ------------ method called once each job just before starting event loop  ------------
void
RecoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
RecoAnalyzer::endJob()
{
   TFile file = TFile("output.root", "RECREATE");
   for (auto& hist : hists)
   {
      hist.second.Write();
   }
   file.Close();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecoAnalyzer);
