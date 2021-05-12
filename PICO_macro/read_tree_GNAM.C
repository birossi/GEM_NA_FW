#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <fstream>

// ROOT includes
#include "TApplication.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"
#include "TLegend.h"

using namespace std;

Float_t MeanValue(Float_t* vec, Int_t size){
  float mean=0;
  int index = 0;
  
  for(Int_t i=0; i<size;i++){
    if(vec[i]!=-9999){
      mean+=vec[i];
      index++;
    }
  }
  mean = mean / index;
  cout << "Mean: "<< mean<<endl;
  
  return mean;
}

Float_t MeanValue(Int_t* vec, Int_t size){
  float mean=0;
  int index = 0;
  
  for(Int_t i=0; i<size;i++){
    if(vec[i]!=-9999){
      mean+=(float)vec[i];
      index++;
    }
  }
  mean = mean / index;
  for(int i=0;i<size;i++)cout << "vec["<<i<<"]= " << vec[i] <<" Mean: "<< mean<< " " << index<<endl;

  return mean;
}

Float_t STD_dev(Float_t* vec, Int_t size){
  float mean=0;
  int index = 0;
  
  for(Int_t i=0; i<size;i++){
    if(vec[i]!=-9999){
      mean+=vec[i];
      index++;
    }
  }
  mean = mean / index;

  float stdev=0;
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999){
      stdev+= pow(vec[i]-mean,2)/index;
    }
  //cout << "stDev: "<< sqrt(stdev)<<endl;

  return sqrt(stdev);
}

Float_t STD_dev(Int_t* vec, Int_t size){
  float mean=0;
  int index = 0;
  
  for(Int_t i=0; i<size;i++){
    if(vec[i]!=-9999){
      mean+=(float)vec[i];
      index++;
    }
  }
  mean = mean / index;
  
  float stdev=0;
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999){
      stdev+= pow(vec[i]-mean,2)/index;
    }
  //cout << "stDev: "<< sqrt(stdev)<<endl;

  return sqrt(stdev);
}

Float_t MaxMinusMin(Float_t* vec, Int_t size){
  float max = 0; 
  
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999&&vec[i]>=max)max=vec[i];
  // cout << "Max: "<< max;

  float min = 99999; 
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999&&vec[i]<=min)min=vec[i];
  
  //cout << "||(float)Max-Min: "<< max-min << "  Min:" << min<< endl;
  
  return max-min;
}

Float_t MaxMinusMin(Int_t* vec, Int_t size){
  float max = 0; 
  
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999&&vec[i]>=max)max=vec[i];
  //cout << "Max: "<< max;

  float min = 99999; 
  for(Int_t i=0; i<size;i++)if(vec[i]!=-9999&&vec[i]<=min)min=vec[i];
  
  //cout << "||(int)Max-Min: "<< max-min << "  Min:" << min<< endl;
  
  return max-min;
}

Float_t GetMin(Float_t* vec, Int_t size){

  float min = 99999; 

  for(Int_t i=0; i<size;i++){
   
    if(vec[i]!=-9999&&vec[i]<=min)min=vec[i];
  }
  //cout << "||Min: "<< min << endl;

  return min;
}

Float_t GetMin(Int_t* vec, Int_t size){

  float min = 99999; 

  for(Int_t i=0; i<size;i++){
   
    if(vec[i]!=-9999&&vec[i]<=min)min=vec[i];
  }
  //cout << "||Min: "<< min << endl;

  return min;
}


