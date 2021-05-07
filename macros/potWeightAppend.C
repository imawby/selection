#include <vector>
void appendPOTBranch(TTree *tree, double pot_nu, double pot_nue, double pot_nutau, double projected_pot=1.){

  double pot = 0;
  double projected_pot_weight = 0;

  //Stuff that will get put in the tree
  TBranch *branch_pot = tree->Branch("POT",&pot);
  TBranch *branch_projected_pot = tree->Branch("ProjectedPOT",&projected_pot);
  TBranch *branch_projected_pot_weight = tree->Branch("ProjectedPOTWeight",&projected_pot_weight);

  //variables that we need to use to figure out what the POT should be for a particular event
  int run;
  int nc;

  tree->SetBranchAddress("Run",&run);
  tree->SetBranchAddress("NC",&nc);

  for (Long64_t entry = 0; entry < tree->GetEntries(); entry++){
    tree->GetEntry(entry);
    if (nc == 1) pot = pot_nu+pot_nue+pot_nutau; //NC evnt //ISOBEL
    else if (run==20000001) pot = pot_nu; //CC event from nu file
    else if (run==20000002) pot = pot_nue; //CC event from nue file
    else if (run==20000003) pot = pot_nutau; //CC event from nutau file
    else if (run==20000004) pot = pot_nu; //CC event from nu file
    else if (run==20000005) pot = pot_nue; //CC event from nue file
    else if (run==20000006) pot = pot_nutau; //CC event from nutau file

    else{
      std::cout<<"Found unknown run number: " << run << " setting POT to 1" << std::endl;
      pot = 1.;
    }
    projected_pot_weight = (projected_pot/pot);

    branch_pot->Fill();
    branch_projected_pot->Fill();
    branch_projected_pot_weight->Fill();
  }

  /*
  float pot = 0;
  float projected_weight = 0;

  TBranch *branch_pot = tree->Branch("pot",&pot);
  TBranch *branch_projected_pot = tree->Branch("projected_pot",&projected_pot);

  TBranch *branch_projected_weight = tree->Branch("projected_weight",&projected_weight);

  int run;
  int itype = -9;
  int ccnc = -9;
  float oscpro;

  TBranch* itype_branch = (TBranch*) tree->GetListOfBranches()->FindObject("itype");
  if (itype_branch){
    std::cout<<"Setting itype branch"<<std::endl;
    tree->SetBranchAddress("itype",&itype);
  } 
  else{
    std::cout<<"Setting ccnc branch"<<std::endl;
    tree->SetBranchAddress("ccnc",&ccnc);
  }
  tree->SetBranchAddress("run",&run);
  tree->SetBranchAddress("oscpro",&oscpro);


  for (Long64_t entry = 0; entry < tree->GetEntries(); entry++){
    tree->GetEntry(entry);
    if (itype_branch){
      if (itype==1){
        ccnc=1;
      }
    }
    if (ccnc == 1) pot = pot_nu+pot_nue+pot_nutau; //NC evnt
    else if (run==20000001) pot = pot_nu; //CC event from nu file
    else if (run==20000002) pot = pot_nue; //CC event from nue file
    else if (run==20000003) pot = pot_nutau; //CC event from nutau file
    else if (run==20000004) pot = pot_nu; //CC event from nu file
    else if (run==20000005) pot = pot_nue; //CC event from nue file
    else if (run==20000006) pot = pot_nutau; //CC event from nutau file

    else{
      std::cout<<"Found unknown run number: " << run << " setting POT to 1" << std::endl;
      pot = 1.;
    }

    projected_weight = (projected_pot/pot) * oscpro;
    //std::cout<<pot<<" "<<oscpro<<" "<<projected_weight<<std::endl;

    branch_pot->Fill();
    branch_projected_pot->Fill();
    branch_projected_weight->Fill();

  }
  */


  return;
}

