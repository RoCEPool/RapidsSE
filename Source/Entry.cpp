/*============================================================================*/
/* Entry.cpp -- buffer entry                                                  */
/*============================================================================*/

#include "DBobjects.h"

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
   m_xId.vOffsets(nOffsets);
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWrk incl Object & Footer */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(EntWrk& xEntWrk)
{
  i8 nEmbeddedBytes = xEntWrk.nEmbeddedSize();
  
  i8 nEntryBytes = Entry::nBytesRequired(nEmbeddedBytes);
  
  return nEntryBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object & Footer (no offsets)       */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nEmbeddedBytes)
{
  //
  // memory layout is:  Entry|EmbeddedObject
  
  i8 nEntryBytes = sizeof(Entry) + nEmbeddedBytes ;
  
  return nEntryBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object, Footer & Offsets           */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nEmbeddedBytes, i8 nOffsets)
{
  //
  // memory layout is:  Entry|EmbeddedObject|Footer|Offsets
  //
  
  i8 nOffsetBytes = RightVec::nBytesRequired(nOffsets);
  
  i8 nTotalBytes = sizeof(Entry)
                    + nEmbeddedBytes
                    + nOffsetBytes;
  
  return nTotalBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Entry Proper ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the entry buffer that this entry references                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* Entry::pReferencedEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  //EntryBuffer* pEntryBuffer = m_xId.pReferencedEntryBuffer();
  EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(m_xId);

  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Row Data ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* Entry::aRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  Row* aRow = reinterpret_cast<Row*>(aDataArea());
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get row data address                                                       */
/*----------------------------------------------------------------------------*/
char* Entry::aRowData(void)                   //! this function just seems wrong
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  i8 nOffsetBytes = RightVec::nBytesRequired(nOffsets());

  char* aKey = aAdd<char*>(aDataArea(),nOffsetBytes);
  // assuming key is CT_INTEGER for hidden index
  char* aRow = aAdd<char*>(aKey, sizeof(i8));                        //ALKA-TODO
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get row data size                                                          */
/*----------------------------------------------------------------------------*/
i8 Entry::nRowSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  //i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  i8 iRowSize = nSizeItem() - (sizeof(Entry) + sizeof(Footer));
  
  return iRowSize;
}

/*----------------------------------------------------------------------------*/
/*-- Data entry --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the dataRow entry matching entry from the tables hidden index          */
/*----------------------------------------------------------------------------*/
Entry* Entry::aDataEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 

  // get the referenced entry buffer of this entries Id
  EntryBuffer* pEntryBuffer = pReferencedEntryBuffer();
  
  // get the row entry
  Entry* aRowEntry = pEntryBuffer->aGetEntryRowNo(m_xId.iItem());
  
  return aRowEntry;
}



/*----------------------------------------------------------------------------*/
/*-- Key ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Index -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set entry given working entry                                              */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(i8 xTxId, Id xId, i8 nOffsets, Stz* aData, i8 nDataSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  ASSERT(nSizeItem());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  m_xId = xId;
  //m_iTxId = xTxId;
  vOffsets(nOffsets);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row
  char* aEntryData = (char*)this->aRow();
  
  if (nDataSize)
    MemLib::vCopy(aEntryData,aData,nDataSize,eClass());

#ifdef WITH_DEBUG_VERSION
  m_iIntKey = aKey()->iGetInt();
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
}

/*----------------------------------------------------------------------------*/
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  m_xId = xKeyWrk.xId();
  //m_iTxId = xKeyWrk.iTxId();
  vOffsets(xKeyWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = xKeyWrk.aKey();
  Key* aDstKey = this->aKey();
  i8 nKeySize = xKeyWrk.nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
}

/*----------------------------------------------------------------------------*/
/* set entry using given row working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  m_xId = xRowWrk.xId();
  //m_iTxId = xRowWrk.iTxId();
  vOffsets(xRowWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row into this entry
  
  Row* aSrcRow = xRowWrk.aRow();
  Row* aDstRow = this->aRow();
  i8 nRowSize = xRowWrk.nRowSize();
 
  MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iOffset, Index* pIndex, bool bShowKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  Key* aKey = this->aKey();
  
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
    Row* aThisRow = aRow();
    vTrace("Row: "_Stz);
    i8 iRowSize = nRowSize();
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
    
  vTrace("Entry %p: ThisBytes %lld %p (%lld bytes) \n"_Stz,
         this,
         //m_iTxId,
         nSizeItem(),
         aRowData(),
         nRowSize()
         );
}

/*----------------------------------------------------------------------------*/
/* extract an index key into KeyWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void Entry::vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  GenVar xVariable;
  
  Row* aThisRow = aRow();
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

/*----------------------------------------------------------------------------*/
/* extract a sort key into keyWrk from this row using given index columns     */
/*----------------------------------------------------------------------------*/
void Entry::vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  w8 wColumnWord;
  
  Row* aThisRow = aRow();
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get address of the column datum from the row for the iColumnNo
    char* aColumnDatum = aThisRow->aColumnDatum(iColumnNo);
    
    // convert column datum into column word
    switch (pColumn->eDatumType()) {
      
      case Datums::DM_BOL:
        wColumnWord = (*((Bol*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_DTE:
        wColumnWord = (*((Dte*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_BCD:
        wColumnWord = (*((Bcd*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_FLT:
        MemLib::vCopy(&wColumnWord,aColumnDatum,sizeof(w8),CN_ENTRY);
        break;
      
      case Datums::DM_INT:
        wColumnWord = (*((Int*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_IVL:
        wColumnWord = (*((Ivl*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_TME:
        wColumnWord = (*((Tme*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_TSP:
        wColumnWord = (*((Tsp*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_VBN:
        NEVER_REACH;
        break;
      
      case Datums::DM_VCH:
        wColumnWord = (w8)aColumnDatum;
        break;
      
      default:
      NEVER_REACH;
    }
    
    // add the column word to the key
    xKeyWrk.vAddWord(wColumnWord);
  }
  
}

/*----------------------------------------------------------------------------*/
/* extract a sort key into keyWrk from this row using given index columns     */
/*----------------------------------------------------------------------------*/
char* Entry::pExtractSortKey(char* aElement, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); 
  
  w8 wColumnWord;
  
  Row* aThisRow = aRow();
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get address of the column datum from the row for the iColumnNo
    char* aColumnDatum = aThisRow->aColumnDatum(iColumnNo);
    
    // convert column datum into column word
    switch (pColumn->eDatumType()) {
        
      case Datums::DM_BOL:
        wColumnWord = (*((Bol*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_DTE:
        wColumnWord = (*((Dte*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_BCD:
        wColumnWord = (*((Bcd*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_FLT:
        MemLib::vCopy(&wColumnWord,aColumnDatum,sizeof(w8),CN_ENTRY);
        break;
        
      case Datums::DM_INT:
        wColumnWord = (*((Int*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_IVL:
        wColumnWord = (*((Ivl*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_TME:
        wColumnWord = (*((Tme*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_TSP:
        wColumnWord = (*((Tsp*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_VBN:
        NEVER_REACH;
        break;
        
      case Datums::DM_VCH:
        wColumnWord = (w8)aColumnDatum;
        break;
        
      default:
        NEVER_REACH;
    }
    
    // add the column word to the element
    MemLib::vCopy(aElement, &wColumnWord, sizeof(w8), CN_TABLE);
    aElement = aAdd<char*>(aElement,sizeof(w8));
  }
  
  return aElement;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
