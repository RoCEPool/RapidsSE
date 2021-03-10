/*============================================================================*/
/* InfoSetBase.cpp -- Information Set Base                                    */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "InfoSetBase.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSetBase* STORE::_pInfoSetBase(void* aAddr)
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
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_sInfoSetNm = C_NO_NAME;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSetBase::InfoSetBase(eCN eClassNo, StreamDef* pStreamDef)
: RowBuffer(eClassNo,pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_sInfoSetNm = pStreamDef->szName();
    
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSetBase::~InfoSetBase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* InfoSetBase::pAllotRow(i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  // allot a row
  Row* ayRow = RowBuffer::pAllotRow();

  Stz* ayDst;

  if ( ! ayRow )
    return nullptr;

  ASSERT(C_VARGS_BEGIN == iVArgsBegin);

  ColPix* axaColPix = RowBox_t::axaColPix();

  va_list xVarArgs;

  va_start(xVarArgs,iVArgsBegin);

  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {

    ColPix* pColPix = &axaColPix[i_Col];

    // STRING_DEPOSIT
    if (DM_VCH == pColPix->m_uDatum) {
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal) {
        ayDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));

        // return early if no more space for the string
        if ( ! ayDst )
          return nullptr;

        StrLib::vCopy((char*)ayDst, szVal->azChar());

        ayRow->vColVal_ofs(i_Col,ayDst);
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
  UNUSED(iVArgsEnd);

  va_end(xVarArgs);

  return ayRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
 Row* InfoSetBase::pAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...)
 {
   _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
   
   // allot a row
   Row* ayRow = RowBuffer::pAllotRow();
   
   Stz* ayDst;
   
   if ( ! ayRow )
     return nullptr;
   
   ASSERT(C_VARGS_BEGIN == iVArgsBegin);
   
   va_list xVarArgs;
   
   va_start(xVarArgs,iVArgsBegin);
   
   for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {
   
     Column* pCol = pStreamDef->pGetColumn(i_Col);
     
     // STRING_DEPOSIT
     if (DM_VCH == pCol->eDatum()) {
       Stz* szVal = va_arg(xVarArgs,Stz*);
       if (szVal) {
         ayDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));
         
         // return early if no more space for the string
         if ( ! ayDst )
           return nullptr;
         
         StrLib::vCopy((char*)ayDst, szVal->azChar());

         ayRow->vColVal_ofs(i_Col,ayDst);
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
   UNUSED(iVArgsEnd);
 
   va_end(xVarArgs);
   
   return ayRow;
 }

/*----------------------------------------------------------------------------*/
/* get name of info set                                                       */
/*----------------------------------------------------------------------------*/
Stz* InfoSetBase::szInfoSetNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_sInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* set name of info set                                                       */
/*----------------------------------------------------------------------------*/
void InfoSetBase::vInfoSetNm(Stz* szInfoSetNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_sInfoSetNm = szInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 InfoSetBase::iToOffset(Stz* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i4 iOffset = static_cast<i4>(nDifference(aItem,this));
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(InfoSetBase);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
