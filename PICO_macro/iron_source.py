import ROOT
from array import array

ROOT.gROOT.SetBatch()
#ROOT.gStyle.SetOptFit(1011)
log = True
fileout = ROOT.TFile("Iron_source.root", "RECREATE")
V_g1t = array('f', [400, 395, 390, 385, 380, 375, 370, 365, 360, 355, 350, 345, 340, 335, 330, 325, 320, 315, 310, 305, 300, 295, 290, 285, 280, 275, 270, 265, 260, 250])#, 240, 200,225,235,180,150,175,100,125,75,50])
I_g1t = array('f', [0.033, 0.026, 0.0235, 0.02175, 0.0185, 0.0165, 0.015, 0.013, 0.01175, 0.0110, 0.00975, 0.0085, 0.00725, 0.00625, 0.00585, 0.00510, 0.00425, 0.00375, 0.00325, 0.00280, 0.00225, 0.0020, 0.0017, 0.0014, 0.0011, 0.0009, 0.00075, 0.0006, 0.0003, 0.000])#, -0.0005, -0.0003, -0.00015, -0.0008, -0.0009, -0.0007, -0.001, -0.0009, -0.001, -0.001])
V_g2t = array('f', [400, 395, 385, 370, 360, 350, 330, 310, 290, 270])
I_g2t = array('f', [0.0020, 0.0020, 0.0015, 0.00125, 0.0012, 0.001, 0.0008, 0.0007, 0.0006, 0.0005])

Err_Vg1t = array('f', [3]*len(V_g1t))
Err_Ig1t = array('f', [abs(i)*0.05 for i in I_g1t])
#print Err_Vg1t, Err_Ig1t

Err_Vg2t = array('f', [3]*len(V_g2t))
Err_Ig2t = array('f', [abs(i)*0.05 for i in I_g2t])
#print Err_Vg2t, Err_Ig2t

g_t1 = ROOT.TGraphErrors(len(V_g1t), V_g1t, I_g1t, Err_Vg1t, Err_Ig1t)
g_t1.SetName("G1T")
g_t1.SetTitle("Pico current monitor with the source; V_{G1T}[V]; I_{G1T}[uA]")
g_t1.SetLineColor(ROOT.kBlack)
g_t1.SetMarkerColor(ROOT.kBlack)
func_t1 = ROOT.TF1("func_t1", "[0]*x^[1]", 270, 400)
func_t1.SetParameters(3.4292E-27, 9.6048)
g_t1.Write()
g_t1.Fit(func_t1)
par1, par2 = func_t1.GetParameter(0), func_t1.GetParameter(1)
c1 = ROOT.TCanvas("g_t1", "c1", 50,50,700,600)
g_t1.Draw("AEP")
func_t1.Draw("SAME")
latex = ROOT.TLatex()
latex.SetTextSize(0.045);
latex.SetTextAlign(13)
latex.DrawLatex(320, max(I_g2t)/2,"I = %.3e*V^{%.3f}" %(par1, par2))
if log:
    c1.SetLogy(1)
c1.Print("g_t1_iron.png")
c1.Print("g_t1_iron.root")

g_t2 = ROOT.TGraphErrors(len(V_g2t), V_g2t, I_g2t, Err_Vg2t, Err_Ig2t)
g_t2.SetName("G2T")
g_t2.SetTitle("Pico current monitor with the source; V_{G2T}[V]; I_{G2T}[uA]")
g_t2.SetLineColor(ROOT.kBlue)
g_t2.SetMarkerColor(ROOT.kBlue)
func_t2 = ROOT.TF1("func_t2", "[0]*x^[1]", 270, 400)
func_t2.SetParameters(2.19031e-12, 3.42068)
g_t2.Write()
g_t2.Fit(func_t2)
par1, par2 = func_t2.GetParameter(0), func_t2.GetParameter(1)
c2 = ROOT.TCanvas("g_t2", "c1", 50,50,700,600)
g_t2.Draw("AEP")
func_t2.Draw("SAME")
latex = ROOT.TLatex()
latex.SetTextSize(0.045);
latex.SetTextAlign(13)
latex.DrawLatex(320, max(I_g2t)/4,"I = %.3e*V^{%.3f}" %(par1, par2))
if log:
    c2.SetLogy(1)
c2.Print("g_t2_iron.png")
c2.Print("g_t2_iron.root")

fileout.Close()

'''
  TF2 *v_pow = new TF2("v_pow", "expo", 320, 450);
  TCanvas* c_tt = new TCanvas("C_tt", "PICO output G3Top", 800, 550);
  g_t2.Draw("AEP");
  g_t2.Write();
'''
