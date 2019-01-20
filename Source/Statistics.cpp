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
  
//  Api.m_nCheckLibraryVersion = 1;
//  Api.m_nCopyBuffer = 1;
//  Api.m_nEndInsertJobs = 1;
//  Api.m_nEndJobs = 1;
//  Api.m_nExecCmdStr = 1;
//  Api.m_nExecuteCommand = 1;
//  Api.m_nFetchNextBuffer = 1;
//  Api.m_nGetRecordBuffer = 1;
//  Api.m_nGetErrorMessage = 1;
//  Api.m_nGetRow = 1;
//  Api.m_nGetRowCount = 1;
//  Api.m_nGetRowCountTableColumn = 1;
//  Api.m_nGetRowTableColumn = 1;
//  Api.m_nInsertRow = 1;
//  Api.m_nInsertRows = 1;
//  Api.m_nLoadResult = 1;
//  Api.m_nPoolOpen = 1;
//  Api.m_nScanNextRow = 1;
//  Api.m_nSessionDelete = 1;
//  Api.m_nSessionInvalid = 1;
//  Api.m_nSessionNew = 1;

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
