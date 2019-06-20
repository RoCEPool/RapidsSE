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
void vTest13(void);
void vTest14(void);
void vTest15(void);
void vTest16(void);
void vTest17(void);
void vTest18(void);
void vTest19(void);
void vTest20(void);
void vTest21(void);
void vTest22(void);
void vTest23(void);
void vTest24(void);
void vTest25(void);

#define ARRAY_ELEMENTS 100

/*----------------------------------------------------------------------------*/
/* buffer of arrays                                                           */
/*----------------------------------------------------------------------------*/
class ArrayBuffer
{
public:
  i8 A[10];
  
  ArrayBuffer(void) {
    A[0] = 901;
    A[1] = 37;
    A[2] = 693;
    A[3] = 747;
    A[4] = 828;
    A[5] = 502;
    A[6] = 966;
    A[7] = 991;
    A[8] = 693;
    A[9] = 334;
  }
  ~ArrayBuffer(void) { };
  
  i8* aElement(i8 iElement) { return &A[iElement]; }
};

/*----------------------------------------------------------------------------*/
/* cursor used by quicksort                                                   */
/*----------------------------------------------------------------------------*/
class BufCursor {
public:
  ArrayBuffer* m_pArrayBuffer;  // array buffer
  i8*          m_aElement;      // element in the array buffer
  
public:
  BufCursor(ArrayBuffer* pBuffer, i8 iElement) {
    m_pArrayBuffer = pBuffer;
    m_aElement = pBuffer->aElement(iElement);
  }
  
  BufCursor(ArrayBuffer* pBuffer, i8* aElement) {
    m_pArrayBuffer = pBuffer;
    m_aElement = aElement;
  }
  
  BufCursor(BufCursor& xBufCursor) {
    m_pArrayBuffer = xBufCursor.m_pArrayBuffer;
    m_aElement = xBufCursor.m_aElement;
  }
  
  BufCursor(BufCursor& xLowCursor, BufCursor& xHighCursor) {
    m_aElement = xLowCursor.m_aElement + (xHighCursor.m_aElement - xLowCursor.m_aElement) / 2;
  }
  
  BufCursor(void) { }
  
  // incremnet
  void vNext()
  {
    m_aElement++;
  }
  
  // decrement
  void vPrevious()
  {
    m_aElement--;
  }
  
  // set value
  void vSet(BufCursor& xCursor) {
    m_aElement = xCursor.m_aElement;
  }
  
  // swap elements of cursor
  void vSwap(BufCursor& xCursor) {
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
  bool operator <(BufCursor xBufCursor)
  {
    if (m_aElement < xBufCursor.m_aElement)
      return true;
    else
      return false;
  }
  
  // greater than
  bool operator >(BufCursor xBufCursor)
  {
    if (m_aElement > xBufCursor.m_aElement)
      return true;
    else
      return false;
  }
  
  // greater than equal to
  bool operator >=(BufCursor xBufCursor)
  {
    if (m_aElement >= xBufCursor.m_aElement)
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
  void vNext()
  {
    m_aElement++;
  }
  
  // decrement
  void vPrevious()
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

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Quicksort using indexes to array elements
i8 iPartition(i8* A, i8 iLow, i8 iHigh);
void vQuickSort(i8* A, i8 iLow, i8 iHigh);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Quicksort using pointers to array elements
i8* aPartition(i8* A, i8* aLow, i8* aHigh);
void vQuickSort(i8* A, i8* aLow, i8* aHigh);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Quicksort using cursor
void vPartition(i8* A, Cursor& xLow, Cursor& xHigh, Cursor& xPivotCursor);
void vPartition_MarkI(i8* A, Cursor& xLowCursor, Cursor& xHighCursor, Cursor& xPartitionCursor);
void vQuickSort(i8* A, Cursor& xLow, Cursor& xHigh);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Quicksort using cursor into an array buffer
void vPartitionBuffer(ArrayBuffer* pArrayBuffer, BufCursor& xLow, BufCursor& xHigh, BufCursor& xPivotCursor);
void vQuickSortBuffer(ArrayBuffer* pArrayBuffer, BufCursor& xLow, BufCursor& xHigh);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Quicksort using cursor into an intbuffer
BufferCursor* pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor);
void vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

