/*============================================================================*/
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_MAX_RECORD = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Record : public Object
  {
  private:
    s16KB m_sRecStr; // the record as a string
  
    char* m_azAppendCursor; // cursor for StrLib::vAppend()
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Record(void);
    ~Record(void) noexcept(false);
  
    static bool bExtractDatum(Datum* pDatum, char** aaSrc,
                              Column* pColumn, Formatters* pFormatters);
    
    static bool bExtract(Datum* pDatum, char** aaSrc,
                         Column* pColumn, Formatters* pFormatters);
  
    static bool bExtractBcdDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractBcdSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractBolDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractBolSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractDteDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractDteSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractFltDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractFltSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractIntDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractIntSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractIvlDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractIvlSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractTmeDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractTmeSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractTspDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractTspSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
    static bool bExtractVchDefault(Datum* pDatum, char** aaSrc,
                                   Column* pColumn, Formatters* pFormatters);
    static bool bExtractVchSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pColumn, Formatters* pFormatters);
  
    void vAppend_Alt(Column* pCol, Datum* pDatum);
    void vAppendBcd(Column* pCol, Datum* pDatum);
    void vAppendBcd_Alt(Column* pCol, Datum* pDatum);
    void vAppendBol(Column* pCol, Datum* pDatum);
    void vAppendBol_Alt(Column* pCol, Datum* pDatum);
    void vAppendDte(Column* pCol, Datum* pDatum);
    void vAppendDte_Alt(Column* pCol, Datum* pDatum);
    void vAppendFlt(Column* pCol, Datum* pDatum);
    void vAppendFlt_Alt(Column* pCol, Datum* pDatum);
    void vAppendInt(Column* pCol, Datum* pDatum);
    void vAppendInt_Alt(Column* pCol, Datum* pDatum);
    void vAppendIvl(Column* pCol, Datum* pDatum);
    void vAppendIvl_Alt(Column* pCol, Datum* pDatum);
    void vAppendTme(Column* pCol, Datum* pDatum);
    void vAppendTme_Alt(Column* pCol, Datum* pDatum);
    void vAppendTsp(Column* pCol, Datum* pDatum);
    void vAppendTsp_Alt(Column* pCol, Datum* pDatum);
    void vAppendVch(Column* pCol, Datum* pDatum);
    void vAppendVch_Alt(Column* pCol, Datum* pDatum);

    void vPrintRow(void);
  
    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);
  
    void vReset(void);
  
    static bool bIsEnclosed(char** aaSrc, Formatters* pFormatters);
    static bool bIsThereFieldData(char* aSrc, Formatters* pFormatters);
    static bool bIsThereRecordData(char* aSrc, Formatters* pFormatters);
    static bool bTestForNull(Column* pColumn,
                             char** aaSrc,
                             Formatters* pFormatters,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pColumn,
                                    char** aaSrc,
                                    Formatters* pFormatters,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
    static bool bValidFieldEnd(char* aSrc, Formatters* pFormatters);
    static bool bValidFieldEndDefault(char* aSrc, Formatters* pFormatters);
    static bool bValidRecordEnd(char* aSrc, Formatters* pFormatters);

    //! REVIEW WHICH FUNCTIONS USE THESE
    static void vSprint(Column* pColumn, Stz* aDst, Stz* aSrc);
    static void vSprintQuotes(Column* pColumn, Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Column* pColumn, Stz* aDst, Stz* aSrc);
  

  private:
    Footer* aFooter(void);
  };
  
  Record* _pRecord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
