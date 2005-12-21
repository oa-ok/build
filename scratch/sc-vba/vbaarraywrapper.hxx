#ifndef SC_VBA_ARRAYWRAPPER_HXX
#define SC_VBA_ARRAYWRAPPER_HXX
#include <cppuhelper/implbase1.hxx>
#include <org/openoffice/vba/XArrayWrapper.hpp>
#include "vbahelper.hxx"

typedef ::cppu::WeakImplHelper1<oo::vba::XArrayWrapper > ArrayWrapperImpl_BASE;

class ScArrayWrapper : public ArrayWrapperImpl_BASE
{		
	css::uno::Any maArray;
	sal_Bool mbZeroBased;
public:
	ScArrayWrapper( const css::uno::Any& aArray, sal_Bool bZeroBased );
	// Attributes
	virtual ::sal_Bool SAL_CALL getIsZeroIndex() throw (css::uno::RuntimeException);
	virtual css::uno::Any SAL_CALL getArray() throw (css::uno::RuntimeException);
};

#endif //SC_VBA_WINDOW_HXX
