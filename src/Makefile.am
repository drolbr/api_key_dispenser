
AM_CXXFLAGS= -std=c++11

bin_PROGRAMS = renew_apikey

renew_apikey_SOURCES = renew_apikey.cc postgresql_wrapper.cc
renew_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
renew_apikey_LDADD = -lpq
