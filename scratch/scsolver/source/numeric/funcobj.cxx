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

#include "numeric/funcobj.hxx"
#include <vector>
#include <string>

namespace scsolver { namespace numeric {

//---------------------------------------------------------------------------
// BaseFuncObj

BaseFuncObj::BaseFuncObj()
{
}

BaseFuncObj::~BaseFuncObj() throw()
{
}

// --------------------------------------------------------------------------

/**
 * Class that wraps an instance of class SinglVarFuncObj in case one needs to 
 * pass the instance as type BaseFuncObj.  The lifetime of this class instance 
 * must end before the enclosed class instance.
 */
class SingleBaseFuncObj : public BaseFuncObj
{
public:
    explicit SingleBaseFuncObj(SingleVarFuncObj* pFuncObj) :
        mpFuncObj(pFuncObj)
    {
        mVars.push_back(pFuncObj->getVar());
    }

    virtual ~SingleBaseFuncObj() throw()
    {
    }

    virtual double eval()
    {
        return mpFuncObj->eval();
    }

    virtual const std::string getFuncString() const
    {
        return mpFuncObj->getFuncString();
    }

    virtual const std::vector<double> & getVars() const
    {
        return mVars;
    }

    virtual void setVar(size_t index, double var)
    {
        if (index != 0)
            return;

        mpFuncObj->setVar(var);
        mVars[0] = var;
    }

    virtual void setVars(const std::vector<double> &vars)
    {
        if ( vars.empty() )
            return;

        mpFuncObj->setVar(vars[0]);
        mVars[0] = vars[0];
    }

private:
    SingleVarFuncObj* mpFuncObj;
    std::vector<double> mVars;
};

SingleVarFuncObj::SingleVarFuncObj()
{
}

SingleVarFuncObj::~SingleVarFuncObj() throw()
{
}

double SingleVarFuncObj::operator()(double var)
{
    setVar(var);
    return eval();
}

BaseFuncObj* SingleVarFuncObj::toBaseFuncObj()
{
    return new SingleBaseFuncObj(this);
}

}}
