#include <signal.h>
#include <iostream>
#include <clocale>

// always include rice headers before ruby.h
#include <rice/Data_Type.hpp>
#include <rice/Constructor.hpp>

#include "edn_parser.h"


namespace edn {

    Rice::Module rb_mEDNT;

    // methods on the ruby side we'll call from here
    VALUE EDNT_MAKE_EDN_SYMBOL = Qnil;
    VALUE EDNT_MAKE_SET_METHOD = Qnil;
    VALUE EDNT_TAGGED_ELEM = Qnil;
    VALUE EDNT_STR_INT_TO_BIGNUM = Qnil;
    VALUE EDNT_STR_DBL_TO_BIGNUM = Qnil;


    void die(int sig)
    {
        exit(-1);
    }
}


//
// ruby calls this to load the extension
extern "C"
void Init_edn_turbo(void)
{
    struct sigaction a;
    a.sa_handler = edn::die;
    sigemptyset(&a.sa_mask);
    a.sa_flags = 0;
    sigaction(SIGINT, &a, 0);

    // pass things back as utf-8
    if (!setlocale( LC_ALL, "" )) {
        std::cerr << "Error setting locale" << std::endl;
        return;
    }

    edn::rb_mEDNT = Rice::define_module("EDNT");

    // bind methods we'll call - these should be defined in edn_turbo.rb
    edn::EDNT_MAKE_EDN_SYMBOL = rb_intern("make_edn_symbol");
    edn::EDNT_MAKE_SET_METHOD = rb_intern("make_set");
    edn::EDNT_TAGGED_ELEM = rb_intern("tagged_element");
    edn::EDNT_STR_INT_TO_BIGNUM = rb_intern("string_int_to_bignum");
    edn::EDNT_STR_DBL_TO_BIGNUM = rb_intern("string_double_to_bignum");

    // bind the ruby Parser class to the C++ one
    Rice::Data_Type<edn::Parser> rb_cParser =
        Rice::define_class_under<edn::Parser>(edn::rb_mEDNT, "Parser")
        .define_constructor(Rice::Constructor<edn::Parser>())
        .define_method("ext_read", &edn::Parser::process, (Rice::Arg("data")))
        ;

    // import whatever else we've defined in the ruby side
    rb_require("edn_turbo/edn_parser");
}