void potWeightAppend(TString file_name, double projected_pot=1.){

  TFile *file = new TFile(file_name,"UPDATE");
  if (!file){
    std::cout<<"Could not open " << file_name << std::endl;
    return;
  }

  //NTrees and treeNames will need changing
  const int NTrees = 1;
  TTree* trees[NTrees];
  TString treeNames[NTrees];
  treeNames[0] = "ccnuselection/ccnusel";
  //treeNames[1] = "mvaselect/sig";
  //treeNames[2] = "mvaselect/nc";
  //treeNames[3] = "mvaselect/cc";
  //treeNames[4] = "mvaselect/MVASelection";


  //POT tree name will need changing
  TTree *pottree = (TTree*) file->Get("ccnuselection/pottree");
  if (!pottree){
    std::cout<<"Could not find POT tree"<<std::endl;
    return;
  }

  std::cout << "pottree entries: " << pottree->GetEntries() << std::endl;

  int run;
  double pot;
  pottree->SetBranchAddress("Run",&run);
  pottree->SetBranchAddress("POT",&pot);

  double pot_nu = 0.;
  double pot_nue = 0.;
  double pot_nutau = 0.;

  for (int i = 0; i < pottree->GetEntries(); i++){
    pottree->GetEntry(i);

    if (run==20000001) pot_nu += pot;
    else if (run==20000002) pot_nue += pot;
    else if (run==20000003) pot_nutau += pot;
    else if (run==20000004) pot_nu += pot;
    else if (run==20000005) pot_nue += pot;
    else if (run==20000006) pot_nutau += pot;
    else{
      std::cout<<"Found unknown run number: " << run << std::endl;
      return;
    }
  }

  std::cout<<"POTs:"<<std::endl;
  std::cout<<"--nu:    " << pot_nu << std::endl;
  std::cout<<"--nue:   " << pot_nue << std::endl;
  std::cout<<"--nutau: " << pot_nutau << std::endl;
  std::cout<<"--nc:    " << pot_nu+pot_nue+pot_nutau<<std::endl;



  for (int i = 0; i < NTrees; i++){
    trees[i] = (TTree*) file->Get(treeNames[i]);
    if (!trees[i]){
      std::cout<<"No tree with name: " << treeNames[i] << std::endl;
      continue;
    }
    appendPOTBranch(trees[i], pot_nu, pot_nue, pot_nutau, projected_pot);
  }

  file->Write();
  file->Close();
  return;
}

void potWeightAppend(std::vector<TTree*> trees, TTree* pottree, double projected_pot){
  int run;
  double pot;
  pottree->SetBranchAddress("Run",&run);
  pottree->SetBranchAddress("POT",&pot);

  double pot_nu = 0.;
  double pot_nue = 0.;
  double pot_nutau = 0.;

  std::cout<<"Getting POT from tree with name: " << pottree->GetName() <<std::endl;
  for (int i = 0; i < pottree->GetEntries(); i++){
    pottree->GetEntry(i);

    if (run==20000001) pot_nu += pot;
    else if (run==20000002) pot_nue += pot;
    else if (run==20000003) pot_nutau += pot;
    else if (run==20000004) pot_nu += pot;
    else if (run==20000005) pot_nue += pot;
    else if (run==20000006) pot_nutau += pot;
    else{
      std::cout<<"Found unknown run number: " << run << std::endl;
      return;
    }
  }

  std::cout<<"POTs:"<<std::endl;
  std::cout<<"--nu:    " << pot_nu << std::endl;
  std::cout<<"--nue:   " << pot_nue << std::endl;
  std::cout<<"--nutau: " << pot_nutau << std::endl;
  std::cout<<"--nc:    " << pot_nu+pot_nue+pot_nutau<<std::endl;



  for (int i = 0; i < trees.size(); i++){
    if (!trees[i]){
      std::cout<<"Invalid tree in vector"<<std::endl;
      continue;
    }
    std::cout<<"Starting append to tree: " << trees[i]->GetName() << std::endl;
    appendPOTBranch(trees[i], pot_nu, pot_nue, pot_nutau, projected_pot);
  }

}


