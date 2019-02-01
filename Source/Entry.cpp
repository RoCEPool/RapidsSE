/*============================================================================*/
/* Entry.cpp -- buffer entry                                                  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t)
{
  vFatal("fatal: Entry class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t, Entry* aEntry)
{
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(eCN eClassNo, i8 nThisBytes, i8 nOffsets)
: Item(eClassNo, nThisBytes)
{
   m_xId.vOffsets(nOffsets);
  
  _checkInitialised_(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Row Data ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* Entry::aRowData(void)
{
  Row* aRow = reinterpret_cast<Row*>(aDataArea());
  
  return aRow;
}


/*----------------------------------------------------------------------------*/
/*-- Index -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set entry given working entry                                              */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(Id xId)
{
  ASSERT(nSizeItem());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  m_xId = xId;
  
#ifdef WITH_DEBUG_VERSION
  m_iIntKey = aKeyData()->iGetInt();
#endif
}

/*----------------------------------------------------------------------------*/
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(KeyWrk& xKeyWrk)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  m_xId = xKeyWrk.xId();
  vOffsets(xKeyWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = xKeyWrk.aKey();
  Key* aDstKey = this->aKeyData();
  i8 nKeySize = xKeyWrk.nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* set entry using given row working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(RowWrk& xRowWrk)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  m_xId = xRowWrk.xId();
  vOffsets(xRowWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row into this entry
  
  Row* aSrcRow = xRowWrk.aRow();
  Row* aDstRow = this->aRowData();
  i8 nRowSize = xRowWrk.nRowSize();
 
  MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iOffset, Index* pIndex, bool bShowKey)
{
  Key* aKey = this->aKeyData();
  
  vTrace("%lld: Entry: %p Size: %llu Offsets: %llu "_Stz,
         iOffset,
         this,
         nSizeItem(),
         nOffsets()
         );
  
  vTrace("Id: %3lli.%03lli "_Stz,
         m_xId.iBuffer(),
         m_xId.iItem()
         );
  
  //vTrace("Tx: %lld.%03lld "_Stz,
  //       Tx::iTxNode(m_iTxId),
  //       Tx::iTxNo(m_iTxId)
  //       );
  
  if (bShowKey && ! pIndex->bIsHidden())
    aKey->vShow(pIndex->pIdxColVec());
  
  if (pIndex->bIsHidden()) {
    //char* pRow = aRowData();
    Row* aThisRow = aRowData();
    vTrace("Row: "_Stz);
    i8 iRowSize = nSizeItem();
    if (iRowSize > 0) {
      aThisRow->vShow(pIndex->pTable()->pColumnVec(), true);
    }
  }
  
  
  vTrace(" offsets:"_Stz);
  for (i8 i_Offset = 0; i_Offset < nOffsets(); i_Offset++)
    vTrace(" %d"_Stz,aNEO()[i_Offset]);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Row ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  vTrace("Entry %p: ThisBytes %lld %p\n"_Stz,
         this,
         //m_iTxId,
         nSizeItem(),
         aRowData()
         );
}

/*----------------------------------------------------------------------------*/
/* extract an index key into KeyWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void Entry::vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  GenVar xVariable;
  
  Row* aThisRow = aRowData();
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value from the row for the iColumnNo
    
    char* pDst = aThisRow->aColumnDatum(iColumnNo);
    
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
    xKeyWrk.vAddDatum(xVariable, pColumn);
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
