/*============================================================================*/
/* KeyWrk.cpp -- Key Working Entry                                            */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

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
: EntWrk(CN_KEYWRK)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
//#ifdef __DEBUG_CHECKS
  //m_xWsp.vFill(0,CN_KEYWRK);                                       ALKA!!!
//#endif
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
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
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(char* aDatum)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = aLatter();
  char* aSrc = (char*)aDatum;
  i8 nSize = StrLib::iSize(aSrc);
  
  MemLib::vCopy(aDst,aSrc,nSize,CN_KEYWRK);
  
  vTake(nSize);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(ValWrk& xValWrk, Column* pCol, Formatters* pFormatters)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  char* pDst = aLatter();
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
  
  vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(ValWrk& xValWrk, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  char* pDst = aLatter();
  char* pSrc;
  
  if (DM_VCH == pCol->eDatumType())
    pSrc = (char*)(_pStrVal(xValWrk.pNestedValueSelf())->aStr());
  else
    pSrc = pAdd<char*>(xValWrk.pNestedValueSelf(),sizeof(Value));
  
  
  nBytes = DtmLib::nCopy(pCol->eDatumType(),pDst,pSrc,pCol);
  
  vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddKey(Key* aKey, IdxColVec* pColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  char* aCursor = aKey->aWspArea();
  i8 nBytes;
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    Column* pColumn = pColVec->pGet(iColumn);
    
    char* aElement = aKey->aEachDatum(pColumn, nBytes,&aCursor);
    
    char* pDst = aLatter();
    char* pSrc = aElement;
    
    DtmLib::vCopy(pColumn->eDatumType(),pDst,pSrc,pColumn);
    
    vTake(nBytes);
  }
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add word to key workspace                                                  */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddWord(w8 wWord)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = aLatter();
  
  MemLib::vCopy(aDst, &wWord, sizeof(w8), CN_KEYWRK);
  
  vTake(sizeof(w8));
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* KeyWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(aEmbedded());
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nKeySize(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return nEmbeddedSize();
}

/*----------------------------------------------------------------------------*/
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void KeyWrk::vKeySize(i8 nKeySize)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  vEmbeddedSize(nKeySize);
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);

  vEmbeddedSize(0);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  Key* aKey = this->aKey();
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
