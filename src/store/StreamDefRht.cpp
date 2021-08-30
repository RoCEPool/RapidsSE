/*============================================================================*/
/* StreamDefRht.cpp -- Stream Definition Right                                */
/*============================================================================*/

#include "BASE.h"

#include "StreamDefRht.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDefRht* STORE::_pStreamDefRht(void* aAddr)
{
  StreamDefRht* pStreamDefRht = reinterpret_cast<StreamDefRht*>(aAddr);
  
  //RASSERT_TYPE(pStreamDefRht,CN_STREAMDEFRHT);
  
  return pStreamDefRht;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StreamDefRht::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_STREAMDEFRHT);

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StreamDefRht::operator delete(void* aAddr)
{
  StreamDefRht* pStreamDefRht = reinterpret_cast<StreamDefRht*>(aAddr);

  Heaps::vFree(pStreamDefRht);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDefRht::StreamDefRht(void)
: StreamDefBase(CN_STREAMDEFRHT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEFRHT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDefRht::StreamDefRht(StreamDefBase* pStreamDef)
: StreamDefBase(CN_STREAMDEFRHT, pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEFRHT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDefRht::~StreamDefRht(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEFRHT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void StreamDefRht::vPostRecv(void* aStreamDefRht)
{
  _static_

  StreamDefRht* pStreamDefRht = _pStreamDefRht(aStreamDefRht);
}

/*----------------------------------------------------------------------------*/
/* repair the logicCompiler                                                   */
/*----------------------------------------------------------------------------*/
void StreamDefRht::vRepair(Object* pObject, eRF eRepair)
{
  _static_

  StreamDefRht* pStreamDefRht = _pStreamDefRht(pObject);

  // delete the streamDefRht
  delete pStreamDefRht;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDefRht);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
