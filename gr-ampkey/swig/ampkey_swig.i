/* -*- c++ -*- */

#define AMPKEY_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "ampkey_swig_doc.i"

%{
#include "ampkey/samp_delay.h"
#include "ampkey/keyer.h"
%}

%include "ampkey/samp_delay.h"
GR_SWIG_BLOCK_MAGIC2(ampkey, samp_delay);
%include "ampkey/keyer.h"
GR_SWIG_BLOCK_MAGIC2(ampkey, keyer);
