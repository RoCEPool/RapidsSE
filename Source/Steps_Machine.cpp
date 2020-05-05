/*============================================================================*/
/* Steps_Machine.cpp - machine step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Machine(void)
{
  vAddStep("machine::coreCount"_Stz,  &Step::vMachineCoreCount);
  vAddStep("machine::cpuFreq"_Stz,    &Step::vMachineCpuFreq);
  vAddStep("machine::hostName"_Stz,   &Step::vMachineHostName);
  vAddStep("machine::memory"_Stz,     &Step::vMachineMemory);
}

/*----------------------------------------------------------------------------*/
/* machine::coreCount -- get core count of this machine                       */
/*----------------------------------------------------------------------------*/
void Step::vMachineCoreCount(void)
{
  ENTRY();
  
  i8 iCores = Hardware::iCoreCount();
  
  m_pDataStack->vPushInt(iCores);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* machine::cpuFreq -- get cpu freq of this machine                           */
/*----------------------------------------------------------------------------*/
void Step::vMachineCpuFreq(void)
{
  ENTRY();
  
  i8 iFreq = Hardware::iCpuFrequency();
  
  m_pDataStack->vPushInt(iFreq);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* machine::hostName -- get host name of this machine                         */
/*----------------------------------------------------------------------------*/
void Step::vMachineHostName(void)
{
  ENTRY();
  
  s32 sHostName;

  Hardware::vHostName(sHostName);

  m_pDataStack->vPushStr(sHostName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* machine::memory -- get amount of memory in this machine                    */
/*----------------------------------------------------------------------------*/
void Step::vMachineMemory(void)
{
  ENTRY();
  
  i8 iMemory = Hardware::iPhysicalMemory();
  
  m_pDataStack->vPushInt(iMemory);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
