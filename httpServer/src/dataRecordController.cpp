#include "dataRecordController.hpp"

void sakurajin::dataRecordController::asyncHandleHttpRequest(const drogon::HttpRequestPtr&                         req,
                                                        std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    if(req->contentType() != drogon::ContentType::CT_APPLICATION_JSON){
        LOG_INFO << "got the wrong content type in record path";

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("<p>Wrong content type</p>");
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    auto jsonPtr = req->getJsonObject();
    const auto& jsonData = *jsonPtr;
    if(!jsonData.isObject() && !jsonData.isArray()){
        LOG_INFO << "got the wrong json data";

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("<p>Wrong json data</p>");
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    try{
        sakurajin::jsonRecorder::record(jsonData, "./dataRecords/data.json");
    }catch(const std::exception& e){
        LOG_INFO << "failed to record data: " << e.what();

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("<p>Failed to record data</p>");
        resp->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
        callback(resp);
        return;
    }

    std::cout << "finished at: " << sakurajin::jsonRecorder::currentTimestamp() << std::endl;
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("<p>Got new data</p>");
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    callback(resp);
}