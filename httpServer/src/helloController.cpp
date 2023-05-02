#include "helloController.hpp"

void sakurajin::helloController::asyncHandleHttpRequest(const drogon::HttpRequestPtr&                         req,
                                                        std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    std::cout << req->body() << std::endl;

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("<p>Hello, World!</p>");
    callback(resp);
}