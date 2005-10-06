#include <comphelper/processfactory.hxx>

#include <com/sun/star/frame/XDesktop.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <org/openoffice/vba/XApplication.hpp>

#include <tools/string.hxx>

#include "vbahelper.hxx"
#include "vbadialog.hxx"
#include "vbaglobals.hxx"

using namespace ::org::openoffice;
using namespace ::org::openoffice::vba;
using namespace ::com::sun::star;

void
ScVbaDialog::Show() throw(uno::RuntimeException)
{
	rtl::OUString aURL;
	uno::Reference< lang::XMultiComponentFactory > xSMgr( m_xContext->getServiceManager(), uno::UNO_QUERY_THROW );
	uno::Reference< frame::XDesktop > xDesktop(
		xSMgr->createInstanceWithContext(::rtl::OUString::createFromAscii("com.sun.star.frame.Desktop"), m_xContext),
							uno::UNO_QUERY_THROW );
                                                                                                                             
        uno::Reference< frame::XModel > xModel( xDesktop->getCurrentComponent(),uno::UNO_QUERY );

	aURL = mapIndexToName( mnIndex );
	if( aURL == rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("") ) )
		throw uno::RuntimeException(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( " Unable to open the specified dialog " ) ),
			uno::Reference< XInterface > () );
	dispatchRequests( xModel, aURL );
}	

uno::Any
ScVbaDialog::getParent() throw (uno::RuntimeException)
{
	uno::Reference< vba::XGlobals > xGlobals = ScVbaGlobals::getGlobalsImpl( m_xContext );
	uno::Reference< vba::XApplication > xApplication = xGlobals->getApplication();
	if ( !xApplication.is() )
	{
		throw uno::RuntimeException(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "ScVbaWorkbooks::getParent: Couldn't access Application				object") ),uno::Reference< XInterface >() );
	}
	return uno::Any(xApplication);
}
                                                                                                                             
::sal_Int32
ScVbaDialog::getCreator() throw (uno::RuntimeException)
{
	SC_VBA_STUB();
	return 0;
}
uno::Reference< vba::XApplication >
ScVbaDialog::getApplication() throw (uno::RuntimeException)
{
	uno::Reference< vba::XApplication > xApplication =  ScVbaGlobals::getGlobalsImpl( m_xContext )->getApplication();
	if ( !xApplication.is() )
	{
		throw uno::RuntimeException(
		::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "ScVbaWorkbooks::getParent: Couldn't access Application object" ) ),
		uno::Reference< XInterface >() );
	}
	return xApplication;
}

static const rtl::OUString aStringList[]=
{
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:Open" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:FormatCellDialog" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:InsertCell" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:Print" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:PasteSpecial" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:ToolProtectionDocument" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:ColumnWidth" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DefineName" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:ConfigureDialog" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:HyperlinkDialog" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:InsertGraphic" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:InsertObject" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:PageFormatDialog" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DataSort" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:RowHeight" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:AutoCorrectDlg" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:ConditionalFormatDialog" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DataConsolidate" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:CreateNames" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:FillSeries" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:Validation") ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DefineLabelRange" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DataFilterAutoFilter" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:DataFilterSpecialFilter" ) ),
	rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".uno:AutoFormat" ) )	
};

const sal_Int32 nSize = sizeof( aStringList ) / sizeof( aStringList[ 0 ] );

rtl::OUString 
ScVbaDialog::mapIndexToName( sal_Int32 nIndex )
{
	if( nIndex < nSize )
		return aStringList[ nIndex ];
	return rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("") );
}

