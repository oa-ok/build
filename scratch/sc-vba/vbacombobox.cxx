#include "vbacombobox.hxx"
#include <vector>

using namespace com::sun::star;
using namespace org::openoffice;


const static rtl::OUString CONTROLSOURCEPROP( RTL_CONSTASCII_USTRINGPARAM("DataFieldProperty") );
const static rtl::OUString ITEMS( RTL_CONSTASCII_USTRINGPARAM("StringItemList") );

const sal_Int32 RANGE_PROPERTY_ID_DFLT=1;
// name is not defineable in IDL so no chance of a false detection of the
// another property/method of the same name
const ::rtl::OUString RANGE_PROPERTY_DFLT( RTL_CONSTASCII_USTRINGPARAM( "_$DefaultProp" ) );

ScVbaComboBox::ScVbaComboBox( const uno::Reference< uno::XComponentContext >& xContext, const uno::Reference< beans::XPropertySet >& xProps ) : OPropertyContainer(GetBroadcastHelper()), m_xContext(xContext), m_xProps( xProps ) 
{
	// grab the default value property name
	m_xProps->getPropertyValue( CONTROLSOURCEPROP ) >>= sSourceName;
	setDfltPropHandler();
}


// Attributes
uno::Any SAL_CALL 
ScVbaComboBox::getValue() throw (uno::RuntimeException)
{
	return m_xProps->getPropertyValue( sSourceName );
}

void SAL_CALL 
ScVbaComboBox::setValue( const uno::Any& _value ) throw (uno::RuntimeException)
{
	m_xProps->setPropertyValue( sSourceName, _value );
}

::rtl::OUString SAL_CALL 
ScVbaComboBox::getText() throw (uno::RuntimeException)
{
	rtl::OUString result;
	getValue() >>= result;
	return result;
}

void SAL_CALL 
ScVbaComboBox::setText( const ::rtl::OUString& _text ) throw (uno::RuntimeException)
{
	setValue( uno::makeAny( _text ) ); // seems the same
}

// Methods
void SAL_CALL 
ScVbaComboBox::AddItem( const uno::Any& pvargItem, const uno::Any& pvargIndex ) throw (uno::RuntimeException)
{
	if ( pvargItem.hasValue()  )
	{
		uno::Sequence< rtl::OUString > sList;
		m_xProps->getPropertyValue( ITEMS ) >>= sList;

		sal_Int32 nIndex = sList.getLength();

		if ( pvargIndex.hasValue() )
			pvargIndex >>= nIndex;

		rtl::OUString sString;
		pvargItem >>= sString;

		// if no index specified or item is to be appended to end of 
		// list just realloc the array and set the last item
		if ( nIndex  == sList.getLength() )
		{
			sal_Int32 nOldSize = sList.getLength();
			sList.realloc( nOldSize + 1 );
			sList[ nOldSize ] = sString; 
		}
		else
		{
			// just copy those elements above the one to be inserted
			std::vector< rtl::OUString > sVec;
			// reserve just the amount we need to copy
			sVec.reserve( sList.getLength() - nIndex );

			// point at first element to copy
			rtl::OUString* pString = sList.getArray() + nIndex;
			const rtl::OUString* pEndString = sList.getArray() + sList.getLength();
			// insert the new element
			sVec.push_back( sString );
			// copy elements	
			for ( ; pString != pEndString; ++pString )
				sVec.push_back( *pString );
		
			sList.realloc(  sList.getLength() + 1 );

			// point at first element to be overwritten
			pString = sList.getArray() + nIndex;
			pEndString = sList.getArray() + sList.getLength();
			std::vector< rtl::OUString >::iterator it = sVec.begin();
			for ( ; pString != pEndString; ++pString, ++it)
				*pString = *it;	
			//
		}

		m_xProps->setPropertyValue( ITEMS, uno::makeAny( sList ) );
		
	}
}

void SAL_CALL 
ScVbaComboBox::Clear(  ) throw (uno::RuntimeException)
{
		setValue( uno::makeAny( rtl::OUString() ) );
		m_xProps->setPropertyValue( ITEMS, uno::makeAny( uno::Sequence< rtl::OUString >() ) );
}

// XInterface

IMPLEMENT_FORWARD_XINTERFACE2( ScVbaComboBox, ComboBoxImpl_BASE, OPropertyContainer )

// XTypeProvider

IMPLEMENT_FORWARD_XTYPEPROVIDER2( ScVbaComboBox, ComboBoxImpl_BASE, OPropertyContainer )

// OPropertySetHelper

::cppu::IPropertyArrayHelper& 
ScVbaComboBox::getInfoHelper(  )
{
    static ::cppu::IPropertyArrayHelper* sProps = 0;
    if ( !sProps )
        sProps = createArrayHelper();
    return *sProps;
}


::cppu::IPropertyArrayHelper* 
ScVbaComboBox::createArrayHelper(  ) const
{
    uno::Sequence< beans::Property > aProps;
    describeProperties( aProps );
    return new ::cppu::OPropertyArrayHelper( aProps );
}

// XPropertySet
uno::Reference< beans::XPropertySetInfo > 
ScVbaComboBox::getPropertySetInfo(  ) throw (uno::RuntimeException)
{
    static uno::Reference< beans::XPropertySetInfo > xInfo( createPropertySetInfo( getInfoHelper() ) );
    return xInfo;
}

void
ScVbaComboBox::setDfltPropHandler()
{

	msDftPropName = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Value" ) );
	registerProperty( RANGE_PROPERTY_DFLT, RANGE_PROPERTY_ID_DFLT,
beans::PropertyAttribute::TRANSIENT | beans::PropertyAttribute::BOUND, &msDftPropName, ::getCppuType( &msDftPropName ) );
}

