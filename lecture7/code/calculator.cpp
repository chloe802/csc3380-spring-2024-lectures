#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <cmath>

std::unordered_map<std::string, double> variables;// store variables

double evaluate_expression(const std::string& expr);// evaluate the expression

double get_number(std::istringstream& iss) {// get the number
    double num;
    iss >> num;// get the number
    return num;
}

double get_term(std::istringstream& iss) {// get the term
    double result = get_number(iss);// get the first number
    char op;// store the operator
    while (iss >> op) {// while there is an operator
        if (op == '*' || op == '/') {// if the operator is * or /
            double num = get_number(iss);// get the next number
            if (op == '*')// multiply the number
                result *= num;// multiply the number
            else if (num == 0)// if the number is zero, throw an exception
                throw std::runtime_error("division by zero");// throw an exception
            else
                result /= num;// divide the number
        } else {
            iss.putback(op);// put the operator back
            break;
        }
    }
    return result;
}

double evaluate_expression(const std::string& expr) {// evaluate the expression
    std::istringstream iss(expr);// create input string stream
    double result = get_term(iss);// get the first term
    char op;// store the operator
    while (iss >> op) {// while there is an operator
        if (op == '+' || op == '-') {// if the operator is + or -
            double term = get_term(iss);// get the next term
            if (op == '+')// add or subtract the term
                result += term;// add the term
            else
                result -= term;// subtract the term
        } else if (op == '%') {// if the operator is %
            double divisor = get_term(iss);// get the divisor
            if (divisor == 0)// if the divisor is zero, throw an exception
                throw std::runtime_error("modulus by zero");// throw an exception
            result = fmod(result, divisor);// calculate the modulus
        }

    }
    return result;
}


int main() {
    std::cout << "Simple Calculator" << std::endl;
    std::cout << "Supported operators: +, -, *, /, %" << std::endl;
    std::cout << "Enter expressions to evaluate or assign variables using the format: *variable_name* =value" << std::endl;
    std::cout << "For example, to assign the value 5 to variable x, enter: x = 5 or y = -9 " << std::endl;

    while (true) {// loop until user enters 'q'
        std::string input;// store user input
        std::cout << "Enter expression (or 'q' to quit): ";// prompt
        std::getline(std::cin, input);// get user input

        if (input == "q")// if user enters 'q', break the loop
            break;

        size_t assignPos = input.find('=');// check if there's an assignment statement

        if (assignPos != std::string::npos) {// if there's an assignment statement
            std::string variableName = input.substr(0, assignPos);// extract variable name
            std::string valueStr = input.substr(assignPos + 1);// extract value as string

            try {// try to convert the value string to a double
                double value = std::stod(valueStr);// convert value string to double
                variables[variableName] = value;// update the variable map
                std::cout << "Assigned value " << value << " to variable " << variableName << std::endl;
            } catch (const std::exception& e) {// catch conversion errors
                std::cerr << "Error: Invalid value for variable " << variableName << std::endl;
            }
        } else {// if there's no assignment statement, evaluate the expression
            try {// try to evaluate the expression
                for (auto& pair : variables) {// replace variables with their values
                    std::string variable_name = pair.first;// get variable name
                    double variable_value = pair.second;// get variable value
                    size_t pos = input.find(variable_name);// find variable name in input
                    while (pos != std::string::npos) {// while variable name is found
                        input.replace(pos, variable_name.length(), std::to_string(variable_value));// replace variable name with its value
                        pos = input.find(variable_name, pos + 1);// find next occurrence of variable name
                    }
                }

                double result = evaluate_expression(input);// evaluate the expression
                std::cout << "Result: " << result << std::endl;// print the result
            } catch (const std::exception& e) {// catch exceptions
                std::cerr << "Error: " << e.what() << std::endl;// print error message
            }
        }
    }

    return 0;
}