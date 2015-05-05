
#line 1 "edn_parser.rl"
#include <iostream>
#include <string>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//


#line 44 "edn_parser.rl"


// ============================================================
// machine for parsing various types
//


#line 26 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 11;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 123 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 43 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 131 "edn_parser.rl"

#line 50 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 34: goto tr0;
		case 40: goto tr2;
		case 45: goto tr3;
		case 58: goto tr4;
		case 91: goto tr5;
		case 102: goto st2;
		case 110: goto st6;
		case 116: goto st8;
		case 123: goto tr9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 71 "edn_parser.rl"
	{
        const char *np = parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr2:
#line 100 "edn_parser.rl"
	{
        const char *np = parse_list(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr3:
#line 76 "edn_parser.rl"
	{
        // try to parse a decimal first
        const char *np = parse_decimal(p, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p, pe, o);
        }

        if (np) {
            {p = (( np))-1;}
            p--;
            {p++; cs = 11; goto _out;}
        }
        else {
            error(*p);
            {p = (( pe))-1;}
        }
    }
	goto st11;
tr4:
#line 66 "edn_parser.rl"
	{
        const char *np = parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr5:
#line 95 "edn_parser.rl"
	{
        const char *np = parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr9:
#line 105 "edn_parser.rl"
	{
        const char *np = parse_map(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr13:
#line 59 "edn_parser.rl"
	{
        o = Qfalse;
    }
	goto st11;
tr15:
#line 56 "edn_parser.rl"
	{
        o = Qnil;
    }
	goto st11;
tr18:
#line 62 "edn_parser.rl"
	{
        o = Qtrue;
    }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 110 "edn_parser.rl"
	{ p--; {p++; cs = 11; goto _out;} }
#line 154 "edn_parser.cc"
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 108 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 115 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 101 )
		goto tr13;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 105 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 108 )
		goto tr15;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 114 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 117 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 101 )
		goto tr18;
	goto st0;
	}
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 132 "edn_parser.rl"

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

#line 254 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 4;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 160 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 271 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 168 "edn_parser.rl"
    p_save = p;

#line 279 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 58 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 95 )
		goto st4;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st4;
	} else if ( (*p) >= 65 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 39: goto st0;
		case 45: goto st4;
		case 47: goto st3;
		case 95: goto st4;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st4;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st4;
	} else
		goto st4;
	goto tr4;
tr4:
#line 155 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 331 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 95 )
		goto st6;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st6;
	} else if ( (*p) >= 65 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 39: goto st0;
		case 45: goto st6;
		case 47: goto st0;
		case 95: goto st6;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st6;
	} else
		goto st6;
	goto tr4;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 170 "edn_parser.rl"

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

#line 398 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 215 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;

#line 418 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 226 "edn_parser.rl"
    p_save = p;

#line 426 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
tr2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 445 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto st3;
	}
	if ( 0 <= (*p) && (*p) <= 31 )
		goto tr2;
	goto st2;
tr3:
#line 197 "edn_parser.rl"
	{
        if (!parse_byte_stream(p_save + 1, p, s)) {
            p--;
            {p++; cs = 8; goto _out;}
        } else {
            {p = (( p + 1))-1;}
        }
    }
#line 206 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 477 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 117 )
		goto st4;
	if ( 0 <= (*p) && (*p) <= 31 )
		goto st0;
	goto st2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st5;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st5;
	} else
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st7;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st7;
	} else
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st2;
	} else
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	break;
#line 562 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 228 "edn_parser.rl"

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

#line 586 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 8;

static const int EDN_decimal_en_main = 1;


#line 258 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 602 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 266 "edn_parser.rl"
    p_save = p;

#line 610 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 45: goto st2;
		case 48: goto st3;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 48 )
		goto st3;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 46: goto st4;
		case 69: goto st5;
		case 101: goto st5;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 69: goto st5;
		case 77: goto st11;
		case 101: goto st5;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st8;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
