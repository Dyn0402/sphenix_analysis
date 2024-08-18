#include "ZDC_SMD.h"

#include <calobase/TowerInfoDefs.h>
#include <caloreco/CaloWaveformFitting.h>

#include <mbd/MbdOut.h>
#include <mbd/MbdPmtContainer.h>
#include <mbd/MbdGeom.h>
#include <mbd/MbdPmtHit.h>

/// Tracking includes
//#include <globalvertex/GlobalVertex.h>
//#include <globalvertex/GlobalVertexMap.h>
//#include <trackbase_historic/SvtxTrack.h>
//#include <trackbase_historic/SvtxTrackMap.h>
//#include <globalvertex/SvtxVertex.h>
//#include <globalvertex/SvtxVertexMap.h>

#include <calobase/TowerInfoDefs.h>
#include <caloreco/CaloWaveformFitting.h>
#include <ffarawobjects/CaloPacketContainerv1.h>
#include <ffarawobjects/CaloPacketv1.h>
#include <ffarawobjects/Gl1Packetv1.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <Event/packet.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/recoConsts.h>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <phool/PHCompositeNode.h>
#include <cmath>
#include <fstream>

R__LOAD_LIBRARY(libuspin.so)

//____________________________________________________________________________..
ZDC_SMD::ZDC_SMD(const std::string &name)
  : SubsysReco(name)
{
  std::cout << "ZDC_SMD::ZDC_SMD(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
ZDC_SMD::~ZDC_SMD()
{
  std::cout << "ZDC_SMD::~ZDC_SMD() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int ZDC_SMD::Init(PHCompositeNode * /*topNode*/)
{
  std::cout << "ZDC_SMD::Init(PHCompositeNode *topNode) Initializing" << std::endl;

  polWaveforms = new TTree();
  polWaveforms = new TTree("polWaveforms", "polWaveforms");
  polWaveforms->SetDirectory(0);
  polWaveforms->Branch("bunchnumber", &bunchnumber, "bunchnumber/I");
  polWaveforms->Branch("waveforms", &waveforms);
  polWaveforms->Branch("mbd_z_vtx", &mbd_z_vtx, "mbd_z_vtx/F");
  polWaveforms->Branch("mbd_z_vtx_err", &mbd_z_vtx_err, "mbd_z_vtx_err/F");
  polWaveforms->Branch("mbd_t0", &mbd_t0, "mbd_t0/F");
  polWaveforms->Branch("mbd_t0_err", &mbd_t0_err, "mbd_t0_err/F");

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ZDC_SMD::InitRun(PHCompositeNode * /*topNode*/)
{
  std::cout << "ZDC_SMD::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;

  evtcnt = 0;

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ZDC_SMD::process_event(PHCompositeNode *topNode)
{
  // std::cout << "ZDC_SMD::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  if (evtcnt % 10000 == 0)
  {
    std::cout << "Event: " << evtcnt << std::endl;
  }

  p_gl1 = findNode::getClass<Gl1Packetv1>(topNode, "GL1Packet");
  zdc_cont = findNode::getClass<CaloPacketContainerv1>(topNode, "ZDCPackets");

  // Find MBD Node
//  mbdNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "MBD"));

  // Get MBD/BBC Output Objects
  mbdout = findNode::getClass<MbdOut>(topNode, "MbdOut");

  mbd_z_vtx = -999.0;
  mbd_z_vtx_err = -999.0;
  mbd_t0 = -999.0;
  mbd_t0_err = -999.0;
  if (mbdout) {
    mbd_z_vtx = mbdout->get_zvtx();
    mbd_z_vtx_err = mbdout->get_zvtxerr();
    mbd_t0 = mbdout->get_t0();
    mbd_t0_err = mbdout->get_t0err();
  }

  if (p_gl1)
  {
    bunchnumber = p_gl1->getBunchNumber();
    if (evtcnt % 1000 == 0)
    {
      std::cout << bunchnumber << std::endl;
    }
    if (zdc_cont->get_npackets() != 1)
    {
      return Fun4AllReturnCodes::EVENT_OK;
    }

    CaloPacket *p_zdc = zdc_cont->getPacket(0);

    if (p_zdc)
    {
      waveforms.clear();
      waveforms.reserve(p_zdc->iValue(0, "CHANNELS"));  // Chris: preallocation = speed improvement

      // in this for loop we get: zdc_adc and smd_adc
      for (int c = 0; c < p_zdc->iValue(0, "CHANNELS"); c++)
      {
        std::vector<float> waveform;  // Chris: preallocation = speed improvement
        waveform.reserve(p_zdc->iValue(0, "SAMPLES"));

        for (int s = 0; s < p_zdc->iValue(0, "SAMPLES"); s++)
        {
          waveform.push_back(p_zdc->iValue(s, c));
        }  // end sample loop
        waveforms.push_back(waveform);
      }  // end channel loop

//      std::vector<std::vector<float>> fitresults_zdc;
//      fitresults_zdc = WaveformProcessingFast->calo_processing_fast(waveforms);
//
//      std::vector<float> result;
//      result = fitresults_zdc.at(0);
//
//      std::vector<float> resultFast = anaWaveformFast(p_zdc, c);  // fast waveform fitting
//      float signalFast = resultFast.at(0);
//      float timingFast = resultFast.at(1);
//      float pedFast = resultFast.at(2);

//      }  // end channel loop
      polWaveforms->Fill();
    }  // end if p_zdc good
  }  // end if p_gl1 good

  evtcnt++;

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ZDC_SMD::End(PHCompositeNode * /*topNode*/)
{
  std::cout << "ZDC_SMD::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  TFile *ofile = new TFile(outfile.c_str(), "RECREATE");

  polWaveforms->Write();

  ofile->Write();
  ofile->Close();
  delete (polWaveforms);

  return Fun4AllReturnCodes::EVENT_OK;
}

std::vector<float> ZDC_SMD::anaWaveformFast(CaloPacket *p, const int channel)
{
  std::vector<float> waveform;
  // Chris: preallocation = speed improvement
  waveform.reserve(p->iValue(0, "SAMPLES"));
  for (int s = 0; s < p->iValue(0, "SAMPLES"); s++)
  {
    waveform.push_back(p->iValue(s, channel));
  }
  std::vector<std::vector<float>> multiple_wfs;
  multiple_wfs.push_back(waveform);

  std::vector<std::vector<float>> fitresults_zdc;
  fitresults_zdc = WaveformProcessingFast->calo_processing_fast(multiple_wfs);

  std::vector<float> result;
  result = fitresults_zdc.at(0);
  return result;
}

void ZDC_SMD::setFileName(const std::string &fname)
{
  outfile = fname;
}
