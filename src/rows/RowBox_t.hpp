/*============================================================================*/
/* RowBox_t.hpp -- Row Container API                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  template <i8 SIZE>
  class RowBox_t : public WspConvergent_t<SIZE,Stz,Row>
  {
  protected:

    StreamPix m_oStreamPix;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);

    RowBox_t(void);
    //RowBox_t(eCN eClass, StreamDef* pStreamDef, bool bAddPtnHash);
   ~RowBox_t(void) noexcept(false);
    
////    Row* aAllotRow(i8 nTailSize);
//
//    Row* aFirstRow(void);
//    Row* aLastRow(i8 nRowBytes);
//    Row* aNextRow(Row* aPriorRow, i8 nRowBytes);
//    void vPrintRows(StreamDef* pStreamDef);
//    void vShow(StreamDef* pStreamDef);
    
  private:
    i8   iToOffset(Stz* aItem);
  
    //Footer* aFooter(void);

  };

}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void* RowBox_t<SIZE>::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowBox class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::RowBox_t(void)
{
  //_precondition_(_ADDRESS_);
  
  //_postcondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//template <i8 SIZE>
//RowBox_t<SIZE>::RowBox_t(eCN eClassNo, StreamDef* pStreamDef, bool bAddPtnHash)
//: Object(eClassNo)
//{
//  _precondition_(_ADDRESS_);
//
//  pStreamDef->vToStreamPix(&m_oStreamPix,bAddPtnHash);
//
//  _postcondition_(_ADDRESS_);
//}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::~RowBox_t(void) noexcept(false)
{
  //_precondition_(_ADDRESS_);

}

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
//template <i8 SIZE>
//Row* RowBox_t<SIZE>::aAllotRow(i8 nTailSize)
//{
//  _precondition_(_ADDRESS_,_MAGIC_);
//
//  // allot using bytes as we don't know the size of the item
//  Row* aRow = WspConvergent_t<SIZE,Stz,Row>::ayAllotTail(nTailSize);
//
//  if (! aRow)
//    return nullptr;
//
//  new(aRow) Row();
//
//  return aRow;
//}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
//template <i8 SIZE>
//Footer* RowBox_t<SIZE>::aFooter(void)
//{
//  return &m_xFooter;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
