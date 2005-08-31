/*************************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  last change: $Author$ $Date$
 *
 *  The Contents of this file are made available subject to the terms of
 *  either of the following licenses
 *
 *         - GNU Lesser General Public License Version 2.1
 *         - Sun Industry Standards Source License Version 1.1
 *
 *  Sun Microsystems Inc., October, 2000
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA  02111-1307  USA
 *
 *
 *  Sun Industry Standards Source License Version 1.1
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.1 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://www.openoffice.org/license.html.
 *
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 *
 *  The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 *
 *  Copyright: 2000 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/

#include <canvas/debug.hxx>
#include "cairo_textlayout.hxx"

#ifndef BOOST_SCOPED_ARRAY_HPP_INCLUDED
#include <boost/scoped_array.hpp>
#endif

#ifndef _SV_METRIC_HXX
#include <vcl/metric.hxx>
#endif
#ifndef _SV_VIRDEV_HXX
#include <vcl/virdev.hxx>
#endif

#ifndef _COM_SUN_STAR_RENDERING_TEXTDIRECTION_HPP_
#include <com/sun/star/rendering/TextDirection.hpp>
#endif
#ifndef _BGFX_MATRIX_B2DHOMMATRIX_HXX
#include <basegfx/matrix/b2dhommatrix.hxx>
#endif
#ifndef _BGFX_NUMERIC_FTOOLS_HXX
#include <basegfx/numeric/ftools.hxx>
#endif

#include <canvas/canvastools.hxx>

#include "cairo_impltools.hxx"

using namespace ::com::sun::star;
using namespace ::cairo;

namespace cairocanvas
{
    namespace
    {
        void setupLayoutMode( OutputDevice& rOutDev,
                              sal_Int8		nTextDirection )	
        {
            // TODO(P3): avoid if already correctly set
            ULONG nLayoutMode;
            switch( nTextDirection )
            {
                default:
                    nLayoutMode = 0;
                    break;
                case rendering::TextDirection::WEAK_LEFT_TO_RIGHT:
                    nLayoutMode = TEXT_LAYOUT_BIDI_LTR;
                    break;
                case rendering::TextDirection::STRONG_LEFT_TO_RIGHT:
                    nLayoutMode = TEXT_LAYOUT_BIDI_LTR | TEXT_LAYOUT_BIDI_STRONG;
                    break;
                case rendering::TextDirection::WEAK_RIGHT_TO_LEFT:
                    nLayoutMode = TEXT_LAYOUT_BIDI_RTL;
                    break;
                case rendering::TextDirection::STRONG_RIGHT_TO_LEFT:
                    nLayoutMode = TEXT_LAYOUT_BIDI_RTL | TEXT_LAYOUT_BIDI_STRONG;
                    break;
            }

            // set calculated layout mode. Origin is always the left edge,
            // as required at the API spec
            rOutDev.SetLayoutMode( nLayoutMode | TEXT_LAYOUT_TEXTORIGIN_LEFT );
        }
    }

    TextLayout::TextLayout( const rendering::StringContext& aText,
                            sal_Int8                        nDirection,
                            sal_Int64                       nRandomSeed,
                            const CanvasFont::ImplRef&      rFont,
			    Cairo*                          pCairo) :
        TextLayout_Base( m_aMutex ),
        maText( aText ),
        maLogicalAdvancements(),
        mpFont( rFont ),
        mnTextDirection( nDirection ),
	mpCairo( pCairo )
    {
	cairo_reference( mpCairo );
    }

    TextLayout::~TextLayout()
    {
	cairo_destroy( mpCairo );
    }

    void SAL_CALL TextLayout::disposing()
    {
        tools::LocalGuard aGuard;

        mpFont.reset();
    }

    // XTextLayout
    uno::Sequence< uno::Reference< rendering::XPolyPolygon2D > > SAL_CALL TextLayout::queryTextShapes(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return uno::Sequence< uno::Reference< rendering::XPolyPolygon2D > >();
    }

    uno::Sequence< geometry::RealRectangle2D > SAL_CALL TextLayout::queryInkMeasures(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return uno::Sequence< geometry::RealRectangle2D >();
    }

    uno::Sequence< geometry::RealRectangle2D > SAL_CALL TextLayout::queryMeasures(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return uno::Sequence< geometry::RealRectangle2D >();
    }

    uno::Sequence< double > SAL_CALL TextLayout::queryLogicalAdvancements(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        return maLogicalAdvancements;
    }

    void SAL_CALL TextLayout::applyLogicalAdvancements( const uno::Sequence< double >& aAdvancements ) throw (lang::IllegalArgumentException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        CHECK_AND_THROW( aAdvancements.getLength() == maText.Length,
                         "TextLayout::applyLogicalAdvancements(): mismatching number of advancements" );

        maLogicalAdvancements = aAdvancements;
    }

    geometry::RealRectangle2D SAL_CALL TextLayout::queryTextBounds(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

	    OSL_TRACE("queryTextBounds");
	geometry::RealRectangle2D aBounds( 0, 0, 0, 0 );

// TODO(rodo)
// 	if( maLogicalAdvancements.getLength() ) {
// 	    cairo_save( mpCairo );
// 	    useFont( mpCairo );
// 	    cairo_font_extents_t aFontExtents;
// 	    cairo_font_extents( mpCairo, &aFontExtents );
// 	    cairo_restore( mpCairo );

// 	    aBounds = geometry::RealRectangle2D( 0, -aFontExtents.ascent,
// 						 maLogicalAdvancements[ maLogicalAdvancements.getLength()-1 ],
// 						 aFontExtents.descent );
// 	} else {

	    cairo_save( mpCairo );

	    ::rtl::OUString aSubText = maText.Text.copy( maText.StartPosition, maText.Length );
	    ::rtl::OString aUTF8String = ::rtl::OUStringToOString( aSubText, RTL_TEXTENCODING_UTF8 );

	    useFont( mpCairo );

	    cairo_font_extents_t aFontExtents;
	    cairo_font_extents( mpCairo, &aFontExtents );
	    cairo_text_extents_t aTextExtents;
	    cairo_text_extents( mpCairo, aUTF8String, &aTextExtents );
	    cairo_restore( mpCairo );

	    OSL_TRACE("text extents, width: %d x_bearing: %d ascent: %d descent: %d\n", aTextExtents.width, aTextExtents.x_bearing, aFontExtents.ascent, aFontExtents.descent);

	    aBounds = geometry::RealRectangle2D( aTextExtents.x_bearing, -aFontExtents.ascent,
						 aTextExtents.x_bearing + aTextExtents.width, aFontExtents.descent );
// 	}

	return aBounds;
    }

    double SAL_CALL TextLayout::justify( double nSize ) throw (lang::IllegalArgumentException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return 0.0;
    }

    double SAL_CALL TextLayout::combinedJustify( const uno::Sequence< uno::Reference< rendering::XTextLayout > >& aNextLayouts, double nSize ) throw (lang::IllegalArgumentException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return 0.0;
    }

    rendering::TextHit SAL_CALL TextLayout::getTextHit( const geometry::RealPoint2D& aHitPoint ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return rendering::TextHit();
    }

    rendering::Caret SAL_CALL TextLayout::getCaret( sal_Int32 nInsertionIndex, sal_Bool bExcludeLigatures ) throw (lang::IndexOutOfBoundsException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return rendering::Caret();
    }

    sal_Int32 SAL_CALL TextLayout::getNextInsertionIndex( sal_Int32 nStartIndex, sal_Int32 nCaretAdvancement, sal_Bool bExcludeLigatures ) throw (lang::IndexOutOfBoundsException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return 0;
    }

    uno::Reference< rendering::XPolyPolygon2D > SAL_CALL TextLayout::queryVisualHighlighting( sal_Int32 nStartIndex, sal_Int32 nEndIndex ) throw (lang::IndexOutOfBoundsException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return uno::Reference< rendering::XPolyPolygon2D >();
    }

    uno::Reference< rendering::XPolyPolygon2D > SAL_CALL TextLayout::queryLogicalHighlighting( sal_Int32 nStartIndex, sal_Int32 nEndIndex ) throw (lang::IndexOutOfBoundsException, uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return uno::Reference< rendering::XPolyPolygon2D >();
    }

    double SAL_CALL TextLayout::getBaselineOffset(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        // TODO(F1)
        return 0.0;
    }

    sal_Int8 SAL_CALL TextLayout::getMainTextDirection(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        return mnTextDirection;
    }

    uno::Reference< rendering::XCanvasFont > SAL_CALL TextLayout::getFont(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        return mpFont.getRef();
    }

    rendering::StringContext SAL_CALL TextLayout::getText(  ) throw (uno::RuntimeException)
    {
        tools::LocalGuard aGuard;

        return maText;
    }

    void TextLayout::useFont( Cairo* pCairo )
    {
	rendering::FontRequest aFontRequest = mpFont->getFontRequest();
	rendering::FontInfo aFontInfo = aFontRequest.FontDescription;
	
	cairo_select_font_face( pCairo, ::rtl::OUStringToOString( aFontInfo.FamilyName, RTL_TEXTENCODING_UTF8 ), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size( pCairo, aFontRequest.CellSize );
    }

    bool TextLayout::draw( Cairo* pCairo )
    {
        tools::LocalGuard aGuard;

	::rtl::OUString aSubText = maText.Text.copy( maText.StartPosition, maText.Length );
	::rtl::OString aUTF8String = ::rtl::OUStringToOString( aSubText, RTL_TEXTENCODING_UTF8 );

	cairo_save( pCairo );
	/* move to 0, 0 as cairo_show_text advances current point and current point is not restored by cairo_restore.
	   before we were depending on unmodified current point which I believed was preserved by save/restore */
	cairo_move_to( pCairo, 0, 0 );
	useFont( pCairo );
	cairo_show_text( pCairo, aUTF8String );
	cairo_restore( pCairo );

        return true;
    }

    namespace
    {
        class OffsetTransformer
        {
        public:
            OffsetTransformer( const ::basegfx::B2DHomMatrix& rMat ) :
                maMatrix( rMat )
            {
            }

            long operator()( const double& rOffset )
            {
                // This is an optimization of the normal rMat*[x,0]
                // transformation of the advancement vector (in x
                // direction), followed by a length calculation of the
                // resulting vector: advancement' =
                // ||rMat*[x,0]||. Since advancements are vectors, we
                // can ignore translational components, thus if [x,0],
                // it follows that rMat*[x,0]=[x',0] holds. Thus, we
                // just have to calc the transformation of the x
                // component.

                // TODO(F2): Handle non-horizontal advancements!
                return ::basegfx::fround( hypot(maMatrix.get(0,0)*rOffset,
												maMatrix.get(1,0)*rOffset) );
            }

        private:
            ::basegfx::B2DHomMatrix maMatrix;
        };
    }

    void TextLayout::setupTextOffsets( long*							outputOffsets,
                                       const uno::Sequence< double >& 	inputOffsets,
                                       const rendering::ViewState& 		viewState,
                                       const rendering::RenderState& 	renderState		) const
    {
        ENSURE_AND_THROW( outputOffsets!=NULL,
                          "TextLayout::setupTextOffsets offsets NULL" );

        ::basegfx::B2DHomMatrix aMatrix;

        ::canvas::tools::mergeViewAndRenderTransform(aMatrix,
                                                     viewState,
                                                     renderState);

        // fill integer offsets
        ::std::transform( const_cast< uno::Sequence< double >& >(inputOffsets).getConstArray(),
                          const_cast< uno::Sequence< double >& >(inputOffsets).getConstArray()+inputOffsets.getLength(),
                          outputOffsets,
                          OffsetTransformer( aMatrix ) );
    }


#define SERVICE_NAME "com.sun.star.rendering.TextLayout"

    ::rtl::OUString SAL_CALL TextLayout::getImplementationName() throw( uno::RuntimeException )
    {
        return ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( TEXTLAYOUT_IMPLEMENTATION_NAME ) );
    }

    sal_Bool SAL_CALL TextLayout::supportsService( const ::rtl::OUString& ServiceName ) throw( uno::RuntimeException )
    {
        return ServiceName.equalsAsciiL( RTL_CONSTASCII_STRINGPARAM ( SERVICE_NAME ) );
    }

    uno::Sequence< ::rtl::OUString > SAL_CALL TextLayout::getSupportedServiceNames()  throw( uno::RuntimeException )
    {
        uno::Sequence< ::rtl::OUString > aRet(1);
        aRet[0] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM ( SERVICE_NAME ) );

        return aRet;
    }
}
