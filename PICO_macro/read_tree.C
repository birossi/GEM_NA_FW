void read_tree(int run, int h_low, int h_high, int fit_l, int fit_h){

  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(1111);
 
  // Int_t time = 0; 
  //Float_t v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0, v7 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0;

 Int_t Time  = 0; 
 Int_t Index = 0;
 Float_t I_drift, I_G1T, I_G2T, I_G3T, V_G1T, V_G2T, V_G3T;
 Float_t V_drift, I_G1B, I_G2B, I_G3B, V_G1B, V_G2B, V_G3B;

 //Float_t xlow  = -100;
  //Float_t xhigh =  hlim;
  //Int_t   xbins = (xhigh-xlow)/Temp;
  Int_t   ybins =    0;
  Float_t ylow  =    0;
  Float_t yhigh =    0;
  
  char ifile[256] = "";

  sprintf(ifile, "PICO_run_%.4i.root", run);
  cout << ifile << endl;
  TFile* f1 = new TFile(ifile);
  if( f1->IsZombie() ){
    cout << " Error opening ROOT File" << endl;
    exit(-1);
  }
  
  char    tree_name[256] = "t1";
  TTree* t1 =(TTree*)f1->Get(tree_name);
  t1->SetBranchAddress("Index",   &Index);
  t1->SetBranchAddress("Time",    &Time);
  t1->SetBranchAddress("V_drift", &V_drift);
  t1->SetBranchAddress("I_drift", &I_drift);
  t1->SetBranchAddress("I_G1T",   &I_G1T);
  t1->SetBranchAddress("V_G1T",   &V_G1T);
  t1->SetBranchAddress("I_G2T",   &I_G2T);
  t1->SetBranchAddress("V_G2T",   &V_G2T);
  t1->SetBranchAddress("I_G3T",   &I_G3T);
  t1->SetBranchAddress("V_G3T",   &V_G3T);
  t1->SetBranchAddress("I_G1B",   &I_G1B);
  t1->SetBranchAddress("V_G1B",   &V_G1B);
  t1->SetBranchAddress("I_G2B",   &I_G2B);
  t1->SetBranchAddress("V_G2B",   &V_G2B);
  t1->SetBranchAddress("I_G3B",   &I_G3B);
  t1->SetBranchAddress("V_G3B",   &V_G3B);

  cout << "Entries= " <<t1->GetEntries() <<"  Time_min="<<t1->GetMinimum("Time") <<"  Time_max="<<t1->GetMaximum("Time") << "  Time_tot="<<t1->GetMaximum("Time")-t1->GetMinimum("Time") <<endl;

  Int_t   time_l   = t1->GetMinimum("Time")-500;
  Int_t   time_h   = t1->GetMaximum("Time")+500;
  Int_t   time_bin = (Int_t)(time_h-time_l+1)/1;
  Int_t   curr_l   =    -22;
  Int_t   curr_h   =     22;
  Int_t   curr_bin =   4000; 
  Int_t   volt_l   =  -4000;
  Int_t   volt_h   =    500;
  Int_t   volt_bin =   4500; 
  // Current
  //TH1F* h1d = new TH1F("h1d", "h1d",  time_bin, time_l, time_h);// curr_bin, curr_l, curr_h);  
  TH2F* ig3t = new TH2F("ig3t", "ig3t",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* ig3b = new TH2F("ig3b", "ig3b",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* ig2t = new TH2F("ig2t", "ig2t",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* ig2b = new TH2F("ig2b", "ig2b",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* ig1t = new TH2F("ig1t", "ig1t",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* ig1b = new TH2F("ig1b", "ig1b",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  TH2F* idri = new TH2F("idri", "idri",  time_bin, time_l, time_h, curr_bin, curr_l, curr_h);  
  // Voltage
  TH2F* vg3t = new TH2F("vg3t","vg3t",2000, time_l, time_h, volt_bin, volt_l, volt_h);  
  TH2F* vg3b = new TH2F("vg3b","vg3b",2000, time_l, time_h, volt_bin, volt_l, volt_h);  
  TH2F* vg2t = new TH2F("vg2t","vg2t",2000, time_l, time_h, volt_bin, volt_l, volt_h);  
  TH2F* vg2b = new TH2F("vg2b","vg2b",2000, time_l, time_h, volt_bin, volt_l, volt_h);  
  TH2F* vg1t = new TH2F("vg1t","vg1t",2000, time_l, time_h, volt_bin, volt_l, volt_h); 
  TH2F* vg1b = new TH2F("vg1b","vg1b",2000, time_l, time_h, volt_bin, volt_l, volt_h);
  TH2F* vdri = new TH2F("vdri","vdri",2000, time_l, time_h, volt_bin, volt_l, volt_h);

  TH1I* h_time = new TH1I("h_time", "title",  2000, 180400, 181400);  
  //Int_t histo_l = -10;
  //Int_t histo_h = 100;
  Int_t bin_factor = 30;
  


  char h_name[64];
  const Int_t size = 7;
  TH1I* h_VGXB[size];
  for(Int_t i=0; i<size; i++){
    sprintf(h_name, "h_vg%ib", i+1);
    h_VGXB[i]= new TH1I(h_name, h_name, 40000, -4000, 0); 
  }

  TH1I* h_IGXB[size];
  for(Int_t i=0; i<size; i++){
    sprintf(h_name, "h_ig%ib", i+1);
    h_IGXB[i]= new TH1I(h_name, h_name, (h_high-h_low)*bin_factor, h_low, h_high); 
  }

  Int_t Vmaxbin[size]; 
  Int_t Vgap[size];
  for(Int_t i=0; i<t1->GetEntries(); i++){
    t1->GetEntry(i);
      
    h_VGXB[0]->Fill(V_G3B);
    h_VGXB[1]->Fill(V_G3T);
    h_VGXB[2]->Fill(V_G2B);
    h_VGXB[3]->Fill(V_G2T);
    h_VGXB[4]->Fill(V_G1B);
    h_VGXB[5]->Fill(V_G1T);
    h_VGXB[6]->Fill(V_drift);
  } 

  for(Int_t i=0; i<size; i++){
    Vmaxbin[i] = (h_VGXB[i]->GetMaximumBin()-40000)/10.;
    cout << "V_maxbin["<<i<<"]= " << Vmaxbin[i] << endl;
    Vgap[0] = abs(Vmaxbin[0]);
    if(i==0)cout << "V_gap["<<0<<"]= " << Vgap[0] << endl;
  }
   for(Int_t i=1; i<size; i++){
     Vgap[i] = abs(Vmaxbin[i] - Vmaxbin[i-1]);
     cout << "V_gap["<<i<<"]= " << Vgap[i] << endl;
   }
    
  for(Int_t i=0; i<t1->GetEntries(); i++){
    t1->GetEntry(i);
    if(V_G2B>Vmaxbin[2]-10 && V_G2B<Vmaxbin[2]+10){
      h_IGXB[0]->Fill(I_G3B);
      h_IGXB[1]->Fill(I_G3T);
      h_IGXB[2]->Fill(I_G2B);
      h_IGXB[3]->Fill(I_G2T);
      h_IGXB[4]->Fill(I_G1B);
      h_IGXB[5]->Fill(I_G1T);
      h_IGXB[6]->Fill(I_drift);
    }
  } 

  TCanvas* c_vg3b = new TCanvas("c_vg2b", "V_G2B", 900, 600);
  c_vg3b->Divide(3,3);
  for(Int_t i=0; i<size; i++){
    c_vg3b->cd(i+1);
    h_VGXB[i]->Draw();
  }
    
  Int_t maxbin[size];
  for(Int_t i=0; i<size; i++){
    maxbin[i] = (h_IGXB[i]->GetMaximumBin()+(h_low*bin_factor))/bin_factor;
    cout << "I_Maxbin["<<i<<"]= " << maxbin[i] << endl;
  }

  char f_name[64];
  TF1* func[size];
  for(Int_t i=0; i<size; i++){
    sprintf(f_name, "func_%i", i);
    func[i] = new TF1(f_name, "gaus", maxbin[i]-fit_l, maxbin[i]+fit_h);
  }
  TCanvas* c_ig3b = new TCanvas("c_ig3b", "I_G3B", 900, 600);
  c_ig3b->Divide(3,3);
  c_ig3b->SetLogy();

  for(Int_t i=0; i<size; i++){
    c_ig3b->cd(i+1);
    h_IGXB[i]->Draw();
    //h_IGXB[i]->GetXaxis()->SetRange(maxbin[i]-(h_low*bin_factor), maxbin[i]+(h_high*bin_factor));
    h_IGXB[i]->GetYaxis()->SetTitle("Entries");
    if(i==4 || i==0 ||i==2)sprintf(f_name, "Current G%iB [nA]", i+1);
    if(i==5 || i==1 ||i==3)sprintf(f_name, "Current G%iT [nA]", i+1);
    if(i==6)sprintf(f_name, "Current Drift [nA]", i+1);
    h_IGXB[i]->GetXaxis()->SetTitle(f_name);
    sprintf(f_name, "func_%i", i);
    h_IGXB[i]->Fit(f_name,"R");  
  }
  Double_t mean[size];
  Double_t meanErr[size];
  Double_t rms[size];     
  Double_t rmsErr[size];
  Double_t chi2[size];
  for(Int_t i=0; i<size; i++){
    rms[i]     = func[i]->GetParameter(2);
    meanErr[i] = func[i]->GetParError(1);
    mean[i]     = func[i]->GetParameter(1);
    rmsErr[i]  = func[i]->GetParError(2);
    chi2[i]    = func[i]->GetChisquare()/func[i]->GetNDF();
    cout << mean[i] << " MeanErr3=" << meanErr[i] << "  rms3:"<< rms[i]<< "  rmsErr3:" << rmsErr[i] << "  chi/ndf:" << chi2[i] << endl;
  }
  char ofile_name[128];
  ofstream ofile;
  sprintf(ofile_name, "./AnaOutput/Output_Run_%.4i.txt", run);
  cout << ofile_name << endl;
  ofile.open (ofile_name);
  for(Int_t i=0; i<size; i++)ofile <<  run << "  " <<mean[i] << "  " << meanErr[i] << "  "<< rms[i]<< " " << rmsErr[i] << "  " << chi2[i] << "  " << Vgap[0] << " " <<Vgap[1]<<" " << Vgap[2]<<" " <<Vgap[3]<<" "<< Vgap[4]<< "  "<< Vgap[5] << "  " <<Vgap[6] <<endl;
  ofile.close();
  
  /* TCanvas* ig3 = new TCanvas("ig3", "I_G3T&B", 900, 600);
  t1->Project("ig3b", "I_G3B:Time");//, "time>18000");
  t1->Project("ig3t", "I_G3T:Time");
  ig3b->SetMarkerColor(kBlue);
  ig3b->SetMarkerSize(1.5);
  ig3b->GetXaxis()->SetTitle("Timestamp");
  ig3b->GetYaxis()->SetTitle("Current_G1");
  ig3b->Draw("L");
  ig3t->SetMarkerColor(kRed);
  ig3t->SetMarkerSize(1.5);
  ig3t->Draw("LSAME");
  */
  /*
  // Plot globali corrente e poi dopo tensione
  TCanvas* current = new TCanvas("current", "Current", 900, 650);
  //t1->Project("ig1b", "I_G3B:Time");//, "time>18000");
  t1->Project("ig1t", "I_G3T:Time");
  //t1->Project("ig2b", "I_G2B:Time");
  //t1->Project("ig2t", "I_G2T:Time");
  //t1->Project("ig3b", "I_G1B:Time");
  //t1->Project("ig3t", "I_G1T:Time");
  //t1->Project("idri", "I_drift:Time");
  ig1t->SetMarkerColor(kRed);
  ig1b->SetMarkerColor(kMagenta);
  ig2t->SetMarkerColor(kGreen);
  ig2b->SetMarkerColor(kYellow);
  ig3t->SetMarkerColor(kCyan);
  ig3b->SetMarkerColor(kBlue);
  idri->SetMarkerColor(kBlack);
  
  ig3t->SetMarkerStyle(7);
  ig3b->SetMarkerStyle(7);
  ig2t->SetMarkerStyle(7);
  ig2b->SetMarkerStyle(7);
  ig1t->SetMarkerStyle(7);
  ig1b->SetMarkerStyle(7);
  idri->SetMarkerStyle(7);

  idri->GetXaxis()->SetTitle("Timestamp");
  idri->GetYaxis()->SetTitle("Current [nA]");
  //  ig1b->SetMarkerSize(1.5);
  idri->Draw("");
  ig1t->Draw("SAME");
  ig2t->Draw("SAME");
  ig3t->Draw("SAME");
  ig1b->Draw("SAME");
  ig2b->Draw("SAME");
  ig3b->Draw("SAME");

  TLegend *leg_cur = new TLegend(0.8, 0.1, 0.9,0.35);
  leg_cur->AddEntry("ig3b","G3B","p");
  leg_cur->AddEntry("ig3t","G3T","p");
  leg_cur->AddEntry("ig2b","G2B","p");
  leg_cur->AddEntry("ig2t","G2T","p");
  leg_cur->AddEntry("ig1b","G1B","p");
  leg_cur->AddEntry("ig1t","G1T","p");
  leg_cur->AddEntry("idri","DRI","p");
  leg_cur->SetFillColor(0);
  leg_cur->Draw();

  TCanvas* voltage = new TCanvas("voltage", "Voltage", 900, 600);
  t1->Project("vg1b", "V_G1B:Time");//, "time>18000");
  t1->Project("vg1t", "V_G1T:Time");
  t1->Project("vg2b", "V_G2B:Time");
  t1->Project("vg2t", "V_G2T:Time");
  t1->Project("vg3b", "V_G3B:Time");
  t1->Project("vg3t", "V_G3T:Time");
  t1->Project("vdri", "V_drift:Time");
  vg3t->SetMarkerStyle(7);
  vg3b->SetMarkerStyle(7);
  vg2t->SetMarkerStyle(7);
  vg2b->SetMarkerStyle(7);
  vg1t->SetMarkerStyle(7);
  vg1b->SetMarkerStyle(7);
  vdri->SetMarkerStyle(7);

  vdri->GetXaxis()->SetTitle("Timestamp");
  vdri->GetYaxis()->SetTitle("Voltage [V]");
  vdri->SetMarkerColor(kRed);
  vg1t->SetMarkerColor(kRed);
  vg1b->SetMarkerColor(kMagenta);
  vg2t->SetMarkerColor(kGreen);
  vg2b->SetMarkerColor(kYellow);
  vg3t->SetMarkerColor(kCyan);
  vg3b->SetMarkerColor(kBlue);
  vdri->SetMarkerColor(kBlack);
  
  vdri->Draw("");
  vg1t->Draw("SAME");
  vg2t->Draw("SAME");
  vg3t->Draw("SAME");
  vg1b->Draw("SAME");
  vg2b->Draw("SAME");
  vg3b->Draw("SAME");

  TLegend *leg_volt = new TLegend(0.8, 0.1, 0.9,0.35);
  leg_volt->AddEntry("vg3b","G3B","p");
  leg_volt->AddEntry("vg3t","G3T","p");
  leg_volt->AddEntry("vg2b","G2B","p");
  leg_volt->AddEntry("vg2t","G2T","p");
  leg_volt->AddEntry("vg1b","G1B","p");
  leg_volt->AddEntry("vg1t","G1T","p");
  leg_volt->AddEntry("vdri","DRI","p");
  leg_volt->SetFillColor(0);
  leg_volt->Draw();
  */
}

void loop(int run_start, int run_stop){

  for(int run=run_start; run<=run_stop;run++)read_tree(run, -80, 80, 8, 8);

}
