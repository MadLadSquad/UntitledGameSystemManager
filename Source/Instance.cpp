#include "Instance.hpp"

UntitledLinuxGameManager::Instance::Instance()
{
    initInfo = UImGui::InitInfo
    {
        .titlebarComponents =
        {
            &mainBar
        },
        .windowComponents =
        {
            &mainView,
            &sidePanel,
            &pin,
            &poweroff,
            &poweron,
            &restart,
            &update,
            &exitWidget,
            &newContainer,
            &showDirectories,
            &about,
        },
    };
}

void UntitledLinuxGameManager::Instance::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Instance::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Instance::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Instance::~Instance()
{

}


struct Node
{
    bool operator<(const Node& a) const
    {
        return index < a.index;
    }

    std::string expression;
    size_t index;
    char prevOperator;
    std::vector<Node*> next;
};

bool operator<(const Node& a, const Node& b)
{
    return a.index < b.index;
}

void findSubexpressions(Node* head, const std::string& expr, size_t i, size_t totalSize)
{
    static bool bFirst = true;
    std::string accum;

    auto* n = new Node();
    head->next.push_back(n);
    head->next.back()->index = i;
    if (bFirst)
        bFirst = false;
    else
        head->next.back()->prevOperator = expr[i - 2]; // Go back to the ( then behind it
    for (; i < totalSize; i++)
    {
        if (expr[i] == '(')
            findSubexpressions(expr, i+1, totalSize);
        else if (expr[i] == ')')
            break;
        else
            head->next.back() += expr[i];
    }
}

void UntitledLinuxGameManager::Instance::onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io)
{
    std::string test = "15+25-(18/4+27/(27+12))/(24*14-23)";
    std::string test2 = "(((1+1)+2+(((1+1)+((1+1)+2)+3)+((1+1)+2)+3)+4)+((1+1)+2)+(1+1))+((1+1)+2)+3)+12";
    Node head;
    findSubexpressions(&head, test, 0, test.size());
}