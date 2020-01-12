/*============================================================================*/
/* BufferSort.cpp -- Buffer Sort                                              */
/*============================================================================*/

#include "SortBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
BufferCursor* BufferSort::pPartitionBuffer(SrtBuffer* pSrtBuffer,
                                           BufferCursor* pLowCursor, BufferCursor* pHighCursor)
{
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // point a cusror at the pivot point
  BufferCursor* pPivotCursor = new BufferCursor(pLowCursor);
  
  //  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
  //         *pPivotCursor->m_aElement,
  //         *pLowCursor->m_aElement,
  //         *pHighCursor->m_aElement);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // define the start and end positions
  BufferCursor* pLeft = new BufferCursor(pLowCursor);
  BufferCursor* pRight = new BufferCursor(pHighCursor);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the pivot element value
  i8 iPivotValue = pPivotCursor->m_pSrtBuffer->aSrtSet(pPivotCursor->m_iElement)->iKey();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    //vTrace("\nLeft %lld Right %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - -
    // move start cursor forward till we go beyond the pivot value
    while (pLeft->bIsLessThan(iPivotValue)) {
      if ( ! pLeft->bMoveRight() )
        break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward till pivot value is beyond it
    while (pRight->bIsGreaterThan(iPivotValue)) {
      if ( ! pRight->bMoveLeft() )
        break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // crossed
    if (pLeft->bIsAfterOrSame(pRight)) {
      // create a Partition cursor intially with a nullptr element
      BufferCursor* pPartitionCursor = new BufferCursor(pRight);
      //vTrace("Returning %lld\n"_Stz, *pRight->m_aElement);
      
      // cleanup
      delete pLeft;
      delete pRight;
      delete pPivotCursor;
      
      return pPartitionCursor;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
    // swap the inverted eIright and left) lements
    //vTrace("Swapping %lld %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    //pLeft->m_pSrtBuffer->vShow(pColumnVec);
    //pRight->m_pSrtBuffer->vShow(pColumnVec);
    pLeft->vSwap(pRight);
    //pLeft->m_pSrtBuffer->vShow(pColumnVec);
    //pRight->m_pSrtBuffer->vShow(pColumnVec);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move start cursor forward
    pLeft->bMoveRight();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward
    pRight->bMoveLeft();
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void BufferSort::vQuickSortBuffer(SrtBuffer* pSrtBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor)
{
  
  if (pLowCursor->bIsBefore(pHighCursor)) {
    BufferCursor* pPartitionCursor = pPartitionBuffer(pSrtBuffer, pLowCursor, pHighCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from low to partition
    vQuickSortBuffer(pSrtBuffer, pLowCursor, pPartitionCursor);
    
    //pSrtBuffer->vShow(pColumnVec);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from partition+1 to high
    pPartitionCursor->bMoveRight();
    BufferCursor* pPartitionNext = new BufferCursor(pPartitionCursor);
    vQuickSortBuffer(pSrtBuffer, pPartitionNext, pHighCursor);
    
    delete pPartitionCursor;
    delete pPartitionNext;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
