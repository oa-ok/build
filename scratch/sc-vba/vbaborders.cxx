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
#include "vbaborders.hxx"

#include <cppuhelper/implbase3.hxx>
#include <org/openoffice/excel/XlBordersIndex.hpp>
#include <org/openoffice/excel/XlBorderWeight.hpp>
#include <org/openoffice/excel/XlLineStyle.hpp>
#include <org/openoffice/excel/XlColorIndex.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/table/TableBorder.hpp>

#include "vbapalette.hxx"

using namespace ::com::sun::star;
using namespace ::org::openoffice;
using namespace ::org::openoffice::excel;


typedef ::cppu::WeakImplHelper1<container::XIndexAccess > RangeBorders_Base;
typedef ::cppu::WeakImplHelper1<excel::XBorder > ScVbaBorder_Base;

// #TODO sort these indexes to match the order in which Excel iterates over the
// borders, the enumeration will match the order in this list
static const sal_Int16 supportedIndexTable[] = {  XlBordersIndex::xlEdgeLeft, XlBordersIndex::xlEdgeTop, XlBordersIndex::xlEdgeBottom, XlBordersIndex::xlEdgeRight, XlBordersIndex::xlDiagonalDown, XlBordersIndex::xlDiagonalUp, XlBordersIndex::xlInsideVertical, XlBordersIndex::xlInsideHorizontal };

const static rtl::OUString sTableBorder( RTL_CONSTASCII_USTRINGPARAM("TableBorder") );

//  Equiv widths in in 1/100 mm
const static sal_Int32 OOLineThin = 35;
const static sal_Int32 OOLineMedium = 88;
const static sal_Int32 OOLineThick = 141;
const static sal_Int32 OOLineHairline = 2;

class ScVbaBorder : public ScVbaBorder_Base
{
private:
	uno::Reference< beans::XPropertySet > m_xProps;
	uno::Reference< uno::XComponentContext > m_xContext;
	sal_Int32 m_LineType;	
	ScVbaPalette m_Palette;
	bool setBorderLine( table::BorderLine& rBorderLine )
	{
		table::TableBorder aTableBorder;
		m_xProps->getPropertyValue( sTableBorder ) >>= aTableBorder;

		switch ( m_LineType )
		{
			case XlBordersIndex::xlEdgeLeft:
				aTableBorder.IsLeftLineValid = sal_True;
				aTableBorder.LeftLine= rBorderLine;
				break;
			case XlBordersIndex::xlEdgeTop:
				aTableBorder.IsTopLineValid = sal_True;
				aTableBorder.TopLine = rBorderLine;
				break;
			
			case XlBordersIndex::xlEdgeBottom:
				aTableBorder.IsBottomLineValid = sal_True;
				aTableBorder.BottomLine = rBorderLine;
				break;
			case XlBordersIndex::xlEdgeRight:
				aTableBorder.IsRightLineValid = sal_True;
				aTableBorder.RightLine = rBorderLine;
				break;
			case XlBordersIndex::xlInsideVertical:
				aTableBorder.IsVerticalLineValid = sal_True;
				aTableBorder.VerticalLine = rBorderLine;
				break;
			case XlBordersIndex::xlInsideHorizontal:
				aTableBorder.IsHorizontalLineValid = sal_True;
				aTableBorder.HorizontalLine = rBorderLine;
				break;
			case XlBordersIndex::xlDiagonalDown:
			case XlBordersIndex::xlDiagonalUp:
				// #TODO have to ignore at the momement, would be
				// nice to investigate what we can do here
				break; 
			default:
					return false;
		}
		m_xProps->setPropertyValue( sTableBorder, uno::makeAny(aTableBorder) );
		return true;
	}	

