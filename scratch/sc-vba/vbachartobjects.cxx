/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  last change: $Author$ $Date$
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/
#include "vbachart.hxx"
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/document/XEmbeddedObjectSupplier.hpp>
#include <com/sun/star/table/XTableChartsSupplier.hpp>
#include <com/sun/star/table/XTableChart.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <org/openoffice/excel/XlChartType.hpp>


#include "vbachartobjects.hxx"
#include "vbachartobject.hxx"
#include "vbaglobals.hxx"
#include "cellsuno.hxx"
#include <vector>
#include <basic/sberrors.hxx>

using namespace ::com::sun::star;
using namespace ::org::openoffice;


class ChartObjectEnumerationImpl : public EnumerationHelperImpl
{
	uno::Reference< drawing::XDrawPageSupplier > xDrawPageSupplier;
	uno::Reference< vba::XHelperInterface > xParent;
	
public:

	ChartObjectEnumerationImpl( const uno::Reference< uno::XComponentContext >& xContext, const uno::Reference< container::XEnumeration >& xEnumeration, const uno::Reference< drawing::XDrawPageSupplier >& _xDrawPageSupplier, const uno::Reference< vba::XHelperInterface >& _xParent ) throw ( uno::RuntimeException ) : EnumerationHelperImpl( xContext, xEnumeration ), xDrawPageSupplier( _xDrawPageSupplier ), xParent( _xParent ) {}
	virtual uno::Any SAL_CALL nextElement(  ) throw (container::NoSuchElementException, lang::WrappedTargetException, uno::RuntimeException) 
	{ 
		uno::Reference< table::XTableChart > xTableChart( m_xEnumeration->nextElement(), uno::UNO_QUERY_THROW );
		// parent Object is sheet
		return uno::makeAny(  uno::Reference< excel::XChartObject > ( new ScVbaChartObject(  xParent, m_xContext, xTableChart, xDrawPageSupplier ) ) );
	}
};


ScVbaChartObjects::ScVbaChartObjects( const css::uno::Reference< oo::vba::XHelperInterface >& _xParent, const css::uno::Reference< css::uno::XComponentContext >& _xContext, const css::uno::Reference< css::table::XTableCharts >& _xTableCharts, const uno::Reference< drawing::XDrawPageSupplier >& _xDrawPageSupplier ) : ChartObjects_BASE(_xParent, _xContext, css::uno::Reference< css::container::XIndexAccess >( _xTableCharts, css::uno::UNO_QUERY ) ), xTableCharts( _xTableCharts ) , xDrawPageSupplier( _xDrawPageSupplier )
{

}

void
ScVbaChartObjects::removeByName(const rtl::OUString& _sChartName)
{
	xTableCharts->removeByName( _sChartName );
}

uno::Sequence< rtl::OUString >
ScVbaChartObjects::getChartObjectNames() throw( css::script::BasicErrorException )
{
	uno::Sequence< rtl::OUString > sChartNames;
	try
	{
		// c++ hackery
		uno::Reference< uno::XInterface > xIf( xDrawPageSupplier, uno::UNO_QUERY_THROW );
		ScCellRangesBase* pUno= dynamic_cast< ScCellRangesBase* >( xIf.get() );
		ScDocShell* pDocShell = NULL;
		if ( !pUno )
			throw uno::RuntimeException( rtl::OUString::createFromAscii("Failed to obtain the impl class from the drawpage"), uno::Reference< uno::XInterface >() );
		pDocShell = pUno->GetDocShell();	
		if ( !pDocShell )
			throw uno::RuntimeException( rtl::OUString::createFromAscii("Failed to obtain the docshell implclass"), uno::Reference< uno::XInterface >() );
			
		uno::Reference< sheet::XSpreadsheetDocument > xSpreadsheetDocument( pDocShell->GetModel(), uno::UNO_QUERY_THROW );
		uno::Reference< sheet::XSpreadsheets > xSpreadsheets = xSpreadsheetDocument->getSheets();
		std::vector< rtl::OUString > aChartNamesVector;

		uno::Sequence< rtl::OUString > sSheetNames = xSpreadsheets->getElementNames();
		sal_Int32 nItems = sSheetNames.getLength();
		for (sal_Int32 i = 0; i < nItems; i++)
		{
			uno::Reference< table::XTableChartsSupplier > xLocTableChartsSupplier( xSpreadsheets->getByName(sSheetNames[i]), uno::UNO_QUERY_THROW );
			uno::Sequence< rtl::OUString > scurchartnames = xLocTableChartsSupplier->getCharts()->getElementNames();
			sal_Int32 nChartNames = scurchartnames.getLength();
			for (sal_Int32 n = 0; n < nChartNames; n++ )
				aChartNamesVector.push_back(scurchartnames[n]);
		}
		sChartNames.realloc( aChartNamesVector.size() );
		std::vector< rtl::OUString > ::const_iterator it = aChartNamesVector.begin();
		std::vector< rtl::OUString > ::const_iterator it_end = aChartNamesVector.end();
		for ( sal_Int32 index = 0 ; it != it_end; ++it, ++index )
			sChartNames[index] = *it;
	} 
	catch (uno::Exception& ) 
	{
		throw script::BasicErrorException( rtl::OUString(), uno::Reference< uno::XInterface >(), SbERR_METHOD_FAILED, rtl::OUString() );
	}
	return sChartNames;
}

