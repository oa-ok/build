#ifndef SC_VBA_WORKSHEET_HXX
#define SC_VBA_WORKSHEET_HXX

#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <org/openoffice/vba/XWorksheet.hpp>
#include <org/openoffice/vba/XRange.hpp>
#include <com/sun/star/lang/XEventListener.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <org/openoffice/vba/XOutline.hpp>
#include <org/openoffice/vba/XChartObjects.hpp>

#include "vbahelper.hxx"

class ScVbaWorksheet : public ::cppu::WeakImplHelper1<oo::vba::XWorksheet> 
{
	css::uno::Reference< css::uno::XComponentContext > m_xContext;
	css::uno::Reference< css::sheet::XSpreadsheet > mxSheet;
	css::uno::Reference< css::frame::XModel > mxModel;
	css::uno::Reference< oo::vba::XChartObjects > mxCharts;
		
	css::uno::Reference< oo::vba::XWorksheet > getSheetAtOffset(int offset) throw (css::uno::RuntimeException);
	css::uno::Reference< oo::vba::XRange > getSheetRange() throw (css::uno::RuntimeException);


protected:

	virtual css::uno::Reference< css::frame::XModel > getModel()
	{ return mxModel; }
	virtual css::uno::Reference< css::sheet::XSpreadsheet > getSheet()
	{ return mxSheet; }
	ScVbaWorksheet( const css::uno::Reference< css::uno::XComponentContext >& xContext );
public:
	ScVbaWorksheet( 
		const css::uno::Reference< css::uno::XComponentContext >& xContext,
		const css::uno::Reference< css::sheet::XSpreadsheet >& xSheet, 
		const css::uno::Reference< css::frame::XModel >& xModel )throw (css::uno::RuntimeException)  ;

	virtual ~ScVbaWorksheet() {}


    // Attributes
	virtual ::rtl::OUString SAL_CALL getName() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setName( const ::rtl::OUString &rName ) throw (css::uno::RuntimeException);
	virtual sal_Bool getVisible() throw (css::uno::RuntimeException);
	virtual void setVisible( sal_Bool bVisible ) throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getStandardWidth() throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getStandardHeight() throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getProtectionMode() throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getProtectContents() throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL getProtectDrawingObjects() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL getUsedRange() throw (css::uno::RuntimeException) ;
	virtual css::uno::Any SAL_CALL ChartObjects( const css::uno::Any& Index ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XOutline > SAL_CALL Outline( ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XWorksheet > SAL_CALL getNext() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XWorksheet > SAL_CALL getPrevious() throw (css::uno::RuntimeException);

    // Methods
	virtual void SAL_CALL Activate() throw (css::uno::RuntimeException);
	virtual void SAL_CALL Select() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Range( const css::uno::Any &rRange ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Move( const css::uno::Any& Before, const css::uno::Any& After ) throw (css::uno::RuntimeException) ;
 	virtual void SAL_CALL Copy( const css::uno::Any& Before, const css::uno::Any& After ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Paste( const css::uno::Any& Destination, const css::uno::Any& Link ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Delete(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Protect( const css::uno::Any& Password, const css::uno::Any& DrawingObjects, const css::uno::Any& Contents, const css::uno::Any& Scenarios, const css::uno::Any& UserInterfaceOnly ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Unprotect( const css::uno::Any& Password ) throw (css::uno::RuntimeException);

	virtual void SAL_CALL Calculate(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL CheckSpelling( const css::uno::Any& CustomDictionary,const css::uno::Any& IgnoreUppercase,const css::uno::Any& AlwaysSuggest, const css::uno::Any& SpellingLang ) throw (css::uno::RuntimeException);
	// Hacks (?)
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Cells( const css::uno::Any &nRow, const css::uno::Any &nCol ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > Rows(const css::uno::Any& aIndex ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > Columns(const css::uno::Any& aIndex ) throw (css::uno::RuntimeException);

	virtual css::uno::Any SAL_CALL Evaluate( const ::rtl::OUString& Name ) throw (css::uno::RuntimeException); 
	virtual css::uno::Any SAL_CALL PivotTables( const css::uno::Any& Index ) throw (css::uno::RuntimeException);
};

#endif /* SC_VBA_WORKSHEET_HXX */