	bool getBorderLine( table::BorderLine& rBorderLine )
	{
		table::TableBorder aTableBorder;
		m_xProps->getPropertyValue( sTableBorder ) >>= aTableBorder;
		switch ( m_LineType )
		{
			case XlBordersIndex::xlEdgeLeft:
				if ( aTableBorder.IsLeftLineValid )
					rBorderLine = aTableBorder.LeftLine;
				break;
			case XlBordersIndex::xlEdgeTop:
				if ( aTableBorder.IsTopLineValid )
					rBorderLine = aTableBorder.TopLine;
				break;
			
			case XlBordersIndex::xlEdgeBottom:
				if ( aTableBorder.IsBottomLineValid )
					rBorderLine = aTableBorder.BottomLine;
				break;
			case XlBordersIndex::xlEdgeRight:
				if ( aTableBorder.IsRightLineValid )
					rBorderLine = aTableBorder.RightLine;
				break;
			case XlBordersIndex::xlInsideVertical:
                if ( aTableBorder.IsVerticalLineValid )
                    rBorderLine = aTableBorder.VerticalLine;
				break;
			case XlBordersIndex::xlInsideHorizontal:
                if ( aTableBorder.IsHorizontalLineValid )
                    rBorderLine = aTableBorder.HorizontalLine;
				break;

			case XlBordersIndex::xlDiagonalDown:
			case XlBordersIndex::xlDiagonalUp:
				// #TODO have to ignore at the momement, would be
				// nice to investigate what we can do here
				break; 
			default:
					return false;
		}
		return true;
	}	
	ScVbaBorder(); // no impl
public:
	ScVbaBorder( const uno::Reference< beans::XPropertySet > & xProps, const uno::Reference< uno::XComponentContext >& xContext, sal_Int32 lineType, ScVbaPalette& rPalette) : m_xProps( xProps ), m_xContext( xContext ), m_LineType( lineType ), m_Palette( rPalette ) {}  	

	// XBorder
	uno::Any SAL_CALL getColor() throw (uno::RuntimeException)
	{
		table::BorderLine aBorderLine;
		if ( getBorderLine( aBorderLine ) )
			return uno::makeAny( OORGBToXLRGB( aBorderLine.Color ) ); 
		throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "No Implementation available" ) ), uno::Reference< uno::XInterface >() );
	}
	void SAL_CALL setColor( const uno::Any& _color ) throw (uno::RuntimeException)
	{
		sal_Int32 nColor;
		_color >>= nColor;
		table::BorderLine aBorderLine;
		if ( getBorderLine( aBorderLine ) )
		{
			aBorderLine.Color = XLRGBToOORGB( nColor );
			setBorderLine( aBorderLine );	
		}
		else
			throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "No Implementation available" ) ), uno::Reference< uno::XInterface >() );
	}

	uno::Any SAL_CALL getColorIndex() throw (uno::RuntimeException)
	{
		sal_Int32 nColor;
		XLRGBToOORGB( getColor() ) >>= nColor;
		uno::Reference< container::XIndexAccess > xIndex = m_Palette.getPalette();
		sal_Int32 nElems = xIndex->getCount();
		sal_Int32 nIndex = -1;
		for ( sal_Int32 count=0; count<nElems; ++count )
		{
			sal_Int32 nPaletteColor;
			xIndex->getByIndex( count ) >>= nPaletteColor;
			if ( nPaletteColor == nColor )
			{					
				nIndex = count + 1;
				break;
			}
		}
		return uno::makeAny(nIndex);
	}

	void SAL_CALL setColorIndex( const uno::Any& _colorindex ) throw (uno::RuntimeException)
	{
		sal_Int32 nColor;
		_colorindex >>= nColor;
		if ( !nColor || nColor == XlColorIndex::xlColorIndexAutomatic )
			nColor = 1;
		setColor( OORGBToXLRGB( m_Palette.getPalette()->getByIndex( --nColor )  ) );
	}
	uno::Any SAL_CALL getWeight() throw (uno::RuntimeException)
	{
		table::BorderLine aBorderLine;
		if ( getBorderLine( aBorderLine ) )
		{
			switch ( aBorderLine.OuterLineWidth )
			{
				case 0:	// Thin = default OO thickness
				case OOLineThin:	
					return uno::makeAny( XlBorderWeight::xlThin );
				case OOLineMedium:	
					return uno::makeAny( XlBorderWeight::xlMedium );
				case OOLineThick:	
					return uno::makeAny( XlBorderWeight::xlThick );
				case OOLineHairline:	
					return uno::makeAny( XlBorderWeight::xlHairline );
				default: 
					break;
			}
		}
		throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "Method failed" ) ), uno::Reference< uno::XInterface >() );
	}
	void SAL_CALL setWeight( const uno::Any& _weight ) throw (uno::RuntimeException)
	{
		sal_Int32 nWeight;
		_weight >>= nWeight;
		table::BorderLine aBorderLine;
		if ( getBorderLine( aBorderLine ) )
		{
			switch ( nWeight )
			{
				case XlBorderWeight::xlThin:	
					aBorderLine.OuterLineWidth = OOLineThin;
					break;
				case XlBorderWeight::xlMedium:
					aBorderLine.OuterLineWidth = OOLineMedium;
					break;
				case XlBorderWeight::xlThick:
					aBorderLine.OuterLineWidth = OOLineThick;
					break;
				case XlBorderWeight::xlHairline:
					aBorderLine.OuterLineWidth = OOLineHairline;
					break;
				default: 
					throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "Bad param" ) ), uno::Reference< uno::XInterface >() );
					break;
			}
			setBorderLine( aBorderLine );
		}
		else
					throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "Method failed" ) ), uno::Reference< uno::XInterface >() );
	}

	uno::Any SAL_CALL getLineStyle() throw (uno::RuntimeException)
	{
		// always return xlContinuous;
		return uno::makeAny( XlLineStyle::xlContinuous );
	}
	void SAL_CALL setLineStyle( const uno::Any& _linestyle ) throw (uno::RuntimeException) 
	{
		// Urk no choice but to silently ignore we don't support this attribute
		// #TODO would be nice to support the excel line styles
        sal_Int32 nLineStyle;
        _linestyle >>= nLineStyle;
        table::BorderLine aBorderLine;
		if ( getBorderLine( aBorderLine ) )
		{
			switch ( nLineStyle )
			{
                case XlLineStyle::xlContinuous:
                case XlLineStyle::xlDash:
                case XlLineStyle::xlDashDot:
                case XlLineStyle::xlDashDotDot:
                case XlLineStyle::xlDot:
                case XlLineStyle::xlDouble:
                case XlLineStyle::xlLineStyleNone:
                case XlLineStyle::xlSlantDashDot:
                    break;
                default:
                    throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "Bad param" ) ), uno::Reference< uno::XInterface >() );
                    break;
            }
			setBorderLine( aBorderLine );
        }
		else
            throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "Method failed" ) ), uno::Reference< uno::XInterface >() );
	}
};

