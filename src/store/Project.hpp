/*============================================================================*/
/* Project.hpp -- Project columns                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Project : public Definition
  {
  public:

    u8      m_uProjectColumns;
    s4K     m_sAsmText[C_MAX_COLUMNS];
    i8      m_iStreamInColumns;
    Column* m_pStreamInCol[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Project(void);
   ~Project(void) noexcept(false);

    eEC eProjectAll(StreamDef* pStreamDefIn);
    eEC eProjectColumns(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sProjectColumns);

  private:
//    void vAddCode(i8 i_Column, eEO eCode);
//    void vAddCodeArg(i8 i_Column, eEO eCode, i8 iArg);
    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);
    eEC eProjectColumn(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);

    Footer* aFooter(void);
  };
  
  Project* _pProject(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
