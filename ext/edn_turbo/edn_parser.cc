
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
//
// many thanks to Florian Frank for json-ruby which was essential in
// helping me learn about ragel
//


#line 61 "edn_parser.rl"


// ============================================================
// machine for parsing various types
//


#line 30 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 3;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 186 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    //                std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;


#line 48 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 195 "edn_parser.rl"

#line 55 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 34: goto tr0;
		case 35: goto tr2;
		case 40: goto tr3;
		case 45: goto tr4;
		case 58: goto tr5;
		case 60: goto tr6;
		case 62: goto tr6;
		case 91: goto tr7;
		case 92: goto st2;
		case 123: goto tr9;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr6;
	} else
		goto tr6;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 109 "edn_parser.rl"
	{
        const char *np = parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
	goto st3;
tr2:
#line 148 "edn_parser.rl"
	{
        const char *np = NULL; //parse_discard(fpc, pe);
        if (np) {
                    std::cerr << "--- PARSE DISCARD - NP is set : '" << np << "'" << std::endl;
        } else {
            //        if (np == NULL) {
            // try a set then
            np = parse_set(p, pe, o);

            if (np == NULL) {
                // not a discard.. try a tagged
                np = parse_tagged(p, pe, o);
            }
        }

        if (np) {
            //        std::cerr << "--- PARSE DISP NP set : '" << np << "'" << std::endl;
            {p = (( np))-1;}

        } else {
            p--; {p++; cs = 3; goto _out;}
            {p = (( pe))-1;}
        }
    }
	goto st3;
tr3:
#line 138 "edn_parser.rl"
	{
        const char *np = parse_list(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
	goto st3;
tr4:
#line 114 "edn_parser.rl"
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
            {p++; cs = 3; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st3;
tr5:
#line 98 "edn_parser.rl"
	{
        const char *np = parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
	goto st3;
tr6:
#line 84 "edn_parser.rl"
	{
        std::string sym;
        const char *np = parse_symbol(p, pe, sym);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {
            if      (sym == "true")  { o = Qtrue; }
            else if (sym == "false") { o = Qfalse; }
            else if (sym == "nil")   { o = Qnil; }
            else {
                o = Parser::make_edn_symbol(sym);
            }
            {p = (( np))-1;}
        }
    }
	goto st3;
tr7:
#line 133 "edn_parser.rl"
	{
        const char *np = parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
	goto st3;
tr9:
#line 143 "edn_parser.rl"
	{
        const char *np = parse_map(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
	goto st3;
tr10:
#line 103 "edn_parser.rl"
	{
        std::string s;
        s += *p;
        o = Rice::String(s);
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 173 "edn_parser.rl"
	{ p--; {p++; cs = 3; goto _out;} }
#line 199 "edn_parser.cc"
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 33 <= (*p) && (*p) <= 126 )
		goto tr10;
	goto st0;
	}
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 196 "edn_parser.rl"

    if (cs >= EDN_value_first_final) {
        return p;
    }
    else if (cs == EDN_value_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_value_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// symbol parsing
//

#line 235 "edn_parser.cc"
static const int EDN_symbol_start = 1;
static const int EDN_symbol_first_final = 3;
static const int EDN_symbol_error = 0;

static const int EDN_symbol_en_main = 1;


#line 224 "edn_parser.rl"



const char* edn::Parser::parse_symbol(const char *p, const char *pe, std::string& s)
{
    //std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;


#line 253 "edn_parser.cc"
	{
	cs = EDN_symbol_start;
	}

#line 233 "edn_parser.rl"
    p_save = p;

#line 261 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st3;
		case 58: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto st0;
st0:
cs = 0;
	goto _out;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr5;
		case 33: goto st3;
		case 44: goto tr5;
		case 59: goto tr7;
		case 95: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto st3;
		} else if ( (*p) >= 9 )
			goto tr5;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else if ( (*p) >= 65 )
			goto st3;
	} else
		goto st3;
	goto tr4;
tr4:
#line 219 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 329 "edn_parser.cc"
	goto st0;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st5;
tr5:
#line 219 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr6:
#line 22 "edn_parser.rl"
	{ line_number++; }
#line 219 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 349 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr5;
		case 33: goto st0;
		case 44: goto tr5;
		case 59: goto tr7;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr5;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr4;
tr7:
#line 219 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 381 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr3;
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr3;
	goto st2;
	}
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 235 "edn_parser.rl"

    if (cs >= EDN_symbol_first_final) {
        uint32_t len = p - p_save;
        std::string buf;
        buf.append(p_save, len);

        s = buf;
        return p;
    }
    else if (cs == EDN_symbol_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_symbol_en_main) {} // silence ragel warning
    return NULL;
}




// ============================================================
// keyword parsing
//

#line 428 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 267 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 445 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 275 "edn_parser.rl"
    p_save = p;

#line 453 "edn_parser.cc"
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
	switch( (*p) ) {
		case 33: goto st3;
		case 58: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 33: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto tr3;
tr3:
#line 264 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 527 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 277 "edn_parser.rl"

    if (cs >= EDN_keyword_first_final) {
        uint32_t len = p - p_save - 1; // don't include leading ':' because Rice::Symbol will handle it
        std::string buf;
        buf.append(p_save + 1, len);
        o = Rice::Symbol(buf);
        return p;
    }
    else if (cs == EDN_keyword_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_keyword_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// string parsing
//

#line 561 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 322 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;

#line 582 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 334 "edn_parser.rl"
    p_save = p;

#line 590 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 609 "edn_parser.cc"
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
#line 304 "edn_parser.rl"
	{
        if (!parse_byte_stream(p_save + 1, p, s)) {
            p--;
            {p++; cs = 8; goto _out;}
        } else {
            {p = (( p + 1))-1;}
        }
    }
#line 313 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 641 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 726 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 336 "edn_parser.rl"

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

#line 750 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 9;

static const int EDN_decimal_en_main = 1;


#line 364 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 766 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 372 "edn_parser.rl"
    p_save = p;

#line 774 "edn_parser.cc"
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
		goto st8;
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
		goto st8;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 46: goto st9;
		case 69: goto st6;
		case 101: goto st6;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st4;
		case 77: goto st12;
		case 101: goto st4;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st9;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
tr10:
#line 357 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 834 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 43: goto st5;
		case 45: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st12;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st11;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st0;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 43: goto st7;
		case 45: goto st7;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st13;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 46: goto st9;
		case 69: goto st6;
		case 101: goto st6;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof12: cs = 12; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof13: cs = 13; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 374 "edn_parser.rl"

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

#line 966 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 397 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 981 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 404 "edn_parser.rl"
    p_save = p;

#line 989 "edn_parser.cc"
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
		goto st6;
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
		goto st6;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 124 )
		goto tr6;
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st5;
	} else if ( (*p) >= 48 )
		goto st0;
	goto tr4;
tr4:
#line 394 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1035 "edn_parser.cc"
	goto st0;
tr6:
#line 394 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1045 "edn_parser.cc"
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st0;
	} else if ( (*p) >= 48 )
		goto st0;
	goto tr4;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 124 )
		goto tr6;
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st5;
	} else if ( (*p) >= 48 )
		goto st6;
	goto tr4;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 406 "edn_parser.rl"

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

#line 441 "edn_parser.rl"


//
// vector-specific machine

#line 1098 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 457 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "  -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;


#line 1122 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 472 "edn_parser.rl"

#line 1129 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1148 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 425 "edn_parser.rl"
	{
        //std::cerr << "--- VECTOR PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
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
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1174 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 62: goto tr4;
		case 93: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
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
tr6:
#line 437 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1217 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1237 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 473 "edn_parser.rl"

    if (cs >= EDN_vector_first_final) {


        o = arr;
        return p + 1;
    }
    else if (cs == EDN_vector_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_vector_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// list parsing machine
//

#line 1266 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 505 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;


#line 1288 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 518 "edn_parser.rl"

#line 1295 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1314 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 425 "edn_parser.rl"
	{
        //std::cerr << "--- VECTOR PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
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
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1340 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 41: goto tr5;
		case 44: goto st2;
		case 59: goto st3;
		case 62: goto tr4;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr5:
#line 437 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1376 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1403 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 519 "edn_parser.rl"

    if (cs >= EDN_list_first_final) {
        o = arr;
        return p + 1;
    }
    else if (cs == EDN_list_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_list_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// set parsing machine
//

#line 1430 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 5;

static const int EDN_set_en_main = 1;


#line 573 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "     -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array set; // store as a vector; then convert to a set once done


#line 1452 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 587 "edn_parser.rl"

#line 1459 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 35 )
		goto st2;
	goto st0;
tr3:
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1478 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 123 )
		goto st3;
	goto st0;
tr4:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st3;
tr5:
#line 546 "edn_parser.rl"
	{
        //std::cerr << "--- SET PARSE VALUE: fpc is '" << fpc << "'" << std::endl;

        {
            Rice::Object set_v;
            const char *np = parse_value(p, pe, set_v);
            if (np == NULL) {
                p--; {p++; cs = 3; goto _out;}
            } else {
                set.push(set_v);
                {p = (( np))-1;}
            }
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1514 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st3;
		case 40: goto tr5;
		case 44: goto st3;
		case 59: goto st4;
		case 62: goto tr5;
		case 125: goto tr7;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr5;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else
			goto tr5;
	} else
		goto tr5;
	goto tr3;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr4;
	goto st4;
tr7:
#line 561 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1557 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3:
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1578 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 588 "edn_parser.rl"

    if (cs >= EDN_set_first_final) {
        o = make_ruby_set(set);
        return p + 1;
    }
    else if (cs == EDN_set_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// hash parsing
//

#line 1601 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 6;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 648 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "map";

    int cs;
    Rice::Hash map;
    Rice::Object k, v;


#line 1623 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 661 "edn_parser.rl"

#line 1630 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
tr7:
#line 633 "edn_parser.rl"
	{
        error(__FUNCTION__, "map pair not found");
        {p = (( pe))-1;}
    }
	goto st0;
#line 1656 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr10:
#line 621 "edn_parser.rl"
	{
        //                std::cerr << "--- MAP PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
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
#line 1681 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st5;
		case 62: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr9:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st3;
tr4:
#line 610 "edn_parser.rl"
	{
        //        std::cerr << "--- MAP PARSE KEY: fpc is '" << fpc << "'" << std::endl;

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
#line 1730 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto st3;
		case 40: goto tr10;
		case 44: goto st3;
		case 59: goto st4;
		case 62: goto tr10;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr10;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr10;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr10;
		} else
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
#line 638 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1779 "edn_parser.cc"
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
#line 55 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
	case 3:
#line 633 "edn_parser.rl"
	{
        error(__FUNCTION__, "map pair not found");
        {p = (( pe))-1;}
    }
	break;
#line 1808 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 662 "edn_parser.rl"

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
// tagged element parsing - #uuid, #inst, #{, #_
//

#line 1833 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 6;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 704 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    std::string sym_name;
    Rice::Object object;

    int cs;


#line 1853 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 715 "edn_parser.rl"

#line 1860 "edn_parser.cc"
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
		case 33: goto tr2;
		case 58: goto tr2;
		case 95: goto tr2;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr2;
		} else if ( (*p) >= 35 )
			goto tr2;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr2;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr2;
		} else
			goto tr2;
	} else
		goto tr2;
	goto st0;
tr2:
#line 692 "edn_parser.rl"
	{
        const char *np = parse_symbol(p, pe, sym_name);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else { {p = (( np))-1;} }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1911 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st4;
		case 34: goto tr6;
		case 35: goto tr7;
		case 40: goto tr6;
		case 42: goto st3;
		case 44: goto st4;
		case 47: goto st3;
		case 59: goto st5;
		case 61: goto st3;
		case 63: goto st3;
		case 95: goto st3;
		case 123: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto st3;
		} else if ( (*p) >= 9 )
			goto st4;
	} else if ( (*p) > 62 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr7;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr7;
		} else
			goto tr6;
	} else
		goto tr7;
	goto st0;
tr4:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1953 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st4;
		case 40: goto tr6;
		case 44: goto st4;
		case 59: goto st5;
		case 62: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr6;
		} else if ( (*p) >= 9 )
			goto st4;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr6;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr6;
		} else
			goto tr6;
	} else
		goto tr6;
	goto st0;
