/*============================================================================*/
/* RowInflator_v2.cpp -- Row Inflator (using Meta Clue)                       */
/*============================================================================*/

#include "CompactBuffers.h"



/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateSet_v2(i8** aaiDstk,
                                 HeadLine* aHeadLine,
                                 StreamDef* pStreamDef)
{
  _static_

  u1* auMetaClue;
  byte* ayDataByte;
  
  i8 nCols = pStreamDef->nTotalColumns();

  vPrepare_v2(auMetaClue,ayDataByte,nCols,aHeadLine);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the set of columns to be retrieved
  
  i8 nWhichCols = 0;
  i8 iWhichCol[C_MAX_COLUMNS];
  
  i8 i_ColToRetrieve;
  for (;;) {
    
    i_ColToRetrieve = *((*aaiDstk)--);
    
    if (-1 == i_ColToRetrieve)
      break;
    
    // must be in ascending order
    ASSERT(0 == nWhichCols ||
           i_ColToRetrieve > iWhichCol[nWhichCols-1]);
    
    iWhichCol[nWhichCols++] = i_ColToRetrieve;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  char* acStrArea = nullptr;
  
  i8* aiDstk_lcl = *aaiDstk;
  
  byte* ayDstByte = (byte*)++aiDstk_lcl;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column

  i8 i_WhichCol = 0;
  i_ColToRetrieve = iWhichCol[i_WhichCol];

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
        
    u1 uMetaClue = *auMetaClue++;
    u1 uHigh = uMetaClue >> 4;
    u1 uLow  = uMetaClue & 0x0F;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {
      vSkip_v2(uHigh,uLow,ayDataByte,aHeadLine);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column

    vInflate_v2(ayDstByte,ayDataByte,aHeadLine,uHigh,uLow,SI_DEST_PST,acStrArea);


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next column to retrieve
    
    if (++i_WhichCol >= nWhichCols)
      break;
    
    i_ColToRetrieve = iWhichCol[i_WhichCol];
  }
  
  // end for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // update callers aiDstk

  aiDstk_lcl = (i8*)ayDstByte;
  
  *aaiDstk = --aiDstk_lcl;

}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateColumn_v2(RowSingle* pRow, HeadLine* aHeadLine, i8 i_ColToRetrieve)
{
  NEVER_REACH;
}


/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateRow_v2(RowSingle* pRow, HeadLine* aHeadLine)
{
  _static_
  
  u1* auMetaClue;
  byte* ayDataByte;

  i8 nCols = pRow->m_nColumns;

  vPrepare_v2(auMetaClue,ayDataByte,nCols,aHeadLine);

  
  byte* ayDstByte = (byte*) & pRow->m_waColVal[0].m_iWrd;
    
  char* acStrArea = (char*)pRow->m_xWsp.ayArea();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    u1 uMetaClue = *auMetaClue++;
    u1 uHigh = uMetaClue >> 4;
    u1 uLow  = uMetaClue & 0x0F;

    vInflate_v2(ayDstByte,ayDataByte,aHeadLine,uHigh,uLow,SI_AREA_ADR,acStrArea);
    
  }
  // for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateRow_v2(RowSlab* pRowSlab, HeadLine* aHeadLine)
{
  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
