import ROOT
from utils import *
from array import array
import time

chain = ROOT.TChain('Data_F')
chain.Add('Compass_run_13/FILTERED/compassF_run_13_1.root')

mWaveform = ROOT.TArrayS()
#TArrayS *mWaveformPnt = &mWaveform;
#chainSetBranchAddress("Samples", &mWaveformPnt);
ch0 = ROOT.TH1F("ch0", "ch0", 5240, 0, 5239)
int_ch0 = ROOT.TH1F("int_ch0", "int_ch0", 1000, -100, 1943051*2)
ch1 = ROOT.TH1F("ch1", "ch1", 5240, 0, 5239)
int_ch1 = ROOT.TH1F("int_ch1", "int_ch1", 1000, -100, 1943051*2)

    
for i in range(1):
#for i in range(chain.GetEntries()):
    chain.GetEvent(i)
    #data_array = [] #
    data_array = array('d', [0]*5240)
    if(chain.Channel==0):
        for j in range(len(chain.Samples)):
        #print(chain.Samples[j])
            ch0.SetBinContent(j, chain.Samples[j])
            #data_array.append(chain.Samples[j])
            data_array[j] = chain.Samples[j]
        hm, hp, hre, him = FFT(ch0)
        print_hist('.', ch0, 'waveform_ch'+str(chain.Channel)+'_evt'+str(i), option = "", log = False, stack = False, title = "")
        print_hist('.', hm.Clone(), 'mag_1st_transform_ch'+str(chain.Channel)+'evt'+str(i), "HIST", True)
        print_hist('.', hp.Clone(), 'phase_1st_transform_ch'+str(chain.Channel)+'evt'+str(i), "HIST", True)
        print_hist('.', hre.Clone(), 'real_part_1st_transform_ch'+str(chain.Channel)+'evt'+str(i), "HIST", True)
        print_hist('.', him.Clone(), 'imaginary_part_1st_transform_ch'+str(chain.Channel)+'evt'+str(i), "HIST", True)
        print('Integral of waveform no. %i is %f' %(i, ch0.Integral()))
        int_ch0.Fill(ch0.Integral(990,1500))
        #graph_ps, back_transf = graphFFT(data_array)
        graph_ps, real, imag, ps, hb = graphFFT(data_array)
        print_hist('.', graph_ps, 'graph_transform_evt'+str(i), "HIST", True)
        print_hist('.', hb, 'first_backward_transform_ch'+str(chain.Channel)+'_evt'+str(i), option = "HIST", log = True)
        back_transf = clean_antiFFT(real, imag, ps)
        print_hist('.', back_transf, 'backward_transform_ch'+str(chain.Channel)+'_evt'+str(i), option = "HIST", log = True)

    if(chain.Channel==1):
        for j in range(len(chain.Samples)):
        #print(chain.Samples[j])
            ch1.SetBinContent(j, chain.Samples[j])
        #print(ch1.Integral(990,1500))
        int_ch1.Fill(ch1.Integral(990,1500))
    #print(chain.Samples[0])
    #print(len(chain.Samples))
    hm.Delete()
    hp.Delete()
    hre.Delete()
    him.Delete()
    '''
    #Following the ROOT tutorial https://root.cern/doc/master/FFT_8C.html
    #A function to sample
    #fsin = ROOT.TF1("fsin", "sin(x)+sin(2*x)+sin(0.5*x)+1", 0, 4*ROOT.TMath::Pi())
    fsin = ROOT.TF1("fsin", "sin(x)", 0, 4*ROOT.TMath::Pi())
    fsin.Draw()
    print_hist('.', int_ch1, 'prova_int_carica_ch1')
    
    n = 25
    hsin = ROOT.TH1D("hsin", "hsin", n+1, 0, 4*ROOT.TMath::Pi())
    
    #Fill the histogram with function values
    for i in range(n):
       x = (i/n)*(4*ROOT.TMath::Pi())
       hsin.SetBinContent(i+1, fsin.Eval(x))
    hsin.Draw("same")
    print_hist('.', int_ch1, 'prova_int_carica_ch1')
    fsin.GetXaxis().SetLabelSize(0.05);
    fsin.GetYaxis().SetLabelSize(0.05);
    '''

print_hist('.', int_ch0, 'prova_int_carica')
print_hist('.', ch1, 'prova_TArrayS_ch1', option = "", log = False, stack = False, title = "")
print_hist('.', int_ch1, 'prova_int_carica_ch1')
