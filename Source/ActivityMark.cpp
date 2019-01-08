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
  m_aNode[0] = '\0';
  m_iJobId   = -1;
  m_aMark[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMark::vWrite(i8 iJobId, char* aMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  m_iTick = Synch::iTicks();
  
  StrLib::vCopyLen(m_aNode,
                   gpAttachMap->m_pGrid->m_pThisNode->aNodeName()->aChar(),
                   sizeof(m_aNode) - 1);
  m_aNode[sizeof(m_aNode) - 1] = '\0';
  
  m_iJobId = iJobId;

  StrLib::vCopyLen(m_aMark,
                   aMark,
                   sizeof(m_aMark) - 1);
  m_aMark[sizeof(m_aMark) - 1] = '\0';
  
  _postcondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMark::vDump(File* pFile,
                         i8 i_Mark, ActivityMark* pPrevMark, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  i8 iDeltaUs = 0;
  
  if (pPrevMark)
    iDeltaUs = iAsMicroseconds(m_iTick - pPrevMark->m_iTick);
  
  iTotalDelta += iDeltaUs;
  
  if (StrLib::bSame(m_aMark,"delta reset"))
    iTotalDelta = 0;
  
  pFile->vPrint("%lld,%lld,%lld,%lld,%s,%lld,%s\n"_Stz,
                m_iMark,
                m_iTick,
                iDeltaUs,
                iTotalDelta,
                
                m_aNode,
                m_iJobId,
                m_aMark);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(ActivityMark)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
