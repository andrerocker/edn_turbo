#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

#include <rice/String.hpp>
#include <rice/Array.hpp>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include "edn_parser.h"

namespace edn
{
    //
    // used to determine max number of chars in string value of a type
    template <typename T>
    static std::size_t get_max_chars(T)
    {
        std::stringstream s;
        s << std::fixed << std::numeric_limits<T>::max();
        return s.str().length();
    }

    static const std::size_t LL_max_chars = get_max_chars<>((long long) 1);
    static const std::size_t LD_max_chars = get_max_chars<>((long double) 1);

    //
    // convert to int.. if string rep has more digits than long can
    // hold, call into ruby to get a big num
    Rice::Object Parser::integer_to_ruby(const char* str, std::size_t len)
    {
        if (len < LL_max_chars)
        {
            return buftotype<long>(str, len);
        }

        // value is outside of range of long type. Use ruby to convert it
        VALUE rb_s = Rice::protect(rb_str_new2, str);
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_STR_INT_TO_BIGNUM, 1, rb_s);
    }

    //
    // as above.. TODO: check exponential
    Rice::Object Parser::float_to_ruby(const char* str, std::size_t len)
    {
        if (len < LD_max_chars)
        {
            return buftotype<double>(str, len);
        }

        // value is outside of range of long type. Use ruby to convert it
        VALUE rb_s = Rice::protect(rb_str_new2, str);
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_STR_DBL_TO_BIGNUM, 1, rb_s);
    }


    //
    // copies the string data, unescaping any present values that need to be replaced
    //
    bool Parser::parse_byte_stream(const char *p_start, const char *p_end, Rice::String& s)
    {
        if (p_end > p_start) {
            std::string buf;

            if (unicode_to_utf8(p_start, p_end - p_start, buf))
            {
                // utf-8 encode
                VALUE vs = Rice::protect( rb_str_new2, buf.c_str() );
                VALUE s_utf8 = Rice::protect( rb_enc_associate, vs, rb_utf8_encoding() );
                s = Rice::String(s_utf8);
                return true;
            }
        }

        return false;
    }

    //
    // handles things like \c, \newline
    //
    bool Parser::parse_escaped_char(const char *p, const char *pe, Rice::Object& o)
    {
        std::string buf;
        std::size_t len = pe - p;
        buf.append(p, len);

        if (len > 1) {
            if      (buf == "newline") buf = "\\n";
            else if (buf == "tab") buf = "\\t";
            else if (buf == "return") buf = "\\r";
            else if (buf == "space") buf = " ";
            else if (buf == "formfeed") buf = "\\f";
            else if (buf == "backspace") buf = "\\b";
            // TODO: is this supported?
            else if (buf == "verticaltab") buf = "\\v";
            else return false;
        }

        o = Rice::String(buf);
        return true;
    }


    //
    // get a set representation from the ruby side. See edn_turbo.rb
    Rice::Object Parser::make_edn_symbol(const std::string& name)
    {
        VALUE rb_s = Rice::protect(rb_str_new2, name.c_str());
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_MAKE_EDN_SYMBOL, 1, rb_s);
    }

    //
    // get a set representation from the ruby side. See edn_turbo.rb
    Rice::Object Parser::make_ruby_set(const Rice::Array& elems)
    {
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_MAKE_SET_METHOD, 1, elems.value());
    }

    //
    // get an object representation from the ruby side using the given symbol name
    Rice::Object Parser::tagged_element(const std::string& name, const Rice::Object& data)
    {
        VALUE rb_s = Rice::protect(rb_str_new2, name.c_str());
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_TAGGED_ELEM, 2, rb_s, data.value());
    }


    //
    // error reporting
    void Parser::error(const std::string& func, const std::string& err, char c) const
    {
        std::cerr << "Parse error "
            //            "from " << func << "() "
            ;
        if (err.length() > 0)
            std::cerr << "(" << err << ") ";
        if (c != '\0')
            std::cerr << "at '" << c << "' ";
        std::cerr << "on line " << line_number << std::endl;
    }
}
