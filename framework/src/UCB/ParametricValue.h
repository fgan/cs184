/*
 * ParametricValue.h
 *
 *  A wrapper around the expression parser
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef PARAMETRICVALUE_H_
#define PARAMETRICVALUE_H_

#include "mathexpr.h"
#include <algebra3.h>

class ParametricValue {
public:
    virtual double getValue() = 0;
    virtual double getValue(double time) = 0;
    virtual bool good() { return true; }
    virtual ~ParametricValue() {};
};

class ConstValue : public ParametricValue {
    double _value;

public:
    ConstValue(double value) : _value(value) {}

    double getValue() { return _value; }
    double getValue(double time) { return _value; }
};

class ExprValue : public ParametricValue {
    double _time;
    double _dgr;
    RVar _timevar;
    RVar _dgrvar;
    RVar* _vararray[2];
    ROperation _op;
    string _expr;

public:
    ExprValue(const char *expr) : _timevar ( "t" , &_time ), _dgrvar( "dgr", &_dgr ), _expr(expr) {
        _vararray[0]=&_timevar;
        _vararray[1]=&_dgrvar;
        _op = ROperation ( _expr.c_str(), 2, _vararray );
        _time = 0;
        _dgr = M_PI/180.0;
    }

    // need special copy and copy assignment to prevent from having _vararray point off to some old class
    // (which would make values not update properly after a copy!)
    ExprValue(const ExprValue &expr) : _time(expr._time), _timevar(expr._timevar), _dgrvar( "dgr", &_dgr ), _expr(expr._expr) {
        _vararray[0]=&_timevar;
        _vararray[1]=&_dgrvar;
        _op = ROperation ( _expr.c_str(), 2, _vararray );
        _dgr = M_PI/180.0;
    }

    ExprValue& operator =(const ExprValue& expr) {
        _time = expr._time;
        _dgr = M_PI/180.0;
        _timevar = RVar(expr._timevar);
        _dgrvar = RVar( "dgr", &_dgr );
        _expr = expr._expr;
        _vararray[0]=&_timevar;
        _vararray[1]=&_dgrvar;
        _op = ROperation ( _expr.c_str(), 2, _vararray );
        return *this;
    }

    bool good() {
        return !_op.HasError();
    }

    double getValue() {
        _time = 0; // set default time
        return _op.Val();
    }

    double getValue(double time) {
        _time = time;
        return _op.Val();
    }

    virtual ~ExprValue() {}
};

#endif /* PARAMETRICVALUE_H_ */
