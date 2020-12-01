/*============================================================================*/
/* RowBox_t.hpp -- Row Container API                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  template <i8 SIZE>
  class RowBox_t : public Object,
                   public WspConvergent_t<SIZE,Stz,Row>
  {
  protected:

    ColPix m_oColPix;
    i8     m_nColumns;  // no cols in stream def

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);

    RowBox_t(eCN eClass);
    RowBox_t(eCN eClass, StreamDef* pStreamDef, bool bAddPtnHash);
   ~RowBox_t(void) noexcept(false);
    
    Row* aAllotRow(i8 nBytes);
    Row* aAllotRow(RowSingle* pRowSingle);
    
    Row* aFirstRow(void);
    Row* aLastRow(i8 nRowBytes);
    Row* aNextRow(Row* aPriorRow, i8 nRowBytes);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
    
  private:
    i8   iToOffset(Stz* aItem);
  
    Footer* aFooter(void);

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
RowBox_t<SIZE>::RowBox_t(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);

  m_nColumns = C_UNKNOWN;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::RowBox_t(eCN eClassNo, StreamDef* pStreamDef, bool bAddPtnHash)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);

  m_nColumns = C_UNKNOWN;
  
  pStreamDef->vToColPix(&m_oColPix,bAddPtnHash);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::~RowBox_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Footer* RowBox_t<SIZE>::aFooter(void)
{
  return &m_xFooter;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
