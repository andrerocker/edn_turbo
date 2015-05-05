#include <iostream>
#include <string>

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
        begin_value    = digit | [:nft\"\-\{\[\(\\];
        begin_dispatch = '#';
        begin_vector   = '[';
        end_vector     = ']';
        begin_map      = '{';
        end_map        = '}';
        begin_list     = '(';
        end_list       = ')';
        string_delim   = '"';
        begin_number   = digit | '-';

        action close_err {
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            fexec pe;
        }
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
        const char *np = parse_keyword(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_string {
        const char *np = parse_string(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_number {
        // try to parse a decimal first
        const char *np = parse_decimal(fpc, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(fpc, pe, o);
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
        const char *np = parse_vector(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_list {
        const char *np = parse_list(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_map {
        const char *np = parse_map(fpc, pe, o);
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
             begin_list >parse_list |
             begin_map >parse_map
             ) %*exit;
}%%


const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
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
    else if (cs == EDN_value_en_main) {} // silence ragel warning
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


const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
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
    else if (cs == EDN_keyword_en_main) {} // silence ragel warning
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
        if (!parse_byte_stream(p_save + 1, p, s)) {
            fhold;
            fbreak;
        } else {
            fexec p + 1;
        }
    }

    action exit { fhold; fbreak; }

    main := string_delim (
                          (^([\"\\] | 0..0x1f) |
                           '\\'[\"\\/bfnrt] |
                           '\\u'[0-9a-fA-F]{4} |
                           '\\'^([\"\\/bfnrtu]|0..0x1f))* %parse_string)
        string_delim @err(close_err)
        @exit;
}%%


const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;
    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_string_first_final) {
        o = s;
        return p + 1;
    }
    else if (cs == EDN_string_error) {
        return pe;
    }
    else if (cs == EDN_string_en_main) {} // silence ragel warning
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


const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
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
    else if (cs == EDN_decimal_en_main) {} // silence ragel warning
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

const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
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
    else if (cs == EDN_integer_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// vector parsing machine. EDN_vector_common is used to parse EDN
// vectors and lists since they're both represented as vectors in ruby
//
%%{
    machine EDN_vector_common;
    include EDN_common;

    action parse_value {
        Rice::Object v;
        const char *np = parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            arr.push(v);
            fexec np;
        }
    }

    action parse_dispatch {
        bool discard = false;
        Rice::Object v;
        const char *np = parse_tagged(fpc, pe, v, discard);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            if (!discard) {
                arr.push(v);
            }
            fexec np;
        }
    }

    action exit { fhold; fbreak; }

    element        = (
                      begin_value >parse_value |
                      begin_dispatch >parse_dispatch
                      );

    next_element  = ignore* element;
}%%

//
// vector-specific machine
%%{
    machine EDN_vector;
    include EDN_vector_common;

    write data;

    main := begin_vector ignore* ((element ignore*)
                                  (ignore* next_element ignore*)*)?
            end_vector @err(close_err)
            @exit;
}%%


//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;

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
    else if (cs == EDN_vector_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// list parsing machine
//
%%{
    machine EDN_list;
    include EDN_vector_common;

    write data;

    main := begin_list ignore*
             ((begin_value >parse_value ignore*)
              (ignore* next_element ignore*)*)?
            end_list @err(close_err)
            @exit;
}%%

//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;

    %% write init;
    %% write exec;

    if (cs >= EDN_list_first_final) {
        o = arr;
        return p + 1;
    }
    else if (cs == EDN_list_error) {
        error(*p);
        return pe;
    }
    else if (cs == EDN_list_en_main) {} // silence ragel warning
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
        const char *np = parse_value(fpc, pe, k);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            fexec np;
        }
    }

    action parse_value {
        const char *np = parse_value(fpc, pe, v);
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

    action exit { fhold; fbreak; }

    pair        = ignore* begin_value >parse_key ignore* begin_value >parse_value @err(pair_err);
    next_pair   = ignore* pair;

    main := (
             begin_map
             (pair (next_pair)*)? ignore*
             end_map @err(close_err)
             ) @exit;
}%%


const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "map";

    int cs;
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
    else if (cs == EDN_map_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// tagged element parsing - any of #uuid, #inst, #{, #(some symbol)
// discard (#_ <ident>) is handled by the top-level machine
//
// NOTE: this is not fully implemented yet
//
%%{
    machine EDN_dispatch;
    include EDN_common;

    begin_discard  = '_';
    begin_set      = '{';
    end_set        = '}';

    write data;

    action exit { fhold; fbreak; }

    main := begin_dispatch (
                            (begin_discard (space)? ([a-zA-Z0-9\-\.]*)) |
                            ('inst ' string_delim ([0-9\-+:\.TZ])* string_delim) |
                            ('uuid ' string_delim ([a-f0-9\-]* string_delim))
                            )
        (^[a-zA-Z0-9:\.\-+ ]* @exit);
}%%


const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o, bool& discard)
{
    int cs;
    Rice::String str;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_dispatch_first_final) {

        //is it a discard? if so, just drop the following token
        if (*(p_save + 1) == '_')
        {
            discard = true;
            return p + 1;
        }

        std::size_t len = p - p_save;
        std::string buf;
        buf.reserve(len);

        if (len > 10)
        {
            // there's enough room to be #inst or #uuid, copy the
            // string portion
            if (std::strncmp(p_save + 1, "inst", 4) == 0) {
                buf.append(p_save + 7, len - 8);
            } else if (std::strncmp(p_save + 1, "uuid", 4) == 0) {
                buf.append(p_save + 7, len - 8);
            }

            o = Rice::String(buf);
            return p;
        }

        // tagged element
        o = Rice::String(buf);
        return p;
    }
    else if (cs == EDN_dispatch_error) {
        error(*p);
        return pe;
    }
    else if (cs == EDN_dispatch_en_main) {} // silence ragel warning
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
        const char* np = parse_vector(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_map {
        const char *np = parse_map(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_list {
        const char *np = parse_list(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    main := ignore* (
                 begin_vector >parse_vector |
                 begin_map >parse_map |
                 begin_list >parse_list
                 ) ignore*;
}%%

//
//
//
Rice::Object edn::Parser::parse(const char* buf, std::size_t len)
{
    int cs;
    const char *p;
    const char *pe;
    Rice::Object result;

    line_number = 1;

    %% write init;
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class
    %% write exec;

    if (cs == EDN_error) {
        error(*p);
        return Qnil;
    }
    else if (cs == EDN_en_main) {} // silence ragel warning
    return result;
}

/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
