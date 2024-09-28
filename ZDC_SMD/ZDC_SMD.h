// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef OFFLINESMDTESTING_DST_H
#define OFFLINESMDTESTING_DST_H

#include <fun4all/SubsysReco.h>
#include <globalvertex/MbdVertex.h>
#include <globalvertex/MbdVertexMapv1.h>

#include <string>
#include <cmath>
#include <map>
#include <vector>

class CaloWaveformFitting;
class TowerInfoContainer;
class PHCompositeNode;
class Gl1Packet;
class CaloPacketContainer;
class CaloPacket;

class MbdOut;

class MbdVertex;
class MbdVertexMap;

class TH1;
class TH2;
class TTree;

using namespace std;

class ZDC_SMD : public SubsysReco
{
 public:

  ZDC_SMD(const std::string &name = "ZDC_SMD");

  ~ZDC_SMD() override;

  int Init(PHCompositeNode *topNode) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  int End(PHCompositeNode *topNode) override;

  void setFileName(const std::string &fname);

 protected:
  CaloWaveformFitting *WaveformProcessingFast = nullptr;
  TTree *polWaveforms;

  vector<vector<float>> waveforms;

  int bunchnumber = 0;
  float mbd_z_vtx = 0.0;
  float mbd_z_vtx_err = 0.0;
  float mbd_t0 = 0.0;
  float mbd_t0_err = 0.0;

  // int evtseq_gl1 = 0;

  // int evtseq_zdc = 0;
  // uint64_t BCO_gl1 = 0;
  // uint64_t BCO_zdc = 0;

  Gl1Packet *p_gl1;
  CaloPacketContainer *zdc_cont;
//  PHCompositeNode *mbdNode;
  MbdOut *mbdout;
  MbdVertexMapv1 *m_mbdvtxmap = nullptr;
  MbdVertex *m_mbdvtx = nullptr;

  std::vector<float> anaWaveformFast(CaloPacket *p, const int channel);

  std::string outfile  = "";
  
  int evtcnt;


};

#endif // OFFLINESMDTESTING_DST_H
