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
  
  m_iMark      = -1;
  m_iTick      = -1;
  m_caNode[0]  = '\0';
  m_iEnumGroup = -1;
  m_iEnum      = -1;
  m_caText[0]  = '\0';
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMark::vWrite(i8 iEnumGroup, i8 iEnum, bool bForeign, char* aText)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  m_iTick = Tsc::iTicks();
  
  StrLib::vCopyLen(m_caNode,
                   gpAttachMap->m_pGrid->m_pThisNode->aNodeName()->aChar(),
                   sizeof(m_caNode) - 1);
  
  m_iEnumGroup = iEnumGroup;
  m_iEnum      = iEnum;
  m_iForeign   = bForeign;

  StrLib::vCopyLen(m_caText,
                   aText,
                   sizeof(m_caText) - 1);
  
  _postcondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMark::vDump(File* pFile, bool bShortForm,
                         ActivityMark* pPrevMark, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  if (bShortForm) {
    vDumpShort(pFile);
    return;
  }
  
  i8 iDeltaUs = 0;
  
  if (pPrevMark)
    iDeltaUs = iAsMicroseconds(m_iTick - pPrevMark->m_iTick);
  
  iTotalDelta += iDeltaUs;
  
  if (StrLib::bSame(m_caText,"delta reset"))
    iTotalDelta = 0;
  

  if (m_iEnum < 0)
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,%s,%s\n"_Stz,
                  m_iMark,
                  m_iTick,
                  iDeltaUs,
                  iTotalDelta,
                  
                  m_caNode,
                  aSR_Short[-m_iEnum],
                  m_caText);
  else
  if (m_iEnum == 0)
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,inf,%s\n"_Stz,
                  m_iMark,
                  m_iTick,
                  iDeltaUs,
                  iTotalDelta,
                  
                  m_caNode,
                  m_caText);
  else
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,%lld,%s\n"_Stz,
                  m_iMark,
                  m_iTick,
                  iDeltaUs,
                  iTotalDelta,
                  
                  m_caNode,
                  m_iEnum,
                  m_caText);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMark::vDumpShort(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMARK,_FOOTER_);
  
  // inf
  if (m_iEnumGroup == -1) {
    pFile->vPrint("[%3lld %s inf] %s\n"_Stz,
                  m_iMark,
                  m_caNode,
                  m_caText);
    return;
  }
  
  // job
  if (m_iEnumGroup  == 0) {
    pFile->vPrint("[%3lld %s %03lld] %s\n"_Stz,
                  m_iMark,
                  m_caNode,
                  m_iEnum,
                  m_caText);
    return;
  }
  
  // enum
  pFile->vPrint("[%3lld %s %s] %s\n"_Stz,
                m_iMark,
                m_caNode,
                EnumReg::szEnumText(m_iEnumGroup,m_iEnum),
                m_caText);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ActivityMark)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
