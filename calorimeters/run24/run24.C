#include <sys/stat.h>
void run24()
{

    int run = 23746;
    int nevents = 0;

    double min_chanvsavg_difference=-0.5;
    double max_chanvsavg_difference=0.5;

    double min_chanvsRMS_difference = 0;
    double max_chanvsRMS_difference = 5;

    double min_chanRMS_earlylate = 0;
    double max_chanRMS_earlylate = 30;

    double min_chanRMS_difference = 0;
    double max_chanRMS_difference = 20;


    char results_directory[99];
    sprintf(results_directory,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d",run);
    gSystem->mkdir(results_directory, kTRUE);

    char file[99];
    sprintf(file,"/home/pedro/Storage/sphenix/calorimeter/run24/RUN%d/rundata_%d_%d_0.root",run,run,nevents);


    char chanvsavg_earlylate[99];
    char chanvsavg_early[99];
    char chanvsavg_late[99];
    char chanvsRMS_earlylate[99];
    char chanvsRMS_early[99];
    char chanvsRMS_late[99];
    char chanvsavg_difference[99];
    char chanvsRMS_difference[99];
    char chanRMS_earlylate[99];
    char chanRMS_difference[99];
    char iphiieta_difference[99];
    char iphiieta_early[99];
    char iphiieta_late[99];
    char signalshape_0[99];
    
    sprintf(chanvsavg_earlylate,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsavg_earlylate.png", run,run);
    sprintf(chanvsavg_early,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsavg_early.png", run,run);
    sprintf(chanvsavg_late,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsavg_late.png", run,run);
    sprintf(chanvsRMS_earlylate,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsRMS_earlylate.png", run,run);
    sprintf(chanvsRMS_early,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsRMS_early.png", run,run);
    sprintf(chanvsRMS_late,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsRMS_late.png", run,run);
    sprintf(chanvsavg_difference,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsavg_difference.png", run,run);
    sprintf(chanvsRMS_difference,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanvsRMS_difference.png", run,run);
    sprintf(chanRMS_earlylate,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanRMS_earlylate.png", run,run);
    sprintf(chanRMS_difference,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_chanRMS_difference.png", run,run);
    sprintf(iphiieta_difference,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_iphiieta_difference.png", run,run);
    sprintf(iphiieta_early,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_iphiieta_early.png", run,run);
    sprintf(iphiieta_late,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_iphiieta_late.png", run,run);
    sprintf(signalshape_0,"/home/pedro/Storage/sphenix/calorimeter/run24/results/RUN%d/%d_signalshape_0.png", run,run);

    char leg[99];
    sprintf(leg,"Run %d",run);

    TFile* f = new TFile(file);

    TH1F * h_avgearly = (TH1F*)f->Get("h_avgearly");
    TH1F * h_avglate = (TH1F*)f->Get("h_avglate");

    TH1F * h_rmsearly = (TH1F*)f->Get("h_rmsearly");
    TH1F * h_rmslate = (TH1F*)f->Get("h_rmslate");

    TH1F * h_avg = (TH1F*)f->Get("h_avg");
    TH1F * h_rms = (TH1F*)f->Get("h_rms");

    TH1F * hchan_rms = (TH1F*)f->Get("hchan_rms");
    TH1F * hchan_rms_early = (TH1F*)f->Get("hchan_rms_early");
    TH1F * hchan_rms_late = (TH1F*)f->Get("hchan_rms_late");

    TH1F * h_signalshape_0 = (TH1F*)f->Get("h_signalshape_12430");

    TProfile2D * hh_amp = (TProfile2D*)f->Get("hh_amp");

    TProfile2D * hh_pedestal = (TProfile2D*)f->Get("hh_pedestal");
    TProfile2D * hh_pedestal_early = (TProfile2D*)f->Get("hh_pedestal_early");
    TProfile2D * hh_pedestal_late = (TProfile2D*)f->Get("hh_pedestal_late");

    TProfile2D * hh_rms = (TProfile2D*)f->Get("hh_rms");
    TProfile2D * hh_rms_early = (TProfile2D*)f->Get("hh_rms_early");
    TProfile2D * hh_rms_late = (TProfile2D*)f->Get("hh_rms_late");



//////// channel vs AVG //////////////////////////////////////////////////
    TCanvas* c1 = new TCanvas("c1","c1",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_avgearly->SetMarkerSize(0.5);
    h_avgearly->SetMarkerStyle(24);
    h_avgearly->SetXTitle("Channels");
    h_avgearly->GetXaxis()->CenterTitle(true);
    h_avgearly->GetXaxis()->SetTitleOffset(1.4);
    h_avgearly->SetYTitle("Pedestal average");
    h_avgearly->GetYaxis()->CenterTitle(true);
    h_avgearly->GetYaxis()->SetTitleOffset(1.4);
    h_avgearly->SetAxisRange(900, 2100,"Y");
    h_avgearly->SetDirectory(0);  
    h_avgearly->Draw("hist p");
    TMarker *m_avgearly = new TMarker(.1,.1,24);
    m_avgearly->SetMarkerSize(2);
    m_avgearly->SetMarkerStyle(24);

    h_avglate->SetDirectory(0);
    h_avglate->SetMarkerSize(0.5);
    h_avglate->SetMarkerStyle(22);
    h_avglate->Draw("hist p sames");
    TMarker *m_avglate = new TMarker(.1,.1,22);
    m_avglate->SetMarkerSize(2);
    m_avglate->SetMarkerStyle(22);

    TLegend *leg1 = new TLegend(0.3,0.15,0.5,0.3);
    leg1->SetTextFont(62);
    leg1->SetTextSize(0.03);
    leg1->SetLineColor(0);
    leg1->SetLineStyle(0);
    leg1->SetLineWidth(1);
    leg1->SetFillColor(0);
    leg1->SetFillStyle(1001);
    leg1->SetHeader(leg,"C");
    leg1->AddEntry(m_avgearly,"Average time bin 0-1","p");
    leg1->AddEntry(m_avglate,"Average time bin 2-3","p");
    leg1->Draw();

    c1->SaveAs(chanvsavg_earlylate);


//////// channel vs AVG early //////////////////////////////////////////////////
    TCanvas* c1e = new TCanvas("c1e","c1e",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_avgearly->SetMarkerSize(0.5);
    h_avgearly->SetMarkerStyle(24);
    h_avgearly->SetXTitle("Channels");
    h_avgearly->GetXaxis()->CenterTitle(true);
    h_avgearly->GetXaxis()->SetTitleOffset(1.4);
    h_avgearly->SetYTitle("Pedestal average");
    h_avgearly->GetYaxis()->CenterTitle(true);
    h_avgearly->GetYaxis()->SetTitleOffset(1.4);
    h_avgearly->SetAxisRange(900, 2100,"Y");
    h_avgearly->SetDirectory(0);  
    h_avgearly->Draw("hist p");
    TMarker *m_avgearlye = new TMarker(.1,.1,24);
    m_avgearlye->SetMarkerSize(2);
    m_avgearlye->SetMarkerStyle(24);


    TLegend *leg1e = new TLegend(0.3,0.15,0.5,0.3);
    leg1e->SetTextFont(62);
    leg1e->SetTextSize(0.03);
    leg1e->SetLineColor(0);
    leg1e->SetLineStyle(0);
    leg1e->SetLineWidth(1);
    leg1e->SetFillColor(0);
    leg1e->SetFillStyle(1001);
    leg1e->SetHeader(leg,"C");
    leg1e->AddEntry(m_avgearlye,"Average time bin 0-1","p");
    leg1e->Draw();

    c1e->SaveAs(chanvsavg_early);

//////// channel vs AVG late //////////////////////////////////////////////////
    TCanvas* c1l = new TCanvas("c1l","c1l",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_avglate->SetMarkerSize(0.5);
    h_avglate->SetMarkerStyle(24);
    h_avglate->SetXTitle("Channels");
    h_avglate->GetXaxis()->CenterTitle(true);
    h_avglate->GetXaxis()->SetTitleOffset(1.4);
    h_avglate->SetYTitle("Pedestal average");
    h_avglate->GetYaxis()->CenterTitle(true);
    h_avglate->GetYaxis()->SetTitleOffset(1.4);
    h_avglate->SetAxisRange(900, 2100,"Y");
    h_avglate->SetDirectory(0);  
    h_avglate->Draw("hist p");
    TMarker *m_avglatel = new TMarker(.1,.1,24);
    m_avglatel->SetMarkerSize(2);
    m_avglatel->SetMarkerStyle(24);


    TLegend *leg1l = new TLegend(0.3,0.15,0.5,0.3);
    leg1l->SetTextFont(62);
    leg1l->SetTextSize(0.03);
    leg1l->SetLineColor(0);
    leg1l->SetLineStyle(0);
    leg1l->SetLineWidth(1);
    leg1l->SetFillColor(0);
    leg1l->SetFillStyle(1001);
    leg1l->SetHeader(leg,"C");
    leg1l->AddEntry(m_avglatel,"Average time bin 2-3","p");
    leg1l->Draw();

    c1l->SaveAs(chanvsavg_late);

//////// channel vs RMS //////////////////////////////////////////////////
    TCanvas* c2 = new TCanvas("c2","c2",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_rmsearly->SetMarkerSize(0.5);
    h_rmsearly->SetMarkerStyle(24);
    h_rmsearly->SetXTitle("Channels");
    h_rmsearly->GetXaxis()->CenterTitle(true);
    h_rmsearly->GetXaxis()->SetTitleOffset(1.4);
    h_rmsearly->SetYTitle("Pedestal RMS");
    h_rmsearly->GetYaxis()->CenterTitle(true);
    h_rmsearly->GetYaxis()->SetTitleOffset(1.4);
    h_rmsearly->SetAxisRange(0, 40,"Y");
    h_rmsearly->SetDirectory(0);  
    h_rmsearly->Draw("hist p");
    TMarker *m_rmsearly = new TMarker(.1,.1,24);
    m_rmsearly->SetMarkerSize(2);
    m_rmsearly->SetMarkerStyle(24);

    h_rmslate->SetDirectory(0);
    h_rmslate->SetMarkerSize(0.5);
    h_rmslate->SetMarkerStyle(22);
    h_rmslate->Draw("hist p sames");
    TMarker *m_rmslate = new TMarker(.1,.1,22);
    m_rmslate->SetMarkerSize(2);
    m_rmslate->SetMarkerStyle(22);

    TLegend *leg2 = new TLegend(0.3,0.7,0.5,0.89);
    leg2->SetTextFont(62);
    leg2->SetTextSize(0.03);
    leg2->SetLineColor(0);
    leg2->SetLineStyle(0);
    leg2->SetLineWidth(1);
    leg2->SetFillColor(0);
    leg2->SetFillStyle(1001);
    leg2->SetHeader(leg,"C");
    leg2->AddEntry(m_rmsearly,"Time bin 0-1","p");
    leg2->AddEntry(m_rmslate,"Time bin 3-4","p");
    leg2->Draw();

    c2->SaveAs(chanvsRMS_earlylate);

//////// channel vs RMS early//////////////////////////////////////////////////
    TCanvas* c2e = new TCanvas("c2e","c2e",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_rmsearly->SetMarkerSize(0.5);
    h_rmsearly->SetMarkerStyle(24);
    h_rmsearly->SetXTitle("Channels");
    h_rmsearly->GetXaxis()->CenterTitle(true);
    h_rmsearly->GetXaxis()->SetTitleOffset(1.4);
    h_rmsearly->SetYTitle("Pedestal RMS");
    h_rmsearly->GetYaxis()->CenterTitle(true);
    h_rmsearly->GetYaxis()->SetTitleOffset(1.4);
    h_rmsearly->SetAxisRange(0, 40,"Y");
    h_rmsearly->SetDirectory(0);  
    h_rmsearly->Draw("hist p");
    TMarker *m_rmsearlye = new TMarker(.1,.1,24);
    m_rmsearlye->SetMarkerSize(2);
    m_rmsearlye->SetMarkerStyle(24);


    TLegend *leg2e = new TLegend(0.3,0.7,0.5,0.89);
    leg2e->SetTextFont(62);
    leg2e->SetTextSize(0.03);
    leg2e->SetLineColor(0);
    leg2e->SetLineStyle(0);
    leg2e->SetLineWidth(1);
    leg2e->SetFillColor(0);
    leg2e->SetFillStyle(1001);
    leg2e->SetHeader(leg,"C");
    leg2e->AddEntry(m_rmsearly,"Average time bin 0-1","p");
    leg2e->Draw();

    c2e->SaveAs(chanvsRMS_early);    

//////// channel vs RMS late //////////////////////////////////////////////////
    TCanvas* c2l = new TCanvas("c2l","c2l",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_rmslate->SetMarkerSize(0.5);
    h_rmslate->SetMarkerStyle(24);
    h_rmslate->SetXTitle("Channels");
    h_rmslate->GetXaxis()->CenterTitle(true);
    h_rmslate->GetXaxis()->SetTitleOffset(1.4);
    h_rmslate->SetYTitle("Pedestal RMS");
    h_rmslate->GetYaxis()->CenterTitle(true);
    h_rmslate->GetYaxis()->SetTitleOffset(1.4);
    h_rmslate->SetAxisRange(0, 40,"Y");
    h_rmslate->SetDirectory(0);  
    h_rmslate->Draw("hist p");
    TMarker *m_rmsearlyl = new TMarker(.1,.1,24);
    m_rmsearlyl->SetMarkerSize(2);
    m_rmsearlyl->SetMarkerStyle(24);


    TLegend *leg2l = new TLegend(0.3,0.7,0.5,0.89);
    leg2l->SetTextFont(62);
    leg2l->SetTextSize(0.03);
    leg2l->SetLineColor(0);
    leg2l->SetLineStyle(0);
    leg2l->SetLineWidth(1);
    leg2l->SetFillColor(0);
    leg2l->SetFillStyle(1001);
    leg2l->SetHeader(leg,"C");
    leg2l->AddEntry(m_rmsearly,"Average time bin 2-3","p");
    leg2l->Draw();

    c2l->SaveAs(chanvsRMS_late);    


//////// channel vs avg difference //////////////////////////////////////////////////
    TCanvas* c3 = new TCanvas("c3","c3",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_avg->SetMarkerSize(0.5);
    h_avg->SetMarkerStyle(24);
    h_avg->SetXTitle("Channels");
    h_avg->GetXaxis()->CenterTitle(true);
    h_avg->GetXaxis()->SetTitleOffset(1.4);
    h_avg->SetYTitle("Pedestal average");
    h_avg->GetYaxis()->CenterTitle(true);
    h_avg->GetYaxis()->SetTitleOffset(1.4);
    h_avg->SetAxisRange(min_chanvsavg_difference, max_chanvsavg_difference,"Y");
    h_avg->SetDirectory(0);  
    h_avg->Draw("hist p");
    TMarker *m_avg = new TMarker(.1,.1,24);
    m_avg->SetMarkerSize(2);
    m_avg->SetMarkerStyle(24);

    TLegend *leg3 = new TLegend(0.3,0.7,0.5,0.89);
    leg3->SetTextFont(62);
    leg3->SetTextSize(0.03);
    leg3->SetLineColor(0);
    leg3->SetLineStyle(0);
    leg3->SetLineWidth(1);
    leg3->SetFillColor(0);
    leg3->SetFillStyle(1001);
    leg3->SetHeader(leg,"C");
    leg3->AddEntry(m_avg,"Average time bin difference","p");
    leg3->Draw();

    c3->SaveAs(chanvsavg_difference);

//////// channel vs RMS difference //////////////////////////////////////////////////
    TCanvas* c4 = new TCanvas("c4","c4",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    h_rms->SetMarkerSize(0.5);
    h_rms->SetMarkerStyle(24);
    h_rms->SetXTitle("Channels");
    h_rms->GetXaxis()->CenterTitle(true);
    h_rms->GetXaxis()->SetTitleOffset(1.4);
    h_rms->SetYTitle("Pedestal RMS");
    h_rms->GetYaxis()->CenterTitle(true);
    h_rms->GetYaxis()->SetTitleOffset(1.4);
    h_rms->SetAxisRange(min_chanvsRMS_difference, max_chanvsRMS_difference,"Y");
    h_rms->SetDirectory(0);  
    h_rms->Draw("hist p");
    TMarker *m_rms = new TMarker(.1,.1,24);
    m_rms->SetMarkerSize(2);
    m_rms->SetMarkerStyle(24);

    TLegend *leg4 = new TLegend(0.3,0.7,0.5,0.89);
    leg4->SetTextFont(62);
    leg4->SetTextSize(0.03);
    leg4->SetLineColor(0);
    leg4->SetLineStyle(0);
    leg4->SetLineWidth(1);
    leg4->SetFillColor(0);
    leg4->SetFillStyle(1001);
    leg4->SetHeader(leg,"C");
    leg4->AddEntry(m_rms,"Average time bin difference","p");
    leg4->Draw();

    c4->SaveAs(chanvsRMS_difference);

//////// Channel RMS early, and late//////////////////////////////////////////////////
    TCanvas* c5 = new TCanvas("c5","c5",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    hchan_rms_early->SetLineColor(kRed);
    hchan_rms_early->SetLineWidth(4);
    hchan_rms_early->SetXTitle("Pedestal RMS");
    hchan_rms_early->GetXaxis()->CenterTitle(true);
    hchan_rms_early->GetXaxis()->SetTitleOffset(1.4);
    hchan_rms_early->SetYTitle("Counts");
    hchan_rms_early->GetYaxis()->CenterTitle(true);
    hchan_rms_early->GetYaxis()->SetTitleOffset(1.4);
    hchan_rms_early->SetAxisRange(min_chanRMS_earlylate, max_chanRMS_earlylate,"X");
    hchan_rms_early->SetDirectory(0);  
    hchan_rms_early->Draw();

    hchan_rms_late->SetLineColor(kBlue);
    hchan_rms_late->SetLineWidth(4);
    hchan_rms_late->Draw("sames");




    TLegend *leg5 = new TLegend(0.55,0.7,0.8,0.89);
    leg5->SetTextFont(62);
    leg5->SetTextSize(0.03);
    leg5->SetLineColor(0);
    leg5->SetLineStyle(0);
    leg5->SetLineWidth(1);
    leg5->SetFillColor(0);
    leg5->SetFillStyle(1001);
    leg5->SetHeader(leg,"C");
    leg5->AddEntry(hchan_rms_early,"Average time bin 0-1","l");
    leg5->AddEntry(hchan_rms_late,"Average time bin 2-3","l");
    leg5->Draw();

    c5->SaveAs(chanRMS_earlylate);

//////// Channel RMS difference//////////////////////////////////////////////////
    TCanvas* c6 = new TCanvas("c6","c6",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    hchan_rms->SetLineColor(kBlack);
    hchan_rms->SetLineWidth(4);
    hchan_rms->SetXTitle("Pedestal RMS");
    hchan_rms->GetXaxis()->CenterTitle(true);
    hchan_rms->GetXaxis()->SetTitleOffset(1.4);
    hchan_rms->SetYTitle("Counts");
    hchan_rms->GetYaxis()->CenterTitle(true);
    hchan_rms->GetYaxis()->SetTitleOffset(1.4);
    hchan_rms->SetAxisRange(min_chanRMS_difference, max_chanRMS_difference,"X");
    hchan_rms->SetDirectory(0);  
    hchan_rms->Draw();




    TLegend *leg6 = new TLegend(0.55,0.7,0.8,0.89);
    leg6->SetTextFont(62);
    leg6->SetTextSize(0.03);
    leg6->SetLineColor(0);
    leg6->SetLineStyle(0);
    leg6->SetLineWidth(1);
    leg6->SetFillColor(0);
    leg6->SetFillStyle(1001);
    leg6->SetHeader(leg,"C");
    leg6->AddEntry(hchan_rms,"Average time bin difference","l");
    leg6->Draw();

    c6->SaveAs(chanRMS_difference);

//////// iphi difference ieta//////////////////////////////////////////////////
    TCanvas* c7 = new TCanvas("c7","c7",2000,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    TPaveText *pt7 = new TPaveText(0.1, 0.95, 0.9, 1, "brNDC"); 
    pt7->SetBorderSize(0);
    pt7->SetFillStyle(0);
    pt7->SetTextAlign(22); 
    pt7->SetTextFont(42);
    pt7->SetTextSize(0.04);
    pt7->AddText("Average time bin difference"); 
    pt7->Draw();


    c7->Divide(2,1,0.01,0.05);

    c7->cd(1);
    gPad->SetRightMargin(0.2);
    hh_pedestal->SetXTitle("i#phi");
    hh_pedestal->GetXaxis()->CenterTitle(true);
    hh_pedestal->GetXaxis()->SetTitleOffset(1.2);
    hh_pedestal->SetYTitle("i#eta");
    hh_pedestal->GetYaxis()->CenterTitle(true);
    hh_pedestal->GetYaxis()->SetTitleOffset(1.2);
    hh_pedestal->SetZTitle("Pedestal average time bin difference");
    hh_pedestal->GetZaxis()->CenterTitle(true);
    hh_pedestal->GetZaxis()->SetTitleOffset(2.2);
    hh_pedestal->SetDirectory(0);  
    hh_pedestal->Draw("COLZ");

    c7->cd(2);
    gPad->SetRightMargin(0.2);
    hh_rms->SetXTitle("i#phi");
    hh_rms->GetXaxis()->CenterTitle(true);
    hh_rms->GetXaxis()->SetTitleOffset(1.2);
    hh_rms->SetYTitle("i#eta");
    hh_rms->GetYaxis()->CenterTitle(true);
    hh_rms->GetYaxis()->SetTitleOffset(1.2);
    hh_rms->SetZTitle("Pedestal RMS time bin difference");
    hh_rms->GetZaxis()->CenterTitle(true);
    hh_rms->GetZaxis()->SetTitleOffset(1.2);
    hh_rms->SetDirectory(0);  
    hh_rms->Draw("COLZ");

    c7->SaveAs(iphiieta_difference);

//////// iphi initial ieta//////////////////////////////////////////////////
    TCanvas* c8 = new TCanvas("c8","c8",2000,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    TPaveText *pt8 = new TPaveText(0.1, 0.95, 0.9, 1, "brNDC"); 
    pt8->SetBorderSize(0);
    pt8->SetFillStyle(0);
    pt8->SetTextAlign(22); 
    pt8->SetTextFont(42);
    pt8->SetTextSize(0.04);
    pt8->AddText("Average time bin 0-1"); 
    pt8->Draw();


    c8->Divide(2,1,0.01,0.05);

    c8->cd(1);
    gPad->SetRightMargin(0.2);
    hh_pedestal_early->SetXTitle("i#phi");
    hh_pedestal_early->GetXaxis()->CenterTitle(true);
    hh_pedestal_early->GetXaxis()->SetTitleOffset(1.2);
    hh_pedestal_early->SetYTitle("i#eta");
    hh_pedestal_early->GetYaxis()->CenterTitle(true);
    hh_pedestal_early->GetYaxis()->SetTitleOffset(1.2);
    hh_pedestal_early->SetZTitle("Pedestal average time bin 0-1");
    hh_pedestal_early->GetZaxis()->CenterTitle(true);
    hh_pedestal_early->GetZaxis()->SetTitleOffset(2.2);
    hh_pedestal_early->SetDirectory(0);  
    hh_pedestal_early->Draw("COLZ");

    c8->cd(2);
    gPad->SetRightMargin(0.2);
    hh_rms_early->SetXTitle("i#phi");
    hh_rms_early->GetXaxis()->CenterTitle(true);
    hh_rms_early->GetXaxis()->SetTitleOffset(1.2);
    hh_rms_early->SetYTitle("i#eta");
    hh_rms_early->GetYaxis()->CenterTitle(true);
    hh_rms_early->GetYaxis()->SetTitleOffset(1.2);
    hh_rms_early->SetZTitle("Pedestal RMS time bin 0-1");
    hh_rms_early->GetZaxis()->CenterTitle(true);
    hh_rms_early->GetZaxis()->SetTitleOffset(1.2);
    hh_rms_early->SetDirectory(0);  
    hh_rms_early->Draw("COLZ");

    c8->SaveAs(iphiieta_early);

//////// iphi final ieta//////////////////////////////////////////////////
    TCanvas* c9 = new TCanvas("c9","c9",2000,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    TPaveText *pt9 = new TPaveText(0.1, 0.95, 0.9, 1, "brNDC"); 
    pt9->SetBorderSize(0);
    pt9->SetFillStyle(0);
    pt9->SetTextAlign(22); 
    pt9->SetTextFont(42);
    pt9->SetTextSize(0.04);
    pt9->AddText("Average time bin 2-3"); 
    pt9->Draw();


    c9->Divide(2,1,0.01,0.05);

    c9->cd(1);
    gPad->SetRightMargin(0.2);
    hh_pedestal_late->SetXTitle("i#phi");
    hh_pedestal_late->GetXaxis()->CenterTitle(true);
    hh_pedestal_late->GetXaxis()->SetTitleOffset(1.2);
    hh_pedestal_late->SetYTitle("i#eta");
    hh_pedestal_late->GetYaxis()->CenterTitle(true);
    hh_pedestal_late->GetYaxis()->SetTitleOffset(1.2);
    hh_pedestal_late->SetZTitle("Pedestal average time bin 2-3");
    hh_pedestal_late->GetZaxis()->CenterTitle(true);
    hh_pedestal_late->GetZaxis()->SetTitleOffset(2.2);
    hh_pedestal_late->SetDirectory(0);  
    hh_pedestal_late->Draw("COLZ");

    c9->cd(2);
    gPad->SetRightMargin(0.2);
    hh_rms_late->SetXTitle("i#phi");
    hh_rms_late->GetXaxis()->CenterTitle(true);
    hh_rms_late->GetXaxis()->SetTitleOffset(1.2);
    hh_rms_late->SetYTitle("i#eta");
    hh_rms_late->GetYaxis()->CenterTitle(true);
    hh_rms_late->GetYaxis()->SetTitleOffset(1.2);
    hh_rms_late->SetZTitle("Pedestal RMS time bin 2-3");
    hh_rms_late->GetZaxis()->CenterTitle(true);
    hh_rms_late->GetZaxis()->SetTitleOffset(1.2);
    hh_rms_late->SetDirectory(0);  
    hh_rms_late->Draw("COLZ");

    c9->SaveAs(iphiieta_late);

//////// signal shape example //////////////////////////////////////////////////
    TCanvas* c10 = new TCanvas("c10","c10",1500,900);
    gStyle->SetOptStat(false);
    gStyle->SetOptTitle(false);
    gPad->SetLeftMargin(0.11);
    gStyle->SetLineWidth(3);

    //h_signalshape_0->SetMarkerSize(1);
    //h_signalshape_0->SetMarkerStyle(24);
    h_signalshape_0->SetXTitle("Time");
    h_signalshape_0->GetXaxis()->CenterTitle(true);
    h_signalshape_0->GetXaxis()->SetTitleOffset(1.4);
    h_signalshape_0->SetYTitle("ADC");
    h_signalshape_0->GetYaxis()->CenterTitle(true);
    h_signalshape_0->GetYaxis()->SetTitleOffset(1.4);
    h_signalshape_0->SetAxisRange(0, 12,"X");
    //h_signalshape_0->SetAxisRange(98000, 100000,"Y");
    h_signalshape_0->SetDirectory(0);  
    h_signalshape_0->Draw();


    TLegend *leg10 = new TLegend(0.5,0.6,0.5,0.7);
    leg10->SetTextFont(62);
    leg10->SetTextSize(0.03);
    leg10->SetLineColor(0);
    leg10->SetLineStyle(0);
    leg10->SetLineWidth(1);
    leg10->SetFillColor(0);
    leg10->SetFillStyle(1001);
    leg10->SetHeader(leg,"C");
    leg10->AddEntry(h_signalshape_0,"Signal shape channel 0","p");
    leg10->Draw();

    c10->SaveAs(signalshape_0);


}