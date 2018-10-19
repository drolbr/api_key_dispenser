
AM_CXXFLAGS= -std=c++11

cgibindir = ${prefix}/cgi-bin
cgibin_PROGRAMS = cgi-bin/make_apikey cgi-bin/renew_apikey cgi-bin/revoke_apikey cgi-bin/retrieve_keys

cgi_bin_make_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc make_apikey.cc
cgi_bin_make_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_make_apikey_LDADD = -lpq

cgi_bin_renew_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc renew_apikey.cc
cgi_bin_renew_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_renew_apikey_LDADD = -lpq

cgi_bin_revoke_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc revoke_apikey.cc
cgi_bin_revoke_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_revoke_apikey_LDADD = -lpq

cgi_bin_retrieve_keys_SOURCES = cgi_helper.cc postgresql_wrapper.cc retrieve_keys.cc
cgi_bin_retrieve_keys_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_retrieve_keys_LDADD = -lpq
