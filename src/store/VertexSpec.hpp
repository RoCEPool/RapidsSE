/*============================================================================*/
/* PipeSpec.hpp -- Pipe Specification                                         */
/*============================================================================*/
#pragma once

#include "eYS.h"


namespace STORE { class Dynamo;
                  class StageSpec; class DeckSpec;
                  class OpSpec; class SubPipeSpecs; }

namespace STORE
{
  class VertexSpec : public ListNL_t<VertexSpec>
                   , public ListItem_t<VertexSpec>
  {
    friend class STORE::Dynamo;

  private:
  
    VertexSpec*  m_pParentVertexSpec; // parent of this vertex
    s32 m_sVertexNm;  // aka sub-pipe name
    
    DeckSpec* m_pDeckSpec;
 
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
    Scanner* m_p_Scanner; // statement token scanner
    TagSet*  m_p_JobTags; // job tag set
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    VertexSpec(VertexSpec* pParentVertexSpec,
               Scanner*    p_Scanner,
               TagSet*     p_JobTags);
   ~VertexSpec(void) noexcept(false);
  
  
    void vAddStageSpec(StageSpec* pStageSpec);
    DeckSpec* pDeckSpec(void);
    StageSpec* pFindStageSpec(Stz* szFindNm);
    TagSet* pJobTags(void);
    Stz* szVertexNm(void);
    i8 nStageSpecs(void);
    StageSpec* pStageSpec(i8 i_Stage);

    Footer* aFooter(void);
  };

  VertexSpec* _pVertexSpec(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
