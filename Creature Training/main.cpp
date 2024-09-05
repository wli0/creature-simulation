#include "Cavern.hpp"

int main()
{
Cavern c("creatures.csv");
c.initializeMysticalStack();
std::stack<Creature*> s = c.getMysticalStack();
c.setAttacks(s);

while(!s.empty())
{
    Creature* c = s.top();
    s.pop();
    std::cout << c->getName() << " Attacks: " << std::endl;
    std::queue<Attack> q = c->getAttackQueue();
    while (!q.empty())
    {
        Attack a = q.front();
        q.pop();
        std::cout << "Name: " << a.name_ << std::endl;
        std::cout << "Type: ";
        for (int i = 0; i < a.type_.size(); i++)
        {
            std::cout << a.type_[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Damage: ";
        for (int i = 0; i < a.damage_.size(); i++)
        {
            std::cout << a.damage_[i] << " ";
        }
        std::cout << std::endl;
    }
}


return 0;

}