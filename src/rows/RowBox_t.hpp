/*============================================================================*/
/* RowBox.hpp -- Container of a single Row                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowBox : public WspConvergent_t<C_WSP_ROWBUFFER,Stz,Row>
  {
  protected:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowBox(void);
    RowBox(eCN eClassNo);
   ~RowBox(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
