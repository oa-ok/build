#ifndef SC_VBA_WINDOW_HXX
#define SC_VBA_WINDOW_HXX
#include <cppuhelper/implbase1.hxx>
#include <org/openoffice/vba/XWindow.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/frame/XModel.hpp>

#include "vbahelper.hxx"
#include "vbaworkbook.hxx"

typedef ::cppu::WeakImplHelper1<oo::vba::XWindow > WindowImpl_BASE;

class ScVbaWindow : public WindowImpl_BASE
{		
	css::uno::Reference< css::uno::XComponentContext > m_xContext;
	css::uno::Reference< css::frame::XModel > m_xModel;
	void  Scroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft, bool bLargeScroll = false ) throw (css::uno::RuntimeException);	
public:
	ScVbaWindow( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::frame::XModel >& xModel ) : m_xContext(xContext), m_xModel( xModel ) {}
	// XWindow
	virtual void SAL_CALL setCaption( const css::uno::Any& _caption ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getCaption() throw (css::uno::RuntimeException);

	// Methods
	virtual void SAL_CALL SmallScroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL LargeScroll( const css::uno::Any& Down, const css::uno::Any& Up, const css::uno::Any& ToRight, const css::uno::Any& ToLeft ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL SelectedSheets( const css::uno::Any& aIndex ) throw (css::uno::RuntimeException); 
	virtual void SAL_CALL ScrollWorkbookTabs( const css::uno::Any& Sheets, const css::uno::Any& Position ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Activate(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Close( const css::uno::Any& SaveChanges, const css::uno::Any& FileName, const css::uno::Any& RouteWorkBook ) throw (css::uno::RuntimeException);
};

#endif //SC_VBA_WINDOW_HXX
