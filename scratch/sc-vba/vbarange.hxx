#ifndef SC_VBA_RANGE_HXX
#define SC_VBA_RANGE_HXX

#include <cppuhelper/implbase1.hxx>
#include <cppuhelper/implbase2.hxx>
#include <cppuhelper/implbase3.hxx>
#include <com/sun/star/container/XEnumerationAccess.hpp>

#include <org/openoffice/vba/XRange.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <org/openoffice/vba/XFont.hpp>
#include <org/openoffice/vba/XComment.hpp>
#include <org/openoffice/vba/xlPasteType.hdl>
#include <org/openoffice/vba/xlPasteSpecialOperation.hdl>

#include <comphelper/proparrhlp.hxx>
#include <comphelper/propertycontainer.hxx>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/PropertyAttribute.hpp>
#include <com/sun/star/script/XTypeConverter.hpp>
#include <com/sun/star/script/XDefaultMethod.hpp>
#include <com/sun/star/sheet/FillDateMode.hpp>
#include <com/sun/star/sheet/FillMode.hpp>
#include <com/sun/star/sheet/FillDirection.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>

#include "vbahelper.hxx"

class ScTableSheetsObj;

typedef ::cppu::WeakImplHelper3< oo::vba::XRange, css::container::XEnumerationAccess, css::script::XDefaultMethod > ScVbaRange_BASE;
class ArrayVisitor
{
public:
	virtual void visitNode( sal_Int32 x, sal_Int32 y, const css::uno::Reference< css::table::XCell >& xCell ) = 0;
};
class ScVbaRange : public ScVbaRange_BASE
    ,public ::comphelper::OMutexAndBroadcastHelper
    ,public ::comphelper::OPropertyContainer

