/*============================================================================*/
/* Filter_Select.cpp -- Select LogicalOp operand                                 */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_Select* LogicalPlans::_pFilter_Select(void* aAddr)
{
  Filter_Select* pOp = reinterpret_cast<Filter_Select*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_Select::operator new(size_t)
{
  Filter_Select* pFilter_Select =
    static_cast<Filter_Select*>(HeapMem::pAllocate(CN_FILTER_SELECT));
  
  return pFilter_Select;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_Select::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_Select::Filter_Select(TokenVec* pTokenVec)
: LogicalOp(CN_FILTER_SELECT,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SELECT,_FOOTER_);

  m_sName = "select"_Stz;
  
  _checkInitialised_(CN_FILTER_SELECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_Select::~Filter_Select(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SELECT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_Select);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
