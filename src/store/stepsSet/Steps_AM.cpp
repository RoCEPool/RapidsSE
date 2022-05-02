/*============================================================================*/
/* Steps_AM.cpp - AM step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AM(void)
{
  vAddStep("AM::SHOW::bBoot"_Stz,           &Step::vAM_Show_Boot);
  vAddStep("AM::SHOW::bDir"_Stz,            &Step::vAM_Show_Dir);
  vAddStep("AM::SHOW::bDir_Failure"_Stz,    &Step::vAM_Show_Dir_Failure);
  vAddStep("AM::SHOW::bConnection"_Stz,     &Step::vAM_Show_Connection);
  vAddStep("AM::SHOW::bException"_Stz,      &Step::vAM_Show_Exception);
  vAddStep("AM::SHOW::bFile"_Stz,           &Step::vAM_Show_File);
  vAddStep("AM::SHOW::bFile_Failure"_Stz,   &Step::vAM_Show_File_Failure);
  vAddStep("AM::SHOW::bLowMem"_Stz,         &Step::vAM_Show_LowMem);
  vAddStep("AM::SHOW::bLowMem_Recv"_Stz,    &Step::vAM_Show_LowMem_Recv);
  vAddStep("AM::SHOW::bRecurring"_Stz,      &Step::vAM_Show_Recurring);
  vAddStep("AM::SHOW::bSocket"_Stz,         &Step::vAM_Show_Socket);
  vAddStep("AM::SHOW::bSocket_Failure"_Stz, &Step::vAM_Show_Socket_Failure);
  
  vAddStep("AM::derefMetricsDisable"_Stz,   &Step::vAM_DerefMetricsDisable);
  vAddStep("AM::derefMetricsEnable"_Stz,    &Step::vAM_DerefMetricsEnable);
  
  vAddStep("AM::jobMetricsDisable"_Stz,     &Step::vAM_JobMetricsDisable);
  vAddStep("AM::jobMetricsEnable"_Stz,      &Step::vAM_JobMetricsEnable);
  
  vAddStep("AM::reapDisable"_Stz,           &Step::vAM_ReapDisable);
  vAddStep("AM::reapEnable"_Stz,            &Step::vAM_ReapEnable);

  vAddStep("AM::sysMetricsDisable"_Stz,     &Step::vAM_SysMetricsDisable);
  vAddStep("AM::sysMetricsEnable"_Stz,      &Step::vAM_SysMetricsEnable);
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bBoot -- <show_boot> ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Boot(void)
{
  ENTRY();

  b8 bBoot = AM::SHOW::bBoot();
  
  m_pDataStack->vPushInt(bBoot);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bDir -- <show_dir> ?                                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Dir(void)
{
  ENTRY();

  b8 bDir = AM::SHOW::bDir();
  
  m_pDataStack->vPushInt(bDir);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bDir_Failure -- <show_dir_failure> ?                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Dir_Failure(void)
{
  ENTRY();

  b8 bDir_Failure = AM::SHOW::bDir_Failure();
  
  m_pDataStack->vPushInt(bDir_Failure);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bConnection -- <show_connection> ?                               */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Connection(void)
{
  ENTRY();

  b8 bConnection = AM::SHOW::bConnection();
  
  m_pDataStack->vPushInt(bConnection);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bException -- <show_exception> ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Exception(void)
{
  ENTRY();

  b8 bException = AM::SHOW::bException();
  
  m_pDataStack->vPushInt(bException);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bFile -- <show_file> ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_File(void)
{
  ENTRY();

  b8 bFile = AM::SHOW::bFile();
  
  m_pDataStack->vPushInt(bFile);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bFile_Failure -- <show_file_failure> ?                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_File_Failure(void)
{
  ENTRY();

  b8 bFile_Failure = AM::SHOW::bFile_Failure();
  
  m_pDataStack->vPushInt(bFile_Failure);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bLowMem -- <show_lowmem> ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_LowMem(void)
{
  ENTRY();

  b8 bLowMem = AM::SHOW::bLowMem();
  
  m_pDataStack->vPushInt(bLowMem);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bLowMem_Recv -- <show_lowmem_recv> ?                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_LowMem_Recv(void)
{
  ENTRY();

  b8 bLowMem_Recv = AM::SHOW::bLowMem_Recv();
  
  m_pDataStack->vPushInt(bLowMem_Recv);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bRecurring -- <show_recurring> ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Recurring(void)
{
  ENTRY();

  b8 bRecurring = AM::SHOW::bRecurring();
  
  m_pDataStack->vPushInt(bRecurring);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bSocket -- <show_socket> ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Socket(void)
{
  ENTRY();

  b8 bSocket = AM::SHOW::bSocket();
  
  m_pDataStack->vPushInt(bSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bSocket_Failure -- <show_socket_failure> ?                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Socket_Failure(void)
{
  ENTRY();

  b8 bSocket_Failure = AM::SHOW::bSocket_Failure();
  
  m_pDataStack->vPushInt(bSocket_Failure);

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* AM::derefMetricsDisable -- disable deref metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_DerefMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::dferefMetricsEnable -- enable deref metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_DerefMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::jobMetricsDisable -- disable job metrics                              */
/*----------------------------------------------------------------------------*/
void Step::vAM_JobMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::jobMetricsEnable -- enable job metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_JobMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::reapDisable -- disble reaping of retired jobs                          */
/*----------------------------------------------------------------------------*/
void Step::vAM_ReapDisable(void)
{
  ENTRY();
  
  AM::SYSTEM::vReapEnabled(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::reapEnable -- enable reaping of retired jobs                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_ReapEnable(void)
{
  ENTRY();
  
  AM::SYSTEM::vReapEnabled(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::sysMetricsDisable -- disable sys metrics                               */
/*----------------------------------------------------------------------------*/
void Step::vAM_SysMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::sysMetricsEnable -- enable sys metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_SysMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(true);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
