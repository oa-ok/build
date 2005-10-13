#include <comphelper/processfactory.hxx>

#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/container/XEnumerationAccess.hpp>
#include <com/sun/star/sheet/XSpreadsheetView.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/lang/IndexOutOfBoundsException.hpp>

#include <org/openoffice/vba/XApplication.hpp>

#include <tools/string.hxx>

#include "vbaglobals.hxx"
#include "vbaworksheet.hxx"
#include "vbaworksheets.hxx"
#include "vbaworkbook.hxx"

using namespace ::org::openoffice;
using namespace ::com::sun::star;

ScVbaWorksheets::ScVbaWorksheets(uno::Reference< ::com::sun::star::uno::XComponentContext > & xContext, uno::Reference< frame::XModel > xModel ):
		mxModel( xModel ), m_xContext( xContext ) 
{ 
	uno::Reference <sheet::XSpreadsheetDocument> xSpreadDoc( mxModel, uno::UNO_QUERY );
        if ( !xSpreadDoc.is() )
        {
		throw uno::RuntimeException(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ": Couldn't obtain XSpreadsheetDocument interface from XModel" ) ), 
			uno::Reference< uno::XInterface >() );
	}
       mxSheets = xSpreadDoc->getSheets();
}

// XEnumerationAccess
uno::Type 
ScVbaWorksheets::getElementType() throw (uno::RuntimeException)
{
	return vba::XWorksheet::static_type(0);
}
::sal_Bool
ScVbaWorksheets::hasElements() throw (uno::RuntimeException)
{
	return sal_True;
}
uno::Reference< container::XEnumeration >
ScVbaWorksheets::createEnumeration() throw (uno::RuntimeException)
{
	return uno::Reference< container::XEnumeration >( NULL );
}

// XCollection
uno::Any
ScVbaWorksheets::getParent() throw (uno::RuntimeException)
{
	OSL_TRACE("In ScVbaWorksheets::getParent()");
	uno::Reference< vba::XApplication > xApplication =
		ScVbaGlobals::getGlobalsImpl( m_xContext )->getApplication();
	uno::Reference< vba::XWorkbook > xWorkbook;
	if ( xApplication.is() )
	{
		xWorkbook = xApplication->getActiveWorkbook();
	}
	if ( !xWorkbook.is() )
	{
		//throw uno::RuntimeException( rtl::OUString::createFromAscii(
		//	"ScVbaWorksheets::getParent - No Parent" ), uno::Reference< uno::XInterface >() );
	}
	
	OSL_TRACE("In ScVbaWorksheets::getParent() returning workbook");
	return uno::Any( xWorkbook );
}
::sal_Int32
ScVbaWorksheets::getCreator() throw (uno::RuntimeException)
{
	SC_VBA_STUB();
	return 0;
}
uno::Reference< vba::XApplication >
ScVbaWorksheets::getApplication() throw (uno::RuntimeException)
{
	return ScVbaGlobals::getGlobalsImpl( m_xContext )->getApplication();
}

::sal_Int32
ScVbaWorksheets::getCount() throw (uno::RuntimeException)
{
	uno::Reference <container::XIndexAccess> xIndex( mxSheets, uno::UNO_QUERY );
	if ( xIndex.is() )
	{
		return xIndex->getCount();
	}
	return 0;
}

uno::Any
ScVbaWorksheets::getItemByStringIndex( const rtl::OUString& sIndex ) throw (uno::RuntimeException)
{
	uno::Reference <container::XNameAccess> xName( mxSheets, uno::UNO_QUERY );
	uno::Reference< vba::XWorksheet > result;

	if ( xName.is() )
	{
		uno::Reference< sheet::XSpreadsheet > xSheet(xName->getByName( sIndex ), uno::UNO_QUERY);
		result =  new ScVbaWorksheet( m_xContext, xSheet, mxModel);
	}
	else
	{
		throw uno::RuntimeException(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( 
			": Unable to obtain XNameAccess to query for spreadsheet name" ) ), 
			uno::Reference< uno::XInterface >() );
	}

	return uno::Any( result );
}