class RangeBorders : public RangeBorders_Base
{
private:
	uno::Reference< table::XCellRange > m_xRange;
	uno::Reference< uno::XComponentContext > m_xContext;
	ScVbaPalette m_Palette;
	sal_Int32 getTableIndex( sal_Int32 nConst )
	{
		// hokay return position of the index in the table
		sal_Int32 nIndexes = getCount();
		sal_Int32 realIndex = 0;
		const sal_Int16* pTableEntry = supportedIndexTable;
		for ( ; realIndex < nIndexes; ++realIndex, ++pTableEntry )
		{
			if ( *pTableEntry == nConst )
				return realIndex;
		}
		return getCount(); // error condition
	}
public:
	RangeBorders(  const uno::Reference< table::XCellRange >& xRange,  const uno::Reference< uno::XComponentContext > & xContext, ScVbaPalette& rPalette ) : m_xRange( xRange ), m_xContext( xContext ), m_Palette( rPalette )
	{
	}
	// XIndexAccess 
	virtual ::sal_Int32 SAL_CALL getCount(  ) throw (uno::RuntimeException)
	{
		return sizeof( supportedIndexTable ) / sizeof( supportedIndexTable[0] );
	}
	virtual uno::Any SAL_CALL getByIndex( ::sal_Int32 Index ) throw (lang::IndexOutOfBoundsException, lang::WrappedTargetException, uno::RuntimeException) 
	{
			
		sal_Int32 nIndex = getTableIndex( Index );
		if ( nIndex >= 0 && nIndex < getCount() )
		{
			uno::Reference< beans::XPropertySet > xProps( m_xRange, uno::UNO_QUERY_THROW );
			return uno::makeAny( uno::Reference< excel::XBorder >( new ScVbaBorder( xProps, m_xContext, supportedIndexTable[ nIndex ], m_Palette )) );
		}
		throw lang::IndexOutOfBoundsException();
	}
	virtual uno::Type SAL_CALL getElementType(  ) throw (uno::RuntimeException)
	{
		return  excel::XBorder::static_type(0);
	}
	virtual ::sal_Bool SAL_CALL hasElements(  ) throw (uno::RuntimeException)
	{
		return sal_True;
	}
};

uno::Reference< container::XIndexAccess > 
rangeToBorderIndexAccess( const uno::Reference< table::XCellRange >& xRange,  const uno::Reference< uno::XComponentContext > & xContext, ScVbaPalette& rPalette )
{
	return new RangeBorders( xRange, xContext, rPalette );
}

class RangeBorderEnumWrapper : public EnumerationHelper_BASE
{
	uno::Reference<container::XIndexAccess > m_xIndexAccess;
	sal_Int32 nIndex;
public:
	RangeBorderEnumWrapper( const uno::Reference< container::XIndexAccess >& xIndexAccess ) : m_xIndexAccess( xIndexAccess ), nIndex( 0 ) {}
	virtual ::sal_Bool SAL_CALL hasMoreElements(  ) throw (uno::RuntimeException)
	{
		return ( nIndex < m_xIndexAccess->getCount() );
	}

