/*============================================================================*/
/* KeyWrk.cpp -- Key Working Object                                           */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: KeyWrk class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t iSize, KeyWrk* pKeyWrk)
{
  RASSERT(iSize);
  return pKeyWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWrk::KeyWrk(void)
: Object(CN_KEYWRK)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  m_xWsp.vFill(0);
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWrk::KeyWrk(Key* aKey, IdxColVec* pColVec)
: KeyWrk()
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  vAddKey(aKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyWrk::~KeyWrk(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddKey(Key* aKey, IdxColVec* pColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  char* aCursor = aKey->aWspArea();
  i8 nBytes;
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    Column* pColumn = pColVec->pGet(iColumn);
    
    char* aElement = aKey->aEachDatum(pColumn, nBytes,&aCursor);
    
    char* pDst = m_xWsp.aLatter();
    char* pSrc = aElement;
    
    DtmLib::vCopy(pColumn->eDatumType(),pDst,pSrc,pColumn);
    
    m_xWsp.vTake(nBytes);
    
  }
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 iKey = *((i8*)m_xWsp.aArea());
  vTrace("KeyWrk %p: bytesUsed %lld value %lld\n"_Str,
         this,
         nWspUsed(),
         iKey);
  
  
}

/*----------------------------------------------------------------------------*/
/* decode show of KeyWrk contents                                             */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(eDM eDatum, ...)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,eDatum);
  
  vTrace("KeyWrk %p: bytesUsed %lld "_Str,
         this,
         nWspUsed()
         );
  
  i8 nBytesTaken;
  s48K sSprintBuffer;
  
  // first element
  DtmLib::vSprint(eDatum,sSprintBuffer.aStr(), (Str*)m_xWsp.aArea(), nullptr);
  vTrace("%s|"_Str,sSprintBuffer.s());
  
  // subsequent elements
  i8 iOffset = nBytesTaken;
  while (eDM eDatum = (eDM)va_arg(xVarArgs,int)) {
    
    DtmLib::vSprint(eDatum,
                    sSprintBuffer.aStr(),
                    (Str*)m_xWsp.aArea() + iOffset,
                    nullptr);
    
    vTrace("%s|"_Str,sSprintBuffer.s());
    
    iOffset += nBytesTaken;
  }

  va_end(xVarArgs);

  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(Values::StrVal* pStrVal, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  char* pDst = m_xWsp.aLatter();
  char* pSrc = (char*)pStrVal->aStr();
  i8 iValue;
  
  switch(pCol->eColumnType()) {
      
    case CT_BOOLEAN:
      iValue = StrLib::iToI8(pSrc);
      nBytes = Bol::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_INTEGER:
      iValue = StrLib::iToI8(pSrc);
      nBytes = Int::nCopy(pDst,(void*)&iValue);
      break;
      
    case CT_VARCHAR:
      nBytes = Vch::nCopy(pDst,pSrc);
      break;
      
    default:
      ASSERT(0);
  }
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(ValWrk& xValWrk, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
    
  char* pDst = m_xWsp.aLatter();
  char* pSrc;
  
  switch(pCol->eColumnType()) {

    case CT_BOOLEAN:
      pSrc = (char*)(_pBolVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Bol::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_DATE:
      pSrc = (char*)(_pDteVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Dte::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_DECIMAL:
      pSrc = (char*)(_pBcdVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Bcd::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_FLOAT:
      pSrc = (char*)(_pFltVal(xValWrk.pNestedValueSelf())->aFlt());
      nBytes = Flt::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_INTEGER:
      pSrc = (char*)(_pIntVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Int::nCopy((char*)pDst,(char*)pSrc);
      break;
    
    case CT_INTERVAL:
      pSrc = (char*)(_pIvlVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Ivl::nCopy((char*)pDst,(char*)pSrc);
      break;
    
    case CT_TIME:
      pSrc = (char*)(_pTmeVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Tme::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_TIMESTAMP:
      pSrc = (char*)(_pTspVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = Tsp::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_VARCHAR:
      pSrc = (char*)(_pStrVal(xValWrk.pNestedValueSelf())->aStr());
      nBytes = Vch::nCopy((char*)pDst,(char*)pSrc);
      break;
    
    default:
      ASSERT(0);
  }
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* KeyWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(m_xWsp.aArea());
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
 
  m_xWsp.vUsed(0);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* KeyWrk::aWspArea(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
Str* KeyWrk::aWspStr(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* get wsp bytes used                                                         */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nWspUsed(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_aFooter_(KeyWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