uno::Any
ScVbaWorksheets::getItemByIntIndex( const sal_Int32 nIndex ) throw (uno::RuntimeException)
{
	if ( nIndex <= 0 )
	{
		throw  lang::IndexOutOfBoundsException( 
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( 
			"index is 0 or negative" ) ), 
			uno::Reference< uno::XInterface >() );
	}

	uno::Reference <container::XIndexAccess> xIndex( mxSheets, uno::UNO_QUERY );

	if ( !xIndex.is() )
	{
		throw uno::RuntimeException(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( 
			": Unable to obtain XIndexAccess to query for spreadsheet name" ) ), 
			uno::Reference< uno::XInterface >() );
	}
	// need to adjust for vba index ( for which first element is 1 )
	uno::Reference< sheet::XSpreadsheet > xSheet(xIndex->getByIndex( nIndex - 1 ), uno::UNO_QUERY);
	uno::Reference< vba::XWorksheet > result =  new ScVbaWorksheet( m_xContext, xSheet, mxModel);
	
	return uno::Any( result );
}

uno::Any
ScVbaWorksheets::Item( const uno::Any& aIndex ) throw (uno::RuntimeException)
{
	if ( aIndex.getValueTypeClass() != uno::TypeClass_STRING )
	{
		sal_Int32 nIndex = 0;

		if ( ( aIndex >>= nIndex ) != sal_True )
		{
			rtl::OUString message;
			message = rtl::OUString::createFromAscii(
				"Couldn't convert index to Int32");
			throw  lang::IndexOutOfBoundsException( message,
				uno::Reference< uno::XInterface >() );
		}
		return 	getItemByIntIndex( nIndex );
	}
	rtl::OUString aStringSheet;

	aIndex >>= aStringSheet;
	return getItemByStringIndex( aStringSheet );

}

// XWorksheets
uno::Any
ScVbaWorksheets::Add( const uno::Any& Before, const uno::Any& After,
					 const uno::Any& Count, const uno::Any& Type ) throw (uno::RuntimeException)
{
	rtl::OUString aStringSheet;
	sal_Bool bBefore(sal_True);
	sal_Int32 nSheetIndex = 0;
	sal_Int32 nNewSheets = 1, nType = 0;
	Count >>= nNewSheets;
	Type >>= nType;
	sal_Int32 nCount = 0;
	Before >>= aStringSheet;
	if (aStringSheet == NULL)
	{
		After >>= aStringSheet;
		bBefore = sal_False;
	}
	if (aStringSheet == NULL)
	{
		aStringSheet = ScVbaGlobals::getGlobalsImpl( 
			m_xContext )->getApplication()->getActiveWorkbook()->getActiveSheet()->getName();
		bBefore = sal_True;
	}
	uno::Reference <container::XIndexAccess> xIndex( mxSheets, uno::UNO_QUERY );
	if ( xIndex.is() )
	{
		nCount = xIndex->getCount();
		for (sal_Int32 i=0; i < nCount; i++)
		{
			uno::Reference< sheet::XSpreadsheet > xSheet(xIndex->getByIndex(i), uno::UNO_QUERY);
			uno::Reference< container::XNamed > xNamed( xSheet, uno::UNO_QUERY_THROW );
			if (xNamed->getName() == aStringSheet)
			{
				nSheetIndex = i;
				break;
			}
		}
	}

	if(!bBefore)
		nSheetIndex++;

	uno::Reference< container::XNameAccess > xNameAccess( mxSheets, uno::UNO_QUERY_THROW );
	sal_Int32 nSheetName = nCount + 1L;
	String aStringBase( RTL_CONSTASCII_USTRINGPARAM("Sheet") );				
	for (sal_Int32 i=0; i < nNewSheets; i++, nSheetName++)
	{
		String aStringName = aStringBase;
		aStringName += String::CreateFromInt32(nSheetName);
		while (xNameAccess->hasByName(aStringName))
		{
			nSheetName++;
			aStringName = aStringBase;
			aStringName += String::CreateFromInt32(nSheetName);
		}
		mxSheets->insertNewByName(aStringName, nSheetIndex + i);
	}
	return uno::Any();
}

void
ScVbaWorksheets::Delete() throw (uno::RuntimeException)
{
	//SC_VBA_STUB();
}
