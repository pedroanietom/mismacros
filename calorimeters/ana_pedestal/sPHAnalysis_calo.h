#ifndef __SPHANALYSIS_H__
#define __SPHANALYSIS_H__

#include <fun4all/SubsysReco.h>

class TFile;
class TNtuple;
class TF1;
class TH1F;
class TH1D;
class TProfile;
class TProfile2D;
class TRandom;
class RawCluster;
class RawClusterContainer;
class RawTowerContainer;
class RawTowerGeomContainer;

class sPHAnalysis_calo: public SubsysReco {


public:

  sPHAnalysis_calo(const std::string &name = "sPHAnalysis_calo", const std::string &filename = "test.root", int neventos = 1);
  virtual ~sPHAnalysis_calo() {}

  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void set_whattodo(int what) { _whattodo = what; }
  void set_neventos(int N) {_nevents = N;}

protected:

  int process_event_prdf(PHCompositeNode *topNode);
  int process_event_shift(PHCompositeNode *topNode);
	
  TFile* OutputNtupleFile;
  std::string OutputFileName;

  TF1* func;

  TH1F* hchan_rms;
  TH1F* hchan_rms_early;
  TH1F* hchan_rms_earlydif;
  TH1F* hchan_rms_late;
  TH1F* hchan_allcount;

  TH1F* h_avgearly;
  TH1F* h_rmsearly;  
  TH1F* h_avgearlydif;
  TH1F* h_rmsearlydif;
  TH1F* h_avglate;
  TH1F* h_rmslate;  
  TH1F* h_avg;
  TH1F* h_rms;
  TProfile2D* hh_amp;
  TProfile2D* hh_pedestal_early;
  TProfile2D* hh_rms_early;
  TProfile2D* hh_pedestal_late;
  TProfile2D* hh_rms_late;
  TProfile2D* hh_pedestal;
  TProfile2D* hh_rms;

  TH1F* h_shiftadc_all;
  TH1F* h_shiftcount_all;
  TH1F* h_shiftcount2_all;

  TH1F* hchan_shiftcount;
  TH1F* hchan_shiftcount2;

  TH1F* h_packetsize[128];

  TH1F* h_ped_early[128*192];
  TH1F* h_ped_late[128*192];
  TH1F* h_pedestal[128*192];
  TH1F* h_pedearly_single[128*192];
  TH1F* h_pedearlyxlate[128*192];

  TH1F* h_signalshape[128*192];
  TH1F* h_count[128*192];

  TH1F* h_shiftcount[128*192];
  TH1F* h_shiftadc[128*192];

  TH1F* h_signalshape_ind[10000];
  int indShift;

  int EventNumber;
  int _whattodo;
  int _nevents;

  int _npackets;
  int _ifirstpacket;
  int _ilastpacket;
  int _nchx;
  
  float pedmin[128*192];
  float pedmax[128*192];

  double fitpar[3];
  TRandom* _rng;

};

#endif


