#include "sPHAnalysis_calo.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tuple>

#include "TFile.h"
#include "TNtuple.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TRandom2.h"

#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/GlobalVertex.h>

#include <calobase/RawClusterContainer.h>
#include <calobase/RawCluster.h>
#include <calobase/RawClusterv1.h>
#include <calobase/RawTowerv2.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/TowerInfoDefs.h>
#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfoContainerv3.h>
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerGeomContainer.h>

#include <Event/Event.h>
#include <Event/packet.h>

#include <phool/getClass.h>
#include <phool/recoConsts.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHRandomSeed.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <gsl/gsl_rng.h>

using namespace std;

//==============================================================

sPHAnalysis_calo::sPHAnalysis_calo(const std::string &name, const std::string &filename, int N) : SubsysReco(name)
{
  OutputNtupleFile = nullptr;
  OutputFileName = filename;
  EventNumber = 0;
  h_avg = nullptr;
  h_rms = nullptr;

  hh_amp = nullptr;

  hh_pedestal_early = nullptr;
  hh_rms_early = nullptr;

  hh_pedestal_late = nullptr;
  hh_rms_late = nullptr;

  hh_pedestal = nullptr;
  hh_rms = nullptr;
//MR
  _whattodo = 0;
  _nevents = N;
  _rng = nullptr;
}

//==============================================================

