#ifndef SC_VBA_RANGE_HXX
#define SC_VBA_RANGE_HXX

#include <cppuhelper/implbase3.hxx>
#include <com/sun/star/container/XEnumerationAccess.hpp>

#include <org/openoffice/vba/XRange.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <org/openoffice/vba/XFont.hpp>
#include <org/openoffice/vba/XComment.hpp>
#include <org/openoffice/vba/XCollection.hpp>
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
#include <com/sun/star/sheet/XSheetCellRangeContainer.hpp>

#include "vbahelper.hxx"

class ScTableSheetsObj;
class ScCellRangesBase;
typedef ::cppu::WeakImplHelper3< oo::vba::XRange, css::container::XEnumerationAccess, css::script::XDefaultMethod > ScVbaRange_BASE;
class ArrayVisitor
{
public:
	virtual void visitNode( sal_Int32 x, sal_Int32 y, const css::uno::Reference< css::table::XCell >& xCell ) = 0;
};

class ValueSetter : public ArrayVisitor
{
public:
	virtual bool processValue( const css::uno::Any& aValue, const css::uno::Reference< css::table::XCell >& xCell ) = 0;


};

class ValueGetter : public ArrayVisitor
{
	
public:
	virtual void processValue( sal_Int32 x, sal_Int32 y, const css::uno::Any& aValue ) = 0;
	virtual const css::uno::Any& getValue() const = 0;
};



class ScVbaRange : public ScVbaRange_BASE
    ,public ::comphelper::OMutexAndBroadcastHelper
    ,public ::comphelper::OPropertyContainer

