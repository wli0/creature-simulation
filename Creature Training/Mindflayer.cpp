/*
CSCI235 Spring 2024
Project 4 - MycoMorsels
Georgina Woo
Dec 24 2023
Mindflayer.cpp implements the constructors and private and public functions of the Mindflayer class
*/

#include "Mindflayer.hpp"

Mindflayer::Mindflayer() : projectiles_{}, summoning_{false}, affinities_{}
{
    setCategory(ALIEN);
}

Mindflayer::Mindflayer(const std::string& name, Category category, int hitpoints, int level, bool tame, std::vector<Projectile> projectiles, bool summoning, std::vector<Variant> affinities) : Creature(name, category, hitpoints, level, tame)
{
    setProjectiles(projectiles);
    summoning_ = summoning;
    setAffinities(affinities);
}

std::vector<Mindflayer::Projectile> Mindflayer::getProjectiles() const
{
    return projectiles_;
}

void Mindflayer::setProjectiles(const std::vector<Projectile>& projectiles)
{
    std::vector<Projectile> temp;
    for(int i = 0; i < projectiles.size(); i++)
    {
        bool found = false;
        for(int j = 0; j < temp.size(); j++)
        {
            if(projectiles[i].type_ == temp[j].type_)
            {
                if(projectiles[i].quantity_ > 0)
                {
                    temp[j].quantity_ += projectiles[i].quantity_;
                    found = true;
                }
            }
        }
        if(!found)
        {
            if(projectiles[i].quantity_ > 0)
            {
                temp.push_back(projectiles[i]);
            }
        }
    }
    projectiles_ = temp;
}

void Mindflayer::setSummoning(const bool& summoning)
{
    summoning_ = summoning;
}

bool Mindflayer::getSummoning() const
{
    return summoning_;
}

std::vector<Mindflayer::Variant> Mindflayer::getAffinities() const
{
    return affinities_;
}

void Mindflayer::setAffinities(const std::vector<Variant>& affinities)
{
    std::vector<Variant> temp;
    for(int i = 0; i < affinities.size(); i++)
    {
        bool found = false;
        for(int j = 0; j < temp.size(); j++)
        {
            if(affinities[i] == temp[j])
            {
                found = true;
            }
        }
        if(!found)
        {
            temp.push_back(affinities[i]);
        }
    }
    affinities_ = temp;
}

std::string Mindflayer::variantToString(const Variant& variant) const
{
    switch(variant)
    {
        case PSIONIC:
            return "PSIONIC";
        case TELEPATHIC:
            return "TELEPATHIC";
        case ILLUSIONARY:
            return "ILLUSIONARY";
        default:
            return "NONE";
    }
}



void Mindflayer::display() const{
    std::cout << "MINDFLAYER - " << getName() << std::endl;
    std::cout << "CATEGORY: " << getCategory() << std::endl;
    std::cout << "HP: " << getHitpoints() << std::endl;
    std::cout << "LVL: " << getLevel() << std::endl;
    std::cout << "TAME: " << (isTame() ? "TRUE" : "FALSE") << std::endl;
    std::cout << "SUMMONING: " << (getSummoning() ? "TRUE" : "FALSE") << std::endl;
    for(int i = 0; i < projectiles_.size(); i++)
    {
        std::cout << variantToString(projectiles_[i].type_) << ": " << projectiles_[i].quantity_ << std::endl;
    }
    if(affinities_.size() > 0)
    {
        std::cout << "AFFINITIES: " << std::endl;
        for(int i = 0; i < affinities_.size(); i++)
        {
            std::cout << variantToString(affinities_[i]) << std::endl;
        }
    }
}