int sPHAnalysis_calo::Init(PHCompositeNode *topNode)
{
  std::cout << "sPHAnalysis_calo::Init started..." << endl;
  OutputNtupleFile = new TFile(OutputFileName.c_str(),"RECREATE");
  std::cout << "sPHAnalysis_calo::Init: output file " << OutputFileName.c_str() << " opened." << endl;
  
  char hname[99];

  cout<<"_whattodo  "<<_whattodo<<endl;


  if((_whattodo==0)||(_whattodo==3)) //EMCal
  {
    _npackets=128; // EMCal has 128 packets each with 192 channels (3 FEMs of 64 channels)
    _ifirstpacket=6001;
    _ilastpacket=6128;
    _nchx=128*192; // 24576 total number of channels
  }
  else if((_whattodo==1)||(_whattodo==4)) //iHCal
  {
    _npackets=8; // iHCal has 8 packets each with 192 channels (4 FEMs of 48 channels)
    _ifirstpacket=7001;
    _ilastpacket=7008;
    _nchx=32*48; // 1536 total number of channels
  } 
  else if((_whattodo==2)||(_whattodo==5)) //oHCal
  {
    _npackets=8; // oHCal has 8 packets each with 192 channels (4 FEMs of 48 channels)
    _ifirstpacket=8001;
    _ilastpacket=8008;
    _nchx=32*48; // 1536 total number of channels
  }
  else
  {
    cerr << "ERROR: wrong choice of what to do." << endl; return Fun4AllReturnCodes::ABORTRUN;
  }


  cout<<"_nchx  "<<_nchx<<endl;

  for(int i=0; i<_npackets; i++)
 {
    int itmp=_ifirstpacket+i;
    sprintf(hname,"h_packetsize_%d",itmp);
    h_packetsize[i] = new TH1F(hname,hname,201, -0.5, 200.5);
  }

  for(int i=0; i<_nchx; i++)
  {
    sprintf(hname,"h_signalshape_%d",i);
    h_signalshape[i] = new TH1F(hname,hname,12,-0.5,11.5);
    }




  if(_whattodo<3)
{


  for(int i=0; i<_nchx; i++)
  {
    sprintf(hname,"h_pedearly_%d",i);
    h_ped_early[i] = new TH1F(hname,hname, 2500, 0., 2500.);

    sprintf(hname,"h_pedearly*late_%d",i);
    h_pedearlyxlate[i] = new TH1F(hname,hname, 2500, 0., 2500.);

    sprintf(hname,"h_pedlate_%d",i);
    h_ped_late[i] = new TH1F(hname,hname, 2500, 0., 2500.);

    sprintf(hname,"h_pedestal_%d",i);
    h_pedestal[i] = new TH1F(hname,hname, 1000, -500., 500.);

   }

  h_avgearly = new TH1F("h_avgearly","",_nchx,-0.5,float(_nchx)-0.5);
  h_rmsearly = new TH1F("h_rmsearly","",_nchx,-0.5,float(_nchx)-0.5);

  h_avglate = new TH1F("h_avglate","",_nchx,-0.5,float(_nchx)-0.5);
  h_rmslate = new TH1F("h_rmslate","",_nchx,-0.5,float(_nchx)-0.5);
  
  h_avg = new TH1F("h_avg","",_nchx,-0.5,float(_nchx)-0.5); // 24575 total number of channels
  h_rms = new TH1F("h_rms","",_nchx,-0.5,float(_nchx)-0.5);

  hchan_rms = new TH1F("hchan_rms"," ", 160, 0., 40.);
  hchan_rms_early = new TH1F("hchan_rms_early"," ", 160, 0., 40.);
  hchan_rms_late = new TH1F("hchan_rms_late"," ", 160, 0., 40.);

  
  hh_amp = new TProfile2D("hh_amp","",256,-0.5,255.5,96,-0.5,95.5);

  hh_pedestal_early = new TProfile2D("hh_pedestal_early","",256,-0.5,255.5,96,-0.5,95.5);
  hh_rms_early = new TProfile2D("hh_rms_early","",256,-0.5,255.5,96,-0.5,95.5);

  hh_pedestal_late = new TProfile2D("hh_pedestal_late","",256,-0.5,255.5,96,-0.5,95.5);
  hh_rms_late = new TProfile2D("hh_rms_late","",256,-0.5,255.5,96,-0.5,95.5);

  hh_pedestal = new TProfile2D("hh_pedestal","",256,-0.5,255.5,96,-0.5,95.5);
  hh_rms = new TProfile2D("hh_rms","",256,-0.5,255.5,96,-0.5,95.5);

  }//_whattodo<3
  else
 {
  indShift=0;
  for(int i=0; i<_nchx; i++)
  {
    sprintf(hname,"h_pedearly_single_%d",i);
    h_pedearly_single[i] = new TH1F(hname,hname, 12500, 0., 12500.);
   }

    h_shiftadc_all = new TH1F("h_shiftadc_all"," ",5000,0.,25000.);
    h_shiftcount_all = new TH1F("h_shiftcount_all"," ",31,-0.5,30.5);
    h_shiftcount2_all = new TH1F("h_shiftcount2_all"," ",31,-0.5,30.5);

    hchan_shiftcount = new TH1F("hchan_shiftcount","",_nchx,-0.5,float(_nchx)-0.5);
    hchan_shiftcount2 = new TH1F("hchan_shiftcount2","",_nchx,-0.5,float(_nchx)-0.5);

  for(int iev=0; iev<10000; iev++)
  {
    sprintf(hname,"h_signalshape_ind%d",iev);
    h_signalshape_ind[iev] = new TH1F(hname,hname,12,-0.5,11.5);
   }

 }//_whattodo>2



  _rng = new TRandom2();
  _rng->SetSeed(0);

  cout << "sPHAnalysis_calo::Init ended." << endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//==============================================================

int sPHAnalysis_calo::InitRun(PHCompositeNode *topNode)
{
  cout << "sPHAnalysis_calo::InitRun started..." << endl;
  cerr << "choice of what to do (0,3=EMCal 1,4=iHCal 2,5=oHCal) =====>  " <<_whattodo<< endl; 

  cout << "sPHAnalysis_calo::InitRun ended." << endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//==============================================================

int sPHAnalysis_calo::process_event(PHCompositeNode *topNode)
{
  if(_whattodo==0||_whattodo==1||_whattodo==2)
  {  
    return process_event_prdf(topNode); // unpack calorimeters for pedestals and waveforms
  }
  else if(_whattodo==3||_whattodo==4||_whattodo==5)
  {
    return process_event_shift(topNode); // unpack calorimeters for waveforms bit shift
  }
  else 
  {
    cerr << "ERROR: wrong choice of what to do." << endl; return Fun4AllReturnCodes::ABORTRUN; 
  }
}

//======================================================================

int sPHAnalysis_calo::process_event_prdf(PHCompositeNode *topNode) {

  EventNumber++;
  if(EventNumber==1) topNode->print();
   if(EventNumber % 100 == 0)cout<<"******  "<<EventNumber<<endl;
//

   
  int m_nsamples = 12;
  int m_nchannels = 192;
//  int m_packet_low = 1001; // MBD
//  int m_packet_high = 1002;
//  int m_packet_low = 1001; // MBD
//  int m_packet_high = 1002;
//  int m_packet_low = 12001; // ZDC
//  int m_packet_high = 12001;
//  int m_packet_low = 7001; // HCALIN
//  int m_packet_high = 7008;
//    int m_packet_low = 6001;  //EMC
//    int m_packet_high = 6128;

      int m_packet_low = _ifirstpacket; 
      int m_packet_high = _ilastpacket;


  std::vector<std::vector<float>> waveforms;

  Event *_event = findNode::getClass<Event>(topNode, "PRDF");
  if (_event == nullptr) {cout << "Event not found" << std::endl; return Fun4AllReturnCodes::DISCARDEVENT;}
  if (_event->getEvtType() >= 8) { return Fun4AllReturnCodes::DISCARDEVENT; }

  for (int pid = m_packet_low; pid <= m_packet_high; pid++)
    {
      Packet *packet = _event->getPacket(pid);
      if (packet)
      {
        int nchannels = packet->iValue(0, "CHANNELS");
        int ih=pid-m_packet_low;
        float xsize=float(nchannels);
        h_packetsize[ih]->Fill(xsize); 
        if (nchannels > m_nchannels) { return Fun4AllReturnCodes::DISCARDEVENT; } // corrupted packet

        for (int channel = 0; channel < nchannels; channel++)
        {
          std::vector<float> waveform;
          waveform.reserve(m_nsamples);
          for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(packet->iValue(samp, channel)); }
          waveforms.push_back(waveform);
          waveform.clear();
        }

        if (nchannels < m_nchannels) // fill missing channels with zero
        {
          for (int channel = 0; channel < m_nchannels - nchannels; channel++)
          {
            std::vector<float> waveform;
            waveform.reserve(m_nsamples);
            for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(0); }
            waveforms.push_back(waveform);
            waveform.clear();
          }
        }
        delete packet;
      } // packets exists
      else
      {
//        cout<<"**** event  "<<EventNumber<<"   missing packet pid "<<pid<<endl;     
          for (int channel = 0; channel < m_nchannels; channel++)

          {
            std::vector<float> waveform;
            waveform.reserve(m_nsamples);
            for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(0); }
            waveforms.push_back(waveform);
            waveform.clear();
          }
      }//packet doesn't exist fill with zero to get correct channel number


    } // end loop over packets


  for( int i=0; i<waveforms.size(); i++) 
  {


    float tmpe=0.;
    float tmpl=0.;
    float tmpd=0.;


    for(unsigned int j = 0; j<m_nsamples; j++)
    {
      float tmp = float(waveforms.at(i).at(j));///1000.;
      float xt=j;
      if(tmp>0)
      {
        //std::cout<<"tmp>0, i: "<< i <<", j: "<< j <<", xt: "<< xt <<", tmp: "<< tmp << endl;
       	h_signalshape[i]->Fill(xt,tmp);
      }// adc>0
     }//loop over time all bins

   for(unsigned int j = 0; j<2; j++)
    {
      float tmp = float(waveforms.at(i).at(j));
      tmpe += tmp;
    }
 
    tmpe=tmpe/2.;
    h_ped_early[i]->Fill(tmpe);

    for(unsigned int j = 2; j<4; j++) 
    {
      tmpl += float(waveforms.at(i).at(j));
    }
    tmpl=tmpl/2.;
    h_ped_late[i]->Fill(tmpl);

    h_pedearlyxlate[i]->Fill(tmpe*tmpl);///5000.);


    tmpd=tmpe-tmpl;
    h_pedestal[i]->Fill(tmpd);
  }//loop over channels

  waveforms.clear();

