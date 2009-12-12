// -*-c++-*-
/* $Id: parr.h 2784 2007-04-20 16:32:00Z max $ */

#pragma once

#include "pub.h"
#include "pub3ast.h"
#include "pub3eval.h"

namespace pub3 {

  const char *okws_version_str ();
  u_int64_t okws_version_int ();

  //-----------------------------------------------------------------------

  class compiled_fn_t : public expr_t, public callable_t {
  public:
    compiled_fn_t (str lib, str n);
    virtual ~compiled_fn_t () {}

    virtual ptr<const expr_t> eval_to_val (publish_t *e, args_t args) const;
    virtual void pub_to_val (publish_t *p, args_t args, cxev_t, CLOSURE) const;
    str to_str () const;
    bool to_xdr (xpub3_expr_t *x) const { return false; }
    str name () const { return _name; }

  protected:

    // evaluate, given that the arguments have been prevaluted...
    virtual ptr<const expr_t> 
    v_eval_1 (publish_t *e, const cargs_t &args) const { return NULL; }

    ptr<const callable_t> to_callable () const { return mkref (this); }
    const char *get_obj_name () const { return "rfn1::runtime_fn_t"; }

    void pub_args (publish_t *p, args_t in, cargs_t *out, evv_t ev, CLOSURE) 
      const;
    void eval_args (publish_t *p, args_t in, cargs_t *out) const;

    str _lib, _name;
  };

  //-----------------------------------------------------------------------

  class compiled_handrolled_fn_t : public compiled_fn_t {
  public:
    compiled_handrolled_fn_t (str lib, str n) : compiled_fn_t (lib, n) {}
    bool might_block () const { return true; }
    ptr<const expr_t> eval_to_val (publish_t *e, args_t args) 
      const { return NULL; }
  };

  //-----------------------------------------------------------------------

  class patterned_fn_t : public compiled_fn_t {
  public:
    patterned_fn_t (str l, str n, str p) : compiled_fn_t (l, n), _arg_pat (p) {}
    virtual bool might_block () const { return false; }
    
  protected:

    struct arg_t {
      arg_t () : _i (0), _u (0), _b (-1), _n (-1) {}
      ptr<const expr_t> _O;
      ptr<rxx> _r;
      str _s;
      ptr<const expr_dict_t> _d;
      ptr<const expr_list_t> _l;
      int64_t _i;
      u_int64_t _u;
      short _b;
      short _n;
    };

    // evaluate, given that the arguments have been prevaluted...
    ptr<const expr_t> v_eval_1 (publish_t *e, const cargs_t &args) const;

    // evaluate, given that the args have been preevaluated and type-checked
    virtual ptr<const expr_t> 
    v_eval_2 (publish_t *e, const vec<arg_t> &args) const = 0;

    virtual bool check_args (publish_t *p, const cargs_t &args, 
			     vec<arg_t> *a) const;

    str _arg_pat;
  };

  //-----------------------------------------------------------------------

  class compiled_fn_t;

  //
  //  A library of runtime functions.  These functions can be
  //  bound into a specified environment.
  //
  class library_t {
  public:
    library_t () {}
    ~library_t () {}
    void bind (ptr<bindtab_t> b);
    ptr<bindtab_t> bind ();
    static void import (ptr<library_t> l);
    static void clear_all ();
  protected:
    vec<ptr<compiled_fn_t> > _functions;
  };

  //-----------------------------------------------------------------------

};