{
	css::uno::Reference< css::table::XCellRange > mxRange;
	css::uno::Reference< css::uno::XComponentContext > m_xContext;
	sal_Bool mbIsRows;
	sal_Bool mbIsColumns;
	rtl::OUString msDftPropName;
	void visitArray( ArrayVisitor& vistor );
	css::uno::Reference< css::script::XTypeConverter > getTypeConverter() throw (css::uno::RuntimeException);

	css::uno::Reference< oo::vba::XRange > getEntireColumnOrRow( bool bEntireRow = true ) throw( css::uno::RuntimeException );

	void fillSeries(  css::sheet::FillDirection nFillDirection, css::sheet::FillMode nFillMode, css::sheet::FillDateMode nFillDateMode, double fStep, double fEndValue ) throw( css::uno::RuntimeException );	 

	void ClearContents( sal_Int32 nFlags ) throw (css::uno::RuntimeException);
	
public:
	ScVbaRange( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::table::XCellRange >& xRange, sal_Bool bIsRows = false, sal_Bool bIsColumns = false ) throw ( css::lang::IllegalArgumentException );

	virtual ~ScVbaRange();

	bool isSingleCellRange(); 

	static css::uno::Reference< css::table::XCellRange > getCellRangeForName( const rtl::OUString& sRangeName, const css::uno::Reference< css::sheet::XSpreadsheet >& xDoc );
    // Attributes
	virtual css::uno::Any SAL_CALL getValue() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setValue( const css::uno::Any& aValue ) throw ( css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getFormula() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormula( const ::rtl::OUString &rFormula ) throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getFormulaR1C1() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormulaR1C1( const ::rtl::OUString &rFormula ) throw (css::uno::RuntimeException);
	virtual double SAL_CALL getCount() throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getRow() throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getColumn() throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getText() throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getFormulaArray() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormulaArray(const ::rtl::OUString &rFormula) throw (css::uno::RuntimeException);
	virtual void SAL_CALL setNumberFormat( const ::rtl::OUString &rNumberFormat ) throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getNumberFormat() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setMergeCells( sal_Bool bMerge ) throw (css::uno::RuntimeException);
	virtual sal_Bool SAL_CALL getMergeCells() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setWrapText( sal_Bool bIsWrapped ) throw (css::uno::RuntimeException);
	virtual sal_Bool SAL_CALL getWrapText() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL getEntireRow() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL getEntireColumn() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XComment > SAL_CALL getComment() throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getHidden() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setHidden( const css::uno::Any& _hidden ) throw (css::uno::RuntimeException);
	// Methods
	sal_Bool IsRows() { return mbIsRows; }
	sal_Bool IsColumns() { return mbIsColumns; }
	virtual void SAL_CALL Clear() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearComments() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearContents() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearFormats() throw (css::uno::RuntimeException);
	virtual sal_Bool SAL_CALL HasFormula() throw (css::uno::RuntimeException);
	virtual void SAL_CALL FillLeft() throw (css::uno::RuntimeException);
	virtual void SAL_CALL FillRight() throw (css::uno::RuntimeException);
	virtual void SAL_CALL FillUp() throw (css::uno::RuntimeException);
	virtual void SAL_CALL FillDown() throw (css::uno::RuntimeException);
	virtual	css::uno::Reference< oo::vba::XRange > SAL_CALL Offset( const css::uno::Any &nRowOffset, const css::uno::Any &nColOffset )
														   throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL CurrentRegion() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL CurrentArray() throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL Characters( const css::uno::Any& nIndex, const css::uno::Any& nCount ) 
												 throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL Address() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Cells( const css::uno::Any &nRow, const css::uno::Any &nCol ) 
														  throw (css::uno::RuntimeException);
	virtual void SAL_CALL Select() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange >  SAL_CALL Rows( const css::uno::Any& nIndex ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange >  SAL_CALL Columns( const css::uno::Any &nIndex ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Copy( const css::uno::Any& Destination ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Cut( const css::uno::Any& Destination ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Resize( const css::uno::Any& RowSize, const css::uno::Any& ColumnSize ) 
														   throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XFont > SAL_CALL Font() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XInterior > SAL_CALL Interior(  ) throw (css::uno::RuntimeException) ;
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Range( const css::uno::Any &Cell1, const css::uno::Any &Cell2 )
															throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getCellRange(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL PasteSpecial( sal_Int16 Paste, sal_Int16 Operation, ::sal_Bool SkipBlanks, ::sal_Bool Transpose ) throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL Replace( const ::rtl::OUString& What, const ::rtl::OUString& Replacement, const css::uno::Any& LookAt, const css::uno::Any& SearchOrder, const css::uno::Any& MatchCase, const css::uno::Any& MatchByte, const css::uno::Any& SearchFormat, const css::uno::Any& ReplaceFormat ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Sort( const css::uno::Any& Key1, const css::uno::Any& Order1, const css::uno::Any& Key2, const css::uno::Any& Type, const css::uno::Any& Order2, const css::uno::Any& Key3, const css::uno::Any& Order3, const css::uno::Any& Header, const css::uno::Any& OrderCustom, const css::uno::Any& MatchCase, const css::uno::Any& Orientation, const css::uno::Any& SortMethod,  const css::uno::Any& DataOption1, const css::uno::Any& DataOption2, const css::uno::Any& DataOption3 ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL End( ::sal_Int32 Direction )  throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XCharacters > SAL_CALL characters( const css::uno::Any& Start, const css::uno::Any& Length ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Delete( const css::uno::Any& Shift ) throw (css::uno::RuntimeException);

	// XPropertySet

	virtual css::uno::Reference< css::beans::XPropertySetInfo > SAL_CALL getPropertySetInfo(  ) throw (css::uno::RuntimeException);  

	// XInterface
	DECLARE_XINTERFACE()

	// XTypeProvider
	DECLARE_XTYPEPROVIDER()

	// XEnumerationAccess
	virtual css::uno::Reference< css::container::XEnumeration > SAL_CALL createEnumeration() throw (css::uno::RuntimeException);

	// XElementAccess
	virtual css::uno::Type SAL_CALL getElementType() throw (css::uno::RuntimeException)
	{
		return oo::vba::XRange::static_type(0);

	}
	virtual sal_Bool SAL_CALL hasElements() throw (css::uno::RuntimeException);
	// XDefaultMethod
	::rtl::OUString SAL_CALL getName(  ) throw (css::uno::RuntimeException);

protected:
	// OPropertySetHelper
	virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

	virtual ::cppu::IPropertyArrayHelper* createArrayHelper() const;
	

};

#endif /* SC_VBA_RANGE_HXX */

