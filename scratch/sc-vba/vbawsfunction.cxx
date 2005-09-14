#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/table/XColumnRowRange.hpp>
#include <com/sun/star/beans/XIntrospection.hpp>
#include <com/sun/star/beans/XIntrospectionAccess.hpp>
#include <com/sun/star/reflection/XIdlMethod.hpp>
#include <com/sun/star/beans/MethodConcept.hpp>
#include <comphelper/processfactory.hxx>
#include <cppuhelper/queryinterface.hxx>

#include "vbawsfunction.hxx"

void SAL_CALL 
ScVbaWSFunction::dummy(  ) throw (::com::sun::star::uno::RuntimeException)
{
}

Reference< XIntrospectionAccess >
ScVbaWSFunction::getIntrospection(void)  throw(RuntimeException)
{
	return uno::Reference<beans::XIntrospectionAccess>();
}

Any SAL_CALL
ScVbaWSFunction::invoke(const OUString& FunctionName, const Sequence< Any >& Params, Sequence< sal_Int16 >& OutParamIndex, Sequence< Any >& OutParam) throw(IllegalArgumentException, CannotConvertException, InvocationTargetException, RuntimeException)
{
	uno::Any aAny;
	uno::Reference< lang::XMultiComponentFactory > xSMgr( m_xContext->getServiceManager(), uno::UNO_QUERY );
	uno::Reference< sheet::XFunctionAccess > xFunctionAccess(
                        xSMgr->createInstanceWithContext(::rtl::OUString::createFromAscii(
                        "com.sun.star.sheet.FunctionAccess"), m_xContext),
                        ::uno::UNO_QUERY);
	uno::Sequence< uno::Any > aParamTemp;
	sal_Int32 nParamCount = Params.getLength();
	aParamTemp.realloc(nParamCount);
	const uno::Any* aArray = Params.getConstArray();
	uno::Any* aArrayTemp = aParamTemp.getArray();

	for (int i=0; i < Params.getLength();i++) 
	{
		uno::Reference<vba::XRange> myRange;
		uno::Any aAnyTemp;
		const uno::Any& rArg = aArray[i];
		if (rArg >>= myRange) 
		{
			aAnyTemp = myRange->getCellRange();
			aArrayTemp[i] = aAnyTemp;
		}
		else
			aArrayTemp[i]= aArray[i];
	}

	aAny = xFunctionAccess->callFunction(FunctionName,aParamTemp);

	return aAny;
}

void SAL_CALL
ScVbaWSFunction::setValue(const rtl::OUString& PropertyName, const Any& Value) throw(UnknownPropertyException, CannotConvertException, InvocationTargetException, RuntimeException)
{
}

uno::Any SAL_CALL
ScVbaWSFunction::getValue(const rtl::OUString& PropertyName) throw(UnknownPropertyException, RuntimeException)
{
	return uno::Any();
}

sal_Bool SAL_CALL
ScVbaWSFunction::hasMethod(const rtl::OUString& Name)  throw(RuntimeException)
{
	return true;
}

sal_Bool SAL_CALL
ScVbaWSFunction::hasProperty(const OUString& Name)  throw(RuntimeException)
{
	 return false;
}
