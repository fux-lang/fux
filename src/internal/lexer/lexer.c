#include "lexer.h"

#define LCURR peek(lex, 0)
#define LPEEK(n) peek(lex, n)
#define LINC_POS(n) inc_pos(lex, n)

struct lexer *new_lexer(const char *input);

static char peek(const struct lexer *lex, size_t num);
static void inc_pos(struct lexer *lex, size_t num);

static struct token tokenize_num(struct lexer *lex);
static struct token tokenize_hex(struct lexer *lex);
static struct token tokenize_oct(struct lexer *lex);
static struct token tokenize_bin(struct lexer *lex);
static struct token tokenize_char(struct lexer *lex);
static struct token tokenize_string(struct lexer *lex);
static struct token tokenize_identifier(struct lexer *lex);

struct tokl *tokenize(struct lexer *lex);

void free_lexer(struct lexer *lex);


// Implementation

struct lexer *new_lexer(const char *input) {
        struct lexer *lex = (struct lexer *)calloc(1, sizeof(struct lexer));
        if (!lex) return NULL;
        lex->input = input;
        lex->input_len = strlen(input);
        lex->line = 1;
        lex->col = 1;
        return lex;
}

static char peek(const struct lexer *lex, size_t num) {
        if (lex->pos + num >= lex->input_len) return '\0';
        return lex->input[lex->pos + num];
}

static void inc_pos(struct lexer *lex, size_t num) {
        for (size_t i = 0; i < num; ++i) {
                if (LCURR == '\n') {
                        ++lex->line;
                        lex->col = 1;
                } else {
                        ++lex->col;
                }
                ++lex->pos;
        }
}

