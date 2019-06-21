/*============================================================================*/
/* RowCompactor.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_METABITS = 8;
  
  static const i8 _1B_C   = 1;  // <C>  1B no. of compressed bytes
  static const i8 _1B_L   = 1;  // <L>  1B string length in bytes
  static const i8 _1B_M   = 1;  // <M>  1B overflowed metacode
  
  static const i8 _2B_FT  = 2;  // |FT| absolute from-tail i_Line
  static const i8 _2B_IT  = 2;  // [IT] relative into-tail i_Line
  static const i8 _2B_OT  = 2;  // [OT] relative over-tail i_Line
  
  static const i8 _3B_DSR = 3; // [DSR] relative distance to shoco

  static const i8 C_OFFSET_UNITS = 0;
  static const i8 C_OFFSET_COMPRESSED = C_OFFSET_UNITS + _1B_U;
  
  static const i8 C_HEAD_MIN_BYTES = 24;

  /*----------------------------------------------------------------------------*/
  
  class RowCompactor : public Object,
                       private RowCoder
  {
  public:
  
    bool       m_bEncoding;    // are we encoding (or copying) ?
    ColumnVec* m_pColVec;
    i8         m_nMetaData;    // size of meta data for this colvec/table
    i8         m_i_BaseOffset; // base offset for this colvec/table
  
    RowNub*    m_pIntoNub;
    i8         m_i_IntoHead;
    HeadLine*  m_aIntoHead;
    i8         m_nHeadBytes;   // size of head line for this colvec/table
    //---
    i8         m_i_IntoOver;
    OverLine*  m_aIntoOver;
    i8         m_nOverBytes;
    bool       m_bCopyOver;    // is there a overflow portion to be copied?
    //---
    i8         m_i_IntoOffset; // encoding offset
    
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    
    IrwBuffer* m_pRowSamples; // the set of sample rows
    
    RowCompactor*  m_pRowEncoder;

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  

    i8 m_nTotalHeads;

//    i8 m_nHeadFreeBytes;
//    i8 m_nHeadFreeBytes_0_7;
//    i8 m_nHeadFreeBytes_8_15;
//    i8 m_nHeadFreeBytes_16_23;
//    i8 m_nHeadFreeBytes_24_31;
//    i8 m_nHeadFreeBytes_32_39;
//    i8 m_nHeadFreeBytes_40_47;
//    i8 m_nHeadFreeBytes_48_55;
//    i8 m_nHeadFreeBytes_56_63;
//    i8 m_nHeadFreeBytes_64_;

//    Freq m_xFreq;
//
//    //---
//
//    m_xFreq.vInit(7,15,23,31,39,47,55,63,-1);
//
//
//    m_xFreq.vAdd(nBytes);
//
//    m_xFreq.nBuckets();
//    m_xFreq.iBucket(i_Bucket);
//    m_xFreq.iDist(i_Bucket)
//    m_xFreq.nSamples();
//    m_xFreq.nSum();
    //---

    i8 m_nTails;
    i8 m_nTailUnits;

    i8 m_nTotalBuffers;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowCompactor(void);
    ~RowCompactor(void) noexcept(false);
    
    void vCompactBegin(CrwBuffer* pCrwBuffer);
    void vCompactEnd(void);
    bool bCompactRow(Row* pRow);
  
  private:
    void vAnalyseFree(void);
    void vCompactEncoded(RowEncoding* pRowEncoding);
    void vHeadCode(eMC eMetaCode, Dtm wBytes, i8 nBytes, Column* pCol);
    
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
