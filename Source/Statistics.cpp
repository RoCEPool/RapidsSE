/*============================================================================*/
/* Statistics.cpp -- Statistics                                               */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Statistics* Measurement::_pStatistics(void* aAddr)
{
  Statistics* pStatistics = reinterpret_cast<Statistics*>(aAddr);
  
  RASSERT_TYPE(pStatistics,CN_STATISTICS);
  
  return pStatistics;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Statistics::operator new(size_t)
{
  Statistics* pStatistics = (Statistics*)HeapMem::pAllocate(CN_STATISTICS);
  
  return pStatistics;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Statistics::operator delete(void*)
{

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Statistics::Statistics(void)
: Object(CN_STATISTICS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);
  
  _checkInitialised_(CN_STATISTICS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Statistics::~Statistics(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* memory monitor profile                                                     */
/*----------------------------------------------------------------------------*/
void Statistics::vMemoryPortrait(s48K& sPortrait)
{
  Statistics* pStat = &gpAttachMap->m_oStatistics;

  s32 sCopyMag; Magnitudes::vByteMag(pStat->Memory.m_iCopyTotal,     sCopyMag);
  s32 sMoveMag; Magnitudes::vByteMag(pStat->Memory.m_iMoveTotal,     sMoveMag);
  s32 sSribMag; Magnitudes::vByteMag(pStat->Memory.m_iScribbleTotal, sSribMag);
  s32 sSetMag;  Magnitudes::vByteMag(pStat->Memory.m_iSetTotal,      sSetMag);
  s32 sZeroMag; Magnitudes::vByteMag(pStat->Memory.m_iZeroTotal,     sZeroMag);

  sPortrait.vSprint("Memory: copy %s move %s scribble %s set %s zero %s\n"_Stz,
                    sCopyMag.aStz(),
                    sMoveMag.aStz(),
                    sSribMag.aStz(),
                    sSetMag.aStz(),
                    sZeroMag.aStz()
                    );
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Statistics::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
