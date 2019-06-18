/*============================================================================*/
/* Test.hpp -- test functions                                                 */
/*============================================================================*/
#pragma once

void vTest1(void);
void vTest2(void);
void vTest3(void);
void vTest4(void);
void vTest5(void);
void vTest6(void);
void vTest7(void);
void vTest8(void);
void vTest9(void);
void vTest10(void);
void vTest11(void);
void vTest12(void);

/*----------------------------------------------------------------------------*/
/* cursor used by quicksort                                                   */
/*----------------------------------------------------------------------------*/
class Cursor {
public:
  i8* m_aElement;

public:
  Cursor(i8* aValue) { m_aElement = aValue; }
  Cursor(Cursor& xCursor) { m_aElement = xCursor.m_aElement; }
  Cursor(Cursor& xLowCursor, Cursor& xHighCursor) {
    m_aElement = xLowCursor.m_aElement + (xHighCursor.m_aElement - xLowCursor.m_aElement) / 2;
  }
  
  ~Cursor(void) { }
  
  // incremnet
  void vInc()
  {
    m_aElement++;
  }
  
  // decreement
  void vDec()
  {
    m_aElement--;
  }

  // set value
  void vSet(Cursor& xCursor) {
    m_aElement = xCursor.m_aElement;
  }
  
  // swap elements of cursor
  void vSwap(Cursor& xCursor) {
    i8 iTemp = *xCursor.m_aElement;
    *xCursor.m_aElement = *m_aElement;
    *m_aElement = iTemp;
  }
  
  // dereference
  i8 operator *(void)
  {
    return *m_aElement;
  }
  
  // less than
  bool operator <(Cursor xCursor)
  {
    if (m_aElement < xCursor.m_aElement)
      return true;
    else
      return false;
  }
  
  // greater than
  bool operator >(Cursor xCursor)
  {
    if (m_aElement > xCursor.m_aElement)
      return true;
    else
      return false;
  }
  
  // greater than equal to
  bool operator >=(Cursor xCursor)
  {
    if (m_aElement >= xCursor.m_aElement)
      return true;
    else
      return false;
  }
  
  // less than value
  bool operator <(i8 iValue)
  {
    if (*m_aElement < iValue)
      return true;
    else
      return false;
  }
  
  // greater than value
  bool operator >(i8 iValue)
  {
    if (*m_aElement > iValue)
      return true;
    else
      return false;
  }
  
};

// Quicksort using cursor
void vPartition(i8* A, Cursor& xLow, Cursor& xHigh, Cursor& xPivotCursor);
void vQuickSort(i8* A, Cursor& xLow, Cursor& xHigh);

// Quicksort using pointers to array elements
i8* aPartition(i8* A, i8* aLow, i8* aHigh);
void vQuickSort(i8* A, i8* aLow, i8* aHigh);

// Quicksort using indexes to array elements
i8 iPartition(i8* A, i8 iLow, i8 iHigh);
void vQuickSort(i8* A, i8 iLow, i8 iHigh);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

