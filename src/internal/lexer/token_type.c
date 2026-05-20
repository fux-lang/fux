#include "src/internal/lexer/token_type.h"

const char *token_type_to_string(enum token_type type) {
        switch (type) {
                case IDENTIFIER: return "IDENTIFIER";
                
                case HEX_LITERAL: return "HEX_LITERAL";
                case OCT_LITERAL: return "OCT_LITERAL";
                case BIN_LITERAL: return "BIN_LITERAL";
                case INT_LITERAL: return "INT_LITERAL";
                case FLOAT_LITERAL: return "FLOAT_LITERAL";
                case CHAR_LITERAL: return "CHAR_LITERAL";
                case STRING_LITERAL: return "STRING_LITERAL";

                case BREAK: return "BREAK";
                case CONTINUE: return "CONTINUE";
                case CONST: return "CONST";
                case DEFER: return "DEFER";
                case ELSE: return "ELSE";
                case ENUM: return "ENUM";
                case FN: return "FN";
                case FOR: return "FOR";
                case IF: return "IF";
                case IMPORT: return "IMPORT";
                case INTERFACE: return "INTERFACE";
                case MATCH: return "MATCH";
                case PACKAGE: return "PACKAGE";
                case RETURN: return "RETURN";
                case STRUCT: return "STRUCT";
                case TYPE: return "TYPE";
                case VAR: return "VAR";

                case PLUS: return "PLUS";
                case PLUSEQ: return "PLUSEQ";
                case INC: return "INC";
                case MINUS: return "MINUS";
                case MINUSEQ: return "MINUSEQ";
                case DEC: return "DEC";
                case STAR: return "STAR";
                case STAREQ: return "STAREQ";
                case SLASH: return "SLASH";
                case SLASHEQ: return "SLASHEQ";
                case MOD: return "MOD";
                case MODEQ: return "MODEQ";

                case ASSIGN: return "ASSIGN";

                case EQ: return "EQ";
                case NEQ: return "NEQ";
                case GT: return "GT";
                case GTEQ: return "GTEQ";
                case LT: return "LT";
                case LTEQ: return "LTEQ";

                case AND: return "AND";
                case OR: return "OR";
                case NOT: return "NOT";

                case AMP: return "AMP";
                case AMPEQ: return "AMPEQ";
                case PIPE: return "PIPE";
                case PIPEQ: return "PIPEQ";
                case TILDA: return "TILDA";
                case TILDAEQ: return "TILDAEQ";
                case XOR: return "XOR";
                case XOREQ: return "XOREQ";

                case COMMA: return "COMMA";
                case COLON: return "COLON";
                case SCOPE: return "SCOPE";
                case SEMICOLON: return "SEMICOLON";
                case DOT: return "DOT";
                case RANGE: return "RANGE";
                case ARGS: return "ARGS";

                case AT: return "AT";
                case HASH: return "HASH";
                case QUESTION: return "QUESTION";

                case ARROW: return "ARROW";
                case BOLD_ARROW: return "BOLD_ARROW";

                case LPAR: return "LPAR";
                case RPAR: return "RPAR";
                case LBRACKET: return "LBRACKET";
                case RBRACKET: return "RBRACKET";
                case LBRACE: return "LBRACE";
                case RBRACE: return "RBRACE";

                case EOF: return "EOF";
        }
}
