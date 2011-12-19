#include <gtest/gtest.h>
#include "stan/prob/distributions_error_handling.hpp"
#include "stan/meta/conversions.hpp"
#include "stan/agrad/agrad.hpp"
#include <limits>

typedef boost::math::policies::policy<
  boost::math::policies::domain_error<boost::math::policies::errno_on_error>, 
  boost::math::policies::pole_error<boost::math::policies::errno_on_error>,
  boost::math::policies::overflow_error<boost::math::policies::errno_on_error>,
  boost::math::policies::evaluation_error<boost::math::policies::errno_on_error> 
  > errno_policy;
typedef boost::math::policies::policy<> default_policy;

using namespace stan::prob;
using stan::convert;
using stan::agrad::var;

//---------- check_x tests ----------
TEST(ProbDistributionsErrorHandling,CheckXDefaultPolicy) {
  const char* function = "check_x (%1%)";
  var x = 0;
  var result;
 
  EXPECT_TRUE (check_x (function, x, &result, default_policy())) << "check_x should be true with finite x: " << x;
  x = std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on Inf: " << x;
  x = -std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on -Inf: " << x;
  x = std::numeric_limits<var>::quiet_NaN();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on NaN: " << x;
}

TEST(ProbDistributionsErrorHandling,CheckXErrnoPolicy) {
  const char* function = "check_x (%1%)";
  var x = 0;
  var result;
 
  EXPECT_TRUE (check_x (function, x, &result, errno_policy())) << "check_x should be true with finite x: " << x;
  x = std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on Inf: " << x;
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN: " << x;

  x = -std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on -Inf: " << x;
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN: " << x;
 
  x = std::numeric_limits<var>::quiet_NaN();
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on NaN: " << x;
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN: " << x;
}


// ---------- check_x: vector tests ----------
TEST(ProbDistributionsErrorHandling,CheckXVectorDefaultPolicy) {
  const char* function = "check_x (%1%)";
  var result;
  std::vector<var> x;
  
  x.clear();
  x.push_back (-1);
  x.push_back (0);
  x.push_back (1);
  ASSERT_TRUE (check_x (function, x, &result, default_policy())) << "check_x should be true with finite x";

  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(std::numeric_limits<var>::infinity());
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on Inf";

  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(-std::numeric_limits<var>::infinity());
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on -Inf";
  
  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(std::numeric_limits<var>::quiet_NaN());
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on NaN";
}

TEST(ProbDistributionsErrorHandling,CheckXVectorErrnoPolicy) {
  const char* function = "check_x (%1%)";
  std::vector<var> x;
  x.push_back (-1);
  x.push_back (0);
  x.push_back (1);
  var result;
 
  result = 0;
  EXPECT_TRUE (check_x (function, x, &result, errno_policy())) << "check_x should be true with finite x";

  result = 0;
  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(std::numeric_limits<var>::infinity());
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on Inf";  
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";


  result = 0;
  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(-std::numeric_limits<var>::infinity());
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on -Inf";
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";


  result = 0;
  x.clear();
  x.push_back(-1);
  x.push_back(0);
  x.push_back(std::numeric_limits<var>::quiet_NaN());
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on NaN";
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";
}

// ---------- check_x: matrix tests ----------
TEST(ProbDistributionsErrorHandling,CheckXMatrixDefaultPolicy) {
  const char* function = "check_x (%1%)";
  var result;
  Eigen::Matrix<var,Eigen::Dynamic,1> x;
  
  result = 0;
  x.resize(3);
  x << -1, 0, 1;
  ASSERT_TRUE (check_x (function, x, &result, default_policy())) << "check_x should be true with finite x";

  result = 0;
  x.resize(3);
  x << -1, 0, std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on Inf";

  result = 0;
  x.resize(3);
  x << -1, 0, -std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on -Inf";
  
  result = 0;
  x.resize(3);
  x << -1, 0, std::numeric_limits<var>::quiet_NaN();
  EXPECT_THROW (check_x (function, x, &result, default_policy()), std::domain_error) << "check_x should throw exception on NaN";
}

