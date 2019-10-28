/*============================================================================*/
/* JobDicPath.cpp -- Dictionary Path part for a Job                           */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* JobDicPath::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: JobDicPath class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDicPath::JobDicPath(void)
{

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDicPath::JobDicPath(Job* pJob)
{
  s32 sDicName;
  
  i8 nDicNames = pJob->m_oDicPath.nDicNames();
  for (i8 i_Name = 0; i_Name < nDicNames; i_Name++) {
    sDicName = pJob->m_oDicPath.sDicName(i_Name);
    m_oDicPath.vDicName(i_Name,sDicName);
  }

  //  s256 sDicPath;
  //  m_oDicPath.vAsString(sDicPath);
  //  vTrace("DicPath: %s\n"_Stz,sDicPath.szStz());
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDicPath::JobDicPath(Session* pSession)
{
  s32 sDicName;
  
  if (pSession) {
    i8 nDicNames = pSession->m_oDicStack.iCount();
    for (i8 i_Name = 0; i_Name < nDicNames; i_Name++) {
      sDicName = pSession->m_oDicStack.pPick(i_Name)->szName();
      m_oDicPath.vDicName(i_Name,sDicName);
    }
  }

//  s256 sDicPath;
//  m_oDicPath.vAsString(sDicPath);
//  vTrace("DicPath: %s\n"_Stz,sDicPath.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobDicPath::~JobDicPath(void) noexcept(false)
{
  
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
