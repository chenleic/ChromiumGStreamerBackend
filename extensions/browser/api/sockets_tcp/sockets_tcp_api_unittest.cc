// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "content/public/test/test_browser_context.h"
#include "extensions/browser/api/api_resource_manager.h"
#include "extensions/browser/api/socket/socket.h"
#include "extensions/browser/api/socket/tcp_socket.h"
#include "extensions/browser/api/sockets_tcp/sockets_tcp_api.h"
#include "extensions/browser/api_unittest.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace extensions {
namespace api {

static scoped_ptr<KeyedService> ApiResourceManagerTestFactory(
    content::BrowserContext* context) {
  return make_scoped_ptr(new ApiResourceManager<ResumableTCPSocket>(context));
}

class SocketsTcpUnitTest : public ApiUnitTest {
 public:
  void SetUp() override {
    ApiUnitTest::SetUp();

    ApiResourceManager<ResumableTCPSocket>::GetFactoryInstance()
        ->SetTestingFactoryAndUse(browser_context(),
                                  ApiResourceManagerTestFactory);
  }
};

TEST_F(SocketsTcpUnitTest, Create) {
  // Get BrowserThread
  content::BrowserThread::ID id;
  CHECK(content::BrowserThread::GetCurrentThreadIdentifier(&id));

  // Create SocketCreateFunction and put it on BrowserThread
  SocketsTcpCreateFunction* function = new SocketsTcpCreateFunction();
  function->set_work_thread_id(id);

  // Run tests
  scoped_ptr<base::DictionaryValue> result(RunFunctionAndReturnDictionary(
      function, "[{\"persistent\": true, \"name\": \"foo\"}]"));
  ASSERT_TRUE(result.get());
}

}  // namespace api
}  // namespace extensions
