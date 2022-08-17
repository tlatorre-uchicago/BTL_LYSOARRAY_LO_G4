#include <string> 
#include <sstream>
#include <iostream>
#include <fstream>

void photonCount()
{ 
   int start = 100000;
   int end = 1000000;
   int step = 100000;
   int n = (end-start)/step + 1;

   int* energy = new int[n];
   double* norm_cons = new double[n];
   double* lambda = new double[n];
   double* norm_cons_error = new double[n];
   double* lambda_error = new double[n];

   for(int j = 0; j < n; j++)
    {
        std::string filepath = "/mnt/d/DMProject/detector_ouside/electron/10MeV_100MeV_10MeV/"
        //std::string filepath = "/mnt/d/DMProject/detector_ouside/electron/50keV_750keV_50/"
        std::string fileName = filepath + "DetectorOutside" + std::to_string(j) + ".root";
        // open file and define TTree    
        TFile *input = new TFile(fileName.c_str(), "read");
        
        TTree *tree = (TTree*)input->Get("Hits");
        
        int entries = tree->GetEntries();
        
        //Int_t fEvent;
        int fEvent;
        int events[entries];
        int runs = 1000;
        
        //match branches or leaves into variables SetBranchAddress(Branch name, Memory Address)
        tree->SetBranchAddress("fEvent", &fEvent);
            
        // make histogram
        TH1I *h = new TH1I("h", "title", runs, 0, runs);
        
        for (int i=0; i<entries; i++)
        {
            tree->GetEntry(i);
            h->Fill(fEvent);
            //events[i] = fEvent;
            //cout << fEvent << endl;
        } 
        
        //h->Draw(); 
        
        TH1I *pc = new TH1I("pc", "Photon Count", 10, 0, 10);
        
        for (int i=0; i<runs; i++)
        { 
            double bin = h->GetBinContent(i);
            /*
            if (bin > 0)
            {
                pc->Fill(bin);
            }
            */
            pc->Fill(bin);
        }
        
        //pc->Draw();  
        //cout << entries << endl;
        //input->Close();
        
        // fitting to Poisson Distribution    
        TF1 *p = new TF1("f1", "[0]*exp(-[1])*[1]^x/TMath::Gamma(x+1)");
        p->SetParameter(0, 1);
        double mean = pc->GetMean();
        cout << mean << endl;
        p->SetParameter(1, mean);
        
        //TFitResultPtr f = pc->Fit(p, "S");
        auto f = pc->Fit(p, "N");

        //f->Draw();
        
        lambda[j] = p->GetParameter(1);
        norm_cons[j] = p->GetParameter(0);
        lambda_error[j] = p->GetParError(1);
        norm_cons_error[j] = p->GetParError(0);

        energy[j] = start+step*j;

    }

    std::ofstream myfile;
    myfile.open("fit_data.csv");
    //myfile << "Normalization Constant, Normalization Constant Error, Lambda, Lambda Error, Energy\n";
    for (int j=0; j<n; j++)
    {
        std::string data_line = std::to_string(norm_cons[j]) + "," + std::to_string(norm_cons_error[j]) + "," + 
                                std::to_string(lambda[j]) + "," + std::to_string(lambda_error[j]) + "," + 
                                std::to_string(energy[j]) + "\n";  
        
        myfile << data_line;
        //cout << norm_cons[j] << " | "<< lambda[j] << " | " << energy[j] << endl;
    }
    myfile.close();
    
}
