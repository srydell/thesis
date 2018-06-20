#include <iostream>
#include "TROOT.h"

using namespace std;

void getrandomh() {
   TH1F *source = new TH1F("source","source hist",100,-3,3);
   source->FillRandom("gaus",1000);
   TH1F *final_hist = new TH1F("final_hist","final_hist hist",100,-3,3);

             // continued...

   for (Int_t i=0;i<10000;i++) {
      final_hist->Fill(source->GetRandom());
   }
   TCanvas *c1 = new TCanvas("c1","c1",800,1000);
   c1->Divide(1,2);
   c1->cd(1);
   source->Draw();
   c1->cd(2);
   final_hist->Draw();
   c1->cd();
}

void myMacro(){
	getrandomh();
}
