
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/nlpnewton.hxx"

#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace scsolver::numeric;
using namespace scsolver::numeric::opres;
using namespace scsolver::numeric::opres::nlp;
using boost::shared_ptr;

class TestFunc : public BaseFuncObj
{
public:
	TestFunc() {}
	virtual ~TestFunc() throw() {}

	virtual double operator()( const vector<double>& cnX ) const
	{
		double x = cnX.at( 0 );
		return cos( x + 1.3 ) + sin( x )*cos( x + 1.5 + sin( x ) );
	}

	virtual string getFuncString() const
	{
		string s = "f(x) = cos(x + 1.3) + sin(x)*cos(x + 1.5 + sin(x))";
		return s;
	}
};

void tcFoo()
{
	cout << "Test Case Foo" << endl;

	shared_ptr<BaseFuncObj> pFunc( new TestFunc() );
	shared_ptr<QuasiNewton> pAlgorithm( new QuasiNewton() );
	Model model;
	model.pushVar(1.0);
	model.setVerbose(true);
	model.setFuncObject(pFunc);
	model.setGoal(GOAL_MINIMIZE);
	model.setPrecision(2);
	model.print();
	model.solve( pAlgorithm );
	Matrix sol = model.getSolution();
	sol.print();
}

int main()
{
	tcFoo();
}
