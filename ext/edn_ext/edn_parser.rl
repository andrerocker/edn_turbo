#include <iostream>
#include <string>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//

%%{
        machine EDN_common;

        cr             = '\n';
        counter        = ( cr @{ line_number++; } );
        cr_neg         = [^\n];
        ws             = [\t\v\f\r ] | ',' | counter;
        comment        = ';' cr_neg* counter;
        ignore         = ws | comment;
        k_nil          = 'nil';
        k_true         = 'true';
        k_false        = 'false';
        begin_keyword  = ':';
        begin_value    = digit | [:nft\"\-\{\[\\#];
        begin_dispatch = '#';
        begin_vector   = '[';
        end_vector     = ']';
        begin_map      = '{';
        end_map        = '}';
        begin_set      = '(';
        end_set        = ')';
        string_delim   = '"';
        begin_number   = digit | '-';

}%%

// ============================================================
// machine for parsing various types
//

%%{
    machine EDN_value;
    include EDN_common;

    write data;

    action parse_nil {
        o = Qnil;
    }
    action parse_false {
        o = Qfalse;
    }
    action parse_true {
        o = Qtrue;
    }

    action parse_keyword {
        const char *np = EDN_parse_keyword(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_string {
        const char *np = EDN_parse_string(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_number {
        // try to parse a decimal first
        const char *np = EDN_parse_decimal(fpc, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = EDN_parse_integer(fpc, pe, o);
        }

        if (np) {
            fexec np;
            fhold;
            fbreak;
        }
        else {
            error(*p);
            fexec pe;
        }
    }

    action parse_vector {
        const char *np = EDN_parse_vector(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_map {
        const char *np = EDN_parse_map(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action exit { fhold; fbreak; }

    main := (
             k_nil @parse_nil |
             k_false @parse_false |
             k_true @parse_true |
             string_delim >parse_string |
             begin_keyword >parse_keyword |
             begin_number >parse_number |
             begin_vector >parse_vector |
             begin_map >parse_map
             ) %*exit;
}%%


const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_value_first_final) {
        return p;
    }
    else if (cs == EDN_value_error) {
        error(*p);
        return pe;
    }
    return NULL;
}


// ============================================================
// keyword parsing
//
%%{
    machine EDN_keyword;
    include EDN_common;

    write data;

    action exit { fhold; fbreak; }

    main := begin_keyword
        ([a-zA-Z_][a-zA-Z_0-9\-]* ('/' [a-zA-Z_][a-zA-Z_0-9\-]*)?)
        (^[a-zA-Z_0-9\-'/']? @exit);
}%%


const char* edn::Parser::EDN_parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_keyword_first_final) {
        uint32_t len = p - p_save - 1; // don't include leading ':' because Rice::Symbol will handle it
        std::string buf;
        buf.append(p_save + 1, len);
        o = Rice::Symbol(buf);
        return p;
    }
    else if (cs == EDN_keyword_error) {
        error(*p);
        return pe;
    }
    return NULL;
}



// ============================================================
// string parsing
//
%%{
    machine EDN_string;
    include EDN_common;

    write data;

    action parse_string {
        if (!EDN_parse_byte_stream(p_save + 1, p, s)) {
            fhold;
            fbreak;
        } else {
            fexec p + 1;
        }
    }

    action exit { fhold; fbreak; }

    main := string_delim ((^([\"\\] | 0..0x1f) | '\\'[\"\\/bfnrt] | '\\u'[0-9a-fA-F]{4} | '\\'^([\"\\/bfnrtu]|0..0x1f))* %parse_string) string_delim @exit;
}%%


//
// copies the string data, unescaping any present values that need to be replaced
//
bool edn::Parser::EDN_parse_byte_stream(const char *p, const char *pe, Rice::String& s)
{
    if (pe > p) {
        std::string buf;
        std::size_t len = pe - p;

        // pre-allocate storage needed
        buf.reserve(len);

        const char* cp = p;
        std::size_t pos = 0;
        char replacement = '?';

        while (cp < pe)
        {
            // append any other character that is not the escaping slash
            if (*cp != '\\') {
                buf.replace(pos++, 1, 1, *cp++);
                continue;
            }

            // looking at a '\' - check what it escapes if there's a
            // following character
            if (++cp == pe)
                break;

            switch (*cp++)
            {
              case 't':
                  replacement = '\t';
                  break;
              case 'n':
                  replacement = '\n';
                  break;
              case 'r':
                  replacement = '\r';
                  break;
              case '\"':
                  replacement = '\"';
                  break;
              case '\\':
                  replacement = '\\';
                  break;
              case '0':
                  replacement = '\0';
                  break;
              default:
                  std::cerr << "value must be unescaped but case is unhandled: '" << *cp << "'" << std::endl;
                  break;
            }
            // substitute the escaped walue
            buf.replace(pos++, 1, 1, replacement);
        }

        // utf-8 encode
        VALUE vs = Rice::protect( rb_str_new2, buf.c_str() );
        VALUE s_utf8 = Rice::protect( rb_enc_associate, vs, rb_utf8_encoding() );
        s = Rice::String(s_utf8);
        return true;
    }

    return false;
}


const char* edn::Parser::EDN_parse_string(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    Rice::String s;
    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_string_first_final) {
        o = s;
        return p + 1;
    }
    else if (cs == EDN_string_error) {
        error(*p);
        return pe;
    }
    return NULL;
}

// ============================================================
// decimal parsing grammar
//
%%{
    machine EDN_decimal;
    include EDN_common;

    write data noerror;

    action exit { fhold; fbreak; }

    main := '-'? (
                  (('0' |
                    [1-9][0-9]*) '.' [0-9]+ ((([Ee] [+\-]?[0-9]+)?) | ([M]?))
                   ) |
                  (('0' | [1-9][0-9]*) ([Ee] [+\-]?[0-9]+))
                  )
        (^[0-9Ee.\-M]? @exit );
}%%


const char* edn::Parser::EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_decimal_first_final) {
        double value;
        o = Parser::buftotype<double>(p_save, p - p_save, value);
        return p + 1;
    }

    return NULL;
}


// ============================================================
// integer parsing grammar
//
%%{
    machine EDN_integer;

    write data noerror;

    action exit { fhold; fbreak; }

    main := '-'? ('0' | [1-9][0-9]* [M]?) (^[0-9M]? @exit);
}%%

const char* edn::Parser::EDN_parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_integer_first_final) {
        int value;
        o = Parser::buftotype<int>(p_save, p - p_save, value);
        return p + 1;
    }
    return NULL;
}


// ============================================================
// vector parsing machine
//
%%{
    machine EDN_vector;
    include EDN_common;

    write data;

    action parse_value {
        Rice::Object v;
        const char *np = EDN_parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            arr.push(v);
            fexec np;
        }
    }

    # action to report missing closing bracket
    action end_vec_err {
        error("closing ']' not found");
        fexec pe;
    }

    action exit { fhold; fbreak; }

    next_element  = ignore* begin_value >parse_value;

    main := begin_vector ignore*
             ((begin_value >parse_value ignore*)
              (ignore* next_element ignore*)*)?
            end_vector @err(end_vec_err)
            @exit;
}%%

//
// vector parsing
//
const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    int cs;
    Rice::Array arr;
    const char *eof = pe;

    %% write init;
    %% write exec;

    if (cs >= EDN_vector_first_final) {
        o = arr;
        return p + 1;
    }
    else if (cs == EDN_vector_error) {
        error(*p);
        return pe;
    }

    return NULL;
}




// ============================================================
// hash parsing
//
%%{
    machine EDN_map;
    include EDN_common;

    write data;

    action parse_key {
        const char *np = EDN_parse_value(fpc, pe, k);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            fexec np;
        }
    }

    action parse_value {
        const char *np = EDN_parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            map[k] = v;
            fexec np;
        }
    }

    # action to report missing value in k/v pair
    action pair_err {
        error("map pair not found");
        fexec pe;
    }

    # action to report missing closed bracket
    action end_map_err {
        error("closing '}' not found");
        fexec pe;
    }

    action exit { fhold; fbreak; }

    pair        = ignore* begin_value >parse_key ignore* begin_value >parse_value @err(pair_err);
    next_pair   = ignore* pair;

    main := (
             begin_map
             (pair (next_pair)*)? ignore*
             end_map @err(end_map_err)
             ) @exit;
}%%


const char* edn::Parser::EDN_parse_map(const char *p, const char *pe, Rice::Object& o)
{
    int cs;
    const char *eof = pe;
    Rice::Hash map;
    Rice::Object k, v;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_map_first_final) {
        o = map;
        return p + 1;
    }
    else if (cs == EDN_map_error) {
        return pe;
    }
    return NULL;
}



// ============================================================
// main parsing machine
//
%%{
    machine EDN;
    include EDN_common;

    write data nofinal;

    action parse_vector {
        const char* np = EDN_parse_vector(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_map {
        const char *np = EDN_parse_map(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    main := ignore* (
                 begin_vector >parse_vector |
                 begin_map >parse_map
                 ) ignore*;
}%%

//
//
//
Rice::Object edn::Parser::process(const char* buf, long len)
{
    int cs;
    const char *p;
    const char *pe;
    const char *eof;
    Rice::Object result;

    %% write init;
    p = &buf[0];
    pe = buf + len;
    eof = pe;
    %% write exec;

    if (cs == EDN_error) {
        error(*p);
        return Qnil;
    }

    return result;
}

/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
