/*============================================================================*/
/* StreamDef.cpp -- Stream Definition                                         */
/*============================================================================*/

#include "MetaData.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDef* MetaData::_pStreamDef(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  RASSERT_TYPE(pStreamDef,CN_STREAMDEF);
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StreamDef::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STREAMDEF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StreamDef::operator delete(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  HeapMem::vFree(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(void)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  //m_sName;
  //m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  m_pColumnVec = new ColumnVec;
  
  m_bTemp = false;
  vTrace("StreamDef permanent %p\n"_Stz, this);
  
  _checkInitialised_(CN_STREAMDEF);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(StreamDef* pStreamDef)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_sName = pStreamDef->m_sName;
  m_sPath = pStreamDef->m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  
  m_pColumnVec = new ColumnVec;
  
  // get no of columns
  i8 nColumns = pStreamDef->m_pColumnVec->nItems();
  
  // add columns to streamdef
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = new Column;
    pColumn->vCopy(pStreamDef->m_pColumnVec->pGet(i_Col));
    m_pColumnVec->vAdd(pColumn);
  }
  
  m_bTemp = true;
  
  _checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDef::~StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  delete m_pColumnVec;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDef);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
