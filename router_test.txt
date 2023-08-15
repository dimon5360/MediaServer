
#include <type_traits>
#include <memory>
#include <map>
#include <functional>

class IRequest {

public:

	using handler = std::function<int(const std::string&)>;

	IRequest() {
		std::cout << "IRequest class constructor\n";
	};
	
	virtual ~IRequest() {
		std::cout << "IRequest class destructor\n";
	}
};

class GetRequest : public IRequest {

public:

	GetRequest(IRequest::handler handle) : IRequest()
	{
		std::cout << "GetRequest class constructor\n";

		this->handle = handle;
	}

	~GetRequest() {
		std::cout << "GetRequest class destructor\n";
	}

	IRequest::handler handle = {};
};


class PostRequest : public IRequest {

public:

	PostRequest(IRequest::handler handle) : IRequest() {
		std::cout << "PostRequest class constructor\n";

		this->handle = handle;
	}

	~PostRequest() {
		std::cout << "PostRequest class destructor\n";
	}

	IRequest::handler handle = {};
};

enum class Method {
	GET, 
	POST,
};

class RequestWrapper {

	Method m;
	std::shared_ptr<IRequest> ptr;

public:

	static std::shared_ptr<RequestWrapper> wrap(Method method, IRequest::handler handle) {
		static std::shared_ptr<RequestWrapper> wrapped_req(new RequestWrapper(method, handle));
		return wrapped_req;
	}

	RequestWrapper(const RequestWrapper&) = delete;
	RequestWrapper(RequestWrapper&&) = delete;
	const RequestWrapper& operator=(const RequestWrapper&) = delete;
	RequestWrapper&& operator=(RequestWrapper&&) = delete;

	~RequestWrapper() {
		std::cout << "RequestWrapper class destructor\n";
	}

	static IRequest::handler unwrap(std::shared_ptr<RequestWrapper> req)
	{
		return req->unwrap();
	}

	decltype(auto) execute(const std::string& req) {
		decltype(auto) handle = this->unwrap();
		return handle(req);
	}

private:

	RequestWrapper(Method method, IRequest::handler handle) :
		m(method)
	{
		switch (m)
		{
			case Method::GET: {
				ptr = std::make_shared<GetRequest>(handle);
				break;
			}
			case Method::POST: {
				ptr = std::make_shared<PostRequest>(handle);
				break;
			}
			// TODO: add other requests handlers
			default: {
				std::cout << "Undefined HTTP-method\n";
				break;
			}
		}

		std::cout << "RequestWrapper class constructor\n";
	}

	IRequest::handler unwrap() {
		
		switch (m)
		{
			case Method::GET: {
				return static_cast<GetRequest*>(ptr.get())->handle;
			}
			case Method::POST: {
				return static_cast<PostRequest*>(ptr.get())->handle;
			}
			// TODO: add other requests handlers
			default: {
				return [](const std::string&) -> int {
					std::cout << "Undefined HTTP-method\n"; 
					return 0;
				};
			}
		}
	}
};

class Router {

	Router() {
		std::cout << "Router class constructor\n";
	}

public:

	static Router& instance() {
		static std::shared_ptr<Router> router(new Router());
		return *router;
	}

	Router(const Router&) = delete;
	Router(Router&&) = delete;
	const Router& operator=(const Router&) = delete;
	Router&& operator=(Router&&) = delete;


	template<Method method>
	void setup_route(const std::string& api, IRequest::handler handle) {
		m_.insert({ api, RequestWrapper::wrap(method, handle) });
	}

	std::shared_ptr<RequestWrapper> operator[](const std::string& api) {
		if (!m_.contains(api)) {
			std::cout << "Undefined API path\n";
			return nullptr;
		}

		return m_.at(api);
	}

private:

	std::map<std::string, std::shared_ptr<RequestWrapper>> m_;
};

void test_router() {

	decltype(auto) router = Router::instance();

	router.setup_route<Method::GET>("/",
		[](const std::string& req) -> int {
			std::cout << "Handling GetRequest:\n";
			std::cout << req << std::endl;
			return 0;
		});

	router.setup_route<Method::POST>("/new/",
		[](const std::string& req) -> int {
			std::cout << "Handling PostRequest:\n";
			std::cout << req << std::endl;
			return 0;
		});

	router["/"]->execute("test get request");
	router["/new/"]->execute("test post request");
}
