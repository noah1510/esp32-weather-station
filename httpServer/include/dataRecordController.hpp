#pragma once

#include <drogon/HttpSimpleController.h>

#include "jsonRecorder.hpp"
#include <iostream>

namespace sakurajin {
    class dataRecordController : public drogon::HttpSimpleController<dataRecordController> {
      public:
        virtual void asyncHandleHttpRequest(const drogon::HttpRequestPtr&                         req,
                                            std::function<void(const drogon::HttpResponsePtr&)>&& callback) override;

        PATH_LIST_BEGIN
        PATH_ADD("/record", drogon::HttpMethod::Post);
        PATH_LIST_END
    };

}
