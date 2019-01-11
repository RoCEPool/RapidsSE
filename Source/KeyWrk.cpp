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
void* KeyWrk::operator new(size_t)
{
  vFatal("fatal: KeyWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t, KeyWrk* pKeyWrk)
{
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
KeyWrk::~KeyWrk(void) noexcept(false)
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
void KeyWrk::vAddDatum(GenVar& xVariable, Column* pCol, Formatters* pFormatters)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  StrVal* aStrVal = _aStrVal(xVariable.aNestedValue());
  
  char* pDst = aLatter();
  char* pSrc = (char*)aStrVal->aStz();
  
  switch(pCol->eDatumType()) {
      
    case DM_BCD: {
      Bcd xBcd;
      bool b = Bcd::bExtract(&pSrc,(Stz*)&xBcd,pCol,pFormatters);
      nBytes = Bcd::nCopy((char*)pDst,(char*)&xBcd);
      break;
    }
      
    case DM_BOL: {
      Bol xBol;
      bool b = Bol::bExtract(&pSrc,(Stz*)&xBol,pCol,pFormatters);
      nBytes = Bol::nCopy((char*)pDst,(char*)&xBol);
      break;
    }
      
    case DM_DTE: {
      Dte xDte;
      bool b = Dte::bExtract(&pSrc,(Stz*)&xDte,pCol,pFormatters);
      nBytes = Dte::nCopy((char*)pDst,(char*)&xDte);
      break;
    }
      
    case DM_INT: {
      i8 iInt = StrLib::iToI8(pSrc);
      nBytes = Int::nCopy(pDst,(void*)&iInt);
      break;
    }
      
    case DM_IVL: {
      Ivl xInterval;
      bool b = Ivl::bExtract(&pSrc,(Stz*)&xInterval,pCol,pFormatters);
      nBytes = Ivl::nCopy(pDst,(void*)&xInterval);
      break;
    }
      
    case DM_FLT: {
      Flt xFloat;
      bool b = Flt::bExtract(&pSrc,(Stz*)&xFloat,pCol,pFormatters);
      nBytes = Flt::nCopy(pDst,(void*)&xFloat);
      break;
    }
      
    case DM_TME: {
      Tme xTime;
      bool b = Tme::bExtract(&pSrc,(Stz*)&xTime,pCol,pFormatters);
      nBytes = Tme::nCopy((char*)pDst,(char*)&xTime);
      break;
    }
      
    case DM_TSP: {
      Tsp xTimestamp;
      bool b = Tsp::bExtract(&pSrc,(Stz*)&xTimestamp,pCol,pFormatters);
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
void KeyWrk::vAddDatum(GenVar& xVariable, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  char* pDst = aLatter();
  char* pSrc;
  
  if (DM_VCH == pCol->eDatumType())
    pSrc = (char*)(_aStrVal(xVariable.aNestedValue())->aStz());
  else
    pSrc = pAdd<char*>(xVariable.aNestedValue(),sizeof(Value));
  
  
  nBytes = DtmLib::nCopy(pCol->eDatumType(),pDst,pSrc);
  
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
    
    DtmLib::vCopy(pColumn->eDatumType(),pDst,pSrc);
    
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
/* get the entry buffer that this key references                              */
/*----------------------------------------------------------------------------*/
EntryBuffer* KeyWrk::pReferencedEntryBuffer(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = xId().pReferencedEntryBuffer();
  
  return pEntryBuffer;
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
