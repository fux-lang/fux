#ifndef _INTERNAL_LEXER_TOKEN_TYPE_H
#define _INTERNAL_LEXER_TOKEN_TYPE_H

enum token_type;

const char *token_type_to_string(enum token_type type);

enum token_type {
        ERR,
        
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
        FOR,
        IF,
        IMPORT,
        MATCH,
        PACKAGE,
        RETURN,
        STRUCT,

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

        SHL, // <<
        SHLEQ, // <<=
        USHL, // <<<
        USHLEQ, // <<<=
        SHR, // >>
        SHREQ, // >>=
        USHR, // >>>
        USHREQ, // >>>=

        AND, // &&
        OR, // ||
        NOT, // !

        AMP, // &
        AMPEQ, // &=
        PIPE, // |
        PIPEEQ, // |=
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

        _EOF
};

#endif // _INTERNAL_LEXER_TOKEN_TYPE_H
