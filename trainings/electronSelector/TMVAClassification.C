// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"


#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void TMVAClassification( TString myMethodList = "" )
{
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   //
   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)

   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // to get access to the GUI and all tmva macros
    TString tmva_dir(TString(gRootDir) + "/tmva");
    if(gSystem->Getenv("TMVASYS"))
       tmva_dir = TString(gSystem->Getenv("TMVASYS"));
    gROOT->SetMacroPath(tmva_dir + "/test/:" + gROOT->GetMacroPath() );
    gROOT->ProcessLine(".L TMVAGui.C");


   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 0; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 0;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 0; // uses Adaptive Boost
   Use["BDTG"]            = 1; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting 
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is 
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Auto" );
   TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset_electronsel");
   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
//   factory->AddVariable( "pmu", "P_{track0}", "GeV/c", 'D' );
//   factory->AddVariable( "thmu", "#theta_{track0}", "degrees", 'D' );
//   factory->AddVariable( "thpro", "#theta_{track1}", "degrees", 'D' );
//   factory->AddVariable( "dedxpro", "dE/dx_{track1}", "MeV/cm", 'D' );


// branch, name, unit, type

   //All pandrizzle variables
   /*
   dataloader->AddVariable("Displacement","Displacement",'F');
   dataloader->AddVariable("dEdxBestPlane","dE/dx (best plane)",'F');
   dataloader->AddVariable("DCA","Distance of closest approach", 'F');
   dataloader->AddVariable("Wideness","Wideness", 'F');
   dataloader->AddVariable("EnergyDensity","Energy density", 'F');
   dataloader->AddVariable("Concentration","Concentration",'F');
   dataloader->AddVariable("CoreHaloRatio","Core halo ratio", 'F');
   dataloader->AddVariable("Conicalness","Conicalness",'F');
   dataloader->AddVariable("EvalRatio","Eval ratio",'F');
   */

   // Modular shower training (also set 25 hit cut)
   /*
   dataloader->AddVariable("EvalRatio","Eval ratio",'F');
   dataloader->AddVariable("CoreHaloRatio","Core halo ratio", 'F');
   dataloader->AddVariable("Conicalness","Conicalness",'F');
   dataloader->AddVariable("dEdxBestPlane","dE/dx (best plane)",'F');
   dataloader->AddVariable("DCA","Distance of closest approach", 'F');
   dataloader->AddVariable("Wideness","Wideness", 'F');
   dataloader->AddVariable("EnergyDensity","Energy density", 'F');
   dataloader->AddVariable("ModularShowerPathwayLengthMin", "ModularShowerPathwayLengthMin", 'F');
   dataloader->AddVariable("ModularShowerMaxNShowerHits", "ModularShowerMaxNShowerHits", 'F');
   dataloader->AddVariable("ModularShowerMaxNuVertexChargeWeightedMeanRadialDistance", "ModularShowerMaxNuVertexChargeWeightedMeanRadialDistance", 'F');
   */

   // All Modular shower variables
   
   //dataloader->AddVariable("ModularShowerPathwayLengthMin", "ModularShowerPathwayLengthMin", 'F');
   //dataloader->AddVariable("ModularShowerPathwayKink3D", "ModularShowerPathwayKink3D", 'F');
   //dataloader->AddVariable("ModularShowerMaxNShowerHits", "ModularShowerMaxNShowerHits", 'F');
   //dataloader->AddVariable("ModularShowerMaxScatterAngle", "ModularShowerMaxScatterAngle", 'F');
   //dataloader->AddVariable("ModularShowerMaxNuVertexChargeAsymmetry", "ModularShowerMaxNuVertexChargeAsymmetry", 'F');
   //dataloader->AddVariable("ModularShowerMaxShowerStartChargeAsymmetry", "ModularShowerMaxShowerStartChargeAsymmetry", 'F');
   //dataloader->AddVariable("ModularShowerMaxNuVertexChargeWeightedMeanRadialDistance", "ModularShowerMaxNuVertexChargeWeightedMeanRadialDistance", 'F');
   //dataloader->AddVariable("ModularShowerMinShowerStartMoliereRadius", "ModularShowerMinShowerStartMoliereRadius", 'F');
   //dataloader->AddVariable("ModularShowerMaxOpeningAngle", "ModularShowerMaxOpeningAngle", 'F');
   //dataloader->AddVariable("ModularShowerMaxFoundHitRatio", "ModularShowerMaxFoundHitRatio", 'F');
   //dataloader->AddVariable("ModularShowerProjectedInitialGapSize", "ModularShowerProjectedInitialGapSize", 'F');
   //dataloader->AddVariable("ModularShowerGlobalInitialGapSize", "ModularShowerGlobalInitialGapSize", 'F');
   //dataloader->AddVariable("ModularShowerLargestProjectedGapSize", "ModularShowerLargestProjectedGapSize", 'F');
   

   // Enhanced Pandrizzle Variables

   dataloader->AddVariable("EvalRatio","Eval ratio",'F');
   dataloader->AddVariable("Concentration","Concentration",'F');
   dataloader->AddVariable("CoreHaloRatio","Core halo ratio", 'F');
   dataloader->AddVariable("Conicalness","Conicalness",'F');
   dataloader->AddVariable("dEdxBestPlane","dE/dx (best plane)",'F');
   dataloader->AddVariable("Displacement","Displacement",'F');
   dataloader->AddVariable("DCA","Distance of closest approach", 'F');
   dataloader->AddVariable("Wideness","Wideness", 'F');
   dataloader->AddVariable("EnergyDensity","Energy density", 'F');
   dataloader->AddVariable("PathwayLengthMin", "PathwayLengthMin", 'F'); 
   dataloader->AddVariable("MaxShowerStartPathwayScatteringAngle2D", "MaxShowerStartPathwayScatteringAngle2D", 'F');
   dataloader->AddVariable("MaxNPostShowerStartHits", "MaxNPostShowerStartHits", 'F');
   dataloader->AddVariable("MaxPostShowerStartScatterAngle", "MaxPostShowerStartScatterAngle", 'F');
   dataloader->AddVariable("MaxPostShowerStartNuVertexEnergyAsymmetry", "MaxPostShowerStartNuVertexEnergyAsymmetry", 'F');
   dataloader->AddVariable("MaxPostShowerStartShowerStartEnergyAsymmetry", "MaxPostShowerStartShowerStartEnergyAsymmetry", 'F');
   dataloader->AddVariable("MaxPostShowerStartNuVertexEnergyWeightedMeanRadialDistance", "MaxPostShowerStartNuVertexEnergyWeightedMeanRadialDistance", 'F');
   dataloader->AddVariable("MinPostShowerStartShowerStartMoliereRadius", "MinPostShowerStartShowerStartMoliereRadius", 'F');
   dataloader->AddVariable("MaxPostShowerStartOpeningAngle", "MaxPostShowerStartOpeningAngle", 'F');
   dataloader->AddVariable("MaxFoundHitRatio", "MaxFoundHitRatio", 'F');
   dataloader->AddVariable("MaxInitialGapSize", "MaxInitialGapSize", 'F');
   dataloader->AddVariable("MinLargestProjectedGapSize", "MinLargestProjectedGapSize", 'F');
   dataloader->AddVariable("NViewsWithAmbiguousHits", "NViewsWithAmbiguousHits", 'F');
   dataloader->AddVariable("AmbiguousHitMaxUnaccountedEnergy", "AmbiguousHitMaxUnaccountedEnergy", 'F');

   /////
   /*
   dataloader->AddVariable("PathwayLengthMin", "PathwayLengthMin", 'F');
   dataloader->AddVariable("MaxShowerStartPathwayScatteringAngle2D", "MaxShowerStartPathwayScatteringAngle2D", 'F');
   dataloader->AddVariable("PathwayMaxEnergySigma", "PathwayMaxEnergySigma", 'F');
   dataloader->AddVariable("MaxNPostShowerStartHits", "MaxNPostShowerStartHits", 'F');
   dataloader->AddVariable("MaxPostShowerStartScatterAngle", "MaxPostShowerStartScatterAngle", 'F');
   dataloader->AddVariable("MaxPostShowerStartNuVertexEnergyAsymmetry", "MaxPostShowerStartNuVertexEnergyAsymmetry", 'F');
   dataloader->AddVariable("MaxPostShowerStartShowerStartEnergyAsymmetry", "MaxPostShowerStartShowerStartEnergyAsymmetry", 'F');
   dataloader->AddVariable("MaxPostShowerStartNuVertexEnergyWeightedMeanRadialDistance", "MaxPostShowerStartNuVertexEnergyWeightedMeanRadialDistance", 'F');
   dataloader->AddVariable("MinPostShowerStartShowerStartMoliereRadius", "MinPostShowerStartShowerStartMoliereRadius", 'F');
   dataloader->AddVariable("MaxOpeningAngleW", "MaxOpeningAngleW", 'F');
   dataloader->AddVariable("InitialRegionDistanceToNuVertex", "InitialRegionDistanceToNuVertex", 'F');
   dataloader->AddVariable("NViewsWithAmbiguousHits", "NViewsWithAmbiguousHits", 'F');
   dataloader->AddVariable("AmbiguousHitMaxUnaccountedEnergy", "AmbiguousHitMaxUnaccountedEnergy", 'F');
   */
   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables
//   factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
//   factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );

   // Read training and test data
   // (it is also possible to use ASCII format as input -> see TMVA Users Guide)

   //TString fname = "/dune/data/users/dbrailsf/oscillation/nu_mu/files/mcc11/seltrees/v07_06_02/A/ccnu/pandizzle/pantree.root";
   //TString fname = "/dune/app/users/dbrailsf/oscillation/nu_mu/cutsel/trainings/electronSelector/treeMaker/outputFile_recoopflux.root";
   //TString fname = "/dune/data/users/dbrailsf/oscillation/nu_mu/files/mcc11/seltrees/v07_06_02/A/ccnu/ccnutree_hadd.root";
   //TString fname = "/dune/app/users/imawby/hybridElectronPlusNuVertexShiftDisplacement/pandrizzleTrainingTree.root";
   //TString fname = "/dune/app/users/imawby/selection/trainings/electronSelector/treeMaker/pandrizzleTrainingTree.root";
   //TString fname = "/dune/app/users/imawby/hybridElectronPlusNuVertex_Matching/ccnutree_FHC_hybridElectronPlusNuVertex_Matching.root";
   //TString fname = "/dune/data/users/imawby/cheatShowersShiftedVertexPlusNuVertex/ccnutree_FHC_cheatPandoraShiftedVisibleVertexElectronGammaPlusNuVertex_POT.root";
   //TString fname = "/dune/data/users/imawby/standard/ccnutree_FHC_standard_POT.root";
   TString fname = "/dune/data/users/imawby/reclustering/ccnutree_reclustering_train.root";

   //TString fname = "/dune/data/users/imawby/realRecoElectronStandardNuVertex_01/ccnutree_FHC_realRecoElectronStandardNuVertex25ModularShowerVars_01_POT.root";


   if (gSystem->AccessPathName( fname ))  // file does not exist in local directory
      gSystem->Exec("wget http://root.cern.ch/files/tmva_class_example.root");
   
   TFile *input = TFile::Open( fname );
   
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;
   
   // --- Register the training and test trees

   //TTree *signal     = (TTree*)input->Get("sigtree");
   //TTree *background = (TTree*)input->Get("bgtree");

   TTree *signal     = (TTree*)input->Get("ccnuselection/DrizzleSigShowerTree");
   TTree *background = (TTree*)input->Get("ccnuselection/DrizzleBgShowerTree");
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;
   
   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree    ( signal,     signalWeight     );
   dataloader->AddBackgroundTree( background, backgroundWeight );
   
   // To give different trees for training and testing, do as follows:
   //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
   //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );
   
   // Use the following code instead of the above two or four lines to add signal and background
   // training and test events "by hand"
   // NOTE that in this case one should not give expressions (such as "var1+var2") in the input
   //      variable definition, but simply compute the expression before adding the event
   //
   //     // --- begin ----------------------------------------------------------
   //     std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
   //     Float_t  treevars[4], weight;
   //     
   //     // Signal
   //     for (UInt_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<signal->GetEntries(); i++) {
   //        signal->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < signal->GetEntries()/2.0) factory->AddSignalTrainingEvent( vars, signalWeight );
   //        else                              factory->AddSignalTestEvent    ( vars, signalWeight );
   //     }
   //   
   //     // Background (has event weights)
   //     background->SetBranchAddress( "weight", &weight );
   //     for (UInt_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<background->GetEntries(); i++) {
   //        background->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
   //        else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
   //     }
         // --- end ------------------------------------------------------------
   //
   // --- end of tree registration 

   // Set individual event weights (the variables must exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
   //factory->SetSignalWeightExpression( "weight" );
   //factory->SetBackgroundWeightExpression( "weight" );
   //factory->SetSignalWeightExpression( "projected_weight" );
   //factory->SetBackgroundWeightExpression( "projected_weight" );
   //factory->SetSignalWeightExpression( "projected_pot*energy_weight" );
   //factory->SetBackgroundWeightExpression( "projected_pot*energy_weight" );



   // Apply additional cuts on the signal and background samples (can be different)
   //TCut mycuts = "PFPNHits > 20"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   //TCut mycuts = "PFPNHits > 100"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycuts = "(PFPNHits > 100) && (AmbiguousHitMaxUnaccountedEnergy > -99)"; //FoundConnectionPathway && (ConnectionBDTScore > -1.0)";
   //TCut mycuts = "(PFPNHits > 25) && (FoundTrackStub == 1) && (AmbiguousHitMaxUnaccountedEnergy < -99)"; // Modular shower cuts
   //TCut mycuts = "";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used 
   // for training, and the other half for testing:
   dataloader->PrepareTrainingAndTestTree( mycuts, mycuts, "SplitMode=random:!V" );
   //dataloader->PrepareTrainingAndTestTree( mycuts, mycuts, "nTrain_Signal=6737:nTrain_Background=13430:nTest_Signal=6737:nTest_Background=13430:SplitMode=random:!V" );

   // To also specify the number of testing events, use:
   //factory->PrepareTrainingAndTestTree( mycuts,mycutb,
   //					"nTrain_Signal=1500:nTrain_Background=1500:nTest_Signal=498:nTest_Background=498:SplitMode=Random:!V" );
   //factory->PrepareTrainingAndTestTree( mycuts, mycutb,
   //"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod(dataloader,  TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod(dataloader,  TMVA::Types::kCuts, "CutsD",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod(dataloader,  TMVA::Types::kCuts, "CutsPCA",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod(dataloader,  TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

   if (Use["CutsSA"])
      factory->BookMethod(dataloader,  TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   // Likelihood ("naive Bayes estimator")
   if (Use["Likelihood"])
      factory->BookMethod(dataloader,  TMVA::Types::kLikelihood, "Likelihood",
                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

   // Decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod(dataloader,  TMVA::Types::kLikelihood, "LikelihoodD",
                           "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

   // PCA-transformed likelihood
   if (Use["LikelihoodPCA"])
      factory->BookMethod(dataloader,  TMVA::Types::kLikelihood, "LikelihoodPCA",
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 

   // Use a kernel density estimator to approximate the PDFs
   if (Use["LikelihoodKDE"])
      factory->BookMethod(dataloader,  TMVA::Types::kLikelihood, "LikelihoodKDE",
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // Use a variable-dependent mix of splines and kernel density estimator
   if (Use["LikelihoodMIX"])
      factory->BookMethod(dataloader,  TMVA::Types::kLikelihood, "LikelihoodMIX",
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

   // Test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
   if (Use["PDERS"])
      factory->BookMethod(dataloader,  TMVA::Types::kPDERS, "PDERS",
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod(dataloader,  TMVA::Types::kPDERS, "PDERSD",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod(dataloader,  TMVA::Types::kPDERS, "PDERSPCA",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod(dataloader,  TMVA::Types::kPDEFoam, "PDEFoam",
                           "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

   if (Use["PDEFoamBoost"])
      factory->BookMethod(dataloader,  TMVA::Types::kPDEFoam, "PDEFoamBoost",
                           "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod(dataloader,  TMVA::Types::kKNN, "KNN",
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod(dataloader,  TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod(dataloader,  TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod(dataloader,  TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod(dataloader,  TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod(dataloader,  TMVA::Types::kFisher, "BoostedFisher", 
                           "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod(dataloader,  TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod(dataloader,  TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

   if (Use["MLPBFGS"])
      factory->BookMethod(dataloader,  TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBNN"])
      factory->BookMethod(dataloader,  TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod(dataloader,  TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  

   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      //factory->BookMethod(dataloader,  TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
      factory->BookMethod(dataloader,  TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=1000:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.01"  ); // n_cycles:#nodes:#nodes:...

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod(dataloader,  TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDTG",
                           //"!H:!V:NTrees=2000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
			   "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTB"]) // Bagging
      factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDTB",
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

   if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
      factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDTMitFisher",
                           "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod(dataloader,  TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // For an example of the category classifier usage, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   // factory->OptimizeAllMethods("ROCIntegral","FitGA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;

   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );
}
