/*============================================================================*/
/* QuickSort.cpp -- Quick Sort                                                */
/*============================================================================*/

#include "SortBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
i8 QuickSort::iPartitionBuffer(SrtBuffer* pSrtBuffer,
                               i8 iLowIndex, i8 iHighIndex)
{
  
  i8 iMaxIndex = pSrtBuffer->nItems();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the pivot element value
  i8 iPivotValue = pSrtBuffer->aSrtSet(iLowIndex)->iKey();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    //vTrace("\nLeft %lld Right %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - -
    // move start cursor forward till we go beyond the pivot value
    while (pSrtBuffer->aSrtSet(iLowIndex)->iKey() < iPivotValue) {
      iLowIndex++;
      if ( iLowIndex == iMaxIndex)
        break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward till pivot value is beyond it
    while (pSrtBuffer->aSrtSet(iHighIndex)->iKey() > iPivotValue) {
      iHighIndex--;
      if ( iHighIndex < 0 )
        break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // crossed
    if (iLowIndex >= iHighIndex) {
      return iHighIndex;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
    // swap the inverted right and left) elements
    pSrtBuffer->vSwap(iLowIndex, iHighIndex);
  
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move start cursor forward
    iLowIndex++;
    if ( iLowIndex == iMaxIndex)
      break;
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward
    iHighIndex--;
    if ( iHighIndex < 0 )
      break;
  }
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void QuickSort::vQuickSortBuffer(SrtBuffer* pSrtBuffer,
                                 i8 iLowIndex, i8 iHighIndex)
{
  
  if (iLowIndex < iHighIndex) {
    i8 iPartitionIndex = iPartitionBuffer(pSrtBuffer, iLowIndex, iHighIndex);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from low to partition
    vQuickSortBuffer(pSrtBuffer, iLowIndex, iPartitionIndex);
    
    //pSrtBuffer->vShow(pColumnVec);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from partition+1 to high
    vQuickSortBuffer(pSrtBuffer, iPartitionIndex + 1, iHighIndex);
    
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
