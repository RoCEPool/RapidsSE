/*============================================================================*/
/* StreamDef.cpp -- Stream Definition                                         */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new row pix                                                                */
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_StreamDefNew(i8 nNumCols, void* pColumnInfo) {
  return (void*)StreamDef::pStreamDefNew(nNumCols, (ColumnInfo*) pColumnInfo);
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDef* STORE::_pStreamDef(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  //RASSERT_TYPE(pStreamDef,CN_STREAMDEF);
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StreamDef::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_STREAMDEF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StreamDef::operator delete(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  Heaps::vFree(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(void)
: StreamDefBase(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(StreamDef* pStreamDef)
: StreamDefBase(CN_STREAMDEF, pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDef::~StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void StreamDef::vPostRecv(void* aStreamDef)
{
  _static_
  UNUSED(aStreamDef);

//  StreamDef* pStreamDef = _pStreamDef(aStreamDef);
}

/*----------------------------------------------------------------------------*/
/* new a streamDef                                                            */
/*----------------------------------------------------------------------------*/
StreamDef* StreamDef::pStreamDefNew(i8 nNumCols, ColumnInfo* pColumnInfo) {

  StreamDef* pStreamDef = new StreamDef;

  for (i8 i_Col = 0; i_Col < nNumCols; i_Col++) {
      pStreamDef->vAddColumn((Stz*)&pColumnInfo[i_Col].m_cColumnNm[0],
														 (Parser::TypeId)pColumnInfo[i_Col].m_iColumnTypeId,
                             pColumnInfo[i_Col].m_iColumnLength,
                             pColumnInfo[i_Col].m_iColumnScale);
  }

  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* repair the streamDef                                                       */
/*----------------------------------------------------------------------------*/
void StreamDef::vRepair(Object* pObject)
{
  _static_

  StreamDef* pStreamDef = _pStreamDef(pObject);

  // delete the streamDef
  delete pStreamDef;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* duplicate this streamDef                                                   */
/*----------------------------------------------------------------------------*/
StreamDef* StreamDef::pDup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);

  StreamDef* pStreamDef_dup = new StreamDef(this);

  return pStreamDef_dup;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void StreamDef::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDef);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
