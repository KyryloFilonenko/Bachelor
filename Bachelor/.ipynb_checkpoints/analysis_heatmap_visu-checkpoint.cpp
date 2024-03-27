#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_heatmap_visu()
{

	gROOT->SetBatch(kTRUE);
	int run_number = 974;	

    TFile* root_heat = new TFile("./heatmap/heatmap_data.root", "READ");
    TTree *map = (TTree*)root_heat->Get("outputTree");
        
    TCanvas* canv = new TCanvas("canv", "canv", 1300, 800);
    TH2D* plot = new TH2D("plot", "plot", 31, -0.5, 30.5, 200, 0.0, 600.0);
    gPad->SetLogz();
    plot->SetStats(0);
	

        int num_hits = 0;
        //double average = 0.0;
        double st_dev = 0.0;

        map->SetBranchAddress("num_hits", &num_hits);
        //map->SetBranchAddress("average", &average);
        map->SetBranchAddress("st_dev", &st_dev);    
            
        int entries = map->GetEntries();
        cout << endl << "Entries: " << entries << endl << endl; 
        for (int j = 0; j < entries; ++j) 
        {
            if(j%1000000 == 0)
            {
                cout << j << endl;
            }
            map->GetEntry(j);
            plot -> Fill(num_hits, st_dev);   
            //cout << "num_hits:" << num_hits << endl << "average:" << average << endl << "st_dev:" << st_dev << endl << endl;
        }
                
    plot->Draw("colz");
    canv->Print("./heatmap/heatmap.png");
    delete plot;
    delete canv;
    
    root_heat->Close();
}
