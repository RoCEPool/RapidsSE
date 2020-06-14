/*============================================================================*/
/* Filter_Output.cpp -- Output Filter operand                                 */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_Output* LogicalPlans::_pFilter_Output(void* aAddr)
{
  Filter_Output* pOp = reinterpret_cast<Filter_Output*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_Output::operator new(size_t)
{
  Filter_Output* pFilter_Output =
    static_cast<Filter_Output*>(HeapMem::pAllocate(CN_FILTER_OUTPUT));
  
  return pFilter_Output;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_Output::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_Output::Filter_Output(TokenVec* pTokenVec)
: Filter(CN_FILTER_OUTPUT,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_OUTPUT,_FOOTER_);

  m_sName = "output"_Stz;
  
  _checkInitialised_(CN_FILTER_OUTPUT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_OUTPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_Output::~Filter_Output(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_OUTPUT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_Output);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
