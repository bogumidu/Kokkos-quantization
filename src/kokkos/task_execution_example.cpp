

#include <fstream>
#include <iostream>

enum json_token {
    TOKEN_OPEN_OBJECT     = '{',
    TOKEN_CLOSE_OBJECT    = '}',
    TOKEN_OPEN_ARRAY      = '[',
    TOKEN_CLOSE_ARRAY     = ']',
    TOKEN_COMMA          = ',',
    TOKEN_COLON          = ':',
    TOKEN_DOUBLE_QUOTE    = '"',
    TOKEN_BACKSLASH      = '\\',
    TOKEN_SLASH          = '/',
    TOKEN_ASTERISK       = '*',
    TOKEN_NEWLINE        = '\n',
    TOKEN_SPACE          = ' ',
    TOKEN_CARRIAGE_RETURN = '\r',
    TOKEN_TAB            = '\t',
    TOKEN_FORM_FEED       = '\f',
    TOKEN_BACKSPACE      = '\b',
    TOKEN_EOF            = 0
};

void parseObject(std::ifstream& file, int* depth_);


bool isWhitespace(char c) {
    return c == TOKEN_SPACE || c == TOKEN_NEWLINE || c == TOKEN_CARRIAGE_RETURN || c == TOKEN_TAB;
}


void skipWhitespace(std::ifstream& file) {
    for (;;) {
        if (isWhitespace(file.peek())) {
            file.get();
            continue;
        }
        // comments are not parsed
        break;
    }
}

void parseValue(std::ifstream& file) {
    if (file.peek() == TOKEN_DOUBLE_QUOTE) {
        file.get();
        parseString(file, c);
        return;
    }
    if (file.peek() == TOKEN_OPEN_OBJECT) {
        file.get();
        c++;
        parseObject(file, c);
        return;
    }
    if (file.peek() == TOKEN_OPEN_ARRAY) {
        file.get();
        c++;
        parseArray(file, c);
        return;
    }
    for (;;) {
        if (isWhitespace(file.peek())) {
            file.get();
            c++;
            continue;
        }
        if (file.peek() == TOKEN_COMMA || file.peek() == TOKEN_CLOSE_OBJECT || file.peek() == TOKEN_CLOSE_ARRAY) {
            break;
        }
        file.get();
        c++;
    }
}


void parseObject(std::ifstream& file) {
    int row = 0;
    bool can_start_object = true;
    bool comma = false;
    char c;
    for (;;) {
        c = file.peek();
        if (file.eof()) {
            break;
        }
        skipWhitespace(file);
        if (c == TOKEN_NEWLINE) {
            file.get();
            row++;
            continue;
        }
        if (c == TOKEN_OPEN_OBJECT) {
            if (!can_start_object) {
                std::cout << "Error at row " << row << ": Unexpected token: " << c << std::endl;
                break;
            }
            can_start_object = false;
            file.get();
            continue;
        }
        if (c == TOKEN_CLOSE_OBJECT) {
            if (can_start_object) {
                std::cout << "Error at row " << row << ": Unexpected token: " << c << std::endl;
                break;
            }
            file.get();
            can_start_object = true;
            continue;
        }
        if (c == TOKEN_DOUBLE_QUOTE && (comma || can_start_object)) {
            c = file.get();
            while (c != TOKEN_DOUBLE_QUOTE) {
                if (file.eof()) {
                    std::cout << "Error at row " << row << ": Unexpected token: " << file.get() << std::endl;
                    break;
                }
                file.get();
            }
            skipWhitespace(file);
            if (c != TOKEN_COLON || c != TOKEN_COMMA) {
                std::cout << "Error at row " << row << ": Unexpected token: " << c << std::endl;
                break;
            }
            file.get();
            continue;
        }
        if (c != TOKEN_COLON || c == TOKEN_COMMA) {
            std::cout << "Error at row " << row << ": Unexpected token: " << c << std::endl;
            break;
        }
        if (c == TOKEN_CLOSE_OBJECT && !can_start_object) {
            file.get();
            continue;
        }
        throw std::runtime_error("Expected comma or close object");
    }
}


void skipWhiteSpaces(std::ifstream& file) {
    while (isWhitespace(file.peek())) {
        file.get();
    }
}


int main(int argc, char *argv[]) {

}