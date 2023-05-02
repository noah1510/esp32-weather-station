#pragma once

#include <drogon/HttpSimpleController.h>
#include <iostream>

namespace sakurajin {
    class helloController : public drogon::HttpSimpleController<helloController> {
      public:
        virtual void asyncHandleHttpRequest(const drogon::HttpRequestPtr&                         req,
                                            std::function<void(const drogon::HttpResponsePtr&)>&& callback) override;

        PATH_LIST_BEGIN
        PATH_ADD("/hello",drogon::HttpMethod::Get,drogon::HttpMethod::Post);
        PATH_LIST_END
    };

} // namespace sakurajin
