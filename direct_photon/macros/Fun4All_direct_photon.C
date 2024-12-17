#ifndef FUN4ALL_DIRECT_PHOTON_C
#define FUN4ALL_DIRECT_PHOTON_C

#include <direct_photon/direct_photon.h>

//  -----------------------

#include <G4_Input.C>
#include <Calo_Calib.C>

#include <caloreco/CaloTowerBuilder.h>

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <zdcinfo/ZdcReco.h>

//  -----------------------

#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <ffamodules/CDBInterface.h>
#include <mbd/MbdReco.h>

#include <iostream>
#include <string>
#include <cstdio> 

// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libdirect_photon.so)
R__LOAD_LIBRARY(libmbd.so)

//  -----------------------
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libcalo_reco.so)
//  -----------------------

void Fun4All_direct_photon(
    const std::string &fname = "DST_TRIGGERED_EVENT_run2pp_new_2024p001-00042797-0000.root",
    const std::string &outname = "direct_photon_42797_0000.root",
    int startEvent = 0,
    int nEvents = 10000,
    const std::string &dbtag = "2024p007")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("in1");
  in1->AddFile(fname);
  se->registerInputManager(in1);

  recoConsts *rc = recoConsts::instance();

  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  CDBInterface::instance()->Verbosity(1);

  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);
  
  DirectPhoton *direct_photon = new DirectPhoton();
  direct_photon->setFileName(outname);
  se->registerSubsystem(direct_photon);

  if (startEvent > 0)
  {
    se->fileopen("in1",fname);
    se->skip(startEvent);
  }

  se->run(nEvents);  
  se->End();
  se->PrintTimer();
  se->Print();
  std::cout << "Ran " << se->EventCounter() << " events" << std::endl;
  delete se;
  CDBInterface::instance()->Print();  // print used DB files
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);

}

#endif
