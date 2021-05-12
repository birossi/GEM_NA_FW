#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>

using namespace std;

void datatotree(TString filename) {

  ifstream in;
  Int_t time = 0;
  Int_t index =0;
  Float_t v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0, v7 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0;
  Int_t nlines = 0;
  in.open(filename+".txt"); // .txt
  TFile *f = new TFile(filename+".root","RECREATE");

  TTree* t1 = new TTree("t1", "t1");
  t1->Branch("Index",    &index, "Index/I");
  t1->Branch("Time",    &time, "Time/I");
  t1->Branch("V_G3B",   &v1,   "V_G3B/F");
  t1->Branch("I_G3B",   &i1,   "I_G3B/F");
  t1->Branch("V_G3T",   &v2,   "V_G3T/F");
  t1->Branch("I_G3T",   &i2,   "I_G3T/F");
  t1->Branch("V_G2B",   &v3,   "V_G2B/F");
  t1->Branch("I_G2B",   &i3,   "I_G2B/F");
  t1->Branch("V_G2T",   &v4,   "V_G2T/F");
  t1->Branch("I_G2T",   &i4,   "I_G2T/F");
  t1->Branch("V_G1B",   &v5,   "V_G1B/F");
  t1->Branch("I_G1B",   &i5,   "I_G1B/F");
  t1->Branch("V_G1T",   &v6,   "V_G1T/F");
  t1->Branch("I_G1T",   &i6,   "I_G1T/F");
  t1->Branch("V_drift", &v7,   "V_drift/F");
  t1->Branch("I_drift", &i7,   "I_drift/F");


  //  TNtuple *ntuple = new TNtuple("Data","Data","time:VoltageCH1:VoltageCH2:VoltageCH3:VoltageCH4:VoltageCH5:VoltageCH6:VoltageCH7:CurrentCH1:CurrentCH2:CurrentCH3:CurrentCH4:CurrentCH5:CurrentCH6:CurrentCH7");
  /*
  while( in && std::getline( in, nlines ) )
    {
      ++line_number;
      
      if( line.find(search_str) != std::string::npos )
        {
	  std::cout << "line " << line_number << ": " << line << '\n' ;
	  std::cout << ++line_number; // read the next line too
	  std::getline(file, line);  // then do whatever you want.
        }
      if(line.good()){
	// line is read stored correctly and you can use it
      }
      else{
	// you are at end of the file and line is not read
	break;
      }
    }
  */
  index =0;
  while(1){
    if (!in.good()) break;
    //cout << "ciao" << endl;
    in >> time >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7;
    //cout << time << "||" << v1 << "||" << v2 << "||" << v3 << "||" << v4 << "||" << v5 << "||" << v6 << "||" << v7 << "||" << endl;
    nlines++;
    in >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7;
 
    t1->Fill();//time, v1, v2, v3, v4, v5, v6, v7, i1, i2, i3, i4, i5, i6, i7);
    nlines++;

    index++;
  }

  //t1->Close();
   f->Write();
   f->Close();
}
