#ifndef SC_VBA_WORKBOOK_HXX
#define SC_VBA_WORKBOOK_HXX

#include <com/sun/star/frame/XModel.hpp>
#include <org/openoffice/vba/XWorkbook.hpp>
#include "vbahelper.hxx"

class ScModelObj;

class ScVbaWorkbook : public org::openoffice::vba::XWorkbook
{
	SC_VBA_UNO_HELPER_MEMBERS;
	uno::Reference< frame::XModel > mxModel;

public:
	ScVbaWorkbook( 	uno::Reference< frame::XModel > xModel ) :
 			mxModel( xModel ), mnRefCount( 1 ) {}
	virtual ~ScVbaWorkbook() {}

	SC_VBA_UNO_HELPER_XINTERFACE;

    // Attributes
	virtual ::rtl::OUString SAL_CALL getName() throw (uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getPath() throw (uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getFullName() throw (uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getProtectStructure() throw (uno::RuntimeException);
	virtual uno::Reference< vba::XWorksheet > SAL_CALL getActiveSheet() throw (uno::RuntimeException);
	virtual uno::Reference< vba::XWorksheets > getWorkSheets() throw (uno::RuntimeException);

	// Methods
	//virtual uno::Reference< vba::XWorksheet > SAL_CALL Worksheets( const ::uno::Any &rSheet ) throw (uno::RuntimeException);
	virtual void SAL_CALL Close( const uno::Any &bSaveChanges,
								 const uno::Any &aFileName,
								 const uno::Any &bRouteWorkbook ) throw (uno::RuntimeException);
	virtual void SAL_CALL Protect() throw (uno::RuntimeException);
};

#endif /* SC_VBA_WORKBOOK_HXX */
