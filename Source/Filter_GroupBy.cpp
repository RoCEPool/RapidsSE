/*============================================================================*/
/* Filter_GroupBy.cpp -- GroupBy LogicalOp operand                               */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_GroupBy* LogicalPlans::_pFilter_GroupBy(void* aAddr)
{
  Filter_GroupBy* pOp = reinterpret_cast<Filter_GroupBy*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_GroupBy::operator new(size_t)
{
  Filter_GroupBy* pFilter_GroupBy =
    static_cast<Filter_GroupBy*>(HeapMem::pAllocate(CN_FILTER_GROUPBY));
  
  return pFilter_GroupBy;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_GroupBy::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_GroupBy::Filter_GroupBy(TokenVec* pTokenVec)
: LogicalOp(CN_FILTER_GROUPBY,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_GROUPBY,_FOOTER_);

  m_sName = "groupBy"_Stz;
  
  _checkInitialised_(CN_FILTER_GROUPBY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_GroupBy::~Filter_GroupBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_GROUPBY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_GroupBy);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
