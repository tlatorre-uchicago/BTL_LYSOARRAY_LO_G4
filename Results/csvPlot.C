void csvPlot()
{
    TCanvas *c = new TCanvas("c", "Energy vs Mean", 1280, 720);
    c->SetLogx();
    TTree t;
    t.ReadFile("fit_data.csv", "NormCons/D:NormConsError:Lambda:LambdaError:Energy/I", ',');
    t.Draw("Energy : Lambda : LambdaError", "", "goff");
    
    //TGraph *g = new TGraph(t.GetSelectedRows(), t.GetV1(), t.GetV2());
    TGraphErrors *g = new TGraphErrors(t.GetEntries(), t.GetV1(), t.GetV2(), 0, t.GetV3());
    g->SetTitle("Photon Count Over Electron Energy");
    g->GetXaxis()->SetTitle("Energy (keV)");
    g->GetXaxis()->CenterTitle(true);
    g->GetXaxis()->SetLabelOffset(0.);
    g->GetYaxis()->SetTitle("Photon Count");
    g->GetYaxis()->CenterTitle(true);
    g->SetMarkerStyle(1);
    g->Draw("ap");  
}
