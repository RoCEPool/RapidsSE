/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_CACHELINES = 256;
  
  typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_CACHELINES> RowParts;
  
  class RowEncoder : public Object
  {
  private:
    RowParts* m_pRowParts;
    i8        m_i_Offset;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowEncoder(RowParts* pRowParts);
    ~RowEncoder(void) noexcept(false);

    RowParts* pRowParts(void);
  
  private:
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
