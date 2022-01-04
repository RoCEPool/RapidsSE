/*============================================================================*/
/* SysReport.cpp -- SysReport                                                 */
/*============================================================================*/

#include "BASE.h"

#include "SysReport.hpp"
#include "SysMetrics.hpp"

#include "NamedBuffer.hpp"
#include "Node.hpp"

#include "Table.hpp"

#include "Tub.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,SysReport,CN_SYSREPORT)
__aNewFunction(SysReport,CN_SYSREPORT)
__vDeleteFunction(SysReport,CN_SYSREPORT)
__aFooterFunction(SysReport)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SysReport::SysReport(void)
: Object(CN_SYSREPORT)
{
  _precondition_(_MAGIC_,CN_SYSREPORT,_FOOTER_);

  _checkInitialised_(CN_SYSREPORT);

  _postcondition_(_MAGIC_,CN_SYSREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SysReport::~SysReport(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SYSREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* system report                                                              */
/*----------------------------------------------------------------------------*/
void SysReport::vSysReport(Tub* pTub)
{
  _static_
  
  SysMetrics* pSysMetricsChain = AM::ATTACHMAP::pSysMetricsChain();
  
//  for (SysMetrics* pSysMetrics = pSysMetricsChain;
//       pSysMetrics;
//       pSysMetrics = pSysMetrics->pNext()) {
//
//    vTrace("SysMetrics Since %lld\n"_Stz
//           ,pSysMetrics->nSince()
//           );
//  }
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the internal jobOperators table
  
  Table* pSysMetricsTable = AM::pTableFind(C_TDEF_SYSMETRICS);
  StreamDef* pStreamDef = pSysMetricsTable->pStreamDef();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a working buffer from the tub
  
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Node* pThisNode = MY::pNode();

  for (SysMetrics* pSysMetrics = pSysMetricsChain;
       pSysMetrics;
       pSysMetrics = pSysMetrics->pNext()) {
      
      
    L_RETRY:
    Row* pRow = pNamedBuffer->pAllotRow
                (
                  C_VARGS_BEGIN
                 
                 ,pThisNode->iNodeNo()
                 ,pThisNode->szNodeNm()


                 ,pSysMetrics->nSince()
                 ,pSysMetrics->nRefTimer()

                 ,pSysMetrics->iPctFree()
                 ,pSysMetrics->nThreadsIdle()

                 ,C_VARGS_END
                );
      
      if (! pRow) {
        pNamedBuffer = new NamedBuffer(pStreamDef);

        // add the named buffer to the tub
        pTub->vAddBuffer(pNamedBuffer);
        goto L_RETRY;
      }

  
    }

}

/*----------------------------------------------------------------------------*/
/*-- Private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
