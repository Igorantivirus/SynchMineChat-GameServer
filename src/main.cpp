#include <iostream>
#include <string>

std::string input(std::string str = {})
{
    std::cout << str;
    std::getline(std::cin, str);

    return str;
}

int main()
{
    std::cout << "Hello, world!" << '\n';
    std::string s = input();
    std::cout << "Your input: " << s << '\n';
    
    return 0;
}