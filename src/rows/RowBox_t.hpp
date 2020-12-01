/*============================================================================*/
/* RowBox.hpp -- Container of a single Row                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
template <i8 SIZE>
  class RowBox : public WspConvergent_t<SIZE,Stz,Row>
  {
  protected:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);

    RowBox(void);
   ~RowBox(void) noexcept(false);
    
    Row* aAllotRow(i8 nBytes);
    Row* aAllotRow(RowSingle* pRowSingle);
    
    Row* aFirstRow(void);
    Row* aLastRow(i8 nRowBytes);
    Row* aNextRow(Row* aPriorRow, i8 nRowBytes);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
    
  private:
    i8   iToOffset(Stz* aItem);
  };

}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void* RowBox<SIZE>::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowBox class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
