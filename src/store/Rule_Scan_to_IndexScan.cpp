/*============================================================================*/
/* Rule_Scan_to_IndexScan.cpp -- Scan to IndexScan Rule                       */
/*============================================================================*/

#include "Rule_Scan_to_IndexScan.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Rule(STORE,Rule_Scan_to_IndexScan,RK_SCAN_TO_INDEXSCAN)
__aNewFunction(Rule_Scan_to_IndexScan,CN_RULE)
__vDeleteFunction(Rule_Scan_to_IndexScan,CN_RULE)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Rule_Scan_to_IndexScan::Rule_Scan_to_IndexScan(void)
: Rule(RK_SCAN_TO_INDEXSCAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RULE,_FOOTER_);

  _checkInitialised_(CN_RULE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Rule_Scan_to_IndexScan::~Rule_Scan_to_IndexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RULE,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
