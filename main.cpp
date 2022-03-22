#include "lib/color_console.hpp"
#include "lib/easywsclient.hpp"
#include "lib/json.hpp"
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;
using easywsclient::WebSocket;
using json = nlohmann::json;

atomic<bool> run_loop(true); // Run thread loops
atomic<bool> close_ws(false); // control websocket
mutex guard; // to protect ws

// For console exit keywords
unordered_set<string> exit_key({ "q", "quit", "exit" });

// For help keywords
unordered_set<string> help_key({ "h", "help" });

// For reconnect ws
unordered_set<string> reconnect_key({ "r", "reset", "reconnect", "restart" });


void websocket(string uri)
{
    while(run_loop) {
        WebSocket::pointer ws = WebSocket::from_url(uri);

        if (ws == NULL) {
            //console.error("WebSocket not connected: " + uri);
            console.info("Trying to reconnect in 5 sec..");
            this_thread::sleep_for(5000ms);
            //console.log("Reconnect: " + uri);
            continue;
        }

        assert(ws);

        console.print("Connected: " + uri, { console.invert });
        //ws->send("goodbye");
        //ws->send("hello");
        while (ws->getReadyState() != WebSocket::CLOSED) {
            ws->poll();
            ws->dispatch([ws](string message) {
                // Check if the message in JSON
                try {
                    json j_complete = json::parse(message);
                    console.info("\n>>> JSON Data:");
                    cout << setw(4) << j_complete << endl;
                } catch (const exception& e) {
                    cout
                        << console.get("\n>>> ", { console.light_blue })
                        << message << endl;
                }
            });

            if (close_ws == true) {
                ws->close();
            }
        }
        close_ws = false;
        delete ws;
        //run_loop = false;
    }
}

void keybord()
{
    while (run_loop) {
        string keyword;
        cin >> keyword;
        if (exit_key.find(keyword) != exit_key.end()) {
            //{
                //const lock_guard<mutex> lock(guard);
                //ws->close();
            //}
            console.debug("Exit");
            close_ws = true;
            run_loop = false;
        }

        if (reconnect_key.find(keyword) != exit_key.end()) {
            console.debug("Reset connection");
            close_ws = true;
        }

        if (help_key.find(keyword) != help_key.end()) {
            console.print("\nHelp ws-client v0.1", { console.invert });
            console.log("More info: https://github.com/memoryInject/ws-client");
        }
    }
}

int main(int argc, char* argv[])
{
    string uri;
    if (argc > 1) {
        uri = argv[1];
    } else {
        uri = "ws://local";
    }


    thread keyListener(keybord);
    thread wsListener(websocket, uri);

    keyListener.join();
    wsListener.join();

    if (close_ws) {
        string msg = "Web Socket Closed: " + uri;
        console.warn(msg);
    }


    return 0;
}
