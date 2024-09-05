/*
CSCI235 Spring 2024
Project 3 - Cavern Class
Georgina Woo
Dec 24 2023
Taven.cpp declares the Cavern class along with its private and public members
*/
#include "Cavern.hpp"
#include "Dragon.hpp"
#include "Ghoul.hpp"
#include "Mindflayer.hpp"
#include <string>
#include <fstream>
#include <sstream>

Cavern::Cavern() : ArrayBag<Creature*>(), level_sum_{0}, tame_count_{0} {
  
}
/**
    @param: the name of an input file (a string reference)
    @pre: Formatting of the csv file is as follows (each numbered item appears separated by comma, only one value for each numbered item):
    1. TYPE: An uppercase string [DRAGON, GHOUL, MINDFLAYER]
    2. NAME: An uppercase string
    3. CATEGORY: An uppercase string [ALIEN, MYSTICAL, UNDEAD]
    4. HITPOINTS: A positive integer
    5. LEVEL: A positive integer
    6. TAME: 0 (False) or 1 (True)
    7. ELEMENT/FACTION: Uppercase string or strings representing the ELEMENT (For Dragons), or FACTION (For Ghouls) of the creature. 
      If the creature is of a different subclass, the value will be NONE
    8. HEADS: A positive integer of the number of heads the Dragon has. If the creature is of a different subclass, the value will be 0
    9. FLIGHT/TRANSFORM/SUMMONING: 0 (False) or 1 (True) representing if the creature can fly (Dragons), transform (Ghouls), 
      or summon a Thoughtspawn (Mindflayers).
    10. DECAY: A non-negative integer representing the level of decay of the Ghoul. 
      If the creature is of a different subclass, the value will be 0
    11. AFFINITIES: Only applicable to Mindflayers. Affinities are of the form [AFFINITY1];
      [AFFINITY2] where multiple affinities are separated by a semicolon. Th value may be NONE for a Mindflayer with no affinities, or creatures of other subclasses.
    12. PROJECTILES: Only applicable to Mindflayers. PROJECTILES are of the form [PROJECTILE TYPE1]-[QUANTITY];
      [PROJECTILE TYPE 2]-[QUANTITY] where multiple types of projectiles are separated by a semicolon. The value may be NONE for a Mindflayer with no projectiles, or creatures of other subclasses.
*/
std::vector<std::string> split(const std::string& input, const char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(input);
   while(std::getline(tokenStream, token, delimiter))
   {
        tokens.push_back(token);
   }

   return tokens;
} 
Creature::Category getCategoryFromString(std::string& string)
{
    if(string == "UNDEAD"){
      return Creature::Category::UNDEAD;
    }
    if(string == "MYSTICAL"){
      return Creature::Category::MYSTICAL;
    }
    if(string == "ALIEN"){
      return Creature::Category::ALIEN;
    }
    return Creature::Category::UNKNOWN;
}

Dragon::Element getElementFromString(std::string& string)
{
    if(string == "FIRE"){
      return Dragon::Element::FIRE;
    }
    if(string == "WATER"){
      return Dragon::Element::WATER;
    }
    if(string == "EARTH"){
      return Dragon::Element::EARTH;
    }
    if(string == "AIR"){
      return Dragon::Element::AIR;
    }
    return Dragon::Element::NONE;
}
Ghoul::Faction getFactionFromString(std::string& string)
{
  if(string == "FLESHGORGER"){
    return Ghoul::Faction::FLESHGORGER;
  }
  if(string == "SHADOWSTALKER"){
    return Ghoul::Faction::SHADOWSTALKER;
  }
  if(string == "PLAGUEWEAVER"){
    return Ghoul::Faction::PLAGUEWEAVER;
  }
  return Ghoul::Faction::NONE;
}

Mindflayer::Variant getVariantFromString(std::string& string){
  if(string == "PSIONIC"){
    return Mindflayer::Variant::PSIONIC;
  }
  if(string == "TELEPATHIC"){
    return Mindflayer::Variant::TELEPATHIC;
  }
  if(string == "ILLUSIONARY"){
    return Mindflayer::Variant::ILLUSIONARY;
  }
  return Mindflayer::Variant::PSIONIC;
}

std::vector<Mindflayer::Variant> getAffinitiesFromString(std::string& string)
{
    std::vector<Mindflayer::Variant> affinities;
    if (string != "NONE")
    {
        std::vector<std::string> afinityStrings = split(string, ';');
        for (std::string s : afinityStrings)
        {
            affinities.push_back(getVariantFromString(s));
        }
    }

    // return empty one;
    return affinities;
}

std::vector<Mindflayer::Projectile> getProjectilesFromString(std::string& string)
{
    std::vector<Mindflayer::Projectile> Projectiles;
    if (string != "NONE")
    {
        std::vector<std::string> projectilePairs = split(string, ';');
        for (std::string s : projectilePairs)
        {
            std::vector<std::string> projectilePair = split(s, '-');
            Mindflayer::Projectile p = Mindflayer::Projectile();
            p.type_ = getVariantFromString(projectilePair[0]);
            p.quantity_ = std::stoi(projectilePair[1]);
            Projectiles.push_back(p);
        }
    }

    return Projectiles;
}