int main(int argc,char** argv)
{ 
  int param = 1;
  if(argc < param){
    std::cout << "USAGE: ./read_tree nome_file" << std::endl;
    exit(1);
  }
  for(int i=0; i<param; i++)std::cout << " argv[" << i << "]= " << argv[i] << std::endl; 
  
  // Command line FLAGS
  string filename; //= argv[1];
  // run_nr          = atoi(argv[2]);
  
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1111);

  const Int_t size_char = 128;

  char name[size_char], title[size_char];
  
  Int_t  ID, eta, CAP, Pressione_h, Pressione_l, Saturometria, VAS_scala, SB_tot, net26_grid4x4, cluster_net26_grid4x4;
  Float_t w_hardness_phy;
  Int_t  Gravidanze, Figli, w_hardness_ment;

   Float_t BMI; //Body Mass Index
   Float_t Peso, Altezza, Vita, Fianchi, Ore_sonno, Ore_sport_week, Ore_lavoro_week;
  char    Sesso[size_char], Nome[size_char], Par_BMI[size_char], Rischio_Cardiovascolare[size_char], Biotipo[size_char], Rischio_patologico[size_char], Idratazione[size_char], Adiposita[size_char], Stato_nutrizionale[size_char], Stato_massa_muscolare[size_char], VAS[size_char], QPGWB_1[size_char], QPGWB_2, QPGWB_3[size_char], QPGWB_4[size_char], QPGWB_5[size_char], Stato_civile[size_char], Alcool_tipo[size_char], Alcool_qta_week[size_char], Intestino[size_char], Acqua_bevuta[size_char], Diuresi[size_char], Azienda[size_char], Data_screening[size_char], Luogo_screening[size_char], Cognome[size_char], Nato_a[size_char], Data_nascita[size_char], Residenza[size_char], Via[size_char], Tel[size_char], Lavoro[size_char]; 
 
  bool Sport, Fumatore, ExFumatore, Alcool, Diabete, Ipertensione, Ipercolesterolemia, Anemia, Ipertrigliceridemia, Celiachia, Patologie_famiglia, Diete, SB1, SB2, SB3, SB4, SB5, SB6, SB7;
  
  //da aggiungere
  bool Ciclo, Contraccettivi, Gravida_ora, Allattamento;

  Float_t TBW      = 0.0; // Acqua corporea Totale
  Float_t ICW      = 0.0; // Acqua intracellulare
  Float_t ECW      = 0.0; // Acqua extracellulare
  Float_t FFM      = 0.0; // Massa Magra
  Float_t FAT      = 0.0; // Massa Grassa
  Float_t BCM      = 0.0; // Massa Cellulare
  Float_t ECM      = 0.0; // Massa Extra Cellulare
  Float_t SM       = 0.0;  // Skeletal Muscle
  Float_t SMI      = 0.0; // Skeletal muscle index
  Float_t Peso_des = 0.0; //Peso desiderabile (stima)
  Float_t BMR      = 0.0; // Metabolismo basale
  
  Int_t   ybins =    0;
  Float_t ylow  =    0;
  Float_t yhigh =    0;
  
  char    tree_name[256] = "t1";
  TFile* f1 = new TFile("./Input_files_from_DB/Input_GNAM_Ver3_0.root");
  if( f1->IsZombie() ){
    cout << " Error opening ROOT File" << endl;
    exit(-1);
  }
  
  TTree* t1 =(TTree*)f1->Get(tree_name);
  t1->SetBranchAddress("ID",             &ID);
  t1->SetBranchAddress("eta",            &eta);
  t1->SetBranchAddress("CAP",            &CAP);
  t1->SetBranchAddress("Pressione_l",    &Pressione_l);
  t1->SetBranchAddress("Pressione_h",    &Pressione_h);
  t1->SetBranchAddress("Saturometria",   &Saturometria);
  t1->SetBranchAddress("VAS_scala",      &VAS_scala);
  t1->SetBranchAddress("net26_grid4x4",  &net26_grid4x4);
  t1->SetBranchAddress("cluster_net26_grid4x4ID", &cluster_net26_grid4x4);

  // Bool variables
  t1->SetBranchAddress("Sport",               &Sport);
  t1->SetBranchAddress("Fumatore",            &Fumatore);
  t1->SetBranchAddress("ExFumatore",          &ExFumatore);
  t1->SetBranchAddress("Alcool",              &Alcool);
  t1->SetBranchAddress("Diabete",             &Diabete);
  t1->SetBranchAddress("Ipertensione",        &Ipertensione);
  t1->SetBranchAddress("Ipercolesterolemia",  &Ipercolesterolemia);
  t1->SetBranchAddress("Anemia",              &Anemia);
  t1->SetBranchAddress("Ipertrigliceridemia", &Ipertrigliceridemia);
  t1->SetBranchAddress("Celiachia",           &Celiachia);
  t1->SetBranchAddress("Patologie_famiglia",  &Patologie_famiglia);
  t1->SetBranchAddress("Diete",               &Diete);
  t1->SetBranchAddress("SB1",                 &SB1);
  t1->SetBranchAddress("SB2",                 &SB2);
  t1->SetBranchAddress("SB3",                 &SB3);
  t1->SetBranchAddress("SB4",                 &SB4);
  t1->SetBranchAddress("SB5",                 &SB5);
  t1->SetBranchAddress("SB6",                 &SB6);
  t1->SetBranchAddress("SB7",                 &SB7);
 
  // float variables
  t1->SetBranchAddress("Peso",           &Peso);
  t1->SetBranchAddress("Altezza",        &Altezza);
  t1->SetBranchAddress("Vita",           &Vita);
  t1->SetBranchAddress("Fianchi",        &Fianchi);
  t1->SetBranchAddress("Ore_sonno",      &Ore_sonno);
  t1->SetBranchAddress("Ore_sport_week", &Ore_sport_week);
  t1->SetBranchAddress("Ore_lavoro_week",&Ore_lavoro_week);
  t1->SetBranchAddress("BMI",            &BMI);
  t1->SetBranchAddress("TBW",            &TBW);
  t1->SetBranchAddress("ICW",            &ICW);
  t1->SetBranchAddress("ECW",            &ECW);
  t1->SetBranchAddress("FFM",            &FFM);
  t1->SetBranchAddress("FAT",            &FAT);
  t1->SetBranchAddress("BCM",            &BCM);
  t1->SetBranchAddress("ECM",            &ECM);
  t1->SetBranchAddress("SM",             &SM);
  t1->SetBranchAddress("SMI",            &SMI);
  t1->SetBranchAddress("Peso_des",       &Peso_des);
  t1->SetBranchAddress("BMR",            &BMR);
  t1->SetBranchAddress("w_hardness_phy", &w_hardness_phy);
  t1->SetBranchAddress("SB_tot",         &SB_tot);
  t1->SetBranchAddress("Ore_lavoro_week",&Ore_lavoro_week);
 
   // string variables
  t1->SetBranchAddress("Sesso",           &Sesso);
  t1->SetBranchAddress("Nome",            &Nome);
  t1->SetBranchAddress("Azienda",         &Azienda); 
  t1->SetBranchAddress("Data_screening",  &Data_screening); 
  t1->SetBranchAddress("Luogo_screening", &Luogo_screening);
  t1->SetBranchAddress("Cognome",         &Cognome);
  t1->SetBranchAddress("Nato_a",          &Nato_a);
  t1->SetBranchAddress("Data_screening",  &Data_nascita);
  t1->SetBranchAddress("Residenza",       &Residenza);
  t1->SetBranchAddress("Lavoro",          &Lavoro);
  t1->SetBranchAddress("Acqua_bevuta",    &Acqua_bevuta);
  
  std::cout << "Entries= " <<t1->GetEntries() <<"  Min_ID="<<t1->GetMinimum("ID") <<std::endl;

  //Variabili array per TGraph
  const Int_t size = 101;
  Int_t   v_ID[size];
  Float_t v_eta[size];
  Int_t   v_CAP[size];
  Int_t   v_Sesso[size];
  Int_t   v_Pressione_l[size];     // mmHg
  Int_t   v_Pressione_h[size];     // mmHg
  Int_t   v_Saturometria[size];    // %
  Float_t v_Peso[size];            // kg
  Float_t v_Vita[size];            // cm
  Float_t v_Altezza[size];         // cm
  Float_t v_Fianchi[size];         // cm
  Float_t v_Ore_sonno[size];       // hours
  Float_t v_Ore_sport_week[size];  // hours
  Float_t v_Ore_lavoro_week[size]; // hours
  Float_t v_BMI[size];  // Body mass index kg/m^2
  Float_t v_TBW[size];
  Float_t v_ICW[size];
  Float_t v_ECW[size];
  Float_t v_FFM[size];
  Float_t v_FAT[size];
  Float_t v_BCM[size];
  Float_t v_ECM[size];
  Float_t v_SM[size];
  Float_t v_SMI[size];
  Float_t v_Peso_des[size];
  Float_t v_BMR[size];
  bool    v_Sport[size];
  bool    v_Fumatore[size];
  bool    v_ExFumatore[size];
  bool    v_Alcool[size];
  bool    v_Diabete[size];
  bool    v_Ipertensione[size];
  bool    v_Ipercolesterolemia[size];
  bool    v_Anemia[size];
  bool    v_Ipertrigliceridemia[size];
  bool    v_Celiachia[size];
  bool    v_Patologie_famiglia[size];
  bool    v_Diete[size];
  bool    v_Azienda[size]; //nome azienda 0=Il_notturno; 1=GDM
  Int_t   v_SB_tot[size];
  Int_t   v_w_hardness_phy[size];
  Int_t   v_VAS_scala[size];
  
  /*
    Int_t   time_l   = t1->GetMinimum("ID")-5;
    Int_t   time_h   = t1->GetMaximum("ID")+5;
    Int_t   time_bin = (Int_t)(time_h-time_l+1)/1;
  */
  
  char woman[2] = "F";
  char GDM[4]   = "GDM";

  const Int_t clusters = 8;
  const Int_t sex      = 2;
  
  //NEURAL NETWORK PLOTS - Elly
  TH2F* h_NNbmi[clusters][sex];

  for(Int_t j=0; j<sex; j++)for(Int_t i=0; i<clusters; i++){
      sprintf(name, "h_NNbmi_%i_%i", i, j);
      h_NNbmi[i][j] = new TH2F(name, name,   100, 17, 45, 100, 20, 150);  
    }
  
  Int_t eta_bin = 25; 
  TH1I* h_eta   = new TH1I("h_eta",   "eta",   eta_bin, 0, 100);  
  TH1I* h_eta_m = new TH1I("h_eta_m", "eta_m", eta_bin, 0, 100);
  TH1I* h_eta_w = new TH1I("h_eta_w", "eta_w", eta_bin, 0, 100);

  TH1I* h_satur = new TH1I("h_satur", "saturometria", 20, 80, 120);
  TH1I* h_pre_l = new TH1I("h_per_l", "Blood pressure low", 10, 20, 120);
  TH1I* h_pre_h = new TH1I("h_per_h", "Blood pressure high", 10, 80, 180);
		       
  //TH1F* h_pesoVsVita = new TH1F("h_pesoVsVita", "WeightVsVita", 80, 20, 180);//, 100, 0, 160);
  TH1F* h_peso      = new TH1F("h_peso",     "Weight", 80, 20, 180);
  TH1F* h_altezza   = new TH1F("h_altezza",  "Height", 60, 100, 220); 
  TH1F* h_bmi       = new TH1F("h_bmi",      "BMI",    30,   0,  60);
  TH1F* h_bmr       = new TH1F("h_bmr",      "Metabolismo Basale", 40,   1000,  3000);
  TH1F* h_ffm_peso  = new TH1F("h_ffm_peso", "Massa Magra/Peso",  20,     0,  1);
  TH1F* h_fat_peso  = new TH1F("h_fat_peso", "Massa Grassa/Peso", 20,     0,  1);
  
  TH1F* h_bcm_peso  = new TH1F("h_ffm_peso", "Massa Cellulare/Peso",       20,  0,  1);
  TH1F* h_ecm_peso  = new TH1F("h_fat_peso", "Massa Extra Cellulare/Peso", 20,  0,  1);
  TH1F* h_sm_peso   = new TH1F("h_fat_peso", "Skeletal Muscle/Peso",       20,  0,  1);
  
  TH1F* h_bcm_ffm  = new TH1F("h_bcm_ffm", "Massa Cellulare/Massa Magra",       30,  0,  1);
  TH1F* h_ecm_ffm  = new TH1F("h_ecm_ffm", "Massa Extra Cellulare/Massa Magra", 30,  0,  1);
  TH1F* h_sm_ffm   = new TH1F("h_sm_ffm",  "Skeletal Muscle/Massa Magra",       30,  0,  1);
    
  TH1F* h_ecw_tbw  = new TH1F("h_ecw_tbw", "Acqua Extracellulare/Acqua Corporea Totale", 60,  0,  3);
  TH1F* h_ecw_icw  = new TH1F("h_ecw_icw", "Acqua Extracellulare/Acqua Intracellulare",  60,  0,  3);
  TH1F* h_ecm_bcm  = new TH1F("h_ecm_bcm", "Massa Extra Cellulare/Massa Cellulare",      50,  0,  5);
      
  TH1F* h_ffm_fat       = new TH1F("h_bcm_ffm", "Massa Magra/Massa Grassa",          25,  0,  10);
  TH1F* h_vita_fianchi  = new TH1F("h_ecm_ffm", "Vita/Fianchi",                      20,  0,   2);
  TH1F* h_peso_peso_des = new TH1F("h_peso_peso_des", "Peso-Peso desiderabile",      50 ,  -50,  200);
    
  for(Int_t i=0; i<t1->GetEntries(); i++){
    t1->GetEntry(i);
    //  Int_t check=33;
    //cout << strcmp(&Sesso[1],pippo) <<endl;  
    //printf("Sex0:%c  ",Sesso[1]); 
    // printf("- Sex1:%c  ",Sesso[5]);
    std::cout << ID << "  eta:" << eta << " sesso:"<< Sesso<< " compare " << strcmp(&Sesso[1],woman) <<  "  woman:"<< woman << std::endl;
    h_eta->Fill(eta);
    if(strcmp(&Sesso[1],woman)==0)h_eta_w->Fill(eta);    
    if(strcmp(&Sesso[1],woman)!=0)h_eta_m->Fill(eta);

    if(strcmp(&Sesso[1],woman)==0)v_Sesso[i]=0;
    else v_Sesso[i]=1;

    cout << "Sesso: " <<v_Sesso[i] << endl;
    
    h_satur->Fill(Saturometria);
    h_pre_h->Fill(Pressione_h);
    h_pre_l->Fill(Pressione_l);
    h_peso->Fill(Peso);
    h_altezza->Fill(Altezza);
    //h_pesoVsVita->Fill(Peso, Vita);

    v_ID[i]              = ID;
    v_eta[i]             = (float)eta;
    cout << eta << endl;
    v_CAP[i]             = CAP;
    v_Pressione_l[i]     = Pressione_l;
    v_Pressione_h[i]     = Pressione_h;
    v_Saturometria[i]    = Saturometria;
    v_Peso[i]            = Peso;
    v_Vita[i]            = Vita;
    v_Altezza[i]         = Altezza;
    v_Fianchi[i]         = Fianchi;
    v_Ore_sonno[i]       = Ore_sonno;
    v_Ore_sport_week[i]  = Ore_sport_week;
    v_Ore_lavoro_week[i] = Ore_lavoro_week;
    v_TBW[i]             = TBW;
    v_ICW[i]             = ICW;
    v_ECW[i]             = ECW;
    v_FFM[i]             = FFM;
    v_FAT[i]             = FAT;
    v_BCM[i]             = BCM;
    v_ECM[i]             = ECM;
    v_SM[i]              = SM;
    v_SMI[i]             = SMI;
    v_Peso_des[i]        = Peso_des;
    v_BMR[i]             = BMR;
  
    // array bool
    v_Sport[i]               = Sport;
    v_Fumatore[i]            = Fumatore;
    v_ExFumatore[i]          = ExFumatore;
    v_Alcool[i]              = Alcool;
    v_Diabete[i]             = Diabete;
    v_Ipertensione[i]        = Ipertensione;
    v_Ipercolesterolemia[i]  = Ipercolesterolemia;
    v_Anemia[i]              = Anemia;
    v_Ipertrigliceridemia[i] = Ipertrigliceridemia;
    v_Celiachia[i]           = Celiachia;
    v_Patologie_famiglia[i]  = Patologie_famiglia;
    v_Diete[i]               = Diete;
    v_SB_tot[i]              = SB_tot;
    v_w_hardness_phy[i]      = w_hardness_phy;
    v_VAS_scala[i]           = VAS_scala;

    //Fill histos
    v_BMI[i] = Peso/pow(Altezza/100.,2);
    h_bmi->Fill(v_BMI[i]);//kg/m^2

    h_bmr->Fill(v_BMR[i]); // calorie
    h_ffm_peso->Fill(v_FFM[i]/v_Peso[i]); // adim
    h_fat_peso->Fill(v_FAT[i]/v_Peso[i]); // adim

    h_bcm_peso->Fill(v_BCM[i]/v_Peso[i]); // adim
    h_ecm_peso->Fill(v_ECM[i]/v_Peso[i]); // adim
    h_sm_peso->Fill( v_SM[i]/v_Peso[i]);  // adim

    h_bcm_ffm->Fill(v_BCM[i]/v_FFM[i]);   // adim
    h_ecm_ffm->Fill(v_ECM[i]/v_FFM[i]);   // adim
    h_sm_ffm->Fill( v_SM[i]/v_FFM[i]);    // adim
    
    h_ecw_tbw->Fill(v_ECW[i]/v_TBW[i]);   // adim
    h_ecw_icw->Fill(v_ECW[i]/v_ICW[i]);   // adim
    h_ecm_bcm->Fill(v_ECM[i]/v_BCM[i]);   // adim
     
    h_ffm_fat->Fill( v_FFM[i]/v_FAT[i]);  // adim
    h_vita_fianchi->Fill(v_Vita[i]/v_Fianchi[i]);//adim
    h_peso_peso_des->Fill(v_Peso[i]-v_Peso_des[i]);//kg
    
    if(strcmp(&Azienda[1], GDM)==0)v_Azienda[i] = 1;
    else v_Azienda[i] = 0;
      // GDM=1; Il_Notturno=0  
    cout << "strcmp(&Azienda[1],GDM)="<< strcmp(&Azienda[1],GDM)<<" Azienda:" << Azienda << " GDM:" << GDM << "  V_azienda: " <<v_Azienda[i]<< endl;
    //cout <<" w_hardness_phy: " << w_hardness_phy <<endl;

    for(Int_t i=0; i<clusters; i++)if(strcmp(&Sesso[1],woman)==0)if(cluster_net26_grid4x4==(i+1))h_NNbmi[i][0]->Fill(BMI, Peso);//Donne
    for(Int_t i=0; i<clusters; i++)if(strcmp(&Sesso[1],woman)!=0)if(cluster_net26_grid4x4==(i+1))h_NNbmi[i][1]->Fill(BMI, Peso);//Uomini  
  } 
  
  TApplication theApp("h1Thread", &argc, argv);

  TCanvas* c_eta = new TCanvas("eta", "Eta", 700, 650);
  h_eta->GetYaxis()->SetTitle("Entries");
  h_eta->GetXaxis()->SetTitle("Eta' [anni]");
  h_eta->SetFillColor(kBlack);
  h_eta->Draw("BAR2");
  h_eta_m->SetFillColor(kBlue);
  h_eta_m->Draw("BAR2 SAME");
  h_eta_w->SetFillColor(kRed);
  h_eta_w->Draw("BAR2 SAME");

  TCanvas* c_sat = new TCanvas("c_sat", "Saturometria", 700, 650);
  h_satur->GetYaxis()->SetTitle("Entries");
  h_satur->GetXaxis()->SetTitle("Saturometria [%]");
  h_satur->Draw("");

  TCanvas* c_pre_h = new TCanvas("c_pre_h", "Blood pressure max", 700, 650);
  h_pre_h->GetYaxis()->SetTitle("Entries");
  h_pre_h->GetXaxis()->SetTitle("Blood pressure max [mmHg]");
  h_pre_h->Draw("");
		       
  TCanvas* c_pre_l = new TCanvas("c_pre_l", "Blood pressure low", 700, 650);
  h_pre_l->GetYaxis()->SetTitle("Entries");
  h_pre_l->GetXaxis()->SetTitle("Blood pressure low [mmHg]");
  h_pre_l->Draw("");
		       
  TCanvas* c_peso = new TCanvas("c_peso", "Weight", 700, 650);
  h_peso->GetYaxis()->SetTitle("Entries");
  h_peso->GetXaxis()->SetTitle("Weight [kg]");
  h_peso->Draw("");

  TCanvas* c_altezza = new TCanvas("c_altezza", "Height", 700, 650);
  h_altezza->GetYaxis()->SetTitle("Entries");
  h_altezza->GetXaxis()->SetTitle("Height [cm]");
  h_altezza->Draw("");

  TGraph* gPesoVita = new TGraph(102, v_Peso, v_Vita);
  TCanvas* c_pesoVita = new TCanvas("c_pesoVita", "WeightVsVita", 700, 650);
  gPesoVita->GetYaxis()->SetTitle("Circonferenza Vita [cm]");
  gPesoVita->GetXaxis()->SetTitle("Weight [kg]");
  gPesoVita->SetMarkerStyle(20);
  gPesoVita->Draw("AP");

  TCanvas* c_bmi = new TCanvas("c_bmi", "Body Mass Index", 700, 650);
  h_bmi->GetYaxis()->SetTitle("Entries");
  h_bmi->GetXaxis()->SetTitle("Body Mass Index [kg/m^{2}]");
  h_bmi->Draw("");

  TCanvas* c_bmr = new TCanvas("c_bmr", "Metabolismo Basale", 700, 650);
  h_bmr->GetYaxis()->SetTitle("Entries");
  h_bmr->GetXaxis()->SetTitle("Metabolismo Basale [calorie]");
  h_bmr->Draw("");

  TCanvas* c_peso2 = new TCanvas("c_peso2", "Ratio on Peso", 700, 850);
  c_peso2->Divide(5,1);
  h_ffm_peso->GetYaxis()->SetTitle("Entries");
  h_fat_peso->GetYaxis()->SetTitle( "Entries");
  h_bcm_peso->GetYaxis()->SetTitle("Entries");
  h_ecm_peso->GetYaxis()->SetTitle( "Entries");
  h_sm_peso->GetYaxis()->SetTitle("Entries");
    
  h_bcm_peso->GetXaxis()->SetTitle("Massa Cellulare/Peso");
  h_ecm_peso->GetXaxis()->SetTitle("Massa Extra Cellulare/Peso");
  h_sm_peso->GetXaxis()->SetTitle("Skeleton Muscle/Peso");
  h_fat_peso->GetXaxis()->SetTitle("Massa Grassa/Peso");
  h_ffm_peso->GetXaxis()->SetTitle("Massa Magra/Peso");
  c_peso2->cd(1);
  h_bcm_peso->Draw("");
  c_peso2->cd(2);
  h_ecm_peso->Draw("");
  c_peso2->cd(3);
  h_ffm_peso->Draw("");
  c_peso2->cd(4);
  h_fat_peso->Draw("");
  c_peso2->cd(5);
  h_sm_peso->Draw("");
  
  TCanvas* c_ffm = new TCanvas("c_ffm", "Ratio on Massa Magra", 700, 650);
  c_ffm->Divide(1,3);
  h_ecm_ffm->GetYaxis()->SetTitle("Entries");
  h_sm_ffm->GetYaxis()->SetTitle( "Entries");
  h_bcm_ffm->GetYaxis()->SetTitle("Entries");
  
  h_bcm_ffm->GetXaxis()->SetTitle("Massa Cellulare/Massa Magra");
  h_ecm_ffm->GetXaxis()->SetTitle("Massa Extra Cellulare/Massa Magra");
  h_sm_ffm->GetXaxis()->SetTitle("Skeleton Muscle/Massa Magra");
  c_ffm->cd(1);
  h_bcm_ffm->Draw("");
  c_ffm->cd(2);
  h_ecm_ffm->Draw("");
  c_ffm->cd(3);
  h_sm_ffm->Draw("");
  
  TCanvas* c_ecw = new TCanvas("c_ecw", "BIA parameters", 700, 650);
  c_ecw->Divide(2,2);
  h_ecw_tbw->GetYaxis()->SetTitle("Entries");
  h_ecw_icw->GetYaxis()->SetTitle( "Entries");
  h_ecm_bcm->GetYaxis()->SetTitle("Entries");
  h_ffm_fat->GetYaxis()->SetTitle("Entries");
  
  h_ecw_tbw->GetXaxis()->SetTitle("Acqua Extracellulare/Acqua Corporea Totale");
  h_ecw_icw->GetXaxis()->SetTitle("Acqua Extracellulare/Acqua Intracellulare");
  h_ecm_bcm->GetXaxis()->SetTitle("Massa Extracellulare/Massa Cellulare");
  h_ffm_fat->GetXaxis()->SetTitle("Massa Magra/Massa Grassa");

  c_ecw->cd(1);
  h_ecw_tbw->Draw("");
  c_ecw->cd(2);
  h_ecw_icw->Draw("");
  c_ecw->cd(3);
  h_ecm_bcm->Draw("");
  c_ecw->cd(4);
  h_ffm_fat->Draw("");
  
  TCanvas* c_vf = new TCanvas("c_vf", "Vita/Fianchi", 700, 650);
  c_vf->Divide(2,1);
  h_vita_fianchi->GetYaxis()->SetTitle("Entries");
  h_peso_peso_des->GetYaxis()->SetTitle( "Entries");
  h_vita_fianchi->GetXaxis()->SetTitle("Circ. Vita/Circ. Fianchi");
  h_peso_peso_des->GetXaxis()->SetTitle("Peso-Peso desiderabile");
  c_vf->cd(1);
  h_vita_fianchi->Draw("");
  c_vf->cd(2);
  h_peso_peso_des->Draw("");

  // Plot from Neural Network SOM - Elly
  /*  TCanvas* c_NN_bmi = new TCanvas("c_NNbmi", "Neural Network clusters", 700, 650);
  h_NNbmi[0][0]->SetStats(0);
  h_NNbmi[0][0]->SetMarkerStyle(22);
  h_NNbmi[0][0]->SetMarkerColor(1); //donne
  h_NNbmi[0][0]->SetMarkerSize(1.1);
  h_NNbmi[0][0]->Draw("");
  h_NNbmi[0][1]->SetMarkerStyle(22);
  h_NNbmi[0][1]->SetMarkerColor(2);//uomini
  h_NNbmi[0][1]->SetMarkerSize(1.1);
  h_NNbmi[0][1]->Draw("SAME");
  
  for(Int_t j=0; j<sex; j++)for(Int_t i=1; i<clusters; i++){
      sprintf(name, "Cluster_%i_%i", i, j);

     h_NNbmi[i][j]->GetXaxis()->SetTitle("BMI");
     h_NNbmi[i][j]->GetYaxis()->SetTitle("Peso (kg)");
     h_NNbmi[i][j]->SetMarkerStyle(i+22);
     h_NNbmi[i][j]->SetMarkerSize(1.5);
     if(j==0)h_NNbmi[i][j]->SetMarkerColor(1);
     else h_NNbmi[i][j]->SetMarkerColor(2);
     h_NNbmi[i][j]->Draw("same");  
   }

   TLegend* legend = new TLegend(0.1,0.5,0.25,0.9);
   for(Int_t i=0; i<clusters; i++)for(Int_t j=0; j<sex; j++){
       sprintf(name, "Cluster_%i_%i", i, j);
     legend->AddEntry(h_NNbmi[i][j], name,"P");   
   }
   legend->Draw();
  */
   ///////////////////////////////////////////////////7
  
  c_eta->SaveAs("./output_plot/eta.pdf");
  c_sat->SaveAs("./output_plot/sat.pdf");
  c_pre_h->SaveAs("./output_plot/pre_h.pdf");
  c_pre_l->SaveAs("./output_plot/pre_l.pdf");
  c_peso->SaveAs("./output_plot/peso.pdf");
  c_altezza->SaveAs("./output_plot/altezza.pdf");
  c_pesoVita->SaveAs("./output_plot/pesoVita.pdf");
  c_bmi->SaveAs("./output_plot/bmi.pdf");
  c_bmr->SaveAs("./output_plot/bmr.pdf");
  c_peso2->SaveAs("./output_plot/peso2.pdf");
  c_ffm->SaveAs("./output_plot/ffm.pdf");
  c_ecw->SaveAs("./output_plot/ecw.pdf");
  c_vf->SaveAs("./output_plot/vf.pdf");

  theApp.Run(kTRUE);
    
    // Plot globali corrente e poi dopo tensione
  /*
    TCanvas* current = new TCanvas("current", "Current", 900, 650);
    t1->Project("ig1b", "I_G3B:Time");//, "time>18000");
    idri->SetMarkerColor(kBlack);
    idri->SetMarkerStyle(7);
    idri->GetXaxis()->SetTitle("Timestamp");
    idri->GetYaxis()->SetTitle("Current [#muA]");
    //  ig1b->SetMarkerSize(1.5);
    
    TLegend *leg_cur = new TLegend(0.8, 0.1, 0.9,0.35);
    leg_cur->AddEntry("idri","DRI","p");
    leg_cur->SetFillColor(0);
    leg_cur->Draw();
  */

  // File di output txt
  ofstream fout;
  fout.open ("large_DB.txt");


  //   in >>ID>>Azienda>>Data_screening>> Luogo_screening>> Cognome>> Nome>> Data_nascita>> eta>>Sesso>>Lavoro>>Ore_lavoro_sett>>w_hardness_phy>>Sport>>Ore_sport_week>>Ore_sonno>>Fumatore>>ExFumatore>>Alcool>>Intestino>>Acqua_bevuta>>Diabete>>Ipertensione>>Ipercolesterolemia>>Anemia>>Ipertrigliceridemia>>Celiachia>>Patologie_famiglia>>Peso>>Altezza>>Vita>>Fianchi>>Pressione_h>>Pressione_l>>BMI>>Par_BMI>>TBW>>ICW>>ECW>>FFM>>FAT>>BCM>>ECM>>SM>>SMI>>Peso_des>>BMR>>Saturometria>>SB1>>SB2>>SB3>>SB4>>SB5>>SB6>>SB7>>SB_tot>>VAS_scala;

  //intestazione V1.0
  //fout<< "eta " << "Pressione_min "<< "Pressione_max "<<"Peso "<< "Vita "<< "Altezza "<< "Fianchi "<< "BMI "<< "TBW "<<"ICW "<<"ECW "<<"FFM "<<"FAT "<<"BCM "<<"ECM "<<"SM "<<"SMI "<<"Peso_des "<< "BMR " << "Sport " << "Fumatore " << "ExFumatore " << "Alcool " << "Diabete " << "Ipertensione " << "Ipercolesterolemia " << "Anemia " << "Ipertrigliceridemia " << "Celiachia "<< "Patologie_famiglia "<<"Diete "<< "SB_tot "<< "Ore_lavoro_week " << "w_hardness_phy "<< "Ore_sport_week "<< "Ore_sonno " << "VAS_scala " << "Sesso " << endl;

  //intestazione V2.0
  //fout<< "eta " << "Pressione_min "<< "Pressione_max "<<"Peso "<< "Vita "<< "Altezza "<< "Fianchi "<< "BMI "<< "TBW "<<"ICW "<<"ECW "<<"FFM "<<"FAT "<<"BCM "<<"ECM "<<"SM "<<"SMI "<<"Peso_des "<< "BMR " << "SB_tot "<< "Ore_lavoro_week " << "w_hardness_phy "<< "Ore_sport_week "<< "Ore_sonno " << "VAS_scala " << "Sesso " << endl;

  //intestazione V3.0
  // fout<< "eta " << "Pressione_min "<< "Pressione_max "<<"Peso "<< "Vita "<< "Altezza "<< "Fianchi "<< "BMI "<< "TBW "<<"ICW "<<"ECW "<<"FFM "<<"FAT "<<"BCM "<<"ECM "<<"SM "<<"SMI "<< "BMR " << "SB_tot "<< "Ore_lavoro_week " << "w_hardness_phy "<< "Ore_sport_week "<< "Ore_sonno " << "VAS_scala " << "Sesso " << endl;

  //intestazione V4.0
  fout<< "eta "<<"Peso "<< "Altezza "<< "BMI "<< "TBW "<<"ICW "<<"ECW "<<"FFM "<<"FAT "<<"BCM "<<"ECM "<<"SM "<<"SMI "<< "BMR " << "Sesso " << endl;
  //cout << "wdoihfliuhf "<<  (v_eta[i]-GetMin(v_eta, size))/MaxMinusMin(v_eta, size) <<endl;
  //if(strcmp(&Azienda[1],GDM)==0)


  //if(v_Azienda[i]==0)// GDM=1; Il_Notturno=0  
  // <<  "  " << (v_Pressione_l[i]-MeanValue(v_Pressione_l, size))/STD_dev(v_Pressione_l, size)
  //				 <<  "  " << (v_Pressione_h[i]-MeanValue(v_Pressione_h, size))/STD_dev(v_Pressione_h, size)
  // <<  "  " << (v_Vita[i]-MeanValue(v_Vita, size))/STD_dev(v_Vita, size)
  //			 <<  "  " << (v_Fianchi[i]-MeanValue(v_Fianchi, size))/STD_dev(v_Fianchi, size)

  for(Int_t i=0;i<size-1;i++){
      cout << "ID= " << v_ID[i] <<endl;
      fout <<  "  " << (v_eta[i]-MeanValue(v_eta, size))/STD_dev(v_eta, size)
	   <<  "  " << (v_Peso[i]-MeanValue(v_Peso, size))/STD_dev(v_Peso, size)
	   <<  "  " << (v_Altezza[i]-MeanValue(v_Altezza, size))/STD_dev(v_Altezza, size)
	   <<  "  " << (v_BMI[i]-MeanValue(v_BMI, size))/STD_dev(v_BMI, size)
	   <<  "  " << (v_TBW[i]-MeanValue(v_TBW, size))/STD_dev(v_TBW, size)
	   <<  "  " << (v_ICW[i]-MeanValue(v_ICW, size))/STD_dev(v_ICW, size)
	   <<  "  " << (v_ECW[i]-MeanValue(v_ECW, size))/STD_dev(v_ECW, size)
	   <<  "  " << (v_FFM[i]-MeanValue(v_FFM, size))/STD_dev(v_FFM, size)
	   <<  "  " << (v_FAT[i]-MeanValue(v_FAT, size))/STD_dev(v_FAT, size)
	   <<  "  " << (v_BCM[i]-MeanValue(v_BCM, size))/STD_dev(v_BCM, size)
	   <<  "  " << (v_ECM[i]-MeanValue(v_ECM, size))/STD_dev(v_ECM, size)
	   <<  "  " << (v_SM[i]-MeanValue( v_SM, size))/STD_dev(v_SM, size)
	   <<  "  " << (v_SMI[i]-MeanValue(v_SMI, size))/STD_dev(v_SMI, size)
	   <<  "  " << (v_BMR[i]-MeanValue(v_BMR, size))/STD_dev(v_BMR, size)
	//	 <<  "  " << (v_SB_tot[i]-MeanValue(v_SB_tot, size))/STD_dev(v_SB_tot, size)
	//			 <<  "  " << (v_Ore_lavoro_week[i]-MeanValue(v_Ore_lavoro_week, size))/STD_dev(v_Ore_lavoro_week, size)
	//		  	 <<  "  " << (v_w_hardness_phy[i]-MeanValue(v_w_hardness_phy, size))/STD_dev(v_w_hardness_phy, size)
	//			 <<  "  " << (v_Ore_sport_week[i]-MeanValue(v_Ore_sport_week, size))/STD_dev(v_Ore_sport_week, size)
	//		   	 <<  "  " << (v_Ore_sonno[i]-MeanValue(v_Ore_sonno, size))/STD_dev(v_Ore_sonno, size)
	//			 <<  "  " << (v_VAS_scala[i]-MeanValue(v_VAS_scala, size))/STD_dev(v_VAS_scala, size)
	   <<  "  " << v_Sesso[i]
						 // <<  "  " << v_Sport[i]
						 //<<  "  " << v_Fumatore[i]
						 //<<  "  " << v_ExFumatore[i]
						 // <<  "  " << v_Alcool[i]
						 //<<  "  " << v_Diabete[i]
						 //<<  "  " << v_Ipertensione[i]
						 //<<  "  " << v_Ipercolesterolemia[i]
						 // <<  "  " << v_Anemia[i]
						 // <<  "  " << v_Ipertrigliceridemia[i]
						 //<<  "  " << v_Celiachia[i]
						 //<<  "  " << v_Patologie_famiglia[i]
						 // <<  "  " << v_Diete[i]
			      
			      /* //<<  "  " << (v_Peso_des[i]-MeanValue(v_Peso_des, size))/STD_dev(v_Peso_des, size)
				 <<  "  " << (v_eta[i]-GetMin(v_eta, size))/MaxMinusMin(v_eta, size)
				 <<  "  " << (v_Pressione_l[i]-GetMin(v_Pressione_l, size))/MaxMinusMin(v_Pressione_l, size)
				 <<  "  " << (v_Pressione_h[i]-GetMin(v_Pressione_h, size))/MaxMinusMin(v_Pressione_h, size)
				 <<  "  " << (v_Peso[i]-GetMin(v_Peso, size))/MaxMinusMin(v_Peso, size)
				 <<  "  " << (v_Vita[i]-GetMin(v_Vita, size))/MaxMinusMin(v_Vita, size)
				 <<  "  " << (v_Altezza[i]-GetMin(v_Altezza, size))/MaxMinusMin(v_Altezza, size)
				 <<  "  " << (v_Fianchi[i]-GetMin(v_Fianchi, size))/MaxMinusMin(v_Fianchi, size)
				 <<  "  " << (v_BMI[i]-GetMin(v_BMI, size))/MaxMinusMin(v_BMI, size)
				 <<  "  " << (v_TBW[i]-GetMin(v_TBW, size))/MaxMinusMin(v_TBW, size)
				 <<  "  " << (v_ICW[i]-GetMin(v_ICW, size))/MaxMinusMin(v_ICW, size)
				 <<  "  " << (v_ECW[i]-GetMin(v_ECW, size))/MaxMinusMin(v_ECW, size)
				 <<  "  " << (v_FFM[i]-GetMin(v_FFM, size))/MaxMinusMin(v_FFM, size)
				 <<  "  " << (v_FAT[i]-GetMin(v_FAT, size))/MaxMinusMin(v_FAT, size)
				 <<  "  " << (v_BCM[i]-GetMin(v_BCM, size))/MaxMinusMin(v_BCM, size)
				 <<  "  " << (v_ECM[i]-GetMin(v_ECM, size))/MaxMinusMin(v_ECM, size)
				 <<  "  " << (v_SM[i]-GetMin( v_SM, size))/MaxMinusMin(v_SM, size)
				 <<  "  " << (v_SMI[i]-GetMin(v_SMI, size))/MaxMinusMin(v_SMI, size)
				 <<  "  " << (v_Peso_des[i]-GetMin(v_Peso_des, size))/MaxMinusMin(v_Peso_des, size)
				 <<  "  " << (v_BMR[i]-GetMin(v_BMR, size))/MaxMinusMin(v_BMR, size)
				 <<  "  " << v_Sport[i]
				 <<  "  " << v_Fumatore[i]
				 <<  "  " << v_ExFumatore[i]
				 <<  "  " << v_Alcool[i]
				 <<  "  " << v_Diabete[i]
				 <<  "  " << v_Ipertensione[i]
				 <<  "  " << v_Ipercolesterolemia[i]
				 <<  "  " << v_Anemia[i]
				 <<  "  " << v_Ipertrigliceridemia[i]
				 <<  "  " << v_Celiachia[i]
				 <<  "  " << v_Patologie_famiglia[i]
				 <<  "  " << v_Diete[i]
			      	 <<  "  " << (v_SB_tot[i]-GetMin(v_SB_tot, size))/MaxMinusMin(v_SB_tot, size)
				 <<  "  " << (v_Ore_lavoro_week[i]-GetMin(v_Ore_lavoro_week, size))/MaxMinusMin(v_Ore_lavoro_week, size)
			  	 <<  "  " << (v_w_hardness_phy[i]-GetMin(v_w_hardness_phy, size))/MaxMinusMin(v_w_hardness_phy, size)
				 <<  "  " << (v_Ore_sport_week[i]-GetMin(v_Ore_sport_week, size))/MaxMinusMin(v_Ore_sport_week, size)
			   	 <<  "  " << (v_Ore_sonno[i]-GetMin(v_Ore_sonno, size))/MaxMinusMin(v_Ore_sonno, size)
				 <<  "  " << (v_VAS_scala[i]-GetMin(v_VAS_scala, size))/MaxMinusMin(v_VAS_scala, size)
			      */
				 << endl;
      
    }
  fout.close();
  
  return 0;
}

