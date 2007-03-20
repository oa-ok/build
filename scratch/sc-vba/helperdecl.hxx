#include <comphelper/servicedecl.hxx>
#include <org/openoffice/vba/XHelperInterface.hpp>

namespace comphelper {
namespace service_decl {

namespace detail {
namespace css = ::com::sun::star;
template <typename ImplT>
class OwnServiceImpl
    : public ImplT,
      private ::boost::noncopyable
{
    typedef ImplT BaseT;
    
public:    
    OwnServiceImpl(
        ServiceDecl const& rServiceDecl,
        css::uno::Sequence<css::uno::Any> const& args,
        css::uno::Reference<css::uno::XComponentContext> const& xContext )
        :BaseT(args, xContext), m_rServiceDecl(rServiceDecl) {}
    OwnServiceImpl(
        ServiceDecl const& rServiceDecl,
        css::uno::Reference<css::uno::XComponentContext> const& xContext )
        : BaseT(xContext), m_rServiceDecl(rServiceDecl) {}
    
    // XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName()
        throw (css::uno::RuntimeException) {
        return m_rServiceDecl.getImplementationName();
    }
    virtual sal_Bool SAL_CALL supportsService( ::rtl::OUString const& name )
        throw (css::uno::RuntimeException) {
        return m_rServiceDecl.supportsService(name);
    }
    virtual css::uno::Sequence< ::rtl::OUString>
    SAL_CALL getSupportedServiceNames() throw (css::uno::RuntimeException) {
        return m_rServiceDecl.getSupportedServiceNames();
    }
    
private:
    ServiceDecl const& m_rServiceDecl;
};


template <typename ImplT, typename WithArgsT>
struct OwnCreateFunc;

template <typename ImplT>
struct OwnCreateFunc<ImplT, with_args<false> > {
    explicit OwnCreateFunc(  )
         {}
    
    css::uno::Reference<css::uno::XInterface>
    operator()( ServiceDecl const& rServiceDecl,
                css::uno::Sequence<css::uno::Any> const&,
                css::uno::Reference<css::uno::XComponentContext>
                const& xContext ) const
    {
        return css::uno::Reference< org::openoffice::vba::XHelperInterface >(new OwnServiceImpl<ImplT>( rServiceDecl, xContext ));
    }
};

template <typename ImplT>
struct OwnCreateFunc<ImplT, with_args<true> > {
    explicit OwnCreateFunc( )
         {}
    
    css::uno::Reference<css::uno::XInterface>
    operator()( ServiceDecl const& rServiceDecl,
                css::uno::Sequence<css::uno::Any> const& args,
                css::uno::Reference<css::uno::XComponentContext>
                const& xContext ) const
    {
        return css::uno::Reference< org::openoffice::vba::XHelperInterface >(
            new OwnServiceImpl<ImplT>( rServiceDecl, args, xContext ) );
    }
};

} // namespace detail

/** Defines a service implementation class.
    
    @tpl ImplT_ service implementation class
    @WithArgsT whether the implementation class ctor expects arguments
               (uno::Sequence<uno::Any>, uno::Reference<uno::XComponentContext>)
               or just (uno::Reference<uno::XComponentContext>)
*/
template <typename ImplT_, typename WithArgsT = with_args<false> >
struct vba_service_class_ {
    typedef ImplT_ ImplT;
    typedef detail::OwnServiceImpl<ImplT_ > ServiceImplT;
    
    detail::CreateFuncF const m_createFunc;
    
    /** Default ctor.  Implementation class without args, expecting
        component context as single argument.
    */
    vba_service_class_() : m_createFunc(
        detail::OwnCreateFunc<ImplT, WithArgsT>() ) {}

};


} // namespace service_decl
} // namespace comphelper

