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


#ifndef _LPSIMPLEX_HXX_
#define _LPSIMPLEX_HXX_

#include "lpbase.hxx"
#include <memory>

namespace scsolver { namespace numeric { namespace opres { namespace lp {

class Model;
class RevisedSimplexImpl;
class BoundedRevisedSimplexImpl;

class RevisedSimplex : public BaseAlgorithm
{
public:
	RevisedSimplex();
	virtual ~RevisedSimplex() throw();

	virtual void solve();

	void setEnableTwoPhaseSearch( bool );

private:
	std::auto_ptr<RevisedSimplexImpl> m_pImpl;
};


class BoundedRevisedSimplex : public BaseAlgorithm
{
public:
	BoundedRevisedSimplex();
	virtual ~BoundedRevisedSimplex() throw();

	virtual void solve();

private:
	std::auto_ptr<BoundedRevisedSimplexImpl> m_pImpl;
};


}}}}


#endif
