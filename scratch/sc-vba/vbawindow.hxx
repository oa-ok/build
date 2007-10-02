/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  last change: $Author$ $Date$
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
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
#ifndef SC_VBA_WINDOW_HXX
#define SC_VBA_WINDOW_HXX
#include <cppuhelper/implbase1.hxx>
#include <org/openoffice/excel/XWindow.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/sheet/XViewPane.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <org/openoffice/excel/XPane.hpp>

#include "vbahelperinterface.hxx"
#include "vbaworkbook.hxx"

typedef InheritedHelperInterfaceImpl1<oo::excel::XWindow > WindowImpl_BASE;

class ScVbaWindow : public WindowImpl_BASE
{		
	css::uno::Reference< css::frame::XModel > m_xModel;
	css::uno::Reference< css::sheet::XViewPane > m_xViewPane;
	css::uno::Reference< oo::excel::XPane > m_xPane;
	void  Scroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft, bool bLargeScroll = false ) throw (css::uno::RuntimeException);	
public:
	ScVbaWindow( const css::uno::Reference< oo::vba::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::frame::XModel >& xModel );
 	css::uno::Reference< oo::excel::XPane > ActivePane();
	// XWindow
	virtual ::sal_Bool SAL_CALL getDisplayGridlines() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setDisplayGridlines( ::sal_Bool _displaygridlines ) throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getDisplayVerticalScrollBar() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setDisplayVerticalScrollBar( ::sal_Bool _displayverticalscrollbar ) throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getDisplayHorizontalScrollBar() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setDisplayHorizontalScrollBar( ::sal_Bool _displayhorizontalscrollbar ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL setCaption( const css::uno::Any& _caption ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getCaption() throw (css::uno::RuntimeException);
    virtual css::uno::Any SAL_CALL getScrollRow() throw (css::uno::RuntimeException) ;
    virtual void SAL_CALL setScrollRow( const css::uno::Any& _scrollrow ) throw (css::uno::RuntimeException) ;
    virtual css::uno::Any SAL_CALL getScrollColumn() throw (css::uno::RuntimeException) ;
    virtual void SAL_CALL setScrollColumn( const css::uno::Any& _scrollcolumn ) throw (css::uno::RuntimeException) ;
    virtual css::uno::Any SAL_CALL getWindowState() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setWindowState( const css::uno::Any& _windowstate ) throw (css::uno::RuntimeException);

	// Methods
	virtual void SAL_CALL SmallScroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL LargeScroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL SelectedSheets( const css::uno::Any& aIndex ) throw (css::uno::RuntimeException); 
	virtual void SAL_CALL ScrollWorkbookTabs( const css::uno::Any& Sheets, const css::uno::Any& Position ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Activate(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Close( const css::uno::Any& SaveChanges, const css::uno::Any& FileName, const css::uno::Any& RouteWorkBook ) throw (css::uno::RuntimeException);
	 virtual css::uno::Reference< oo::excel::XRange > SAL_CALL ActiveCell(  ) throw (css::script::BasicErrorException, css::uno::RuntimeException); 
	 virtual css::uno::Any SAL_CALL Selection(  ) throw (css::script::BasicErrorException, css::uno::RuntimeException); 
	// XHelperInterface
	virtual rtl::OUString& getServiceImplName();
	virtual css::uno::Sequence<rtl::OUString> getServiceNames();
};

#endif //SC_VBA_WINDOW_HXX
