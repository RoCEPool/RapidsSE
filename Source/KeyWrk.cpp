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
    
    switch(pColumn->eColumnType()) {
      case CT_DATE:      ValLib::vCopyDteDatum((char*)pDst,(char*)pSrc); break;
      case CT_FLOAT:     ValLib::vCopyFltDatum((char*)pDst,(char*)pSrc); break;
      case CT_INTEGER:   ValLib::vCopyIntDatum((char*)pDst,(char*)pSrc); break;
      case CT_INTERVAL:  ValLib::vCopyIvlDatum((char*)pDst,(char*)pSrc); break;
      case CT_TIME:      ValLib::vCopyTmeDatum((char*)pDst,(char*)pSrc); break;
      case CT_TIMESTAMP: ValLib::vCopyTspDatum((char*)pDst,(char*)pSrc); break;
      case CT_VARCHAR:   ValLib::vCopyStrDatum((char*)pDst,(char*)pSrc); break;
      default:
        ASSERT(0);
    }
    
    
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
  Datums::vSprint(sSprintBuffer.aStr(), (Str*)m_xWsp.aArea(), eDatum, nBytesTaken);
  vTrace("%s|"_Str,sSprintBuffer.s());
  
  // subsequent elements
  i8 iOffset = nBytesTaken;
  while (eDM eDatum = (eDM)va_arg(xVarArgs,int)) {
    
    Datums::vSprint(sSprintBuffer.aStr(),
                    (Str*)m_xWsp.aArea() + iOffset,
                    eDatum,
                    nBytesTaken);
    
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
      nBytes = ValLib::nCopyBolDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_INTEGER:
      iValue = StrLib::iToI8(pSrc);
      nBytes = ValLib::nCopyIntDatum(pDst,(void*)&iValue);
      break;
      
    case CT_VARCHAR:
      nBytes = ValLib::nCopyStrDatum(pDst,pSrc);
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
      nBytes = ValLib::nCopyBolDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_DATE:
      pSrc = (char*)(_pDteVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyDteDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_FLOAT:
      pSrc = (char*)(_pFltVal(xValWrk.pNestedValueSelf())->aFlt());
      nBytes = ValLib::nCopyFltDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_INTEGER:
      pSrc = (char*)(_pIntVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyIntDatum((char*)pDst,(char*)pSrc);
      break;
    
    case CT_INTERVAL:
      pSrc = (char*)(_pIvlVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyIvlDatum((char*)pDst,(char*)pSrc);
      break;
    
    case CT_TIME:
      pSrc = (char*)(_pTmeVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyTmeDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_TIMESTAMP:
      pSrc = (char*)(_pTspVal(xValWrk.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyTspDatum((char*)pDst,(char*)pSrc);
      break;
      
    case CT_VARCHAR:
      pSrc = (char*)(_pStrVal(xValWrk.pNestedValueSelf())->aStr());
      nBytes = ValLib::nCopyStrDatum((char*)pDst,(char*)pSrc);
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
/* extract element from row                                                   */
/*----------------------------------------------------------------------------*/
bool KeyWrk::bExtractElement(s48K& sRow, i8 iElement, eDM eDatum)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  vTrace("Extracting element %lld %s from %s\n"_Str,
         iElement,
         aDM[eDatum],
         sRow.s());
  
  Str* aElem = sRow.aStr();
  i8 iFound = 0;
  
  while (iFound != iElement) {
    if ('|' == *aElem->s())
      iFound++;
    aElem++;
  }
  
  i8 nBytesTaken;
  bool b = Datums::bExtractDatum((Str*)m_xWsp.aLatter(),
                                 aElem,eDatum,nBytesTaken);
  
  m_xWsp.vTake(nBytesTaken);

  MemLib::vHexDump(m_xWsp.aArea(),m_xWsp.nUsed());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(KeyWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
