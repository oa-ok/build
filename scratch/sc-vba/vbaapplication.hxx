#ifndef SC_VBA_APPLICATION_HXX
#define SC_VBA_APPLICATION_HXX

#include <cppuhelper/implbase1.hxx>

#include <org/openoffice/vba/XWorksheetFunction.hpp>
#include <org/openoffice/vba/XApplication.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "vbahelper.hxx"

typedef ::cppu::WeakImplHelper1< org::openoffice::vba::XApplication > ScVbaApplication_BASE;

class ScVbaApplication : public ScVbaApplication_BASE
{
private:
	uno::Reference< ::com::sun::star::uno::XComponentContext > m_xContext;
public:
	ScVbaApplication( uno::Reference< ::com::sun::star::uno::XComponentContext >& m_xContext );
	virtual ~ScVbaApplication();

    // XApplication
	virtual uno::Reference< vba::XRange > getSelection() throw (uno::RuntimeException);
	virtual uno::Reference< vba::XWorkbook > getActiveWorkbook() throw (uno::RuntimeException);
	virtual uno::Reference< vba::XRange > getActiveCell() throw (uno::RuntimeException);
	virtual sal_Bool getScreenUpdating() throw (uno::RuntimeException);
	virtual void setScreenUpdating(sal_Bool bUpdate) throw (uno::RuntimeException);
	virtual uno::Any SAL_CALL Workbooks( const uno::Any& aIndex ) throw (uno::RuntimeException);
	virtual ::com::sun::star::uno::Reference< ::org::openoffice::vba::XWorksheetFunction > SAL_CALL WorksheetFunction( ) throw (::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Any SAL_CALL Evaluate( const ::rtl::OUString& Name ) throw (::com::sun::star::uno::RuntimeException); 
	virtual ::com::sun::star::uno::Any SAL_CALL Dialogs( const ::com::sun::star::uno::Any& DialogIndex ) throw (::com::sun::star::uno::RuntimeException);

};
#endif /* SC_VBA_APPLICATION_HXX */
