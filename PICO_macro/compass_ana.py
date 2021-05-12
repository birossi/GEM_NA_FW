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

def FFT(histo):
    #Compute the transform and look at the magnitude of the output
    hm = None #ROOT.TH1F("hm", "Magnitude of the 1st transform", histo.GetNbinsX()*2, 0, histo.GetNbinsX()*2-1)
    ROOT.TVirtualFFT.SetTransform(0)
    hm = histo.FFT(hm, "MAG M")
    hm.SetNameTitle("hm", "Magnitude of the 1st transform")
    hm.Scale(1/histo.GetNbinsX())

    #Look at the phase of the output
    hp = None #ROOT.TH1F("hp", "Phase of the 1st transform", histo.GetNbinsX()*2, 0, histo.GetNbinsX()*2-1)
    #ROOT.TVirtualFFT.SetTransform(0)
    hp = histo.FFT(hp, "PH M")
    hp.SetNameTitle("hp", "Phase of the 1st transform")

    #Look at the real part of the output
    hre = None #ROOT.TH1F("hre", "Real part of the 1st transform", histo.GetNbinsX()*2, 0, histo.GetNbinsX()*2-1)
    #ROOT.TVirtualFFT.SetTransform(0)
    hre = histo.FFT(hre, "RE M")
    hre.SetNameTitle("hre", "Real part of the 1st transform")

    #Look at the imaginary part of the output
    him = None #ROOT.TH1F("him", "Imaginary part of the 1st transform", histo.GetNbinsX()*2, 0, histo.GetNbinsX()*2-1)
    #ROOT.TVirtualFFT.SetTransform(0)
    him = histo.FFT(him, "IM M")
    him.SetNameTitle("him", "Imaginary part of the 1st transform")

    #Look at the DC component and the Nyquist harmonic:
    #re = array('f', [0])
    #im = array('f', [0])
    re = ROOT.Double(0)
    im = ROOT.Double(0)
    #That's the way to get the current transform object:
    fft = ROOT.TVirtualFFT.GetCurrentTransform()
    #Use the following method to get just one point of the output
    fft.GetPointComplex(0, re, im)
    print("1st transform: DC component: %f\n", re)
    fft.GetPointComplex(histo.GetNbinsX()/2+1, re, im)
    print("1st transform: Nyquist harmonic: %f\n", re)
    #Use the following method to get the full output:
    #re_full = []
    #im_full = []
    #fft.GetPointsComplex(re_full, im_full)
    return hm, hp, hre, him

def graphFFT(in_array):
    size = array('i', [0])
    size[0] = len(in_array)
    n = len(in_array)
    print n
    range_sec = float(n*4E-9) # in seconds
    points = []
    ps = []  #// until Nyquist
    m = 0
    m2 = 0
     
    #//double phase[n/2 + 1];  // until Nyquist
    for i in range(n):
        points.append(in_array[i])
        m += points[-1]
        m2 += points[-1]*points[-1]
     
    s2 = m2/n - m*m/n/n
     
    fft = ROOT.TVirtualFFT.FFT(1, size, "R2C")
    fft.SetPoints(in_array)
    fft.Transform()

    real = array('d', [0])
    imag = array('d', [0])
    g_ps = ROOT.TGraph()
    #g_ps = ROOT.TH1F('h', "Power Spectrum; Frequency [Hz]; Spectral Power", n/2 + 1, 0, n/2)
    for i in range(n/2 + 1):
        re = ROOT.Double(0)
        im = ROOT.Double(0)
        fft.GetPointComplex(i, re, im)
        real.append(re)
        imag.append(im)
        ps.append(re*re + im*im/n/s2) #// Delta chi2, as in Lomb Scargle
        g_ps.SetPoint(i, i/range_sec, ps[-1])
        #g_ps.Fill(int(i/range_sec), ps[-1])
        #print(i, i/range_sec, ps[-1])       
       #//phase[i] = atan2(Im, Re);
        #print real, imag
    g_ps.SetTitle("Power Spectrum; Frequency [Hz]; Spectral Power")
    g_ps.SetLineColor(ROOT.kGreen+2)
    g_ps.SetMarkerColor(ROOT.kGreen+2)
    g_ps.SetLineWidth(2)

    #let's make the anti transform
    #re_full = array('d', [0]*(n/2-1))
    #im_full = array('d', [0]*(n/2-1))
    re_full = array('d', [0]*(n))
    im_full = array('d', [0]*(n))
    fft.GetPointsComplex(re_full, im_full)
    print(len(re_full), len(im_full))
    #real_clean = array('d', [0])
    #imag_clean = array('d', [0])
    fft_back = ROOT.TVirtualFFT.FFT(1, size, "C2R M K")
    fft_back.SetPointsComplex(re_full, im_full)
    fft_back.Transform()
    hb = None
    #//Let's look at the output
    hb = ROOT.TH1.TransformHisto(fft_back, hb, "Re")
    hb.SetTitle("The backward transform result")
    #return g_ps, hb
    return g_ps, real, imag, ps, hb

def clean_antiFFT(real, imag, ps):
    ps_clean = []
    trhd = 1e15
    real_clean = array('d', [0])
    imag_clean = array('d', [0])
    for i in range(len(real)):
        #if ps_value<trhd:
        real_clean.append(real[i])
        imag_clean.append(imag[i])
        #ps_clean.append(ps[i])
    size = array('i', [0])
    size[0] = (len(real_clean)-1)*2
    print len(real_clean), len(imag_clean)
    print size
    #print real_clean, imag_clean
    fft_back = ROOT.TVirtualFFT.FFT(1, size, "C2R M K")
    #fft_back.SetPointsComplex(real_clean, imag_clean)
    fft_back.SetPointsComplex(real, imag)
    fft_back.Transform()
    hb = None
    #//Let's look at the output
    hb = ROOT.TH1.TransformHisto(fft_back, hb, "Re")
    hb.SetTitle("The backward transform result")
    return hb
    
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
