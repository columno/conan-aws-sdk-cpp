#include <iostream>
#include <memory>

#include <aws/core/Aws.h>
#include <aws/core/client/DefaultRetryStrategy.h>

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/utils/Outcome.h>

#include <aws/dynamodb/DynamoDBClient.h>

namespace
{
  class NoRetryStrategy : public Aws::Client::DefaultRetryStrategy
  {
  public:
    long GetMaxAttempts() const override
    {
      return 0;
    }

    bool ShouldRetry(const Aws::Client::AWSError<Aws::Client::CoreErrors>&, long) const override
    {
      return false;
    }

    long CalculateDelayBeforeNextRetry(const Aws::Client::AWSError<Aws::Client::CoreErrors>&,
                                       long) const override
    {
      return 0;
    }
  };
} // namespace

int main() {
  Aws::SDKOptions options;
  Aws::InitAPI(options);

  auto config = Aws::Client::ClientConfiguration();
  config.connectTimeoutMs = 300;
  config.requestTimeoutMs = 300;
  config.region = "eu-west-1";
  config.endpointOverride = "localhost:8000/";
  config.scheme = Aws::Http::Scheme::HTTP;
  config.retryStrategy = std::make_shared<NoRetryStrategy>();

  auto client = Aws::DynamoDB::DynamoDBClient(Aws::Auth::AWSCredentials{"something", "arbitrary"},
                                            config);

  Aws::ShutdownAPI(options);
  return 0;
}

