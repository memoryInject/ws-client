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

using namespace std;
using easywsclient::WebSocket;
using json = nlohmann::json;

atomic<bool> run_loop(true);
mutex guard; // to protect ws

// For console exit keywords
unordered_set<string> exit_key({ "q", "quit", "exit" });

// For help keywords
unordered_set<string> help_key({ "h", "help" });

void websocket(WebSocket::pointer ws)
{
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
    }
}

void keybord(WebSocket::pointer ws)
{
    while (run_loop) {
        string keyword;
        cin >> keyword;
        if (exit_key.find(keyword) != exit_key.end()) {
            {
                const lock_guard<mutex> lock(guard);
                ws->close();
            }
            run_loop = false;
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
        cout << uri << endl;
    } else {
        uri = "ws://local";
    }

    WebSocket::pointer ws = WebSocket::from_url(uri);
    assert(ws);
    console.print("Connected: " + uri, { console.invert });
    ws->send("goodbye");
    ws->send("hello");

    thread keyListener(keybord, ws);
    thread wsListener(websocket, ws);

    keyListener.join();
    wsListener.join();

    if (ws->getReadyState() == WebSocket::CLOSED) {
        string msg = "Web Socket Closed: " + uri;
        console.warn(msg);
    }

    delete ws;

    return 0;
}
