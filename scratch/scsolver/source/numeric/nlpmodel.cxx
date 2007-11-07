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

#include "numeric/nlpmodel.hxx"
#include "numeric/nlpbase.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/exception.hxx"
#include "tool/global.hxx"
#include <vector>
#include <iostream>

#include <stdio.h>

using namespace ::scsolver::numeric;
using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

//---------------------------------------------------------------------------
// IMPLEMENTATION

/** Decision variable data structure */
struct DecVar
{
    double Value;

    bool   UpperBoundEnabled;
    double UpperBoundValue;

    bool   LowerBoundEnabled;
    double LowerBoundValue;

    DecVar() :
        Value(0.0),
        UpperBoundEnabled(false), 
        UpperBoundValue(0.0),
        LowerBoundEnabled(false), 
        LowerBoundValue(0.0)
    {}
};

class ModelImpl
{
public:
	ModelImpl( Model* );
	ModelImpl( const ModelImpl& );
	~ModelImpl() throw();

	void setPrecision( unsigned long n ) { m_nPrec = n; }
	unsigned long getPrecision() const { return m_nPrec; }

	void setGoal( Goal e ) { m_eGoal = e; }
	Goal getGoal() const { return m_eGoal; }

	void setVerbose( bool b ) { m_bVerbose = b; }
	bool getVerbose() const { return m_bVerbose; }

	void setFuncObject(BaseFuncObj* pFuncObj);
	BaseFuncObj* getFuncObject() const { return m_pFuncObj; }

	void pushVar( double value )
	{
        DecVar var;
        var.Value = value;
		m_cnVars.push_back(var);
	}

	void getVars( vector<double>& vars ) const
	{
        size_t n = m_cnVars.size();
        vector<double> tmp;
        tmp.reserve(n);
		vector<DecVar>::const_iterator itr = m_cnVars.begin(), itrEnd = m_cnVars.end();
        for (; itr != itrEnd; ++itr)
            tmp.push_back(itr->Value);
        vars.swap(tmp);
	}

    double getVarBound( size_t index, Bound bound ) const
    {
        switch (bound)
        {
        case BOUND_LOWER:
            if ( !m_cnVars.at(index).LowerBoundEnabled )
                throw UnboundedException();
            return m_cnVars.at(index).LowerBoundValue;
        case BOUND_UPPER:
            if ( !m_cnVars.at(index).UpperBoundEnabled )
                throw UnboundedException();
            return m_cnVars.at(index).UpperBoundValue;
        }
        return 0.0;
    }

    void setVarBound( size_t index, Bound bound, double value )
    {
        switch (bound)
        {
        case BOUND_LOWER:
            m_cnVars.at(index).LowerBoundValue = value;
            m_cnVars.at(index).LowerBoundEnabled = true;
            break;
        case BOUND_UPPER:
            m_cnVars.at(index).UpperBoundValue = value;
            m_cnVars.at(index).UpperBoundEnabled = true;
            break;
        }
    }

    bool isVarBounded( size_t index, Bound bound ) const
    {
        switch (bound)
        {
        case BOUND_LOWER:
            return m_cnVars.at(index).LowerBoundEnabled;
        case BOUND_UPPER:
            return m_cnVars.at(index).UpperBoundEnabled;
        }
        throw AssertionWrong();
        return false;
    }

	void print() const;

private:
	Model* m_pSelf;
	unsigned long m_nPrec;
	Goal m_eGoal;
	bool m_bVerbose;
	vector<DecVar> m_cnVars;

    BaseFuncObj* m_pFuncObj;
};

ModelImpl::ModelImpl( Model* p ) : 
	m_pSelf( p ),
	m_nPrec( 9 ), 
	m_eGoal( GOAL_UNKNOWN ), 
	m_bVerbose( false ),
    m_pFuncObj(NULL)
{
}

ModelImpl::ModelImpl( const ModelImpl& other ) : 
	m_nPrec( other.m_nPrec ),
	m_pFuncObj( other.m_pFuncObj )
{
}

ModelImpl::~ModelImpl() throw()
{
}

void ModelImpl::setFuncObject(BaseFuncObj* pFuncObj)
{
	m_pFuncObj = pFuncObj;
}

void ModelImpl::print() const
{
    FILE* fs = stdout;

    // Objective function and goal.
    fprintf(fs, "--------------------------------------------------------------------\n");
    fprintf(fs, "objective: ");
	if (m_pFuncObj)
        fprintf(fs, "%s\n", m_pFuncObj->getFuncString().c_str());
	else
        fprintf(fs, "object function not set!\n");

    fprintf(fs, "goal: ");
	switch ( m_eGoal )
	{
    case GOAL_MAXIMIZE:
        fprintf(fs, "maximize");
		break;
    case GOAL_MINIMIZE:
        fprintf(fs, "minimize");
		break;
    default:
        fprintf(fs, "unknown");
	}
    fprintf(fs, "\n");
    fprintf(fs, "--------------------------------------------------------------------\n");

    size_t n = m_cnVars.size();
    for (size_t i = 0; i < n; ++i)
    {
        const DecVar& var = m_cnVars.at(i);
        fprintf(fs, "var %d: ", i);
        if (var.LowerBoundEnabled)
            fprintf(fs, "%.2f", var.LowerBoundValue);
        else 
            fprintf(fs, "[unbounded]");
        fprintf(fs, " - ");

        if (var.UpperBoundEnabled)
            fprintf(fs, "%.2f", var.UpperBoundValue);
        else
            fprintf(fs, "[unbounded]");
        fprintf(fs, "\n");
    }
    fflush(fs);
}

//---------------------------------------------------------------------------
// ENVELOPE

Model::Model() : m_pImpl( new ModelImpl( this ) )
{
}

Model::Model( const Model& other ) : m_pImpl( new ModelImpl( *other.m_pImpl.get() ) )
{
}

Model::~Model() throw()
{
}

void Model::print() const
{
	m_pImpl->print();
}

void Model::setPrecision( unsigned long n )
{
	m_pImpl->setPrecision( n );
}

unsigned long Model::getPrecision() const
{
	return m_pImpl->getPrecision();
}

void Model::setGoal( Goal e )
{
	m_pImpl->setGoal( e );
}

Goal Model::getGoal() const
{
	return m_pImpl->getGoal();
}

void Model::setVerbose( bool b )
{
	m_pImpl->setVerbose( b );
}

bool Model::getVerbose() const
{
	return m_pImpl->getVerbose();
}

void Model::setFuncObject(BaseFuncObj* pFuncObj)
{
	m_pImpl->setFuncObject(pFuncObj);
}

BaseFuncObj* Model::getFuncObject() const
{
	return m_pImpl->getFuncObject();
}

void Model::pushVar( double var )
{
	m_pImpl->pushVar(var);
}

void Model::getVars( vector<double>& vars ) const
{
	m_pImpl->getVars(vars);
}

double Model::getVarBound( size_t index, Bound bound ) const
{
    return m_pImpl->getVarBound(index, bound);
}

void Model::setVarBound( size_t index, Bound bound, double value )
{
    m_pImpl->setVarBound(index, bound, value);
}

bool Model::isVarBounded( size_t index, Bound bound ) const
{
    return m_pImpl->isVarBounded(index, bound);
}

}}}

