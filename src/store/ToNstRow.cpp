/*============================================================================*/
/* ToNstRow.cpp -- Convert row to normalised string row slab buffers          */
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

__pCastFunction(STORE,ToNstRow,CN_TONSTROW)
__aNewFunction(ToNstRow,CN_TONSTROW)
__vDeleteFunction(ToNstRow,CN_TONSTROW)
__aFooterFunction(ToNstRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToNstRow::ToNstRow(void)
: Object(CN_TONSTROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TONSTROW,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TONSTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToNstRow::~ToNstRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TONSTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ToNstRow                                                        */
/*----------------------------------------------------------------------------*/
void ToNstRow::vRepair(Object* pObject)
{
  _static_

  ToNstRow* pToNstRow = _pToNstRow(pObject);

  // delete the toNstRow
  delete pToNstRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToNstRow::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TONSTROW,_FOOTER_);
  
  pRowBuffer->vNormaliseStrings();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
