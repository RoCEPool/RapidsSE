/*============================================================================*/
/* KeyWrk.cpp -- Key Working Entry                                            */
/*============================================================================*/

#include "Data.h"
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
  
//#ifdef WITH_PREPOST_CHECKING
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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
  
  char* aDst = aLatter();
  char* aSrc;
  
  if (DM_VCH == pCol->eDatumType())
    aSrc = (char*)(_aStrVal(xVariable.aNestedValue())->aStz());
  else
    aSrc = aAdd<char*>(xVariable.aNestedValue(),sizeof(Value));
  
  
  nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,aSrc);
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddKey(Key* aKey, IdxColVec* pColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  char* aCursor = aKey->aByteSequence();
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
/* extract an index key into KeyWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void KeyWrk::vExtractKey(Entry* aRowEntry, IdxColVec* pIdxColVec)
{
  GenVar xVariable;
  
  RowData* aRowEntryData = aRowEntry->aRowData();
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value from the row for the iColumnNo
    
    char* pDst = aRowEntryData->aColumnDatum(iColumnNo);
    
    switch(pColumn->eDatumType()) {
        
      case DM_BOL: {
        Bol xBol = *((Bol*)pDst);
        xVariable = xBol;
        break;
      }
        
      case DM_DTE: {
        Dte xDte = *((Dte*)pDst);
        xVariable = xDte;
        break;
      }
        
      case DM_BCD: {
#ifdef USE_VLQ_DATUMS
        i8 iInt = IntLib::iVlqToInt((byte*)pDst);
        Bcd xBcd = *((Bcd*)&iInt);
        xVariable = xBcd;
        break;
#else
        Bcd xBcd = *((Bcd*)pDst);
        xVariable = xBcd;
        break;
#endif
      }
        
      case DM_FLT: {
        Flt xFlt = *((Flt*)pDst);
        xVariable = xFlt.dAsD8();
        break;
      }
        
      case DM_INT: {
#ifdef USE_VLQ_DATUMS
        i8 iInt = IntLib::iVlqToInt((byte*)pDst);
        Int xInt = *((Int*)&iInt);
        xVariable = xInt.iAsI8();
        break;
#else
        Int xInt = *((Int*)pDst);
        xVariable = xInt.iAsI8();
        break;
#endif
        
      }
        
      case DM_IVL: {
        Ivl xIvl = *((Ivl*)pDst);
        xVariable = xIvl;
        break;
      }
        
      case DM_TME: {
        Tme xTme = *((Tme*)pDst);
        //bool b = Tme::bExtract(pDst,&xTme,iLength,pColumn);
        //ASSERT(b);
        xVariable = xTme;
        break;
      }
        
      case DM_TSP: {
        Tsp xTsp = *((Tsp*)pDst);
        //bool b = Tsp::bExtract(pDst,&xTsp,iLength,pColumn);
        //ASSERT(b);
        xVariable = xTsp;
        break;
      }
        
      case DM_VBN: {
        NEVER_REACH;
        break;
      }
        
      case DM_VCH: {
        s17KB s;
        s = (Stz*)pDst;
        xVariable = s;
        break;
      }
        
      default:
        NEVER_REACH;
    }
    
    // add the column value to the key
    vAddDatum(xVariable, pColumn);
  }
  
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
  
  EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(xId());

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
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void KeyWrk::vSetEntry(Entry* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  pEntry->vId(xId());
  pEntry->vOffsets(nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = aKey();
  Key* aDstKey = pEntry->aKeyData();
  i8 nKeySize  = this->nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENTRY);
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
