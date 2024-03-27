#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_heatmap_root()
{

	gROOT->SetBatch(kTRUE);
	
	int run_number = 974;	

	int num;
	//cout << "How many folders do you want to analyse?" << endl;
	//cin >> num;
    num = 133;
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
        
        TFile* outputRoot = new TFile("./heatmap/heatmap_data.root", "RECREATE");
        TTree* outputTree = new TTree("outputTree", "Output Tree");
            
        // Declare variables to be written to the output tree
        int num_hits_out;
        double average_out, st_dev_out;
            
        // Create branches in the output tree
        outputTree->Branch("num_hits", &num_hits_out, "num_hits/I");
        outputTree->Branch("average", &average_out, "average/D");
        outputTree->Branch("st_dev", &st_dev_out, "st_dev/D");
            
		
		for(int i = START; i < num; i++)
		{
			root[i] = new TFile(Form("../../%d/root_%d.root", i, i), "read");
			data[i] = (TTree*) root[i]->Get("data");
			
			TKEvent* event = new TKEvent();
			data[i]->SetBranchAddress("TKEvent", &event);
			
			int entries = data[i]->GetEntries();
			cout << endl;
			cout << "Folder " << i << endl << "Entries in this folder:" << entries << endl << endl;
			
			double likelihood, num_hits;
     
			for(int j = 0; j < entries; j++)
            //for(int j = 0; j < 100; j++)
			{
				data[i]->GetEntry(j);
				
				if(j%50000 == 0)
				{
					cout << "Event " << i * 100000 + j << endl;
				}
                
				for(int k = 0; k < event->get_tracks().size(); k++)
				{
					num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
             
                    double radius[int(num_hits)];
                    for(int p = 0; p < num_hits; p++)
                    {
                        radius[p] = event->get_tr_hits().at(p)->get_r();
                    }
                    sort(radius, radius + int(num_hits));
                    
                    for(int l = 0; l < num_hits; l++)
                    {
                       double sum = 0.0;
                       for(int p = 0; p <= l; p++)
                       {
                           sum += radius[p];
                       }
                       double average = sum / (l + 1);
                        
                       double s_sum = 0.0;
                       for(int p = 0; p <= l; p++)
                       {
                            s_sum += pow(average - radius[p], 2); 
                       }
                       double st_dev = s_sum / (l + 1);
                        
                       num_hits_out = l + 1;
                       average_out = average;
                       st_dev_out = st_dev;
                    
                       outputTree->Fill();
                    }
				}
			}			
			root[i]->Close();
		}
        outputRoot->Write();
        outputRoot->Close();
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