//  cout << "sPHAnalysis_calo::process_event_prdf() ended." << endl;

  return Fun4AllReturnCodes::EVENT_OK;
}

// **************** PEDESTAL SHIFT CODE *******************************
//======================================================================


int sPHAnalysis_calo::process_event_shift(PHCompositeNode *topNode) {

  EventNumber++;
  if(EventNumber==1) topNode->print();
  ofstream shiftedTextFile("shifted.txt",ios::app);
  ofstream shiftedTextFile2("shifted2.txt",ios::app);

 float xmin,xmax;
 if(EventNumber==1)
  {  
        ifstream textFile("means.txt");
        int ix;
	float  xm, xs, y, z;

	while (textFile >> ix >> xm>>xs>>y >> z)
	{
            pedmin[ix]=xm-(25.*xs);
            pedmax[ix]=xm+(25.*xs);
	}
   }
   if(EventNumber % 100 == 0)cout<<"******  "<<EventNumber<<endl;
//


  int m_nsamples = 12;
  int m_nchannels = 192;
//  int m_packet_low = 1001; // MBD
//  int m_packet_high = 1002;
//  int m_packet_low = 1001; // MBD
//  int m_packet_high = 1002;
//  int m_packet_low = 12001; // ZDC
//  int m_packet_high = 12001;
//  int m_packet_low = 7001; // HCALIN
//  int m_packet_high = 7008;
//    int m_packet_low = 6001;  //EMC
//    int m_packet_high = 6128;

      int m_packet_low = _ifirstpacket;
      int m_packet_high = _ilastpacket;


  std::vector<std::vector<float>> waveforms;

  Event *_event = findNode::getClass<Event>(topNode, "PRDF");
  if (_event == nullptr) {cout << "Event not found" << std::endl; return Fun4AllReturnCodes::DISCARDEVENT;}
  if (_event->getEvtType() >= 8) { return Fun4AllReturnCodes::DISCARDEVENT; }

  for (int pid = m_packet_low; pid <= m_packet_high; pid++)
    {
      Packet *packet = _event->getPacket(pid);
      if (packet)
      {
       	int nchannels = packet->iValue(0, "CHANNELS");
        int ih=pid-m_packet_low;
        float xsize=float(nchannels);
        h_packetsize[ih]->Fill(xsize);
        if (nchannels > m_nchannels) { return Fun4AllReturnCodes::DISCARDEVENT; } // corrupted packet

        for (int channel = 0; channel < nchannels; channel++)
        {
          std::vector<float> waveform;
          waveform.reserve(m_nsamples);
          for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(packet->iValue(samp, channel)); }
          waveforms.push_back(waveform);
          waveform.clear();
        }
	if (nchannels < m_nchannels) // fill missing channels with zero
        {
          for (int channel = 0; channel < m_nchannels - nchannels; channel++)
          {
            std::vector<float> waveform;
            waveform.reserve(m_nsamples);
            for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(0); }
            waveforms.push_back(waveform);
            waveform.clear();
          }
	}
	delete packet;
      } // packets exists
      else
      {
//        cout<<"**** event  "<<EventNumber<<"   missing packet pid "<<pid<<endl;
          for (int channel = 0; channel < m_nchannels; channel++)

          {
            std::vector<float> waveform;
            waveform.reserve(m_nsamples);
            for (int samp = 0; samp < m_nsamples; samp++) { waveform.push_back(0); }
            waveforms.push_back(waveform);
            waveform.clear();
          }
      }//packet doesn't exist fill with zero to get correct channel number


    } // end loop over packets


  for( int i=0; i<waveforms.size(); i++)
  {
    float tmpe=0.;
    float tmpl=0.;
    float tmpd=0.;
    float ishift=0.;

//look at waveforms and count shifts in pedestal region
    for(unsigned int j = 0; j<m_nsamples; j++)
    {
      float tmp = float(waveforms.at(i).at(j))/1000.;
      float tmpev = float(waveforms.at(i).at(j));
      float xt=j;
      if(tmp>0)
      {
       	h_signalshape[i]->Fill(xt,tmp);
        if(j<3)h_pedearly_single[i]->Fill(tmpev);
        if(pedmax[i]>0)
        {
          if(j<4||j>20)
          {
             if(tmpev<pedmin[i]||tmpev>pedmax[i])
                {
                  h_shiftcount_all->Fill(xt,1);
                  h_shiftadc_all->Fill(tmpev);
                  cout<<"event "<<EventNumber<<"  ch "<<i<<" time bin "<<j<<"   adc  "<<tmpev<<"   "<<pedmin[i]<<" "<<pedmax[i]<<endl;
                  ishift=tmpev;
                  if(ishift>3000.)h_shiftcount2_all->Fill(xt,1);
                }// early or late shifted bin
           }//early or late
        }// check only good channels
      }// adc>0
    }//loop over time bins
   

  if(ishift>0.)
  {
    hchan_shiftcount->Fill(i,1);
    cout<<"**** shifted event    "<<EventNumber<<" chan "<<i<<" adc "<<ishift<<" ind Shift   "<<indShift<<endl;
    shiftedTextFile<<EventNumber<<"  "<<i<<"   "<<ishift<<"   "<<indShift<<endl;
    if(indShift<10000)
    {
      for(unsigned int j = 0; j<m_nsamples; j++)
      {
        float tmp = float(waveforms.at(i).at(j));
        float xt=j;
        h_signalshape_ind[indShift]->Fill(xt,tmp);
      }//loop over time bins
    }//only save 10000 shifted events

    if(ishift>3000.)
    {
      hchan_shiftcount2->Fill(i,1);
      shiftedTextFile2<<EventNumber<<"  "<<i<<"   "<<ishift<<"   "<<indShift<<endl;
    }//large shift was found

    indShift++;
  } //a shift was found
  }//loop over channels

  waveforms.clear();

