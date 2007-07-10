/*************************************************************************
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Kohei Yoshida.
 *    1039 Kingsway Dr., Apex, NC 27502, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/

#ifndef _SCSOLVER_NUMERIC_FUNCOBJ_HXX_
#define _SCSOLVER_NUMERIC_FUNCOBJ_HXX_

#include <vector>
#include <string>

namespace scsolver { namespace numeric {

/**
 * Base class for function object to be used for NLP algorithms.
 */
class BaseFuncObj
{
public:
    BaseFuncObj();
    virtual ~BaseFuncObj() throw() = 0;

    virtual const ::std::vector<double>& getVars() const = 0;

    virtual void setVars(const ::std::vector<double>& vars) = 0;

    virtual void setVar(size_t index, double var) = 0;

    virtual double eval() = 0;

    /**
     * Return a display-friendly function string. 
     */
    virtual const std::string getFuncString() const = 0;
};

// --------------------------------------------------------------------------

/** 
 * Non-linear function object that only has one variable.  Used for a
 * line-search algorithm.
 */
class SingleVarFuncObj
{
public:
    SingleVarFuncObj();
    virtual ~SingleVarFuncObj() throw() = 0;

    virtual double eval(double var) const = 0;

    /** 
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4). 
     */
    virtual const ::std::string getFuncString() const = 0;
};


}}

#endif

