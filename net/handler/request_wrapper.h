
#ifndef NET_REQUEST_WRAPPER_H
#define NET_REQUEST_WRAPPER_H

#include "handlers.h"

#include <string>
#include <memory>
#include <utility>
#include <map>
#include <functional>
#include <exception>

namespace Net::Handler {

class RequestWrapper {

	boost::beast::http::verb m;
	std::shared_ptr<IRequest> ptr;

public:

	static std::shared_ptr<RequestWrapper> wrap(boost::beast::http::verb method, IRequest::handler handle) {
		static std::shared_ptr<RequestWrapper> wrapped_req(new RequestWrapper(method, handle));
		return wrapped_req;
	}

	RequestWrapper(const RequestWrapper&) = delete;
	RequestWrapper(RequestWrapper&&) = delete;
	const RequestWrapper& operator=(const RequestWrapper&) = delete;
	RequestWrapper&& operator=(RequestWrapper&&) = delete;

	~RequestWrapper() {
		spdlog::info("RequestWrapper class destructor");
	}

	static IRequest::handler unwrap(std::shared_ptr<RequestWrapper> req)
	{
		return req->unwrap();
	}

	Handler::http::message_generator execute(Handler::http::request<Handler::http::string_body>&& req, const std::string& doc_root) {

        try 
        {
            decltype(auto) handle = this->unwrap();
            return handle(std::move(req), doc_root);
        }
        catch (std::exception& ex) 
        {
            return IRequest::wrong_request(ex.what(), req);
        }
	}

private:

	RequestWrapper(boost::beast::http::verb method, IRequest::handler handle) :
		m(method)
	{
		switch (m)
		{
			case boost::beast::http::verb::get: {
				ptr = std::make_shared<GetRequest>(handle);
				break;
			}
			case boost::beast::http::verb::post: {
				ptr = std::make_shared<PostRequest>(handle);
				break;
			}
			// TODO: add other requests handlers
			default: {
                spdlog::info("Undefined HTTP-method");
				break;
			}
		}

		spdlog::info("RequestWrapper class constructor");
	}

	IRequest::handler unwrap() {
		
		switch (m)
		{
			case boost::beast::http::verb::get: {
				return static_cast<GetRequest*>(ptr.get())->handle;
			}
			case boost::beast::http::verb::post: {
				return static_cast<PostRequest*>(ptr.get())->handle;
			}
			// TODO: add other requests handlers
			default: {
                throw std::invalid_argument("Undefined HTTP-method");
			}
		}
	}
};

}

#endif // NET_REQUEST_WRAPPER_H