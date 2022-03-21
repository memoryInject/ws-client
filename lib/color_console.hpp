/*
 * Color Console - header only cpp utility for prettier console outputs
 * more info on coloring bash consoles - https://misc.flogisoft.com/bash/tip_colors_and_formatting
 * */

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class ColorConsole {
private:
    std::unordered_map<std::string, std::vector<std::string>> _default_formats = {
        { "log_format", { "0" } },
        { "info_format", { "92" } },
        { "debug_format", { "94" } },
        { "warn_format", { "33" } },
        { "error_format", { "31" } },
    };
    std::vector<std::string> log_format = _default_formats["log_format"];
    std::vector<std::string> info_format = _default_formats["info_format"];
    std::vector<std::string> debug_format = _default_formats["debug_format"];
    std::vector<std::string> warn_format = _default_formats["warn_format"];
    std::vector<std::string> error_format = _default_formats["error_format"];

public:
    // Format and reset all
    std::string reset = "0";
    std::string bold = "1";
    std::string dim = "2";
    std::string underline = "4";
    std::string blink = "5";
    std::string invert = "7";
    std::string hide = "8";

    // Forground (text)
    std::string black = "30";
    std::string red = "31";
    std::string green = "32";
    std::string yellow = "33";
    std::string blue = "34";
    std::string magenta = "35";
    std::string cyan = "36";
    std::string gray = "37";
    std::string dark_gray = "90";
    std::string light_red = "91";
    std::string light_green = "92";
    std::string light_yellow = "93";
    std::string light_blue = "94";
    std::string light_magenta = "95";
    std::string light_cyan = "96";
    std::string white = "97";

    // Background
    std::string bg_black = "40";
    std::string bg_red = "41";
    std::string bg_green = "42";
    std::string bg_yellow = "43";
    std::string bg_blue = "44";
    std::string bg_magenta = "45";
    std::string bg_cyan = "46";
    std::string bg_gray = "47";
    std::string bg_dark_gray = "100";
    std::string bg_light_red = "101";
    std::string bg_light_green = "102";
    std::string bg_light_yellow = "103";
    std::string bg_light_blue = "104";
    std::string bg_light_magenta = "105";
    std::string bg_light_cyan = "106";
    std::string bg_white = "107";

    // Get formated console string
    std::string get(std::string s, std::vector<std::string> color_codes)
    {
        if (color_codes.empty())
            return s;

        std::string format_start = "\e[";

        if (color_codes.size() == 1) {
            format_start = format_start + color_codes[0] + "m";
        } else {
            for (auto i : color_codes) {
                format_start = format_start + i + ";";
            }
            format_start.replace(
                format_start.end() - 1, format_start.end(), "m");
        }

        std::string format_end = "\e[" + this->reset + "m";

        std::string formatted = format_start + s + format_end;

        return formatted;
    }

    // Print string based on custom color code vector
    // eg: print("Hello World!", {console.red, console.bold}) -> this will
    // output red bold 'Hello World!' on console output.
    void print(std::string s, std::vector<std::string> color_codes)
    {
        std::cout << this->get(s, color_codes) << std::endl;
    }

    // Presets
    void log(std::string s)
    {
        this->print(s, this->log_format);
    }

    void info(std::string s)
    {
        this->print(s, this->info_format);
    }

    void debug(std::string s)
    {
        this->print(s, this->debug_format);
    }

    void warn(std::string s)
    {
        this->print(s, this->warn_format);
    }

    void error(std::string s)
    {
        this->print(s, this->error_format);
    }

    void warning(std::string s)
    {
        this->print(s, this->warn_format);
    }

    // Change default preset behaviour
    void set_log(std::vector<std::string> color_codes)
    {
        this->log_format = color_codes;
    }

    void set_info(std::vector<std::string> color_codes)
    {
        this->info_format = color_codes;
    }

    void set_debug(std::vector<std::string> color_codes)
    {
        this->debug_format = color_codes;
    }

    void set_warn(std::vector<std::string> color_codes)
    {
        this->warn_format = color_codes;
    }

    void set_warning(std::vector<std::string> color_codes)
    {
        this->set_warn(color_codes);
    }

    void set_error(std::vector<std::string> color_codes)
    {
        this->error_format = color_codes;
    }

    // Resest presets to defaults
    void reset_log()
    {
        this->log_format = this->_default_formats["log_format"];
    }

    void reset_info()
    {
        this->info_format = this->_default_formats["info_format"];
    }

    void reset_debug()
    {
        this->debug_format = this->_default_formats["debug_format"];
    }

    void reset_warn()
    {
        this->warn_format = this->_default_formats["warn_format"];
    }

    void reset_warning()
    {
        this->reset_warn();
    }

    void reset_error()
    {
        this->error_format = this->_default_formats["error_format"];
    }

    void reset_all()
    {
        this->reset_log();
        this->reset_info();
        this->reset_debug();
        this->reset_warn();
        this->reset_error();
    }

    // Constructor
    ColorConsole() {};

    // Destructor
    ~ColorConsole() {};
};

ColorConsole console;
