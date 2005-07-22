#ifndef SC_VBA_GLOBALS
#define SC_VBA_GLOBALS

#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <org/openoffice/vba/XGlobals.hpp>

#include <cppuhelper/implbase2.hxx>


    // =============================================================================
    // class ScVbaGlobals
    // =============================================================================

    typedef ::cppu::WeakImplHelper2<    
        ::org::openoffice::vba::XGlobals,
        ::com::sun::star::lang::XServiceInfo > ScVbaGlobals_BASE;


    class ScVbaGlobals : public ScVbaGlobals_BASE
    {
    private:
            ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > m_xContext;
            ::com::sun::star::uno::Reference< ::org::openoffice::vba::XApplication > mxApplication;
    public:
	
        ScVbaGlobals(
            const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& rxContext );
        virtual ~ScVbaGlobals();

		static 	com::sun::star::uno::Reference< org::openoffice::vba::XGlobals > getGlobalsImpl(::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& ) throw (com::sun::star::uno::RuntimeException);

        // XServiceInfo
        virtual ::rtl::OUString SAL_CALL getImplementationName(  ) 
            throw (::com::sun::star::uno::RuntimeException);
        virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ServiceName ) 
            throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames(  ) 
            throw (::com::sun::star::uno::RuntimeException);

        // XGlobals
        virtual ::com::sun::star::uno::Reference<
                        ::org::openoffice::vba::XApplication > SAL_CALL getApplication()
                        throw (com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Reference< ::org::openoffice::vba::XWorkbook > SAL_CALL getActiveWorkbook() throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Reference< ::org::openoffice::vba::XWorksheet > SAL_CALL getActiveSheet() throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Reference< ::org::openoffice::vba::XWorksheets > SAL_CALL getWorkSheets() throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Reference< ::org::openoffice::vba::XWorkbooks > SAL_CALL getWorkBooks() throw (::com::sun::star::uno::RuntimeException);

	virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any > SAL_CALL getGlobals(  ) throw (::com::sun::star::uno::RuntimeException);


    };
#endif // 