tr9:
#line 249 "edn_parser.rl"
	{ p--; {p++; cs = 9; goto _out;} }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 676 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 43: goto st6;
		case 45: goto st6;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st10;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st0;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 46: goto st4;
		case 69: goto st5;
		case 101: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 268 "edn_parser.rl"

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

#line 770 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 290 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 785 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 297 "edn_parser.rl"
    p_save = p;

#line 793 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 45: goto st2;
		case 48: goto st3;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st5;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 48 )
		goto st3;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st5;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 77 )
		goto st0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st0;
	goto tr4;
tr4:
#line 287 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 836 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 77 )
		goto st3;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st5;
	goto tr4;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 299 "edn_parser.rl"

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

#line 351 "edn_parser.rl"


//
// vector-specific machine

#line 880 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 365 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;


#line 903 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 379 "edn_parser.rl"

#line 910 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 91 )
		goto st2;
	goto st0;
tr2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 929 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 318 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	goto st2;
tr5:
#line 329 "edn_parser.rl"
	{
        bool discard = false;
        Rice::Object v;
        const char *np = parse_tagged(p, pe, v, discard);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            if (!discard) {
                arr.push(v);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 970 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 34: goto tr4;
		case 35: goto tr5;
		case 40: goto tr4;
		case 44: goto st2;
		case 45: goto tr4;
		case 59: goto st3;
		case 93: goto tr7;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st2;
	} else if ( (*p) > 58 ) {
		if ( 91 <= (*p) && (*p) <= 92 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
tr7:
#line 343 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1010 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1030 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 380 "edn_parser.rl"

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

#line 1057 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 6;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 409 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;


#line 1079 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 422 "edn_parser.rl"

#line 1086 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 40 )
		goto st2;
	goto st0;
tr2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1105 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1117 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 34: goto tr4;
		case 40: goto tr4;
		case 41: goto tr5;
		case 44: goto st2;
		case 45: goto tr4;
		case 59: goto st5;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st2;
	} else if ( (*p) > 58 ) {
		if ( 91 <= (*p) && (*p) <= 92 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr8:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st3;
tr4:
#line 318 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 3; goto _out;}
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	goto st3;
tr9:
#line 329 "edn_parser.rl"
	{
        bool discard = false;
        Rice::Object v;
        const char *np = parse_tagged(p, pe, v, discard);
        if (np == NULL) {
            p--; {p++; cs = 3; goto _out;}
        } else {
            if (!discard) {
                arr.push(v);
            }
            {p = (( np))-1;}
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1178 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto st3;
		case 34: goto tr4;
		case 35: goto tr9;
		case 40: goto tr4;
		case 41: goto tr5;
		case 44: goto st3;
		case 45: goto tr4;
		case 59: goto st4;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st3;
	} else if ( (*p) > 58 ) {
		if ( 91 <= (*p) && (*p) <= 92 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr5:
#line 343 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1211 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr8;
	goto st4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto tr3;
	goto st5;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1248 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 423 "edn_parser.rl"

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

#line 1275 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 6;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 482 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "map";

    int cs;
    Rice::Hash map;
    Rice::Object k, v;


#line 1296 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 494 "edn_parser.rl"
    p_save = p;

#line 1304 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto st2;
	goto st0;
tr2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	goto st0;
tr7:
#line 467 "edn_parser.rl"
	{
        error("map pair not found");
        {p = (( pe))-1;}
    }
	goto st0;
#line 1330 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr10:
#line 456 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            map[k] = v;
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1354 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 34: goto tr4;
		case 40: goto tr4;
		case 44: goto st2;
		case 45: goto tr4;
		case 59: goto st5;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st2;
	} else if ( (*p) > 58 ) {
		if ( 91 <= (*p) && (*p) <= 92 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr9:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st3;
tr4:
#line 447 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, k);
        if (np == NULL) {
            p--; {p++; cs = 3; goto _out;}
        } else {
            {p = (( np))-1;}
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1397 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto st3;
		case 34: goto tr10;
		case 40: goto tr10;
		case 44: goto st3;
		case 45: goto tr10;
		case 59: goto st4;
		case 102: goto tr10;
		case 110: goto tr10;
		case 116: goto tr10;
		case 123: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st3;
	} else if ( (*p) > 58 ) {
		if ( 91 <= (*p) && (*p) <= 92 )
			goto tr10;
	} else
		goto tr10;
	goto tr7;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr9;
	goto st4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto tr3;
	goto st5;
tr6:
#line 472 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1442 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 38 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(s.str());
            {p = (( pe))-1;}
        }
	break;
	case 3:
#line 467 "edn_parser.rl"
	{
        error("map pair not found");
        {p = (( pe))-1;}
    }
	break;
#line 1471 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 496 "edn_parser.rl"

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

#line 1500 "edn_parser.cc"
static const int EDN_dispatch_start = 1;
static const int EDN_dispatch_first_final = 15;
static const int EDN_dispatch_error = 0;

static const int EDN_dispatch_en_main = 1;


#line 534 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o, bool& discard)
{
    int cs;
    Rice::String str;


#line 1518 "edn_parser.cc"
	{
	cs = EDN_dispatch_start;
	}

#line 543 "edn_parser.rl"
    p_save = p;

#line 1526 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 35 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 95: goto st15;
		case 105: goto st3;
		case 117: goto st9;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 32: goto st17;
		case 43: goto st0;
		case 58: goto st0;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st17;
		} else if ( (*p) >= 9 )
			goto tr17;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st17;
		} else if ( (*p) >= 65 )
			goto st17;
	} else
		goto st17;
	goto tr16;
tr16:
#line 526 "edn_parser.rl"
	{ p--; {p++; cs = 16; goto _out;} }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1581 "edn_parser.cc"
	switch( (*p) ) {
		case 32: goto st0;
		case 43: goto st0;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st0;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr16;
tr17:
#line 526 "edn_parser.rl"
	{ p--; {p++; cs = 17; goto _out;} }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1606 "edn_parser.cc"
	switch( (*p) ) {
		case 32: goto st0;
		case 43: goto st0;
		case 58: goto st0;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st17;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st17;
		} else if ( (*p) >= 65 )
			goto st17;
	} else
		goto st17;
	goto tr16;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 110 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 115 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 116 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 32 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 34 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 34: goto st16;
		case 43: goto st8;
		case 84: goto st8;
		case 90: goto st8;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 58 )
			goto st8;
	} else if ( (*p) >= 45 )
		goto st8;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 117 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 105 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 100 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 32 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 34 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 34: goto st16;
		case 45: goto st14;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st14;
	} else if ( (*p) >= 48 )
		goto st14;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof15: cs = 15; goto _test_eof;
	_test_eof16: cs = 16; goto _test_eof;
	_test_eof17: cs = 17; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof12: cs = 12; goto _test_eof;
	_test_eof13: cs = 13; goto _test_eof;
	_test_eof14: cs = 14; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 545 "edn_parser.rl"

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

#line 1792 "edn_parser.cc"
static const int EDN_start = 1;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 615 "edn_parser.rl"


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


#line 1815 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 630 "edn_parser.rl"
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class

#line 1825 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 1839 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto st1;
		case 40: goto tr3;
		case 44: goto st1;
		case 59: goto st3;
		case 91: goto tr5;
		case 123: goto tr6;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st1;
	goto st0;
st0:
cs = 0;
	goto _out;
tr8:
#line 18 "edn_parser.rl"
	{ line_number++; }
	goto st4;
tr3:
#line 605 "edn_parser.rl"
	{
        const char *np = parse_list(p, pe, result);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr5:
#line 595 "edn_parser.rl"
	{
        const char* np = parse_vector(p, pe, result);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr6:
#line 600 "edn_parser.rl"
	{
        const char *np = parse_map(p, pe, result);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1884 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto st4;
		case 44: goto st4;
		case 59: goto st2;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st4;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr8;
	goto st2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr2;
	goto st3;
	}
	_test_eof1: cs = 1; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 634 "edn_parser.rl"

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
