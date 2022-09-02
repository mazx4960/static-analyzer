#include "simplegrammar.h"
#include "token.h"
#include "exceptions.h"

ProgramNode* ProgramGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<ProcedureNode*> procedures{};
  ProcedureGrammarRule procedureParser{};
  procedures.push_back(procedureParser.parseNode(tokenStream));
  while (*tokenStream != EndOfFileToken()) {
    procedures.push_back(procedureParser.parseNode(tokenStream));
  }
  return new ProgramNode(procedures);
}

ProcedureNode* ProcedureGrammarRule::parseNode(TokenIterator& tokenStream) {
  if (*tokenStream != KeywordToken("procedure")) {
    throw ParseSyntaxError("procedure expected at" + tokenStream->value);
  }
  ++tokenStream;
  if (tokenStream->type != TokenType.SYMBOL || tokenStream.type != TokenType.KEYWORD) {
    throw ParseSyntaxError("procedure name expected at" + tokenStream->value);
  }
  std::string procedureName = tokenStream->value;
  ++tokenStream;
  if (tokenStream != SeparatorToken("{")) {
    throw ParseSyntaxError("'{' expected at" + tokenStream->value);
  }
  ++tokenStream;
  StatementListNode statementList = StatementListGrammarRule().parseNode(tokenStream);
  if (tokenStream != SeparatorToken("}")) {
    throw ParseSyntaxError("'}' expected at" + tokenStream->value);
  }
  ++tokenStream;
  return new ProcedureNode(procedureName, statementList);
}