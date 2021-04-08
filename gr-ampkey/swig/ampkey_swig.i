/* -*- c++ -*- */

#define AMPKEY_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "ampkey_swig_doc.i"

%{
#include "ampkey/samp_delay.h"
#include "ampkey/amp_ptt.h"
%}

%include "ampkey/samp_delay.h"
GR_SWIG_BLOCK_MAGIC2(ampkey, samp_delay);

%include "ampkey/amp_ptt.h"
GR_SWIG_BLOCK_MAGIC2(ampkey, amp_ptt);
