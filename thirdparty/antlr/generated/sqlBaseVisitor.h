
// Generated from /Users/alka/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "sqlVisitor.h"


namespace Parser {

/**
 * This class provides an empty implementation of sqlVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  sqlBaseVisitor : public sqlVisitor {
public:

  virtual std::any visitStatements(sqlParser::StatementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(sqlParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatementChoice(sqlParser::StatementChoiceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTags(sqlParser::TagsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyValueTag(sqlParser::KeyValueTagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyOnlyTag(sqlParser::KeyOnlyTagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTagName(sqlParser::TagNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQueryCmd(sqlParser::QueryCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuery(sqlParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQueryHint(sqlParser::QueryHintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHint(sqlParser::HintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectQuery(sqlParser::SelectQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectExpr(sqlParser::SelectExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSetQuery(sqlParser::SetQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSetExpr(sqlParser::SetExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCorrClause(sqlParser::CorrClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWithQuery(sqlParser::WithQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubquery(sqlParser::SubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNaturalJoin(sqlParser::NaturalJoinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinUsing(sqlParser::JoinUsingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinOn(sqlParser::JoinOnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFetch(sqlParser::FetchContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValuesSubquery(sqlParser::ValuesSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCrossJoin(sqlParser::CrossJoinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableReference(sqlParser::TableReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinType(sqlParser::JoinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExtendedJoinType(sqlParser::ExtendedJoinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValuesExpr(sqlParser::ValuesExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnidxCmd(sqlParser::AnidxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAntblCmd(sqlParser::AntblCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBpaCmd(sqlParser::BpaCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompactIdxCmd(sqlParser::CompactIdxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCountCmd(sqlParser::CountCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateCmd(sqlParser::CreateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateTableCmd(sqlParser::CreateTableCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexCmd(sqlParser::CreateIndexCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeleteCmd(sqlParser::DeleteCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDisconnectCmd(sqlParser::DisconnectCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDloadCmd(sqlParser::DloadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDocolCmd(sqlParser::DocolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDropCmd(sqlParser::DropCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDropTableCmd(sqlParser::DropTableCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDropIndexCmd(sqlParser::DropIndexCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEmptyCmd(sqlParser::EmptyCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainCmd(sqlParser::ExplainCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainOptions(sqlParser::ExplainOptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainStageList(sqlParser::ExplainStageListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainRange(sqlParser::ExplainRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainRangeChoice(sqlParser::ExplainRangeChoiceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExplainFormat(sqlParser::ExplainFormatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExportCmd(sqlParser::ExportCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFileCmd(sqlParser::FileCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertCmd(sqlParser::InsertCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoadCmd(sqlParser::LoadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoadReplicatedCmd(sqlParser::LoadReplicatedCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoadOption(sqlParser::LoadOptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoadOptionKeys(sqlParser::LoadOptionKeysContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogCmd(sqlParser::LogCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLscolCmd(sqlParser::LscolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLsidxCmd(sqlParser::LsidxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLsidxcolCmd(sqlParser::LsidxcolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLsptncolCmd(sqlParser::LsptncolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLstblCmd(sqlParser::LstblCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMemoryleaksCmd(sqlParser::MemoryleaksCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMetricsCtlCmd(sqlParser::MetricsCtlCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNoopCmd(sqlParser::NoopCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineCmd(sqlParser::PipelineCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineStage(sqlParser::PipelineStageContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineOpSeq(sqlParser::PipelineOpSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineOperator(sqlParser::PipelineOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineOpParam(sqlParser::PipelineOpParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelineTypeNames(sqlParser::PipelineTypeNamesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSetTraceCmd(sqlParser::SetTraceCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSleepCmd(sqlParser::SleepCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSleepUnits(sqlParser::SleepUnitsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSpaceCmd(sqlParser::SpaceCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatusCmd(sqlParser::StatusCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSzidxCmd(sqlParser::SzidxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSztblCmd(sqlParser::SztblCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableSizesCmd(sqlParser::TableSizesCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTraceOption(sqlParser::TraceOptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTruncateCmd(sqlParser::TruncateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTrackmemoryCmd(sqlParser::TrackmemoryCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUpdateCmd(sqlParser::UpdateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnloadCmd(sqlParser::UnloadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableDefinition(sqlParser::TableDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTablePartitioning(sqlParser::TablePartitioningContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnDefinition(sqlParser::ColumnDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnNameList(sqlParser::ColumnNameListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIndexDefinition(sqlParser::IndexDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIndexMethod(sqlParser::IndexMethodContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDataType(sqlParser::DataTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSqlDataType(sqlParser::SqlDataTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntervalType(sqlParser::IntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitYearMonthIntervalType(sqlParser::YearMonthIntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDayTimeIntervalType(sqlParser::DayTimeIntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFixedLenChar(sqlParser::FixedLenCharContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVaryingLenChar(sqlParser::VaryingLenCharContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsSubquery(sqlParser::AsSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectOrValues(sqlParser::SelectOrValuesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectList(sqlParser::SelectListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectListItem(sqlParser::SelectListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupByList(sqlParser::GroupByListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupByListItem(sqlParser::GroupByListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByList(sqlParser::OrderByListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrderByListItem(sqlParser::OrderByListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinUsingList(sqlParser::JoinUsingListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinUsingListItem(sqlParser::JoinUsingListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCorrList(sqlParser::CorrListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCorrListItem(sqlParser::CorrListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColAliases(sqlParser::ColAliasesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColAliasList(sqlParser::ColAliasListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColAliasListItem(sqlParser::ColAliasListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyList(sqlParser::PropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty(sqlParser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValueList(sqlParser::ValueListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPredicate(sqlParser::PredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJoinPredicate(sqlParser::JoinPredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCasePredicate(sqlParser::CasePredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCountAll(sqlParser::CountAllContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExtractFunc(sqlParser::ExtractFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScalarFunc(sqlParser::ScalarFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOp(sqlParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCoalesceOp(sqlParser::CoalesceOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarRef(sqlParser::VarRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPositionFunc(sqlParser::PositionFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColRef(sqlParser::ColRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCaseOp(sqlParser::CaseOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteralVal(sqlParser::LiteralValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOverlayFunc(sqlParser::OverlayFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfNullOp(sqlParser::IfNullOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIsNullOp(sqlParser::IsNullOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInOp(sqlParser::InOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTrimFunc(sqlParser::TrimFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLikeOp(sqlParser::LikeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeOfOp(sqlParser::TypeOfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNullIfOp(sqlParser::NullIfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInlineCast(sqlParser::InlineCastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBinaryOp(sqlParser::BinaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBetweenOp(sqlParser::BetweenOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAggFunc(sqlParser::AggFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecodeOp(sqlParser::DecodeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConcatOp(sqlParser::ConcatOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubqueryExpr(sqlParser::SubqueryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfOp(sqlParser::IfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubStringFunc(sqlParser::SubStringFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSubqueryInOp(sqlParser::SubqueryInOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicOp(sqlParser::LogicOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCastOp(sqlParser::CastOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenthetic(sqlParser::ParentheticContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodCall(sqlParser::MethodCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableName(sqlParser::VariableNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnRef(sqlParser::ColumnRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnWildcard(sqlParser::ColumnWildcardContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassName(sqlParser::ClassNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionName(sqlParser::FunctionNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodName(sqlParser::MethodNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCatalogName(sqlParser::CatalogNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSchemaName(sqlParser::SchemaNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableName(sqlParser::TableNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableAlias(sqlParser::TableAliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnName(sqlParser::ColumnNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnAlias(sqlParser::ColumnAliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollationName(sqlParser::CollationNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIndexName(sqlParser::IndexNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyName(sqlParser::PropertyNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHintName(sqlParser::HintNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprHint(sqlParser::ExprHintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyValue(sqlParser::PropertyValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHintValue(sqlParser::HintValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdentifier(sqlParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQualifier(sqlParser::QualifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrecision(sqlParser::PrecisionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScale(sqlParser::ScaleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSize(sqlParser::SizeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(sqlParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleLiteral(sqlParser::SimpleLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBooleanLiteral(sqlParser::BooleanLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntegerLiteral(sqlParser::IntegerLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignedIntegerLiteral(sqlParser::SignedIntegerLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecimalLiteral(sqlParser::DecimalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignedDecimalLiteral(sqlParser::SignedDecimalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloatLiteral(sqlParser::FloatLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignedFloatLiteral(sqlParser::SignedFloatLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDateLiteral(sqlParser::DateLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTimeLiteral(sqlParser::TimeLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTimestampLiteral(sqlParser::TimestampLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStringLiteral(sqlParser::StringLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntervalLiteral(sqlParser::IntervalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitYearMonthInterval(sqlParser::YearMonthIntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDayTimeInterval(sqlParser::DayTimeIntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNonReservedWord(sqlParser::NonReservedWordContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace Parser
