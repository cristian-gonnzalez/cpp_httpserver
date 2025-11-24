#include <iostream>
#include <asio.hpp>
#include <memory>

#include "server.h"
#include "log_director.h"
#include "console_logger.h"

int main()
{
    // Setup logging
    auto logger = std::make_shared<app::log::CLogger>();
    auto* director = app::log::LogDirector::get();
    director->add(logger);
    director->set_level(app::log::LogLevel::fatal);

    try {
        asio::io_context ctx;

        // Setup async signal handler 
        asio::signal_set signals(ctx);
        signals.add(SIGINT);
        signals.add(SIGTERM);

        signals.async_wait( [&](const asio::error_code&, int s)
                            {
                                std::cout << "Received signal: " << s << "\n";
                                ctx.stop();            // triggers shutdown
                            } );

        auto server = std::make_shared<Server>(8080);

        // Run server in background thread
        std::jthread worker( [&]() 
                             {
                                server->run();
                             });

        // Runs until signal arrives
        ctx.run();

        // Clean shutdown
        std::cout << "Stopping server...\n";
        server->stop();
        std::cout << "Exit.\n";

    } catch (std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
