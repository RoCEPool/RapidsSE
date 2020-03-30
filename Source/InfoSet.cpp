/*============================================================================*/
/* InfoSet.cpp -- Information Set                                             */
/*============================================================================*/

#include "InfoData.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* InfoData::_pInfoSet(void* aAddr)
{
  InfoSet* pInfoSet = reinterpret_cast<InfoSet*>(aAddr);
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* InfoSet::operator new(size_t)
{
  InfoSet* pInfoSet =
  static_cast<InfoSet*>(HeapMem::pAllocate(CN_INFOSET));
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void InfoSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(void)
: RowBuffer(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_sInfoSetNm = C_NO_NAME;
  m_nRowSize   = C_UNKNOWN;
  m_nInfoRows  = 0;
  
  _checkInitialised_(CN_INFOSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(StreamDef* pStreamDef)
: RowBuffer(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_nRowSize  = Row::nSizeBytes(pStreamDef);
  m_nInfoRows = 0;
  
  _checkInitialised_(CN_INFOSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSet::~InfoSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);

  Row* pRow = nullptr;

  if (nFreeBytes() >= m_nRowSize) {
    pRow = RowBuffer::aAllotRow(m_nRowSize);
    m_nInfoRows++;
  }

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aAllotRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  Row* pRow = RowBuffer::aAllotRow(pRowSingle);
  m_nInfoRows++;
  
  return pRow;
}


/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
 Row* InfoSet::aAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...)
 {
   _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
   
   // allot a row
   Row* ayRow = RowBuffer::aAllotRow(Row::nSizeBytes(pStreamDef));
   m_nInfoRows++;
   
   Stz* ayTail;
   
   if ( ! ayRow )
     return nullptr;
   
   ASSERT(C_VARGS_BEGIN == iVArgsBegin);
   
   va_list xVarArgs;
   
   va_start(xVarArgs,iVArgsBegin);
   
   for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(0); i_Col++) {
   
     Column* pCol = pStreamDef->pGetColumn(i_Col);
     
     if (DM_VCH == pCol->eDatum()) {
     Stz* szVal = va_arg(xVarArgs,Stz*);
     if (szVal) {
       ayTail = (Stz*)ayAllotTail(StrLib::iSize(szVal->azChar()));
       
       // return early if no more space for the string
       if ( ! ayTail )
         return nullptr;
       
       StrLib::vCopy((char*)ayTail, szVal->azChar());
       ayRow->m_waColVal[i_Col].m_iWrd = iToOffset(ayTail);
     }
     else
       ayRow->m_waColVal[i_Col].vSetNull();
     }
     else {
       i8 iVal = va_arg(xVarArgs,i8);
       ayRow->m_waColVal[i_Col].m_iWrd = iVal;
     }
   }
   
   i8 iVArgsEnd = va_arg(xVarArgs,i8);
   ASSERT(C_VARGS_END == iVArgsEnd);
   
   va_end(xVarArgs);
   
   return ayRow;
 }

/*----------------------------------------------------------------------------*/
/* get first row                                                              */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return RowBuffer::aFirstRow();
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
i8 InfoSet::nInfoRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_nInfoRows;
}

/*----------------------------------------------------------------------------*/
/* get name of info set                                                       */
/*----------------------------------------------------------------------------*/
Stz* InfoSet::szInfoSetNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_sInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* set name of info set                                                       */
/*----------------------------------------------------------------------------*/
void InfoSet::vInfoSetNm(Stz* szInfoSetNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_sInfoSetNm = szInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aNextRow(Row* aPrevRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return RowBuffer::aNextRow(aPrevRow, m_nRowSize);
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 InfoSet::iToOffset(Stz* aItem)
{
  i4 iOffset = static_cast<i4>(nSub(aItem,this));
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
