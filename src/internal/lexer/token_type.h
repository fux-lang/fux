#ifndef _INTERNAL_LEXER_TOKEN_TYPE_H
#define _INTERNAL_LEXER_TOKEN_TYPE_H

enum token_type;

const char *token_type_to_string(enum token_type type);

enum token_type {
        IDENTIFIER,

        HEX_LITERAL,
        OCT_LITERAL,
        BIN_LITERAL,
        INT_LITERAL,
        FLOAT_LITERAL,
        CHAR_LITERAL,
        STRING_LITERAL,

        BREAK,
        CONTINUE,
        CONST,
        DEFER,
        ELSE,
        ENUM,
        FN,
        FOR,
        IF,
        IMPORT,
        INTERFACE,
        MATCH,
        PACKAGE,
        RETURN,
        STRUCT,
        TYPE,
        VAR,

        PLUS, // +
        PLUSEQ, // +=
        INC, // ++
        MINUS, // -
        MINUSEQ, // -=
        DEC, // --
        STAR, // *
        STAREQ, // *=
        SLASH, // /
        SLASHEQ, // /=
        MOD, // %
        MODEQ, // %=

        ASSIGN, // =

        EQ, // ==
        NEQ, // !=
        GT, // >
        GTEQ, // >=
        LT, // <
        LTEQ, // <=

        AND, // &&
        OR, // ||
        NOT, // !

        AMP, // &
        AMPEQ, // &=
        PIPE, // |
        PIPEQ, // |=
        TILDA, // ~
        TILDAEQ, // ~=
        XOR, // ^
        XOREQ, // ^=

        COMMA, // ,
        COLON, // :
        SCOPE, // ::
        SEMICOLON, // ;
        DOT, // .
        RANGE, // ..
        ARGS, // ...

        AT, // @
        HASH, // #
        QUESTION, // ?

        ARROW, // ->
        BOLD_ARROW, // =>

        LPAR, // (
        RPAR, // )
        LBRACKET, // [
        RBRACKET, // ]
        LBRACE, // {
        RBRACE, // }

        EOF
};

#endif // _INTERNAL_LEXER_TOKEN_TYPE_H
