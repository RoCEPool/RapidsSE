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
  
  RASSERT( !pActivityMark || CN_ACTIVITYMARK == pActivityMark->eClass() );
 
  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMark::operator new(size_t iSize)
{
  RASSERT(iSize);
  
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
ActivityMark::~ActivityMark(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
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
                   sizeof(m_aNode));
  m_aNode[sizeof(m_aNode) - 1] = '\0';
  
  m_iJobId = iJobId;

  StrLib::vCopyLen(m_aMark,aMark,sizeof(m_aMark));
  m_aMark[sizeof(m_aMark) - 1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMark::vDump(File* pFile, i8 i_Mark, ActivityMark* pPrevMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  pFile->vPrint("%lld,%lld,%lld,%s,%lld,%s\n"_Stz,
                i_Mark,
                m_iTick,
                pPrevMark ? m_iTick - pPrevMark->m_iTick : 0,
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