TEST(ProbDistributionsErrorHandling,CheckXMatrixErrnoPolicy) {
  const char* function = "check_x (%1%)";
  var result;
  Eigen::Matrix<var,Eigen::Dynamic,1> x;
  
  result = 0;
  x.resize(3);
  x << -1, 0, 1;
  EXPECT_TRUE (check_x (function, x, &result, errno_policy())) << "check_x should be true with finite x";

  result = 0;
  x.resize(3);
  x << -1, 0, std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on Inf";
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";

  result = 0;
  x.resize(3);
  x << -1, 0, -std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on -Inf";
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";

  result = 0;
  x.resize(3);
  x << -1, 0, std::numeric_limits<var>::quiet_NaN(); 
  EXPECT_FALSE (check_x (function, x, &result, errno_policy())) << "check_x should return FALSE on NaN";
  EXPECT_TRUE (std::isnan (result)) << "check_x should have returned NaN";
  }

// ---------- check_bounded_x tests ----------
TEST(ProbDistributionsErrorHandling,CheckBoundedXDefaultPolicyX) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;
 
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;
  
  x = low;
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " equal to the lower bound: " << low;

  x = high;
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " equal to the lower bound: " << low;

  x = low-1;
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;  
  
  x = high+1;
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;

  x = std::numeric_limits<var>::quiet_NaN();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;

  x = -std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;

  x = std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;
  
}
TEST(ProbDistributionsErrorHandling,CheckBoundedXDefaultPolicyLow) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;
 
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) << "check_bounded_x should be true x: " << x << " and bounds: " << low << ", " << high;
  
  low = -std::numeric_limits<var>::infinity();
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;

  low = std::numeric_limits<var>::quiet_NaN();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;

  low = std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;
}
TEST(ProbDistributionsErrorHandling,CheckBoundedXDefaultPolicyHigh) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;
 
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) << "check_bounded_x should be true x: " << x << " and bounds: " << low << ", " << high;

  high = std::numeric_limits<var>::infinity();
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, default_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;
  
  high = std::numeric_limits<var>::quiet_NaN();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;

  high = -std::numeric_limits<var>::infinity();
  EXPECT_THROW (check_bounded_x (function, x, low, high, &result, default_policy()), std::domain_error) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;

}


TEST(ProbDistributionsErrorHandling,CheckBoundedXErrnoPolicyX) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;
 
  result = 0;
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;

  result = 0;
  x = low;
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " equal to the lower bound: " << low;

  result = 0;
  x = high;
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " equal to the lower bound: " << low;

  result = 0;
  x = low-1;
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy()))
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;  
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;


  result = 0;
  x = high+1;
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy()))
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;

  result = 0;
  x = std::numeric_limits<var>::quiet_NaN();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy()))
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;

  result = 0;
  x = -std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy()))
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;

  result = 0;
  x = std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy()))
    << "check_bounded_x should throw with x: " << x << " and bounds: " << high << ", " << low;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;
}
TEST(ProbDistributionsErrorHandling,CheckBoundedXErrnoPolicyLow) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;

  result = 0; 
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) << "check_bounded_x should be true x: " << x << " and bounds: " << low << ", " << high;

  result = 0;
  low = -std::numeric_limits<var>::infinity();
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;

  result = 0;
  low = std::numeric_limits<var>::quiet_NaN();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;
 
  result = 0;
  low = std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;
}
TEST(ProbDistributionsErrorHandling,CheckBoundedXErrnoPolicyHigh) {
  const char* function = "check_bounded_x (%1%)";
  var x = 0;
  var low = -1;
  var high = 1;
  var result;

  result = 0; 
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) << "check_bounded_x should be true x: " << x << " and bounds: " << low << ", " << high;

  result = 0;
  high = std::numeric_limits<var>::infinity();
  EXPECT_TRUE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should be TRUE with x: " << x << " and bounds: " << low << ", " << high;
  
  result = 0;
  high = std::numeric_limits<var>::quiet_NaN();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;

  result = 0;
  high = -std::numeric_limits<var>::infinity();
  EXPECT_FALSE (check_bounded_x (function, x, low, high, &result, errno_policy())) 
    << "check_bounded_x should throw with x: " << x << " and bounds: " << low << ", " << high;
  EXPECT_TRUE (std::isnan (result)) << "check_bounded_x should set return value to NaN: " << result;
}

// ----------  ----------
//TEST(ProbDistributionsErrorHandling,)
