#ifndef FUN4ALL_ZDC_SMD_C
#define FUN4ALL_ZDC_SMD_C

#include <zdc_smd/ZDC_SMD.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <string>
#include <cstdio> 

// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libZDC_SMD.so)


void Fun4All_ZDC_SMD(const std::string &fname = "DST_TRIGGERED_EVENT_run2pp_new_2024p001-00042797-0000.root", const std::string &outname = "zdc_smd_42797_0000.root", int startEvent = 0, int nEvents = 10000)

{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("in1");
  in1->AddFile(fname);
  se->registerInputManager(in1);
  
  ZDC_SMD *zdc_smd = new ZDC_SMD();
  zdc_smd->setFileName(outname);
//  zdc_smd->setGainMatch("/sphenix/user/dloomis/analysis/ZDCNeutronLocPol/SMDrelativegains.dat");
  se->registerSubsystem(zdc_smd);

  if (startEvent > 0)
  {
    se->fileopen("in1",fname);
    se->skip(startEvent);
  }

  se->run(nEvents);  
  se->End();
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);




}

#endif