//  cout << "sPHAnalysis_calo::process_event_shift() ended." << endl;

  return Fun4AllReturnCodes::EVENT_OK;
}

//======================================================================
int sPHAnalysis_calo::End(PHCompositeNode *topNode)
{

  int igood[_nchx];
  fill_n(igood, _nchx, 1);
  float ycount=EventNumber;

  float maxrms=7.;
  if(_whattodo==0)maxrms=30.;


  if(_whattodo<3)
  {

  TowerInfoContainer* _towersCEMC = findNode::getClass<TowerInfoContainer>(topNode, "TOWERS_CEMC");
  if (!_towersCEMC) { std::cerr<<"No TOWERS_CEMC Node"<<std::endl; return Fun4AllReturnCodes::ABORTEVENT; }

  for(int channel=0; channel<_nchx; channel++)
  { 
    h_ped_early[channel]->GetXaxis()->SetRangeUser(2, 2500.);
    h_ped_late[channel]->GetXaxis()->SetRangeUser(2, 2500.); // To get rid of the bin at zero (filled when adc board channels are missing or packets are missing)  

    for(int it=0; it<31; it++)
    {
//    normalize signal shape per event in adc counts, factor accounts takes into account that histogram weight was adc/1000
//define normalization factor for signal shape histograms
      float factor=1000./ycount;
      float yadc=factor*(h_signalshape[channel]->GetBinContent(it+1));
      h_signalshape[channel]->SetBinContent(it+1,yadc);
//
// use errors for signal shape time bins using the early pedestal rms*(sqrt(3)) /sqrt(nevents)
// sqrt(3) takes into account that early pedestal is average of 3 time bins
//
      float eyadc=(1.732*h_ped_early[channel]->GetRMS())/sqrt(ycount);
      h_signalshape[channel]->SetBinError(it+1,eyadc);
    }//loop over time bins

    float xrms=h_ped_early[channel]->GetRMS();

    if(xrms>maxrms)
    {
      igood[channel]=0;
      cout<< "Bad channel: "<< channel << " " << xrms <<"  masked in hists "<<igood[channel] << endl;
    }
    if(igood[channel]>0)
    {

    TowerInfo *cemcinfo_raw = _towersCEMC->get_tower_at_channel(channel);

    float raw_amplitude = cemcinfo_raw->get_energy();
    float raw_time = cemcinfo_raw->get_time();
    unsigned int key = _towersCEMC->encode_key(channel);
    unsigned int iphi = _towersCEMC->getTowerPhiBin(key);
    unsigned int ieta = _towersCEMC->getTowerEtaBin(key);

    hh_amp->Fill(iphi,ieta,raw_amplitude);

    cout<< "Channel: "<< channel << ", Mean: "<<h_ped_early[channel]->GetMean()<<", RMS: "<< h_ped_early[channel]->GetRMS() << endl;
    hh_pedestal_early->Fill(iphi,ieta,h_ped_early[channel]->GetMean());
    hh_rms_early->Fill(iphi,ieta,h_ped_early[channel]->GetRMS());

    hh_pedestal_late->Fill(iphi,ieta,h_ped_late[channel]->GetMean());
    hh_rms_late->Fill(iphi,ieta,h_ped_late[channel]->GetRMS());

    hh_pedestal->Fill(iphi,ieta,h_pedestal[channel]->GetMean());
    hh_rms->Fill(iphi,ieta,h_pedestal[channel]->GetRMS());

    h_avgearly->Fill(channel,h_ped_early[channel]->GetMean());
    h_avgearly->SetBinError(channel+1, xrms);
    h_rmsearly->Fill(channel,h_ped_early[channel]->GetRMS());
    h_rmsearly->SetBinError(channel+1, h_ped_early[channel]->GetRMSError());


    h_avglate->Fill(channel,h_ped_late[channel]->GetMean());
    h_avglate->SetBinError(channel, h_ped_late[channel]->GetRMS());

    h_rmslate->Fill(channel,h_ped_late[channel]->GetRMS());
    h_rmslate->SetBinError(channel, h_ped_late[channel]->GetRMSError());

    h_avg->Fill(channel,h_pedestal[channel]->GetMean());
    h_avg->SetBinError(channel, h_pedestal[channel]->GetRMS());

    h_rms->Fill(channel,h_pedestal[channel]->GetRMS());
    h_rms->SetBinError(channel, h_pedestal[channel]->GetRMSError());

    hchan_rms ->Fill(h_pedestal[channel]->GetRMS());
    hchan_rms_early ->Fill(h_ped_early[channel]->GetRMS());

    hchan_rms_late ->Fill(h_ped_late[channel]->GetRMS());
    }//good channels
   h_ped_early[channel]->GetXaxis()->SetRangeUser(0., 2500.);//reset early pedestal to include 0
   h_ped_late[channel]->GetXaxis()->SetRangeUser(0., 2500.);//reset late pedestal to include 0

  }//loop over channels

 }//whattodo<3
 else
 {
  float eyadc=15.;
  if(indShift>10000)indShift=10000;

  for(int channel=0; channel<_nchx; channel++)
  {
    for(int it=0; it<31; it++)
    {
     	h_signalshape[channel]->SetBinError(it+1,eyadc);
    }//loop over time bins
  }//loop over channels

  for(int ind=0; ind<indShift; ind++)
  {
    for(int it=0; it<31; it++)
    {
     	h_signalshape_ind[ind]->SetBinError(it+1,eyadc);
    }//loop over time bins
  }//loop over shifted channels

 }//whatotodo>2

  std::cout << "sPHAnalysis_calo::End() started, Number of processed events = " << EventNumber << endl;
  OutputNtupleFile->Write();
  OutputNtupleFile->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}


