#ifndef SC_VBA_PICTUREFORMAT_HXX
#define SC_VBA_PICTUREFORMAT_HXX

#include <com/sun/star/drawing/XShape.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <org/openoffice/msforms/XPictureFormat.hpp>
#include "vbahelperinterface.hxx"

typedef InheritedHelperInterfaceImpl1< oo::msforms::XPictureFormat > ScVbaPictureFormat_BASE;

class ScVbaPictureFormat : public ScVbaPictureFormat_BASE
{
private:
    css::uno::Reference< css::drawing::XShape > m_xShape;
    css::uno::Reference< css::beans::XPropertySet > m_xPropertySet;
protected:
    virtual rtl::OUString& getServiceImplName();
    virtual css::uno::Sequence<rtl::OUString> getServiceNames();
private:
    void checkParameterRangeInDouble( double nRange, double nMin, double nMax ) throw (css::uno::RuntimeException);
public:
    ScVbaPictureFormat( const css::uno::Reference< oo::vba::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, css::uno::Reference< css::drawing::XShape > xShape ) throw( css::lang::IllegalArgumentException );

    // Attributes
    virtual double SAL_CALL getBrightness() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setBrightness( double _brightness ) throw (css::uno::RuntimeException);
    virtual double SAL_CALL getContrast() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setContrast( double _contrast ) throw (css::uno::RuntimeException);

    // Methods
    virtual void SAL_CALL IncrementBrightness( double increment ) throw (css::uno::RuntimeException);
    virtual void SAL_CALL IncrementContrast( double increment ) throw (css::uno::RuntimeException);
};

#endif//SC_VBA_PICTUREFORMAT_HXX