tr6:
#line 696 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, object);
        if (np == NULL) { p--; {p++; cs = 6; goto _out;} } else { {p = (( np))-1;} }
    }
#line 701 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1993 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto tr4;
	goto st5;
tr7:
#line 696 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, object);
        if (np == NULL) { p--; {p++; cs = 7; goto _out;} } else { {p = (( np))-1;} }
    }
#line 701 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2015 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st4;
		case 34: goto tr6;
		case 35: goto tr7;
		case 40: goto tr6;
		case 42: goto st3;
		case 44: goto st4;
		case 47: goto st3;
		case 59: goto st5;
		case 61: goto st3;
		case 63: goto st3;
		case 95: goto st3;
		case 123: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto st3;
		} else if ( (*p) >= 9 )
			goto st4;
	} else if ( (*p) > 62 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr7;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr7;
		} else
			goto tr6;
	} else
		goto tr7;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 716 "edn_parser.rl"

    if (cs >= EDN_tagged_first_final) {
        //        std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << object << std::endl;
        o = Parser::tagged_element(sym_name, object);
        return p + 1;
    }
    else if (cs == EDN_tagged_error) {
        return pe;
    }
    else if (cs == EDN_tagged_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// discard
//

#line 2080 "edn_parser.cc"
static const int EDN_discard_start = 1;
static const int EDN_discard_first_final = 6;

static const int EDN_discard_en_main = 1;


#line 758 "edn_parser.rl"



const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
        std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;


#line 2097 "edn_parser.cc"
	{
	cs = EDN_discard_start;
	}

#line 767 "edn_parser.rl"

#line 2104 "edn_parser.cc"
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
	if ( (*p) == 95 )
		goto st3;
	goto st0;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2132 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 40: goto tr4;
		case 44: goto st3;
		case 59: goto st5;
		case 62: goto tr4;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr4:
#line 741 "edn_parser.rl"
	{
                                        std::cerr << "--- DISCARD PARSE VALUE: fpc is '" << p << "'" << std::endl;
        Rice::Object dummy;
        const char* np = parse_value(p, pe, dummy);
        if (np == NULL) { p--; {p++; cs = 6; goto _out;} } else {
            //            fexec np;
            np = parse_value(np, pe, dummy);
            {p = (( np))-1;}
        }
    }
	goto st6;
tr7:
#line 22 "edn_parser.rl"
	{ line_number++; }
#line 752 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr8:
#line 752 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2186 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr7;
		case 32: goto tr8;
		case 44: goto tr8;
		case 59: goto st4;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr8;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr7;
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
	_out: {}
	}

#line 768 "edn_parser.rl"

    if (cs >= EDN_discard_first_final) {
        return p + 1;
    }
    //    else if (cs == EDN_discard_error) {
        //        error(__FUNCTION__, *p);
    //        return pe;
    //    }
    else if (cs == EDN_discard_en_main) {} // silence ragel warning

    return NULL;
}


// ============================================================
// main parsing machine
//

#line 2240 "edn_parser.cc"
static const int EDN_start = 1;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 799 "edn_parser.rl"


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


#line 2263 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 814 "edn_parser.rl"
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class

#line 2273 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 2287 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto st1;
		case 40: goto tr3;
		case 44: goto st1;
		case 59: goto st3;
		case 62: goto tr3;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto st1;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 60 )
				goto tr3;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr3;
		} else
			goto tr3;
	} else
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr6:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st4;
tr3:
#line 791 "edn_parser.rl"
	{
        const char* np = parse_value(p, pe, result);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2332 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr6;
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
		goto tr6;
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

#line 818 "edn_parser.rl"

    if (cs == EDN_error) {
        error(__FUNCTION__, *p);
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
