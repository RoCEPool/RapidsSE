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
  //m_oColVecAry
  
  m_bTemp = false;
  //vTrace("StreamDef permanent %p\n"_Stz, this);
  
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
  
  i8 nColVecs = pStreamDef->m_oColVecAry.nItems();
  m_oColVecAry.vItems(nColVecs);
  
  for (i8 i_ColVec = 0; i_ColVec < nColVecs; i_ColVec++) {
    ColumnVec* pThisColumnVec = m_oColVecAry.pGet(i_ColVec);
    ColumnVec* pStreamColVec = pStreamDef->m_oColVecAry.pGet(i_ColVec);
    
    // get no of columns
    i8 nColumns = pStreamColVec->nItems();
    
    // add columns to streamdef
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
      Column* pColumn = new Column;
      pColumn->vCopy(pStreamColVec->pGet(i_Col));
      pThisColumnVec->vAdd(pColumn);
    }
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
  
}

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
void StreamDef::vAdd(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_oColVecAry.vItems(m_oColVecAry.nItems() + 1);
  ColumnVec* pThisColumnVec = m_oColVecAry.pGet(m_oColVecAry.nItems() - 1);
  ColumnVec* pStreamColVec = pStreamDef->m_oColVecAry.pGet(0);
  
  // get no of columns
  i8 nColumns = pStreamColVec->nItems();
  
  // add columns to streamdef
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = new Column;
    pColumn->vCopy(pStreamColVec->pGet(i_Col));
    pThisColumnVec->vAdd(pColumn);
  }
  
  m_bTemp = true;
  
  _checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get column vector at index                                                 */
/*----------------------------------------------------------------------------*/
ColumnVec* StreamDef::pColumnVec(i8 iIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return m_oColVecAry.pGet(iIndex);
}

/*----------------------------------------------------------------------------*/
/* get column vector at index                                                 */
/*----------------------------------------------------------------------------*/
i8 StreamDef::iFindColumn(char* sColumnNm, i8& i_ColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  for (i_ColVec = 0; i_ColVec < m_oColVecAry.nItems(); i_ColVec++) {
    ColumnVec* pColumnVec = m_oColVecAry.pGet(i_ColVec);
    i8 nColumns = pColumnVec->nItems();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
      Column* pCol = pColumnVec->pGet(i_Col);
      if (StrLib::bEqualTo(sColumnNm,pCol->sColumnName().aChar(),C_NO_CASE)) {
        return i_Col;
      }
    }
  }
  
  return C_UNKNOWN;
}
/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDef);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
