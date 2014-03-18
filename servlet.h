#ifndef __SERVLET_H__
#define __SERVLET_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>

#include "bson/bson_parser.h"

class CBsonObject;
class PlayerSession;
class ServerResource;
class Session;

namespace Windnet {
	namespace Net {
		class Socket;
	}
	class Message;
}

class Servlet {
public:
	typedef boost::shared_ptr<Servlet> ptr;

	virtual void setupServlet()  {}
	virtual bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res,
						   const std::string &token, PlayerSession *ps, Windnet::BSON::Object *request) = 0;
};

class ServletDispatch {
public:
	void init();
	bool dispatchMessage(boost::shared_ptr<ServerResource> res, Windnet::Message *msg);

	bool dispatch(boost::shared_ptr<ServerResource> res, PlayerSession *ps, Windnet::BSON::Object &request);

private:
	std::map<std::string, Servlet::ptr> m_servlets;
};

#endif