static struct token tokenize_num(struct lexer *lex) {
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        struct char_list *val = char_list_new();
        if (!val) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_'
                                || LCURR == '.') {
                        LINC_POS(1);
                }
                return new_token(ERR, "006", start_line, start_col);
        }
        size_t dot_amount = 0;
        while (isdigit((unsigned char)LCURR) || LCURR == '.') {
                if (LCURR == '.') {
                        ++dot_amount;
                        if (dot_amount >= 2) {
                                char_list_free(val);
                                val = NULL;
                                while (isalpha((unsigned char)LCURR)
                                                || isdigit((unsigned char)LCURR)
                                                || LCURR == '.'
                                                || LCURR == '_') {
                                        LINC_POS(1); 
                                }
                                return new_token(ERR, "001", start_line,
                                                start_col);
                        }
                }
                char_list_push(val, LCURR);
                LINC_POS(1);
        }
        if (isalpha((unsigned char)LCURR) || LCURR == '_') {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1); 
                }
                char_list_free(val);
                val = NULL;
                return new_token(ERR, "001", start_line, start_col);
        }
        char_list_push(val, '\0');
        struct token tok = new_token(dot_amount == 0? INT_LITERAL:FLOAT_LITERAL,
                        val->data, start_line, start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_hex(struct lexer *lex) {
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        struct char_list *val = char_list_new();
        if (!val) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                return new_token(ERR, "006", start_line, start_col);
        }
        while (isxdigit((unsigned char)LCURR)) {
                char_list_push(val, LCURR);
                LINC_POS(1);
        }
        if (isalpha((unsigned char)LCURR) || LCURR == '_' || val->size == 0) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                char_list_free(val);
                val = NULL;
                return new_token(ERR, "009", start_line, start_col);
        }
        char_list_push(val, '\0');
        struct token tok = new_token(HEX_LITERAL, val->data, start_line,
                        start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_oct(struct lexer *lex) {
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        struct char_list *val = char_list_new();
        if (!val) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                return new_token(ERR, "006", start_line, start_col);
        }
        while (isdigit((unsigned char)LCURR) && LCURR != '9' && LCURR != '8') {
                char_list_push(val, LCURR);
                LINC_POS(1);
        }
        if (isalpha((unsigned char)LCURR) || isdigit((unsigned char)LCURR)
                        || LCURR == '_' || val->size == 0){
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                char_list_free(val);
                val = NULL;
                return new_token(ERR, "008", start_line, start_col);
        }
        char_list_push(val, '\0');
        struct token tok = new_token(OCT_LITERAL, val->data, start_line,
                        start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_bin(struct lexer *lex) {
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        struct char_list *val = char_list_new();
        if (!val) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                return new_token(ERR, "006", start_line, start_col);
        }
        while (LCURR == '0' || LCURR == '1') {
                char_list_push(val, LCURR);
                LINC_POS(1);
        }
        if (isalpha((unsigned char)LCURR) || isdigit((unsigned char)LCURR)
                        || LCURR == '_' || val->size == 0){
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR) 
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                char_list_free(val);
                val = NULL;
                return new_token(ERR, "007", start_line, start_col);
        }
        char_list_push(val, '\0');
        struct token tok = new_token(BIN_LITERAL, val->data, start_line,
                        start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_char(struct lexer *lex) {
        char ch = '\0';
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        LINC_POS(1);
        if (LCURR == '\\' && LPEEK(2) == '\'') {
                switch(LPEEK(1)) {
                case 'n':
                        ch = '\n';
                        LINC_POS(3);
                        break;
                case 't':
                        ch = '\t';
                        LINC_POS(3);
                        break;
                case 'r':
                        ch = '\r';
                        LINC_POS(3);
                        break;
                case '\'':
                        ch = '\'';
                        LINC_POS(3);
                        break;
                case '\"':
                        ch = '\"';
                        LINC_POS(3);
                        break;
                case '\\':
                        ch = '\\';
                        LINC_POS(3);
                        break;
                case '0':
                        ch = '\0';
                        LINC_POS(3);
                        break;
                default:
                        while (LCURR != '\'' && LCURR != '\0') LINC_POS(1);
                        if (LCURR == '\'') LINC_POS(1);
                        return new_token(ERR, "002", start_line, start_col);
                }
        } else if (LPEEK(1) == '\'') {
                ch = LCURR;
                LINC_POS(2);
        } else if (LCURR == '\'') {
                LINC_POS(1);
                return new_token(ERR, "004", start_line, start_col);
        } else {
                while (LCURR != '\'' && LCURR != '\0') LINC_POS(1);
                if (LCURR == '\'') LINC_POS(1);
                return new_token(ERR, "004", start_line, start_col);
        }
        struct char_list *val = char_list_new();
        if (!val) {
                while (LCURR != '\'' && LCURR != '\0') LINC_POS(1);
                if (LCURR == '\'') LINC_POS(1);
                return new_token(ERR, "006", start_line, start_col);
        }
        char_list_push(val, ch);
        char_list_push(val, '\0');
        struct token tok = new_token(CHAR_LITERAL, val->data, start_line,
                        start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_string(struct lexer *lex) {
        struct char_list *val = char_list_new();
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        LINC_POS(1);
        if (!val) {
                while (LCURR != '\"' && LCURR != '\0') LINC_POS(1);
                if (LCURR == '\"') LINC_POS(1);
                return new_token(ERR, "006", start_line, start_col);
        }
        while (LCURR != '\"' && LCURR != '\0') {
                if (LCURR == '\\') {
                        switch(LPEEK(1)) {
                        case 'n':
                                char_list_push(val, '\n');
                                LINC_POS(2);
                                break;
                        case 't':
                                char_list_push(val, '\t');
                                LINC_POS(2);
                                break;
                        case 'r':
                                char_list_push(val, '\r');
                                LINC_POS(2);
                                break;
                        case '\'':
                                char_list_push(val, '\'');
                                LINC_POS(2);
                                break;
                        case '\"':
                                char_list_push(val, '\"');
                                LINC_POS(2);
                                break;
                        case '\\':
                                char_list_push(val, '\\');
                                LINC_POS(2);
                                break;
                        case '0':
                                char_list_push(val, '\0');
                                LINC_POS(2);
                                break;        
                        default:
                                char_list_free(val);
                                val = NULL;
                                LINC_POS(2);
                                while (LCURR != '\"' && LCURR != '\0') {
                                        LINC_POS(1);
                                }
                                if (LCURR == '\"') LINC_POS(1);
                                return new_token(ERR, "002", start_line,
                                                start_col);
                        }
                } else {
                        char_list_push(val, LCURR);
                        LINC_POS(1);
                }
        }
        if (LCURR == '\0') {
                struct token tok = new_token(ERR, "005", start_line, start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        }
        LINC_POS(1);
        char_list_push(val, '\0');
        struct token tok = new_token(STRING_LITERAL, val->data, start_line,
                        start_col);
        char_list_free(val);
        val = NULL;
        return tok;
}

static struct token tokenize_identifier(struct lexer *lex) {
        size_t start_line = lex->line;
        size_t start_col = lex->col;
        struct char_list *val = char_list_new();
        if (!val) {
                while (isalpha((unsigned char)LCURR)
                                || isdigit((unsigned char)LCURR)
                                || LCURR == '_') {
                        LINC_POS(1);
                }
                return new_token(ERR, "006", start_line, start_col);
        }
        while (isalpha((unsigned char)LCURR) || isdigit((unsigned char)LCURR)
                        || LCURR == '_') {
                char_list_push(val, LCURR);
                LINC_POS(1);
        }
        if (strcmp(val->data, "break") == 0) {
                struct token tok = new_token(BREAK, "break", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "continue") == 0) {
                struct token tok = new_token(CONTINUE, "continue", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "const") == 0) {
                struct token tok = new_token(CONST, "const", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "defer") == 0) {
                struct token tok = new_token(DEFER, "defer", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "else") == 0) {
                struct token tok = new_token(ELSE, "else", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "enum") == 0) {
                struct token tok = new_token(ENUM, "enum", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "for") == 0) {
                struct token tok = new_token(FOR, "for", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "if") == 0) {
                struct token tok = new_token(IF, "if", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "import") == 0) {
                struct token tok = new_token(IMPORT, "import", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "match") == 0) {
                struct token tok = new_token(MATCH, "match", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "package") == 0) {
                struct token tok = new_token(PACKAGE, "package", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "return") == 0) {
                struct token tok = new_token(RETURN, "return", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else if (strcmp(val->data, "struct") == 0) {
                struct token tok = new_token(STRUCT, "struct", start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        } else {
                char_list_push(val, '\0');
                struct token tok = new_token(IDENTIFIER, val->data, start_line,
                                start_col);
                char_list_free(val);
                val = NULL;
                return tok;
        }
}

struct tokl *tokenize(struct lexer *lex) {
        struct tokl *tokens = new_tokl();
        while (LCURR) {
                switch (LCURR) {
                case '+':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(PLUSEQ, "+=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else if (LPEEK(1) == '+') {
                                push_tokl(tokens, new_token(INC, "++",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(PLUS, "+",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '-':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(MINUSEQ, "-=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else if (LPEEK(1) == '-') {
                                push_tokl(tokens, new_token(DEC, "--",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(MINUS, "-",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '*':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(STAREQ, "*=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(STAR, "*",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '/':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(SLASHEQ, "/=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else if (LPEEK(1) == '/') {
                                while (LCURR != '\n' && LCURR != '\0') {
                                        LINC_POS(1);      
                                }
                        } else if (LPEEK(1) == '*') {
                                while (!(LCURR == '*' && LPEEK(1) == '/')
                                                && LCURR != '\0') {
                                        LINC_POS(1);
                                }
                                if (LCURR == '\0') {
                                                push_tokl(tokens, new_token(
                                                                ERR, "010",
                                                                lex->line,
                                                                lex->col));
                                } else LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(SLASH, "/",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '%':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(MODEQ, "%=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(MOD, "%",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '=':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(EQ, "==",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(ASSIGN, "=",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '!':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(NEQ, "!=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(NOT, "!",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;             
                case '>':
                        if (LPEEK(1) == '>') {
                                if (LPEEK(2) == '>') {
                                        if (LPEEK(3) == '=') {
                                                push_tokl(tokens, new_token(
                                                                USHREQ,
                                                                ">>>=",
                                                                lex->line,
                                                                lex->col));
                                                LINC_POS(4);
                                        } else {
                                                push_tokl(tokens, new_token(
                                                                USHR,
                                                                ">>>",
                                                                lex->line,
                                                                lex->col));
                                                LINC_POS(3);
                                        }
                                } else if (LPEEK(2) == '=') {
                                        push_tokl(tokens, new_token(SHREQ,
                                                        ">>=", lex->line,
                                                        lex->col));
                                        LINC_POS(3);
                                } else {
                                        push_tokl(tokens, new_token(SHR,
                                                        ">>", lex->line,
                                                        lex->col));
                                        LINC_POS(2);
                                }
                        } else if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(GTEQ, ">=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(GT, ">", lex->line,
                                                lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '<':
                        if (LPEEK(1) == '<') {
                                if (LPEEK(2) == '<') {
                                        if (LPEEK(3) == '=') {
                                                push_tokl(tokens, new_token(
                                                                USHLEQ,
                                                                "<<<=",
                                                                lex->line,
                                                                lex->col));
                                                LINC_POS(4);
                                        } else {
                                                push_tokl(tokens, new_token(
                                                                USHL,
                                                                "<<<",
                                                                lex->line,
                                                                lex->col));
                                                LINC_POS(3);
                                        }
                                } else if (LPEEK(2) == '=') {
                                        push_tokl(tokens, new_token(SHLEQ,
                                                        "<<=", lex->line,
                                                        lex->col));
                                        LINC_POS(3);
                                } else {
                                        push_tokl(tokens, new_token(SHL,
                                                        "<<", lex->line,
                                                        lex->col));
                                        LINC_POS(2);
                                }
                        } else if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(LTEQ, "<=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(LT, "<", lex->line,
                                                lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '&':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(AMPEQ, "&=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else if (LPEEK(1) == '&') {
                                push_tokl(tokens, new_token(AND, "&&",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(AMP, "&", lex->line,
                                                lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '|':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(PIPEEQ, "|=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else if (LPEEK(1) == '|') {
                                push_tokl(tokens, new_token(OR, "||", lex->line,
                                                lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(PIPE, "|",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '~':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(TILDAEQ, "~=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(TILDA, "~",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '^':
                        if (LPEEK(1) == '=') {
                                push_tokl(tokens, new_token(XOREQ, "^=",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(XOR, "^", lex->line,
                                                lex->col));
                                LINC_POS(1);
                        }
                        break;
                case ',':
                        push_tokl(tokens, new_token(COMMA, ",", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case ':':
                        if (LPEEK(1) == ':') {
                                push_tokl(tokens, new_token(SCOPE, "::",
                                                lex->line, lex->col));
                                LINC_POS(2);
                        } else {
                                push_tokl(tokens, new_token(COLON, ":",
                                                lex->line, lex->col));
                                LINC_POS(1);
                        }
                        break;
                case ';':
                        push_tokl(tokens, new_token(SEMICOLON, ";", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '.':
                        if (LPEEK(1) == '.') {
                                if (LPEEK(2) == '.') {
                                        push_tokl(tokens, new_token(ARGS, "...",
                                                        lex->line, lex->col));
                                        LINC_POS(3);
                                } else {
                                        push_tokl(tokens, new_token(RANGE, "..",
                                                        lex->line, lex->col));
                                        LINC_POS(2);
                                }
                        } else {
                                push_tokl(tokens, new_token(DOT, ".", lex->line,
                                                lex->col));
                                LINC_POS(1);
                        }
                        break;
                case '@':
                        push_tokl(tokens, new_token(AT, "@", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '#':
                        push_tokl(tokens, new_token(HASH, "#", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '?':
                        push_tokl(tokens, new_token(QUESTION, "?", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '(':
                        push_tokl(tokens, new_token(LPAR, "(", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case ')':
                        push_tokl(tokens, new_token(RPAR, ")", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '[':
                        push_tokl(tokens, new_token(LBRACKET, "[", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case ']':
                        push_tokl(tokens, new_token(RBRACKET, "]", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '{':
                        push_tokl(tokens, new_token(LBRACE, "{", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '}':
                        push_tokl(tokens, new_token(RBRACE, "}", lex->line,
                                        lex->col));
                        LINC_POS(1);
                        break;
                case '\'':
                        push_tokl(tokens, tokenize_char(lex));
                        break;
                case '\"':
                        push_tokl(tokens, tokenize_string(lex));
                        break;
                case '0':
                        if (LPEEK(1) == 'x' || LPEEK(1) == 'X') {
                                LINC_POS(2);
                                push_tokl(tokens, tokenize_hex(lex));
                        } else if (LPEEK(1) == 'o' || LPEEK(1) == 'O') {
                                LINC_POS(2);
                                push_tokl(tokens, tokenize_oct(lex));
                        } else if (LPEEK(1) == 'b' || LPEEK(1) == 'B') {
                                LINC_POS(2);
                                push_tokl(tokens, tokenize_bin(lex));
                        } else {
                                push_tokl(tokens, tokenize_num(lex));
                        }
                        break;
                case ' ': LINC_POS(1); break;
                case '\n': LINC_POS(1); break;
                case '\t': LINC_POS(1); break;
                case '\r': LINC_POS(1); break;
                default:
                        if (isdigit((unsigned char)LCURR)) {
                                push_tokl(tokens, tokenize_num(lex));
                        } else if (isalpha((unsigned char)LCURR)
                                        || LCURR == '_') {
                                push_tokl(tokens, tokenize_identifier(lex));
                        } else {
                                fprintf(stderr, "@fuxc/Lexer: Unexpected token \
%c at %li:%li\n", LCURR, lex->line, lex->col);
                                LINC_POS(1);
                        }
                        break;
                }
        }
        return tokens;
}

void free_lexer(struct lexer *lex) {
        if (!lex) return;
        free(lex);
}
