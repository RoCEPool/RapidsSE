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
  vFatal("fatal: KeyWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
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
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vFill(0,CN_KEYWRK);
#endif
  
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
  vTrace("KeyWrk %p: bytesUsed %lld value %lld\n"_Stz,
         this,
         nWspUsed(),
         iKey);
  
  
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(ValWrk& xValWrk, Column* pCol, Formatters* pFormatters)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  char* pDst = m_xWsp.aLatter();
  char* pSrc = (char*)pStrVal->aStr();
  i8 iValue;
  
  switch(pCol->eDatumType()) {
      
    case DM_BCD: {
      Bcd xBcd;
      bool b = Bcd::bExtract(&pSrc,&xBcd,pCol,pFormatters);
      nBytes = Bcd::nCopy((char*)pDst,(char*)&xBcd);
      break;
    }
      
    case DM_BOL: {
      Bol xBoolean;
      bool b = Bol::bExtract(&pSrc,&xBoolean,pCol,pFormatters);
      nBytes = Bol::nCopy((char*)pDst,(char*)&xBoolean);
      break;
    }
      
    case DM_DTE: {
      Dte xDate;
      bool b = Dte::bExtract(&pSrc,&xDate,pCol,pFormatters);
      nBytes = Dte::nCopy((char*)pDst,(char*)&xDate);
      break;
    }
      
    case DM_INT:
      iValue = StrLib::iToI8(pSrc);
      nBytes = Int::nCopy(pDst,(void*)&iValue);
      break;
      
    case DM_IVL: {
      Ivl xInterval;
      bool b = Ivl::bExtract(&pSrc,&xInterval,pCol,pFormatters);
      nBytes = Ivl::nCopy(pDst,(void*)&xInterval);
      break;
    }
      
    case DM_FLT: {
      Flt xFloat;
      bool b = Flt::bExtract(&pSrc,&xFloat,pCol,pFormatters);
      nBytes = Flt::nCopy(pDst,(void*)&xFloat);
      break;
    }
      
    case DM_TME: {
      Tme xTime;
      bool b = Tme::bExtract(&pSrc,&xTime,pCol,pFormatters);
      nBytes = Tme::nCopy((char*)pDst,(char*)&xTime);
      break;
    }
      
    case DM_TSP: {
      Tsp xTimestamp;
      bool b = Tsp::bExtract(&pSrc,&xTimestamp,pCol,pFormatters);
      nBytes = Tsp::nCopy((char*)pDst,(char*)&xTimestamp);
      break;
    }
      
    case DM_VCH:
      nBytes = Vch::nCopy(pDst,pSrc);
      break;
      
    default:
      NEVER_REACH;
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
  
  if (DM_VCH == pCol->eDatumType())
    pSrc = (char*)(_pStrVal(xValWrk.pNestedValueSelf())->aStr());
  else
    pSrc = pAdd<char*>(xValWrk.pNestedValueSelf(),sizeof(Value));


  nBytes = DtmLib::nCopy(pCol->eDatumType(),pDst,pSrc,pCol);
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddStz(Stz* aStz)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = m_xWsp.aLatter();
  char* aSrc = (char*)aStz;
  i8 nSize = StrLib::iSize(aSrc);
  
  MemLib::vCopy(aDst,aSrc,nSize,CN_KEYWRK);
  
  m_xWsp.vTake(nSize);
}

/*----------------------------------------------------------------------------*/
/* add word to key workspace                                                  */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddWord(w8 wWord)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = m_xWsp.aLatter();
  
  MemLib::vCopy(aDst, &wWord, sizeof(w8), CN_KEYWRK);
  
  m_xWsp.vTake(sizeof(w8));
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
Stz* KeyWrk::aWspStr(void)
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