// XChartObjects
uno::Any SAL_CALL 
ScVbaChartObjects::Add( double _nX, double _nY, double _nWidth, double _nHeight ) throw (script::BasicErrorException)
{
	try 
	{
		uno::Sequence< table::CellRangeAddress > aCellRangeAddress( 1 );
		awt::Rectangle aRectangle;
		aRectangle.X =  Millimeter::getInHundredthsOfOneMillimeter(_nX);
		aRectangle.Y = Millimeter::getInHundredthsOfOneMillimeter(_nY);
		aRectangle.Width = Millimeter::getInHundredthsOfOneMillimeter(_nWidth);
		aRectangle.Height = Millimeter::getInHundredthsOfOneMillimeter(_nHeight); 
		// Note the space at the end of the stem ("Chart "). In ChartSheets only "Chart" is the stem
		rtl::OUString sPersistChartName = ContainerUtilities::getUniqueName( getChartObjectNames(), rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Chart " ) ) , rtl::OUString(), 1);
		xTableCharts->addNewByName(sPersistChartName, aRectangle, aCellRangeAddress, true, false );
		uno::Reference< excel::XChartObject > xChartObject( getItemByStringIndex( sPersistChartName ), uno::UNO_QUERY_THROW ); 
		xChartObject->getChart()->setChartType(excel::XlChartType::xlColumnClustered);
		return uno::makeAny( xChartObject );
	} 
	catch ( uno::Exception& ex) 
	{
		OSL_TRACE("AddItem caught exception ->%s", rtl::OUStringToOString( ex.Message, RTL_TEXTENCODING_UTF8 ).getStr() );
	}
	return aNULL();
}
void SAL_CALL ScVbaChartObjects::Delete(  ) throw (script::BasicErrorException)
{
	uno::Sequence< rtl::OUString > sChartNames = xTableCharts->getElementNames();
	sal_Int32 ncount = sChartNames.getLength();
	for (sal_Int32 i = 0; i < ncount ; i++)
		removeByName(sChartNames[i]);
}

// XEnumerationAccess

uno::Reference< container::XEnumeration >
ScVbaChartObjects::createEnumeration() throw (uno::RuntimeException)
{
	css::uno::Reference< container::XEnumerationAccess > xEnumAccess( xTableCharts, uno::UNO_QUERY_THROW );
	return new ChartObjectEnumerationImpl( mxContext, xEnumAccess->createEnumeration(), xDrawPageSupplier, getParent() /* sheet */);
}

// XElementAccess

uno::Type 
ScVbaChartObjects::getElementType() throw (uno::RuntimeException)
{
	return excel::XChartObject::static_type(0);
}

// ScVbaCollectionBaseImpl
uno::Any
ScVbaChartObjects::createCollectionObject( const css::uno::Any& aSource )
{	
	uno::Reference< table::XTableChart > xTableChart( aSource, uno::UNO_QUERY_THROW );
	// correct parent object is sheet
	return uno::makeAny( uno::Reference< excel::XChartObject > ( new ScVbaChartObject( getParent(), mxContext, xTableChart, xDrawPageSupplier ) ) );
}

rtl::OUString& 
ScVbaChartObjects::getServiceImplName()
{
	static rtl::OUString sImplName( RTL_CONSTASCII_USTRINGPARAM("ScVbaChartObjects") );
	return sImplName;
}

css::uno::Sequence<rtl::OUString> 
ScVbaChartObjects::getServiceNames()
{
	static uno::Sequence< rtl::OUString > sNames;
	if ( sNames.getLength() == 0 )
	{
		sNames.realloc( 1 );
		sNames[0] = rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("org.openoffice.excel.ChartObjects") );
	}
	return sNames;
}

