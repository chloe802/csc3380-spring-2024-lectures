#include <iostream>
#include <stdexcept>

struct token
{
    char kind;       // what kind of token
    double value;    // for numbers: a value

    // constructors
    token(char ch)
            : kind(ch), value(0)
    {
    }
    token(char ch, double val)
            : kind(ch), value(val)
    {
    }
};

class token_stream
{
    // representation: not directly accessible to users:
    bool full;       // is there a token in the buffer?
    token buffer;    // here is where we keep a Token put back using
    // putback()

public:
    // user interface:
    token get();            // get a token
    void putback(token);    // put a token back into the token_stream

    // constructor: make a token_stream, the buffer starts empty
    token_stream()
            : full(false), buffer(0)
    {
    }
};

void token_stream::putback(token t)
{
    if (full)
        throw std::runtime_error("putback() into a full buffer");
    buffer = t;
    full = true;
}

token token_stream::get()    // read a token from the token_stream
{
    if (full)
    {
        full = false;
        return buffer;
    }    // check if we already have a Token ready

    char ch;
    std::cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch)
    {
        case '(': case ')': case ';': case 'q':
        case '+': case '-': case '*': case '/': case '%':
            return token(ch);    // let each character represent itself
        case '.':
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7': case '8':
        case '9':
        {
            std::cin.putback(ch);    // put digit back into the input stream
            double val;
            std::cin >> val;                // read a floating-point number
            return token('8', val);    // let ‘8’ represent “a number”
        }
        default:
            throw std::runtime_error("Bad token");
    }
}

double expression(token_stream& ts);  // declare expression so primary can call it

double primary(token_stream& ts)
{
    token t = ts.get();
    if (t.kind == '(') {    // handle '(' expression ')'
        double d = expression(ts);
        t = ts.get();
        if (t.kind != ')') throw std::runtime_error("')' expected");
        return d;
    } else if (t.kind == '8') { // we use '8' to represent a number
        return t.value;   // return the number's value
    } else {
        throw std::runtime_error("primary expected");
    }
}

double term(token_stream& ts)
{
    double left = primary(ts);
    token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary(ts);
                t = ts.get();
                break;
            case '/':
            {
                double d = primary(ts);
                if (d == 0) throw std::runtime_error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);     // put t back into the
                // token stream
                return left;
        }
    }
}

double expression(token_stream& ts)
{
    double left = term(ts);
    token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
            case '+':
                left += term(ts);    // evaluate term and add
                t = ts.get();
                break;
            case '-':
                left -= term(ts);    // evaluate term and subtract
                t = ts.get();
                break;
            default:
                ts.putback(t);     // put t back into the token stream
                return left;       // finally: no more + or -: return the answer
        }
    }
}

int main()
{
    token_stream ts;// create a token stream

    while (true) {
        try {
            std::cout << "Enter an expression: ";// prompt
            double result = expression(ts);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return 1; // Exit with error code if an error occurs
        }
    }

    return 0;
}