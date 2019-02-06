/*============================================================================*/
/* ActivityMark.cpp - Activity Mark                                           */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ActivityMark* Measurement::_pActivityMark(void* aAddr)
{
  ActivityMark* pActivityMark = reinterpret_cast<ActivityMark*>(aAddr);
  
  RASSERT_TYPE(pActivityMark,CN_ACTIVITYMARK);
 
  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMark::operator new(size_t)
{
  void* pObject = nullptr;
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMark::operator delete(void* pDataAddress)
{
  ActivityMark* pActivityMark = reinterpret_cast<ActivityMark*>(pDataAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityMark::ActivityMark(void)
: Object(CN_ACTIVITYMARK)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  _checkInitialised_(CN_ACTIVITYMARK);
  
  _postcondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActivityMark::~ActivityMark(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tick delta to microseconds                                         */
/*----------------------------------------------------------------------------*/
i8 ActivityMark::iAsMicroseconds(i8 iTickDelta)
{
  i8 iMicroseconds = iTickDelta / C_1K;
  
  return iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void ActivityMark::vInit(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  m_iMark    = -1;
  m_iTick    = -1;
  m_caNode[0] = '\0';
  m_iJobId   = -1;
  m_caMark[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMark::vWrite(i8 iJobId, char* aMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  m_iTick = Tsc::iTicks();
  
  StrLib::vCopyLen(m_caNode,
                   gpAttachMap->m_pGrid->m_pThisNode->aNodeName()->aChar(),
                   sizeof(m_caNode) - 1);
  m_caNode[sizeof(m_caNode) - 1] = '\0';
  
  m_iJobId = iJobId;

  StrLib::vCopyLen(m_caMark,
                   aMark,
                   sizeof(m_caMark) - 1);
  m_caMark[sizeof(m_caMark) - 1] = '\0';
  
  _postcondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMark::vDump(File* pFile, bool bShortForm,
                         i8 i_Mark, ActivityMark* pPrevMark, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  i8 iDeltaUs = 0;
  
  if (pPrevMark)
    iDeltaUs = iAsMicroseconds(m_iTick - pPrevMark->m_iTick);
  
  iTotalDelta += iDeltaUs;
  
  if (StrLib::bSame(m_caMark,"delta reset"))
    iTotalDelta = 0;
  
  if (bShortForm)
    pFile->vPrint("%lld,%s,%03lld,%s\n"_Stz,
                  m_iMark,
                  m_caNode,
                  m_iJobId,
                  m_caMark);
  else
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,%lld,%s\n"_Stz,
                  m_iMark,
                  m_iTick,
                  iDeltaUs,
                  iTotalDelta,
                  
                  m_caNode,
                  m_iJobId,
                  m_caMark);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ActivityMark)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
