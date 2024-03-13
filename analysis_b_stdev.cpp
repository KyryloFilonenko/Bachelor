#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_b_stdev()
{

	gROOT->SetBatch(kTRUE);
	
	int run_number = 974;	

	int num;
	//cout << "How many folders do you want to analyse?" << endl;
	//cin >> num;
    num = 1;
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
		
		for(int i = START; i < num; i++)
		{
			root[i] = new TFile(Form("../%d/root_%d.root", i, i), "read");
			data[i] = (TTree*) root[i]->Get("data");
			
			TKEvent* event = new TKEvent();
			data[i]->SetBranchAddress("TKEvent", &event);
			
			int entries = data[i]->GetEntries();
			cout << endl;
			cout << "Entries in this folder:" << entries << endl << endl;
			
			double likelihood, num_hits;
			
			//for(int j = 0; j < entries; j++)
            for(int j = 0; j < 1000; j++)
			{
				data[i]->GetEntry(j);
				
				if(j%100 == 0)
				{
					cout << "Event " << j << endl;
				}
                
				for(int k = 0; k < event->get_tracks().size(); k++)
				{
					num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
                    TCanvas* Canvas = new TCanvas("Canvas", "Canvas", 1200, 600);
                    //TH2D* plot = new TH2D("plot", "plot", 16, -0.5, 15.5, 10000, -1.0, 100.0);
                    
                    double x[int(num_hits)];
                    double y[int(num_hits)];
                    
                    for(int l = 0; l < num_hits; l++)
                    {
                           double radius[l+1];
                           for(int p = 0; p <= l; p++)
                           {
                                radius[p] = event->get_tr_hits().at(p)->get_r();
                               //cout << radius[p] << "+" << endl;
                           }
                           sort(radius, radius + l+1);
                        
                           double sum = 0.0;
                           for(int p = 0; p <= l; p++)
                           {
                                sum += radius[p];
                               //cout << radius[p] << endl;
                           }
                           double average = sum / (l + 1);
                        
                           double s_sum = 0.0;
                           for(int p = 0; p <= l; p++)
                           {
                                s_sum += pow(average - radius[p], 2); 
                           }
                           double st_dev = s_sum / (l + 1);
                        //cout << st_dev << endl;
                           //plot -> Fill(l+1, st_dev);
                        x[l] = l + 1;
                        y[l] = st_dev;
                        //cout << "l " << l+1 << "   " << st_dev << endl;
                    }
                    TGraph *graph = new TGraph(num_hits, x, y);
                    
                    graph->SetLineColor(kRed);
                    graph->SetMarkerColor(kBlue);
                    graph->SetMarkerStyle(21); // Circular markers

                    graph->Draw("AP"); // "A" to draw axes, "P" to draw markers

                    graph->GetXaxis()->SetTitle("Number of hits");
                    graph->GetYaxis()->SetTitle("St_dev");
                    graph->SetTitle(Form("Event %d, track %d", j, k+1));

                    Canvas->Update();
                    
                    //plot->Draw("*");
                    Canvas->Print(Form("./b_stdev/%d_%d.png", j, k+1));
                    //delete plot;
                    delete graph;
                    delete Canvas;
				}
				
				//event->make_top_projection(2);
				//event->build_event();
			}
			
			root[i]->Close();
		}
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}

