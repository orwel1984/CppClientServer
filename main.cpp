#include <iostream>
#include <list>
#include <string>
#include <locale>         // std::locale, std::tolower
#include <algorithm>      // std::transform


class Node
{
  public:
    Node(std::string name, std::list<Node> children = {})
    {
        name_ = name;
        children_=children;
    };
    
    static int cToLower(int c) {
        return tolower(c);
    }

    int count_nodes_containing_string(std::string needle) const
    {
        int count = 0;
        
        // 1. check this node - case insensitive comparison
        std::string lowerName = name_;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), cToLower);
        
        std::string lowerNeedle = needle;
        std::transform(lowerNeedle.begin(), lowerNeedle.end(), lowerNeedle.begin(), cToLower);
        
        if (lowerName.find(lowerNeedle) != std::string::npos) {
            count++;
        }
        
        // 2. check the children recursively
        for (const auto& child : children_) {
            count += child.count_nodes_containing_string(needle);
        }
        
        return count;
    }
    
private:
    std::string name_;
    std::list<Node> children_;
};

int main()
{
    //Create an example tree
    Node n("root",{
        {"MagaZino",{
            {"I"},
            {"Love"},
            {"magazino"}
            }},
        {"Hello",{
            {"Hello",{
                {"Hello",{
                    {"World"}
                }}
            }}
        }}
    });
    //Cout the solution
    std::cout << n.count_nodes_containing_string("test")<<std::endl;
    std::cout << n.count_nodes_containing_string("hello")<<std::endl;
    std::cout << n.count_nodes_containing_string("magazino")<<std::endl;
}