Cavern::Cavern(const std::string& fileName) : ArrayBag<Creature*>(){

    std::ifstream file;
    file.open(fileName);

    std::string line;
    while(getline(file, line)){
      std::vector<std::string> tokens = split(line, ',');
      if(tokens[0] == "DRAGON"){
        Dragon* d = new Dragon(tokens[1], 
                                getCategoryFromString(tokens[2]), 
                                std::stoi(tokens[3]), 
                                std::stoi(tokens[4]), 
                                std::stoi(tokens[5]),
                                getElementFromString(tokens[6]), 
                                std::stoi(tokens[7]), 
                                (bool) std::stoi(tokens[8]));
      enterCavern(d);
      }
      //name, category, hitpoints, level, tame, faction, transformation, decay
      else if(tokens[0] == "GHOUL"){
        Ghoul* g = new Ghoul(tokens[1],
                              getCategoryFromString(tokens[2]),
                              std::stoi(tokens[3]),
                              std::stoi(tokens[4]),
                              (bool) std::stoi(tokens[5]),
                              std::stoi(tokens[9]),                              
                              getFactionFromString(tokens[6]),
                              (bool) std::stoi(tokens[8])
                              );
      enterCavern(g);
      }
      //name, category, hitpoints, level, tame, projectiles, summoning, affinities
      else if(tokens[0] == "MINDFLAYER"){
        Mindflayer* m = new Mindflayer(tokens[1],
                                        getCategoryFromString(tokens[2]),
                                        std::stoi(tokens[3]),
                                        std::stoi(tokens[4]),
                                        (bool) std::stoi(tokens[5]),
                                        getProjectilesFromString(tokens[11]),
                                        (bool) std::stoi(tokens[8]),
                                        getAffinitiesFromString(tokens[10])
                                    
        );
        enterCavern(m);                                                     
      }

    }

}


bool Cavern::enterCavern(Creature* new_creature) {
  if (getIndexOf(new_creature) == -1) {
    if (add(new_creature)){
      level_sum_ += new_creature->getLevel();
      if (new_creature->isTame()) {
        tame_count_++;
      }
      return true;
    }
  }
  return false;
}

bool Cavern::exitCavern(Creature* creature_to_remove) {
  if (remove(creature_to_remove)) {
    level_sum_ -= creature_to_remove->getLevel();
    if (creature_to_remove->isTame()) {
      tame_count_--;
    }
    return true;
  }
  return false;
}

int Cavern::getLevelSum() const {
  return level_sum_;
}

int Cavern::calculateAvgLevel() const {
  if (isEmpty()) {
    return 0;
  }
  return round(level_sum_ / getCurrentSize());
}

int Cavern::getTameCount() const {
  return tame_count_;
}

double Cavern::calculateTamePercentage() const {
  if (isEmpty()) {
    return 0;
  }
  double tame_percent = (tame_count_>0) ?  (double(tame_count_) / item_count_) * 100: 0.0;
  // return the tame percentage rounded up to two decimal places

  return std::ceil(tame_percent * 100) / 100;
 
}

int Cavern::tallyCategory(const std::string& category) const {
  if(category != "UNKNOWN" && category != "UNDEAD" && category != "MYSTICAL" && category != "ALIEN") {
    return 0;
  }
  int count = 0;
  for (int i = 0; i < getCurrentSize(); i++) {
    if (items_[i]->getCategory() == category) {
      count++;
    }
  }
  return count;
}

int Cavern::releaseCreaturesBelowLevel(int level) {
  int count = 0;
  if (level < 0) {
    return 0;
  }
  else if (level == 0) {
    count = getCurrentSize();
    clear();
    return count;
  }
  else {
    int size = getCurrentSize();
    for (int i = 0; i < size; i++) {
      if (items_[i]->getLevel() < level) {
        exitCavern(items_[i]);
        count++;
      }
    }
    return count;
  }
}

int Cavern::releaseCreaturesOfCategory(const std::string& category) {
  int count = 0;
  if (category == "ALL") {
    count = getCurrentSize();
    clear();
    return count;
  }
  else if (category != "UNKNOWN" && category != "UNDEAD" && category != "MYSTICAL" && category != "ALIEN") {
    return 0;
  }
  else {
    int size = getCurrentSize();
    for (int i = 0; i < size; i++) {
    if (items_[i]->getCategory() == category) {
      exitCavern(items_[i]);
      count++;
    }
  }
  return count;
  }
}

void Cavern::cavernReport() const {
  std::cout << "UNKNOWN: " << tallyCategory("UNKNOWN") << std::endl;
  std::cout << "UNDEAD: " << tallyCategory("UNDEAD") << std::endl;
  std::cout << "MYSTICAL: " << tallyCategory("MYSTICAL") << std::endl;
  std::cout << "ALIEN: " << tallyCategory("ALIEN") << std::endl;
  std::cout << std::endl;

  std::cout << "AVERAGE LEVEL: " << calculateAvgLevel() << std::endl;
  std::cout << "TAME: " << calculateTamePercentage() << "%" << std::endl;
}

void Cavern::displayCreatures() const {
  for (int i = 0; i < getCurrentSize(); i++) //for every creature in the cavern
  {
  items_[i]->display();
  }
}

/**
    @param: a string reference to a category
    @post: For every creature in the cavern of the given category (only exact matches to the input string), displays each creature's information

*/

void Cavern::displayCategory(const std::string& category) const {
  for (int i = 0; i < getCurrentSize(); i++) {
    if (items_[i]->getCategory() == category) //if the creature's category matches the input category
    {
      items_[i]->display();
    }
  }
}

/**
    @post: Every creature in the cavern eats a MycoMorsel.
*/
void Cavern::mycoMorselFeast() {
  for (int i = 0; i < getCurrentSize(); i++) {
    items_[i]->eatMycoMorsel();
  }
}