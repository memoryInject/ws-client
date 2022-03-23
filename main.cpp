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
#include <chrono>
#include <mutex>
#include <thread>

using namespace std;
using namespace std::chrono_literals;
using easywsclient::WebSocket;
using json = nlohmann::json;

atomic<bool> run_loop(true);  // Run thread loops
atomic<bool> close_ws(false); // control websocket
atomic<bool> send_ws(false);  // send message to WebSocket
mutex guard;                  // to protect ws

// For console exit keywords
unordered_set<string> exit_key({ "q", "quit", "exit" });

// For help keywords
unordered_set<string> help_key({ "h", "help" });

// For reconnect ws
unordered_set<string> reconnect_key({ "r", "reset", "reconnect", "restart" });

// For sending messages
unordered_set<string> message_key({ "res", "send", "response" });

// Message for send
string send_msg;

// Pretty print format for JSON and plain string
void print_message(string message, bool send)
{
    string icon = "\n>>> ";
    if (send) {
        icon = "\n<<< ";
    }

    // Check if the message in JSON
    try {
        json j_complete = json::parse(message);
        console.info(icon + "JSON Data:");
        cout << setw(4) << j_complete << endl;
    } catch (const exception& e) {
        cout
            << console.get(icon, { console.light_blue })
            << message << endl;
    }
}

void websocket(string uri)
{
    while (run_loop) {
        WebSocket::pointer ws = WebSocket::from_url(uri);

        if (ws == NULL) {
            // Animation in console
            // more info: https://stackoverflow.com/questions/8486181/how-to-make-a-loading-animation-in-console-application-written-in-c
            cout << console.get(
                "Trying to reconnect in:",
                { console.light_magenta, console.underline })
                 << "  ";
            int wait_for = 5;
            for (int i = 0; i < wait_for; i++) {
                if (close_ws) {
                    break;
                }

                cout << "\b\b" + to_string(wait_for - i) + "s" << flush;
                this_thread::sleep_for(1000ms);
            }
            if (!close_ws) {
                cout << "\b\b0s" << endl;
            }

            continue;
        }

        assert(ws);

        console.print("Connected: " + uri, { console.invert });
        //ws->send("goodbye");
        //ws->send("hello");
        while (ws->getReadyState() != WebSocket::CLOSED) {
            ws->poll();
            ws->dispatch([ws](string message) {
                print_message(message, false);
            });

            if (send_ws) {
                print_message(send_msg, true);

                ws->send(send_msg);
                send_ws = false;
            }

            if (close_ws) {
                ws->close();
                close_ws = false;
            }
        }
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
            console.debug("Exit");
            close_ws = true;
            run_loop = false;
        }

        if (reconnect_key.find(keyword) != reconnect_key.end()) {
            console.debug("Reset connection");
            close_ws = true;
        }

        if (message_key.find(keyword) != message_key.end()) {
            if (!send_ws) {
                send_msg = "";
                char c;

                cout << "Message: ";
                // To get raw input from user
                // ref: https://stackoverflow.com/questions/53797121/how-to-read-raw-input-in-cpp
                cin.get(c); // To skip first '\n' when enter 'res'
                while (cin.get(c)) {
                    if (c == '\n') {
                        break;
                    }
                    send_msg.push_back(c);
                }
                send_ws = true;
            } else {
                console.warn("WebSocket is busy...");
            }
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

    string msg = "Web Socket Closed: " + uri;
    console.warn(msg);

    return 0;
}