/**
    @post   : If the creature is UNDEAD, it becomes tame if not already, as it appreciates the gesture, even though as an UNDEAD it does not really eat. It gains 1 hitpoint from the mysterious powers it receives by wearing the mushroom as a hat. Nothing else happens.
                If the creature is MYSTICAL, if it can summon a Thoughtspawn, it gives the mushroom to the Thoughtspawn instead. Nothing else happens.
                If it cannot summon a Thoughtspawn and is tame, it eats the mushroom to be polite. If it only had 1 hitpoint left, it remains at 1 hitpoint and becomes untame, else it loses 1 hitpoint. Nothing else happens.
                If it cannot summon a Thoughtspawn and is untame, it decides it doesn't have to deal with this and it leaves the Cavern. Nothing else happens.
                If the creature is an ALIEN, if it has Telepathic affinity, it convinces Selfa Ensert to eat the mushroom instead, and gains 1 hitpoint from laughing at their resulting illness (as laughter is good for the soul).
                If it is an ALIEN and does not have Telepathic affinity, but has a Telepathic projectile, it uses one of its Telepathic projectiles to achieve the same effect. (Remember to remove the projectile from the vector if it only had 1 left)
                If it is an ALIEN and does not have Telepathic affinity or a Telepathic projectile, it eats the mushroom and gains 2 hitpoints. As it turns out, the mushroom was good for it. It becomes tame if it was not already. Nothing else happens.
    @return   : true if the creature leaves the Cavern, false otherwise
*/
bool Mindflayer::eatMycoMorsel(){
    if(getCategory() == "UNDEAD")
    {
        setTame(true);
        setHitpoints(getHitpoints() + 1);
        return false;
    }
    else if(getCategory() == "MYSTICAL")
    {
        if(getSummoning())
        {
            return false;
        }
        else if(isTame())
        {
            if(getHitpoints() == 1)
            {
                setTame(false);
                return false;
            }
            else
            {
                setHitpoints(getHitpoints() - 1);
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    else if(getCategory() == "ALIEN")
    {
        bool telepathic = false;
        for(int i = 0; i < affinities_.size(); i++)
        {
            if(affinities_[i] == TELEPATHIC)
            {
                telepathic = true;
            }
        }
        bool telepathicProjectile = false;
        for(int i = 0; i < projectiles_.size(); i++)
        {
            if(projectiles_[i].type_ == TELEPATHIC)
            {
                telepathicProjectile = true;
                if(projectiles_[i].quantity_ == 1)
                {
                    projectiles_.erase(projectiles_.begin() + i);
                }
                else
                {
                    projectiles_[i].quantity_--;
                }
            }
        }
        if(telepathic || telepathicProjectile)
        {
            setHitpoints(getHitpoints() + 1);
            return false;
        }
        else
        {
            setTame(true);
            setHitpoints(getHitpoints() + 2);
            return false;
        }
    }
    else
    {
        return false;
    }

}


//Function to add the attack damage and type to the attack in the queue.
void Mindflayer::addAttackStat(Attack& attack, std::string type, int damage){
    attack.type_.push_back(type);
    attack.damage_.push_back(damage);
}


/**
* @param: A const reference to int corresponding to the attack to be added to the attack queue.
* @post: Adds an attack to the attack queue based on the int parameter.
* Here are the attacks for the Mindflayer:
*
* 1: PSIONIC BOLT/TENTACLE SLAP
*  If the Mindflayer has a PSIONIC projectile:
*      Attack name: PSIONIC BOLT
*      If the Mindflayer has a PSIONIC affinity:
*          Damage: 3 PSIONIC
*      Else:
*          Damage: 2 PSIONIC
* If the Mindflayer does not have a PSIONIC projectile:
*     Attack name: TENTACLE SLAP
*     Damage: 1 PHYSICAL, 1 EMOTIONAL
*
* 2: TELEPATHIC BOLT/TENTACLE SLAP
* If the Mindflayer has a TELEPATHIC projectile:
*      Attack name: TELEPATHIC BOLT
*      If the Mindflayer has a TELEPATHIC affinity:
*      Damage: 3 TELEPATHIC
*      Else:
*          Damage: 2 TELEPATHIC
* If the Mindflayer does not have a TELEPATHIC projectile:
*      Attack name: TENTACLE SLAP
*      Damage: 1 PHYSICAL, 1 EMOTIONAL
*
* 3: ILLUSIONARY BOLT/TENTACLE SLAP
* If the Mindflayer has an ILLUSIONARY projectile:
*      Attack name: ILLUSIONARY BOLT
*      If the Mindflayer has an ILLUSIONARY affinity:
*          Damage: 3 ILLUSIONARY
*      Else:
*          Damage: 2 ILLUSIONARY
* If the Mindflayer does not have an ILLUSIONARY projectile:
*      Attack name: TENTACLE SLAP
*      Damage: 1 PHYSICAL, 1 EMOTIONAL
* 
*/


void Mindflayer::addAttack(const int &attacks) {
    Attack new_attack;
    bool attackAdded = false; //flag that sets to true when an attack is added

    switch(attacks){
        case 1:
            new_attack.name_ = "PSIONIC BOLT";
            for(int i = 0; i < projectiles_.size(); i++){
                if(projectiles_[i].type_ == PSIONIC){
                    bool affinityMatchFound = false;
                    for(int j = 0; j < affinities_.size(); j++){
                        if(affinities_[j] == PSIONIC){
                            addAttackStat(new_attack, "PSIONIC", 3);
                            affinityMatchFound = true;
                            break;
                        }
                    }
                    if (!affinityMatchFound) {
                        addAttackStat(new_attack, "PSIONIC", 2);
                    }
                    attackAdded = true;
                    break;
                }
            }
            break;
        case 2:
            new_attack.name_ = "TELEPATHIC BOLT";
            for(int i = 0; i < projectiles_.size(); i++){
                if(projectiles_[i].type_ == TELEPATHIC){
                    bool affinityMatchFound = false;
                    for(int j = 0; j < affinities_.size(); j++){
                        if(affinities_[j] == TELEPATHIC){
                            addAttackStat(new_attack, "TELEPATHIC", 3);
                            affinityMatchFound = true;
                            break;
                        }
                    }
                    if (!affinityMatchFound) {
                        addAttackStat(new_attack, "TELEPATHIC", 2);
                    }
                    attackAdded = true;
                    break;
                }
            }
            break;
        case 3:
            new_attack.name_ = "ILLUSIONARY BOLT";
            for(int i = 0; i < projectiles_.size(); i++){
                if(projectiles_[i].type_ == ILLUSIONARY){
                    bool affinityMatchFound = false;
                    for(int j = 0; j < affinities_.size(); j++){
                        if(affinities_[j] == ILLUSIONARY){
                            addAttackStat(new_attack, "ILLUSIONARY", 3);
                            affinityMatchFound = true;
                            break;
                        }
                    }
                    if (!affinityMatchFound) {
                        addAttackStat(new_attack, "ILLUSIONARY", 2);
                    }
                    attackAdded = true;
                    break;
                }
            }
            break;
        default:
            break;
    }

    if (!attackAdded) {
        new_attack.name_ = "TENTACLE SLAP";
        addAttackStat(new_attack, "PHYSICAL", 1);
        addAttackStat(new_attack, "EMOTIONAL", 1);
    }

    Creature::addAttack(new_attack); 
}

/**
 @post     : displays the attacks of the Mindflayer in the form:
[MINDFLAYER] Choose an attack (1-3):\n1: PSIONIC BOLT\t\t2: TELEPATHIC BOLT\t\t3: ILLUSIONARY BOLT\n
*/

void Mindflayer::displayAttacks() const {
     std::cout << "[MINDFLAYER] Choose an attack (1-3):" << "\n1: PSIONIC BOLT\t\t" << "2:TELEPATHIC BOLT\t\t" << "3: ILLUSIONARY BOLT\n";
}