	virtual uno::Any SAL_CALL nextElement(  ) throw (container::NoSuchElementException, lang::WrappedTargetException, uno::RuntimeException)
	{
		if ( nIndex < m_xIndexAccess->getCount() )
			return m_xIndexAccess->getByIndex( nIndex++ );
		throw container::NoSuchElementException();
	}
};

ScVbaBorders::ScVbaBorders( const uno::Reference< uno::XComponentContext > & xContext, const uno::Reference< table::XCellRange >& xRange, ScVbaPalette& rPalette  ):  ScVbaBorders_BASE( xContext, rangeToBorderIndexAccess( xRange ,xContext, rPalette ) ) 
{
}

uno::Reference< container::XEnumeration >
ScVbaBorders::createEnumeration() throw (uno::RuntimeException)
{
	return new RangeBorderEnumWrapper( m_xIndexAccess );
}

uno::Any
ScVbaBorders::createCollectionObject( const css::uno::Any& aSource )
{
	return aSource; // its already a Border object
}

uno::Type 
ScVbaBorders::getElementType() throw (uno::RuntimeException)
{
	return excel::XBorders::static_type(0);
}

uno::Any
ScVbaBorders::getItemByIntIndex( const sal_Int32 nIndex )  throw (uno::RuntimeException)
{
	return createCollectionObject( m_xIndexAccess->getByIndex( nIndex ) );
}


uno::Any SAL_CALL ScVbaBorders::getColor() throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    uno::Any color;
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        if( XlBordersIndex::xlDiagonalDown != supportedIndexTable[i] && XlBordersIndex::xlDiagonalUp != supportedIndexTable[i] )
        {
            uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
            if( color.hasValue() )
            {
                if( color != xBorder->getColor() )
                    return uno::makeAny( uno::Reference< uno::XInterface >() );
            }
            else
                color = xBorder->getColor();
        }
    }
    return  color;
}
void SAL_CALL ScVbaBorders::setColor( const uno::Any& _color ) throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
        xBorder->setColor( _color );
    }
}
uno::Any SAL_CALL ScVbaBorders::getColorIndex() throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    uno::Any nColorIndex;
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        if( XlBordersIndex::xlDiagonalDown != supportedIndexTable[i] && XlBordersIndex::xlDiagonalUp != supportedIndexTable[i] )
        {
            uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
            if( nColorIndex.hasValue() )
            {
                if( nColorIndex != xBorder->getColorIndex() )
                    return uno::makeAny( uno::Reference< uno::XInterface >() );
            }
            else
                nColorIndex = xBorder->getColorIndex();
        }
    }
    return  nColorIndex;
}
void SAL_CALL ScVbaBorders::setColorIndex( const uno::Any& _colorindex ) throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
        xBorder->setColorIndex( _colorindex );
    }
}
uno::Any SAL_CALL ScVbaBorders::getLineStyle() throw (uno::RuntimeException)
{
     //TODO if OOo support
	throw uno::RuntimeException( rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( "No Implementation available" ) ), uno::Reference< uno::XInterface >() );
}
void SAL_CALL ScVbaBorders::setLineStyle( const uno::Any& _linestyle ) throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
        xBorder->setLineStyle( _linestyle );
    }
}
uno::Any SAL_CALL ScVbaBorders::getWeight() throw (uno::RuntimeException)
{
    sal_Int32 count = getCount();
    uno::Any weight;
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        if( XlBordersIndex::xlDiagonalDown != supportedIndexTable[i] && XlBordersIndex::xlDiagonalUp != supportedIndexTable[i] )
        {
            uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
            if( weight.hasValue() )
            {
                if( weight != xBorder->getWeight() )
                    return uno::makeAny( uno::Reference< uno::XInterface >() );
            }
            else
                weight = xBorder->getWeight();
        }
    }
    return  weight;
}
void SAL_CALL ScVbaBorders::setWeight( const uno::Any& _weight ) throw (uno::RuntimeException) 
{
    sal_Int32 count = getCount();
    for( sal_Int32 i = 0; i < count ; i++ )
    {
        uno::Reference< XBorder > xBorder( getItemByIntIndex( supportedIndexTable[i] ), uno::UNO_QUERY_THROW );
        xBorder->setWeight( _weight );
    }
}
