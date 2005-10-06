#ifndef SC_VBA_DIALOG_HXX
#define SC_VBA_DIALOG_HXX

#include <cppuhelper/implbase1.hxx>

#include <com/sun/star/uno/XComponentContext.hpp>
#include <org/openoffice/vba/XGlobals.hpp>
#include <org/openoffice/vba/XApplication.hpp>
#include <org/openoffice/vba/XDialog.hpp>
 
#include "vbahelper.hxx"
#include "vbadialog.hxx"

typedef ::cppu::WeakImplHelper1< org::openoffice::vba::XDialog > ScVbaDialog_BASE;

class ScVbaDialog : public ScVbaDialog_BASE
{
	sal_Int32 mnIndex;
	uno::Reference< ::com::sun::star::uno::XComponentContext > m_xContext;
public:
	ScVbaDialog( sal_Int32 nIndex, uno::Reference< ::com::sun::star::uno::XComponentContext > xContext ):
		mnIndex( nIndex ),
		m_xContext( xContext ) {}
	virtual ~ScVbaDialog() {}

	// Attributes
	virtual uno::Reference< vba::XApplication > SAL_CALL getApplication() throw (uno::RuntimeException);
	virtual uno::Any SAL_CALL getParent() throw (uno::RuntimeException);
	virtual sal_Int32 SAL_CALL getCreator() throw(uno::RuntimeException);
	// Methods
	virtual void SAL_CALL Show() throw (uno::RuntimeException);
	rtl::OUString mapIndexToName( sal_Int32 nIndex );
};

#endif /* SC_VBA_DIALOG_HXX */
