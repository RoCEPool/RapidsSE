/*============================================================================*/
/* InfoSetBase.cpp -- Information Set Base                                    */
/*============================================================================*/

#include "InfoData.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSetBase* InfoData::_pInfoSetBase(void* aAddr)
{
  InfoSetBase* pInfoSetBase = reinterpret_cast<InfoSetBase*>(aAddr);
  
  return pInfoSetBase;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSetBase::InfoSetBase(eCN eClassNo)
: RowBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_sInfoSetNm = C_NO_NAME;
  m_nRowSize   = C_UNKNOWN;
  m_nInfoRows  = 0;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSetBase::InfoSetBase(eCN eClassNo, StreamDef* pStreamDef)
: RowBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_sInfoSetNm = pStreamDef->szName();
  m_nRowSize  = Row::nSizeBytes(pStreamDef);
  m_nInfoRows = 0;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSetBase::~InfoSetBase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSetBase::aAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
Row* InfoSetBase::aAllotRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Row* pRow = RowBuffer::aAllotRow(pRowSingle);
  m_nInfoRows++;
  
  return pRow;
}


/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
 Row* InfoSetBase::aAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...)
 {
   _precondition_(_ADDRESS_,_MAGIC_);
   
   // allot a row
   Row* ayRow = RowBuffer::aAllotRow(Row::nSizeBytes(pStreamDef));
   m_nInfoRows++;
   
   Stz* ayDst;
   
   if ( ! ayRow )
     return nullptr;
   
   ASSERT(C_VARGS_BEGIN == iVArgsBegin);
   
   va_list xVarArgs;
   
   va_start(xVarArgs,iVArgsBegin);
   
   for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(0); i_Col++) {
   
     Column* pCol = pStreamDef->pGetColumn(i_Col);
     
     // STRING_DEPOSIT
     if (DM_VCH == pCol->eDatum()) {
       Stz* szVal = va_arg(xVarArgs,Stz*);
       if (szVal) {
         ayDst = (Stz*)ayAllotHead(StrLib::nSize(szVal->azChar()));
         
         // return early if no more space for the string
         if ( ! ayDst )
           return nullptr;
         
         StrLib::vCopy((char*)ayDst, szVal->azChar());
         ayRow->vColVal_ofs(i_Col,iToOffset(ayDst));
       }
       else {
         ayRow->vSetColNull(i_Col);
       }
     }
     else {
       i8 iVal = va_arg(xVarArgs,i8);
       ayRow->vColVal_int(i_Col,iVal);
     }
     // END STRING_DEPOSIT

   }
   
   i8 iVArgsEnd = va_arg(xVarArgs,i8);
   ASSERT(C_VARGS_END == iVArgsEnd);
   
   va_end(xVarArgs);
   
   return ayRow;
 }

/*----------------------------------------------------------------------------*/
/* get first row                                                              */
/*----------------------------------------------------------------------------*/
Row* InfoSetBase::aFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return RowBuffer::aFirstRow();
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
i8 InfoSetBase::nInfoRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nInfoRows;
}

/*----------------------------------------------------------------------------*/
/* get name of info set                                                       */
/*----------------------------------------------------------------------------*/
Stz* InfoSetBase::szInfoSetNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* set name of info set                                                       */
/*----------------------------------------------------------------------------*/
void InfoSetBase::vInfoSetNm(Stz* szInfoSetNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sInfoSetNm = szInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSetBase::aNextRow(Row* aPrevRow)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return RowBuffer::aNextRow(aPrevRow, m_nRowSize);
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 InfoSetBase::iToOffset(Stz* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i4 iOffset = static_cast<i4>(nDifference(aItem,this));
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
