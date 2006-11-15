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

#ifndef _BASELISTENER_HXX_
#define _BASELISTENER_HXX_

#ifndef _CPPU_MACROS_HXX_
#include "cppu/macros.hxx"
#endif

#include "unoglobal.hxx"

#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/awt/XActionListener.hpp>
#include <com/sun/star/awt/XItemListener.hpp>
#include <com/sun/star/awt/XFocusListener.hpp>
#include <com/sun/star/awt/XMouseListener.hpp>
#include <com/sun/star/awt/XTopWindowListener.hpp>

#include <memory>

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;

namespace scsolver {

class BaseDialog;

//--------------------------------------------------------------------------
// Listener Class for Dialog Widgets

/**
 *  All listener objects keeps a pointer to the dialog they
 *  belong to.  Such pointer can be retrieved by calling
 *  getDialog().
 */
class Listener
{
public:
	Listener( BaseDialog* pDlg ) { m_pDlg = pDlg; }
	virtual ~Listener() throw() = 0;
	
protected:
	BaseDialog* getDialog() const { return m_pDlg; }
	
private:
	BaseDialog* m_pDlg;
};

class SimpleActionObject
{
public:
	SimpleActionObject();
	virtual ~SimpleActionObject() throw() = 0;
	virtual void execute( BaseDialog* dlg ) = 0;
};

/**
 * This class defines action for ActionListener object.
 */
class ActionObject
{
public:
	ActionObject();
	virtual ~ActionObject() throw() = 0;
	virtual void execute( BaseDialog* dlg, const awt::ActionEvent& e ) = 0;
};

/**
 * Generic action listener class.  You can either derive a child
 * class from it, or directly instantiate it with an instance of
 * ActionObject which defines the action to be performed.  If an
 * instance of ActionObject is passed as a pointer, the memory
 * of that instance must be managed by the calling function.
 */
class ActionListener : public ::cppu::WeakImplHelper1< awt::XActionListener >, public Listener
{
public:
	ActionListener( BaseDialog* pDlg );
	ActionListener( BaseDialog* pDlg, ActionObject* pAction );
	virtual ~ActionListener() throw();
	
	virtual void SAL_CALL disposing( const lang::EventObject& e ) throw ( RuntimeException );
	virtual void SAL_CALL actionPerformed( const awt::ActionEvent& e ) throw ( RuntimeException );

private:
	ActionObject* m_pAction;
};

class ItemListener : public ::cppu::WeakImplHelper1< awt::XItemListener >, public Listener
{
public:
	ItemListener( BaseDialog* pDlg ) : Listener( pDlg ) {}
	virtual ~ItemListener() throw() = 0;

	virtual void SAL_CALL disposing( const lang::EventObject& ) throw ( RuntimeException ) {}
	virtual void SAL_CALL itemStateChanged( const awt::ItemEvent& ) throw ( RuntimeException ) {}
};


class FocusListener : public ::cppu::WeakImplHelper1< awt::XFocusListener >, public Listener
{
public:
	FocusListener( BaseDialog* pDlg ) : Listener( pDlg ) {}
	virtual ~FocusListener() throw() = 0;
	
	virtual void SAL_CALL focusGained( const awt::FocusEvent& ) throw( RuntimeException ) {}
	virtual void SAL_CALL focusLost( const awt::FocusEvent& ) throw( RuntimeException ) {}
	virtual void SAL_CALL disposing( const lang::EventObject& ) throw ( RuntimeException ) {}
};	

class MouseListener : public ::cppu::WeakImplHelper1< awt::XMouseListener >, public Listener
{
public:
	MouseListener( BaseDialog* pDlg ) : Listener( pDlg ) {}
	virtual ~MouseListener() throw() = 0;
	
	virtual void SAL_CALL mousePressed( const awt::MouseEvent& ) throw( RuntimeException ) {}
	virtual void SAL_CALL mouseReleased( const awt::MouseEvent& ) throw( RuntimeException ) {}
	virtual void SAL_CALL mouseEntered( const awt::MouseEvent& ) throw( RuntimeException ) {}
	virtual void SAL_CALL mouseExited( const awt::MouseEvent& ) throw( RuntimeException ) {}
	
	virtual void SAL_CALL disposing( const lang::EventObject& ) throw ( RuntimeException ) {}
};

class TopWindowListenerImpl;

class TopWindowListener : public ::cppu::WeakImplHelper1<awt::XTopWindowListener>, public Listener
{
public:
	TopWindowListener( BaseDialog* pDlg );
	virtual ~TopWindowListener() throw();

	virtual void SAL_CALL windowOpened( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowClosing( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowClosed( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowMinimized( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowNormalized( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowActivated( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL windowDeactivated( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& e )
			throw (RuntimeException);

	void setActionClosing( SimpleActionObject* p );

private:
	::std::auto_ptr<TopWindowListenerImpl> m_pImpl;
};

}

#endif