{
	css::uno::Reference< oo::vba::XCollection > m_Areas;
	css::uno::Reference< oo::vba::XCollection > m_Borders;
	css::uno::Reference< css::table::XCellRange > mxRange;
	css::uno::Reference< css::uno::XComponentContext > m_xContext;
	css::uno::Reference< css::sheet::XSheetCellRangeContainer > mxRanges;
	sal_Bool mbIsRows;
	sal_Bool mbIsColumns;
	rtl::OUString msDftPropName;
	double getCalcColWidth( const css::table::CellRangeAddress& ) throw (css::uno::RuntimeException);
	double getCalcRowHeight( const css::table::CellRangeAddress& ) throw (css::uno::RuntimeException);
	void visitArray( ArrayVisitor& vistor );
	css::uno::Reference< css::script::XTypeConverter > getTypeConverter() throw (css::uno::RuntimeException);

	css::uno::Reference< oo::vba::XRange > getEntireColumnOrRow( bool bColumn = true ) throw( css::uno::RuntimeException );

	void fillSeries(  css::sheet::FillDirection nFillDirection, css::sheet::FillMode nFillMode, css::sheet::FillDateMode nFillDateMode, double fStep, double fEndValue ) throw( css::uno::RuntimeException );	 

	void ClearContents( sal_Int32 nFlags ) throw (css::uno::RuntimeException);
	virtual void   setValue( const css::uno::Any& aValue, ValueSetter& setter) throw ( css::uno::RuntimeException);
	virtual css::uno::Any getValue( ValueGetter& rValueGetter ) throw (css::uno::RuntimeException);
	css::uno::Reference< oo::vba::XRange > getArea( sal_Int32 nIndex  ) throw( css::uno::RuntimeException );
	void setDfltPropHandler();
	ScCellRangesBase* getCellRangesBase() throw ( css::uno::RuntimeException );
	SfxItemSet* getCurrentDataSet( )  throw ( css::uno::RuntimeException );

public:
	ScVbaRange( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::table::XCellRange >& xRange, sal_Bool bIsRows = false, sal_Bool bIsColumns = false ) throw ( css::lang::IllegalArgumentException );
	ScVbaRange( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::sheet::XSheetCellRangeContainer >& xRanges, sal_Bool bIsRows = false, sal_Bool bIsColumns = false ) throw ( css::lang::IllegalArgumentException );

	virtual ~ScVbaRange();

	bool isSingleCellRange(); 

        static css::uno::Reference< css::table::XCellRange > getCellRangeForName( const rtl::OUString& sRangeName, const css::uno::Reference< css::sheet::XSpreadsheet >& xDoc );
    // Attributes
	virtual css::uno::Any SAL_CALL getValue() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setValue( const css::uno::Any& aValue ) throw ( css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getFormula() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormula( const css::uno::Any& rFormula ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getFormulaArray() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormulaArray(const css::uno::Any& rFormula) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getFormulaR1C1() throw (css::uno::RuntimeException);
	virtual void   SAL_CALL setFormulaR1C1( const css::uno::Any &rFormula ) throw (css::uno::RuntimeException);
	virtual double SAL_CALL getCount() throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getRow() throw (css::uno::RuntimeException);
	virtual ::sal_Int32 SAL_CALL getColumn() throw (css::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getText() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setNumberFormat( const css::uno::Any& rNumberFormat ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getNumberFormat() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setMergeCells( sal_Bool bMerge ) throw (css::uno::RuntimeException);
	virtual sal_Bool SAL_CALL getMergeCells() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setWrapText( const css::uno::Any& bIsWrapped ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getWrapText() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL getEntireRow() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL getEntireColumn() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XComment > SAL_CALL getComment() throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getHidden() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setHidden( const css::uno::Any& _hidden ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getColumnWidth() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setColumnWidth( const css::uno::Any& _columnwidth ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getRowHeight() throw (css::uno::RuntimeException);
	virtual void SAL_CALL setRowHeight( const css::uno::Any& _rowheight ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getWidth() throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getHeight() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XWorksheet > SAL_CALL getWorksheet() throw (css::uno::RuntimeException);
	// Methods
	sal_Bool IsRows() { return mbIsRows; }
	sal_Bool IsColumns() { return mbIsColumns; }
	virtual css::uno::Reference< oo::vba::XComment > SAL_CALL AddComment( const css::uno::Any& Text ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Clear() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearComments() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearContents() throw (css::uno::RuntimeException);
	virtual void SAL_CALL ClearFormats() throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL HasFormula() throw (css::uno::RuntimeException);
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

	virtual ::rtl::OUString SAL_CALL Address( const css::uno::Any& RowAbsolute, const css::uno::Any& ColumnAbsolute, const css::uno::Any& ReferenceStyle, const css::uno::Any& External, const css::uno::Any& RelativeTo ) throw (css::uno::RuntimeException);

	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Cells( const css::uno::Any &nRow, const css::uno::Any &nCol ) 
														  throw (css::uno::RuntimeException);
	virtual void SAL_CALL Select() throw (css::uno::RuntimeException);
	virtual void SAL_CALL Activate() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange >  SAL_CALL Rows( const css::uno::Any& nIndex ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange >  SAL_CALL Columns( const css::uno::Any &nIndex ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Copy( const css::uno::Any& Destination ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Cut( const css::uno::Any& Destination ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Resize( const css::uno::Any& RowSize, const css::uno::Any& ColumnSize ) 
														   throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XFont > SAL_CALL Font() throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XInterior > SAL_CALL Interior(  ) throw (css::uno::RuntimeException) ;
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL Range( const css::uno::Any &Cell1, const css::uno::Any &Cell2 ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > Range( const css::uno::Any &Cell1, const css::uno::Any &Cell2, bool bForceUseInpuRangeTab ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getCellRange(  ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL PasteSpecial( const css::uno::Any& Paste, const css::uno::Any& Operation, const css::uno::Any& SkipBlanks, const css::uno::Any& Transpose ) throw (css::uno::RuntimeException);
	virtual ::sal_Bool SAL_CALL Replace( const ::rtl::OUString& What, const ::rtl::OUString& Replacement, const css::uno::Any& LookAt, const css::uno::Any& SearchOrder, const css::uno::Any& MatchCase, const css::uno::Any& MatchByte, const css::uno::Any& SearchFormat, const css::uno::Any& ReplaceFormat ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Sort( const css::uno::Any& Key1, const css::uno::Any& Order1, const css::uno::Any& Key2, const css::uno::Any& Type, const css::uno::Any& Order2, const css::uno::Any& Key3, const css::uno::Any& Order3, const css::uno::Any& Header, const css::uno::Any& OrderCustom, const css::uno::Any& MatchCase, const css::uno::Any& Orientation, const css::uno::Any& SortMethod,  const css::uno::Any& DataOption1, const css::uno::Any& DataOption2, const css::uno::Any& DataOption3 ) throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XRange > SAL_CALL End( ::sal_Int32 Direction )  throw (css::uno::RuntimeException);
	virtual css::uno::Reference< oo::vba::XCharacters > SAL_CALL characters( const css::uno::Any& Start, const css::uno::Any& Length ) throw (css::uno::RuntimeException);
	virtual void SAL_CALL Delete( const css::uno::Any& Shift ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL Areas( const css::uno::Any& ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL Borders( const css::uno::Any& ) throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL BorderAround( const css::uno::Any& LineStyle, 
                const css::uno::Any& Weight, const css::uno::Any& ColorIndex, const css::uno::Any& Color ) throw (css::uno::RuntimeException);

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

// #TODO completely rewrite ScVbaRange, its become a hackfest
// it needs to be closer to ScCellRangeBase in that the underlying
// object model should probably be a ScRangelst. 
//     * would be nice to be able to construct a range from an addres only
//     * or a list of address ( multi-area )
//     * object should be a lightweight as possible
//     * we shouldn't need hacks like this below
	static css::uno::Reference< oo::vba::XRange > ApplicationRange( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Any &Cell1, const css::uno::Any &Cell2 ) throw (css::uno::RuntimeException);

protected:
	// OPropertySetHelper
	virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

	virtual ::cppu::IPropertyArrayHelper* createArrayHelper() const;

};
#endif /* SC_VBA_RANGE_HXX */

