#include "crow.h"
#include "crow/logging.h"
#include "crow/middlewares/cors.h"
#include <crow/app.h>
#include <cstdlib>
#include <string>

int run_server() {
  crow::logger::setLogLevel(crow::LogLevel::Debug);
  crow::App<crow::CORSHandler> app;

  CROW_LOG_INFO << "Starting server...";

  auto &cors = app.get_middleware<crow::CORSHandler>();
  cors.global()
      .origin("http://any-origin.com")
      .methods("POST"_method, "OPTIONS"_method)
      .headers("content-type", "some-custom-header");

  const char *port_env = std::getenv("PORT");
  int port = port_env ? std::atoi(port_env) : 8080;

  const char *listen_host_env = std::getenv("LISTEN_HOST");
  std::string listen_host = listen_host_env ? listen_host_env : "0.0.0.0";

  CROW_LOG_INFO << "Server configuration: host=" << listen_host
                << ", port=" << port;

  auto available_cores = std::thread::hardware_concurrency() - 1;
  auto crow_thread_pool_size = available_cores;

  CROW_LOG_INFO << "Thread pool configuration:";
  CROW_LOG_INFO << "  Hardware cores: " << std::thread::hardware_concurrency();
  CROW_LOG_INFO << "  Crow thread pool size: " << crow_thread_pool_size;

  CROW_ROUTE(app, "/hello")
      .methods("POST"_method)([&](const crow::request &req) {
        return crow::response(200, "Hello, world!");
      });

  CROW_LOG_INFO << "Starting server on " << listen_host << ":" << port;

  try {
    app.port(port)
        .bindaddr(listen_host)
        .concurrency(crow_thread_pool_size)
        .run();
  } catch (const std::exception &e) {
    CROW_LOG_ERROR << "Server crashed with exception: " << e.what();
    return 1;
  } catch (...) {
    CROW_LOG_ERROR << "Server crashed with unknown exception";
    return 1;
  }

  return 0;
}

int main() { return run_server(); }
