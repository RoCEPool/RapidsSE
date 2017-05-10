/*============================================================================*/
/* KeyWsp.cpp -- Key Workspace                                                */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: KeyWsp class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWsp::operator new(size_t iSize, KeyWsp* pKeyWsp)
{
  RASSERT(iSize);
  return pKeyWsp;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWsp::KeyWsp(void)
: Object(CN_KEYWSP)
{
  _precondition_(_MAGIC_,CN_KEYWSP);
  
  _checkInitialisedSize_(sizeof(KeyWsp));
  m_xWsp.vFill(0);
  
  _postcondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyWsp::~KeyWsp(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void KeyWsp::vAddKey(Key* pKey, IdxColVec* pColVec)
{
  _precondition_(_MAGIC_,CN_KEYWSP);
  
  char* aCursor = pKey->aWspArea();
  i8 nBytes;
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    Column* pColumn = pColVec->pGet(iColumn);
    
    char* aElement = pKey->aElement(pColumn, nBytes,&aCursor);
    
    char* pDst = m_xWsp.aLatter();
    char* pSrc = aElement;
    
    switch(pColumn->eColumnType()) {
      case CT_INTEGER:  ValLib::vCopyIntDatum((char*)pDst,(char*)pSrc); break;
      case CT_INTERVAL: ValLib::vCopyIvlDatum((char*)pDst,(char*)pSrc); break;
      case CT_VARCHAR:  ValLib::vCopyStrDatum((char*)pDst,(char*)pSrc); break;
      default:
        ASSERT(0);
    }
    
    
    m_xWsp.vTake(nBytes);
    
  }
  
  _checkInitialisedSize_(sizeof(KeyWsp));
  
  _postcondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWsp::vShow(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  i8 iKey = *((i8*)m_xWsp.aArea());
  vTrace("KeyWsp %p: bytesUsed %lld value %lld\n"_Str,
         this,
         nWspUsed(),
         iKey);
  
  
}

/*----------------------------------------------------------------------------*/
/* decode show of keywsp contents                                             */
/*----------------------------------------------------------------------------*/
void KeyWsp::vShow(eDM eDatum, ...)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,eDatum);
  
  vTrace("KeyWsp %p: bytesUsed %lld "_Str,
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
void KeyWsp::vAddElement(Values::StrVal* pStrVal, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  i8 nBytes;
  
  char* pDst = m_xWsp.aLatter();
  char* pSrc = (char*)pStrVal->aStr();
  i8 iValue;
  
  switch(pCol->eColumnType()) {
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
void KeyWsp::vAddElement(ValWsp& xValWsp, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  i8 nBytes;
    
  char* pDst = m_xWsp.aLatter();
  char* pSrc;
  
  switch(pCol->eColumnType()) {

    case CT_INTEGER:
      pSrc = (char*)(_pIntVal(xValWsp.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyIntDatum((char*)pDst,(char*)pSrc);
      break;
    
    case CT_INTERVAL:
      pSrc = (char*)(_pIvlVal(xValWsp.pNestedValueSelf())->aInt());
      nBytes = ValLib::nCopyIvlDatum((char*)pDst,(char*)pSrc);
      break;
    
    case CT_VARCHAR:
      pSrc = (char*)(_pStrVal(xValWsp.pNestedValueSelf())->aStr());
      nBytes = ValLib::nCopyStrDatum((char*)pDst,(char*)pSrc);
      break;
    
    default:
      ASSERT(0);
  }
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void KeyWsp::vReset(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
 
  m_xWsp.vUsed(0);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* KeyWsp::aWspArea(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
Str* KeyWsp::aWspStr(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* get wsp bytes used                                                         */
/*----------------------------------------------------------------------------*/
i8 KeyWsp::nWspUsed(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/* extract element from row                                                   */
/*----------------------------------------------------------------------------*/
bool KeyWsp::bExtractElement(s48K& sRow, i8 iElement, eDM eDatum)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
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
_pFooter_(KeyWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
