/*============================================================================*/
/* ToLglPtnNst.cpp -- Convert row buffer to have normalised strings           */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"
#include "PathIdentifier.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ToLglPtnNst,CN_TOLGLPTNNST)
__aNewFunction(ToLglPtnNst,CN_TOLGLPTNNST)
__vDeleteFunction(ToLglPtnNst,CN_TOLGLPTNNST)
__aFooterFunction(ToLglPtnNst)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToLglPtnNst::ToLglPtnNst(void)
: Object(CN_TOLGLPTNNST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNNST,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNNST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToLglPtnNst::~ToLglPtnNst(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNNST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ToLglPtnNst                                                     */
/*----------------------------------------------------------------------------*/
void ToLglPtnNst::vRepair(Object* pObject)
{
  _static_

  ToLglPtnNst* pToLglPtnNst = _pToLglPtnNst(pObject);

  // delete the toNstRow
  delete pToLglPtnNst;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToLglPtnNst::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNNST,_FOOTER_);
  
  pRowBuffer->vNormaliseStrings();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
