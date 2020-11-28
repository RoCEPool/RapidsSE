/*============================================================================*/
/* Row.cpp -- Computable form of Row                                        */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* Rows::_pRow(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Row class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(void)
  :Object(CN_ROW)
{
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void) noexcept(false)
{
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size of row (needed for MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem))  */
/*----------------------------------------------------------------------------*/
i8 Row::nSizeBytes(StreamDef* pStreamDef)
{
  i8 nSize = sizeof(Row) + (pStreamDef->nColumns(0) - 1) * sizeof(Dtm);
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* dump the row                                                               */
/*----------------------------------------------------------------------------*/
void Row::vDump(StreamDef* pStreamDef)
{
  
  vTrace("ROW: %p"_Stz, this);
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (m_waColVal[i_Col].bIsNull()) {
        vTrace(" NULL "_Stz);
        continue;
      }
      
      Stz* acSrcString = szStzGet(i_Col);
      
      vTrace(" %s "_Stz, acSrcString);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    vTrace(" %ld "_Stz, m_waColVal[i_Col].m_iWrd);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get string column                                                          */
/*----------------------------------------------------------------------------*/
Stz* Row::szStzGet(i8 i_Col)
{
  // get the poolBuffer from the row address
  PoolBuffer* pPoolBuffer = PoolBuffer::pWithinPoolBuffer(this);
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(pPoolBuffer);
  
  Object* pRowBuffer =
  reinterpret_cast<Object*>(pHeapBuffer->aFirstChunk());
  
  // get the offset of the string
  i8 i_Offset = m_waColVal[i_Col].m_iWrd;
  
  // get address of string within pool buffer
  Stz* pStz = pAdd<Stz*>(pRowBuffer,i_Offset);
  
  return pStz;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
