/*
CSCI235 Spring 2024
Project 6 - Cavern.cpp
William Li
April 19, 2024
Cavern.cpp declares the Cavern class along with its private and public members
*/
#include "Cavern.hpp"


Cavern::Cavern() : ArrayBag<Creature*>(), level_sum_{0}, tame_count_{0} {
}

/**
    @param  : the name of an input file
    @pre    : Formatting of the csv file is as follows (each numbered item appears separated by comma, only one value for each numbered item):
            1. TYPE: An uppercase string [DRAGON, GHOUL, MINDFLAYER]
            2. NAME: An uppercase string
            3. CATEGORY: An uppercase string [ALIEN, MYSTICAL, UNDEAD]
            4. HITPOINTS: A positive integer
            5. LEVEL: A positive integer
            6. TAME: 0 (False) or 1 (True)
            7. ELEMENT/FACTION: Uppercase string or strings representing the ELEMENT (For Dragons), or FACTION (For Ghouls) of the creature. If the creature is of a different subclass, the value will be NONE
            8. HEADS: A positive integer of the number of heads the Dragon has. If the creature is of a different subclass, the value will be 0
            9. FLIGHT/TRANSFORM/SUMMONING: 0 (False) or 1 (True) representing if the creature can fly (Dragons), transform (Ghouls), or summon a Thoughtspawn (Mindflayers).
            10. DECAY: A non-negative integer representing the level of decay of the Ghoul. If the creature is of a different subclass, the value will be 0
            11. AFFINITIES: Only applicable to Mindflayers. Affinities are of the form [AFFINITY1];[AFFINITY2] where multiple affinities are separated by a semicolon. Th value may be NONE for a Mindflayer with no affinities, or creatures of other subclasses.
            12. PROJECTILES: Only applicable to Mindflayers. PROJECTILES are of the form [PROJECTILE TYPE1]-[QUANTITY];[PROJECTILE TYPE 2]-[QUANTITY] where multiple types of projectiles are separated by a semicolon. The value may be NONE for a Mindflayer with no projectiles, or creatures of other subclasses.
*/
Cavern::Cavern(const std::string& filename)
{
  std::ifstream fin(filename);
  std::string line;
  std::getline(fin, line); //getting junk header
  std::string type, name, str_category, str_hitpoints, str_level, str_tame, str_element, str_heads, str_flight, str_decay, str_affinities, str_projectiles;
  int hitpoints, level, heads, decay;
  bool tame, flight;
  Creature::Category category;
  Dragon::Element element;
  Ghoul::Faction faction;
  std::vector<Mindflayer::Variant> affinities;
  std::vector<Mindflayer::Projectile> projectiles;

  while(std::getline(fin, line))
  {
    std::istringstream iss(line);
    std::getline(iss, type, ',');
    std::getline(iss, name, ',');
    std::getline(iss, str_category, ',');
    std::getline(iss, str_hitpoints, ',');
    std::getline(iss, str_level, ',');
    std::getline(iss, str_tame, ',');
    std::getline(iss, str_element, ',');
    std::getline(iss, str_heads, ',');
    std::getline(iss, str_flight, ',');
    std::getline(iss, str_decay, ',');
    std::getline(iss, str_affinities, ',');
    std::getline(iss, str_projectiles, ',');

    std::istringstream(str_hitpoints) >> hitpoints;
    std::istringstream(str_level) >> level;
    std::istringstream(str_tame) >> tame;
    std::istringstream(str_heads) >> heads;
    std::istringstream(str_flight) >> flight;
    std::istringstream(str_decay) >> decay;

    // Convert the category to the category enum
    if(str_category == "ALIEN")
    {
      category = Creature::ALIEN;
    }
    else if(str_category == "MYSTICAL")
    {
      category = Creature::MYSTICAL;
    }
    else if(str_category == "UNDEAD")
    {
      category = Creature::UNDEAD;
    }
    else
    {
      category = Creature::UNKNOWN;
    }

    if(type == "DRAGON")
    {
      if(str_element == "FIRE")
      {
        element = Dragon::FIRE;
      }
      else if(str_element == "WATER")
      {
        element = Dragon::WATER;
      }
      else if(str_element == "EARTH")
      {
        element = Dragon::EARTH;
      }
      else if(str_element == "AIR")
      {
        element = Dragon::AIR;
      }
      else
      {
        element = Dragon::NONE;
      }
      Dragon* new_dragon = new Dragon(name, category, hitpoints, level, tame, element, heads, flight);
      enterCavern(new_dragon);
    }
    else if(type == "GHOUL")
    {
      if(str_element == "FLESHGORGER")
      {
        faction = Ghoul::FLESHGORGER;
      }
      else if(str_element == "SHADOWSTALKER")
      {
        faction = Ghoul::SHADOWSTALKER;
      }
      else if(str_element == "PLAGUEWEAVER")
      {
        faction = Ghoul::PLAGUEWEAVER;
      }
      
      else
      {
        faction = Ghoul::NONE;
      }

      Ghoul* new_ghoul = new Ghoul(name, category, hitpoints, level, tame, decay, faction, flight);
      enterCavern(new_ghoul);
    }
    else if(type == "MINDFLAYER")
    {
      //clear the affinities and projectiles vectors
      affinities.clear();
      projectiles.clear();
      // Read the affinities
      std::string affinity;
      std::istringstream iss_affinities(str_affinities);
      while(std::getline(iss_affinities, affinity, ';'))
      {
        // Convert the affinity to the enum
        if(affinity == "PSIONIC")
        {
          affinities.push_back(Mindflayer::PSIONIC);
        }
        else if(affinity == "TELEPATHIC")
        {
          affinities.push_back(Mindflayer::TELEPATHIC);
        }
        else if(affinity == "ILLUSIONARY")
        {
          affinities.push_back(Mindflayer::ILLUSIONARY);
        }
        
      }

      // Read the projectiles
      std::string projectile;
      std::istringstream iss_projectiles(str_projectiles);
      while(std::getline(iss_projectiles, projectile, ';'))
      {
        std::string type;
        int quantity;
        Mindflayer::Projectile new_projectile;
        std::istringstream iss_projectile(projectile);
        std::getline(iss_projectile, type, '-');
        std::getline(iss_projectile, str_projectiles, '-');
        std::istringstream(str_projectiles) >> quantity;
        if(type == "PSIONIC")
        {
          new_projectile.type_ = Mindflayer::PSIONIC;
        }
        else if(type == "TELEPATHIC")
        {
          new_projectile.type_ = Mindflayer::TELEPATHIC;
        }
        else if(type == "ILLUSIONARY")
        {
          new_projectile.type_ = Mindflayer::ILLUSIONARY;
        }
        new_projectile.quantity_ = quantity;
        projectiles.push_back(new_projectile);
      }
      Mindflayer* new_mindflayer = new Mindflayer(name, category, hitpoints, level, tame, projectiles, flight, affinities);
      enterCavern(new_mindflayer);
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
  return std::ceil(tame_percent*100.0) / 100.0; //round up to to decimal places
 
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

int Cavern::releaseCreaturesOfCategory(std::string category) {
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
  for (int i = 0; i < getCurrentSize(); i++) {
    items_[i]->display();
  }
}

void Cavern::displayCategory(const std::string& category) const {
  if(category != "UNKNOWN" && category != "UNDEAD" && category != "MYSTICAL" && category != "ALIEN") {
    return;
  }
  for (int i = 0; i < getCurrentSize(); i++) {
    if (items_[i]->getCategory() == category) {
      items_[i]->display();
    }
  }
}

void Cavern::mycoMorselFeast() {
  int size = getCurrentSize();
  for (int i = 0; i < size; i++) {
    if(items_[i]->eatMycoMorsel()) {
      exitCavern(items_[i]);
    }
  }
}

    /**
      * @post: Stores the ALIEN Creatures of highest level in the cavern's alien stack, in the order in which they appear in the Cavern (i.e., starting from index 0 in items_, thus, if the highest level is 5 and there are 3 ALIEN creatures with level 5, the one with lowest index in items_ is at the bottom of the stack and the one with highest index in item_ is at the top of the stack, with a total of 3 ALIEN Creatures on the stack)
              
      *      : Empty the stack before beginning.
      */
    void Cavern::initializeAlienStack(){
      clearAlienStack();
      int max = 0;
      for(int i = 0; i < getCurrentSize(); i++){
        if(items_[i]->getCategory() == "ALIEN"){
          if(items_[i]->getLevel() > max){
            max = items_[i]->getLevel();
          }
        }
      }
      for(int j = 0; j < getCurrentSize(); j++){
        if(items_[j]->getCategory() == "ALIEN" && items_[j]->getLevel() == max){
          alien_stack_.push(items_[j]);
        }
      }

    }

    /**
      * @post: Stores the UNDEAD Creatures of highest level in the cavern's undead stack, in the order in which they appear in the Cavern (i.e., starting from index 0 in items_, thus, if the highest level is 5 and there are 3 UNDEAD creatures with level 5, the one with lowest index in items_ is at the bottom of the stack and the one with highest index in item_ is at the top of the stack, with a total of 3 UNDEAD Creatures on the stack)
            : Empty the stack before beginning.
      */
    void Cavern::initializeUndeadStack(){
      clearUndeadStack();
      int max = 0;
      for(int i = 0; i < getCurrentSize(); i++){
        if(items_[i]->getCategory() == "UNDEAD"){
          if(items_[i]->getLevel() > max){
            max = items_[i]->getLevel();
          }
        }
      }
      for(int j = 0; j < getCurrentSize(); j++){
        if(items_[j]->getCategory() == "UNDEAD" && items_[j]->getLevel() == max){
          undead_stack_.push(items_[j]);
        }
      }


    }

    /**
      * @post: Stores the MYSTICAL Creatures of highest level in the cavern's mystical stack, in the order in which they appear in the Cavern (i.e., starting from index 0 in items_, thus, if the highest level is 5 and there are 3 MYSTICAL creatures with level 5, the one with lowest index in items_ is at the bottom of the stack and the one with highest index in item_ is at the top of the stack, with a total of 3 MYSTICAL Creatures on the stack)
      *      : Empty the stack before beginning.
      */
    void Cavern::initializeMysticalStack(){
      clearMysticalStack();
      int max = 0;
      for(int i = 0; i < getCurrentSize(); i++){
        if(items_[i]->getCategory() == "MYSTICAL"){
          if(items_[i]->getLevel() > max){
            max = items_[i]->getLevel();
          }
        }
      }
      for(int j = 0; j < getCurrentSize(); j++){
        if(items_[j]->getCategory() == "MYSTICAL" && items_[j]->getLevel() == max){
          mystical_stack_.push(items_[j]);
        }
      }

    }

    /**
      * @post: Stores the UNKNOWN Creatures of highest level in the cavern's unknown stack, in the order in which they appear in the Cavern (i.e., starting from index 0 in items_, thus, if the highest level is 5 and there are 3 UNKNOWN creatures with level 5, the one with lowest index in items_ is at the bottom of the stack and the one with highest index in item_ is at the top of the stack, with a total of 3 UNKNOWN Creatures on the stack)
      *      : Empty the stack before beginning.
      */
    void Cavern::initializeUnknownStack(){
      clearUnknownStack();
      int max = 0;
      for(int i = 0; i < getCurrentSize(); i++){
        if(items_[i]->getCategory() == "UNKNOWN"){
          if(items_[i]->getLevel() > max){
            max = items_[i]->getLevel();
          }
        }
      }
      for(int j = 0; j < getCurrentSize(); j++){
        if(items_[j]->getCategory() == "UNKNOWN" && items_[j]->getLevel() == max){
          unknown_stack_.push(items_[j]);
        }
      }

    }


    /**
      * @return: A copy of the stack of highest level Aliens in the cavern
      */ 
    std::stack<Creature*> Cavern::getAlienStack(){
      return alien_stack_;
    }


    /**
      * @return: A copy of the stack of highest level Undeads in the cavern
      */
    std::stack<Creature*> Cavern::getUndeadStack(){
      return undead_stack_;
    }


    /**
      * @return: A copy of the stack of highest level Mysticals in the cavern
      */
    std::stack<Creature*> Cavern::getMysticalStack(){
      return mystical_stack_;
    }


    /**
      * @return: A copy of the stack of highest level Unknowns in the cavern
      */
    std::stack<Creature*> Cavern::getUnknownStack(){
      return unknown_stack_;
    }


    /**
      * @post: clears the stack of highest level Aliens in the cavern
      */ 
    void Cavern::clearAlienStack(){
      while(!alien_stack_.empty()){
        alien_stack_.pop();
      }
    }


    /**
      * @post: clears the stack of highest level Undeads in the cavern
      */
    void Cavern::clearUndeadStack(){
      while(!undead_stack_.empty()){
        undead_stack_.pop();
      }
    }


    /**
      * @post: clears the stack of highest level Mysticals in the cavern
      */
    void Cavern::clearMysticalStack(){
      while(!mystical_stack_.empty()){
        mystical_stack_.pop();
      }
    }


    /**
      * @post: clears the stack of highest level Unknowns in the cavern 
      */
    void Cavern::clearUnknownStack(){
      while(!unknown_stack_.empty()){
        unknown_stack_.pop();
      }
    }


    /**
      * @param: A stack of creature pointers 
      * @pre:   All the creature on the input stack are of same category and same (highest) level
      * @post:  For each creature in the stack, rebuild the Cavern's appropriate stack. (For example, if the creatures given are of category ALIEN, this function should build the Cavern's Alien stack.)
      *        Clear the Cavern's stack of the given category before adding the creatures to the stack.
      *        Before adding each creature to the Cavern's stack, prompt the user to select 2 attacks for the creature.
      *        Preserve the order of the creatures in the stack given. (E.g. The creature at the top of the given stack should also become the creature at the top of the Cavern's stack)
      *        If the input is invalid (valid inputs will be 1,2 or 3 only), keep prompting for a non-negative number that is within range, by printing "INVALID INPUT. TRY AGAIN.\n" and prompt for input again.
      *        When a valid action is read, it is passed to the creature's addAttack function to add the corresponding attack to the creature's attack queue.
      *        Prompting for attacks should be done in the following form (hint: use the creature's displayAttacks function):
      *        SELECT 2 ATTACKS FOR [CREATURE NAME] 
              [[CREATURE TYPE]] Choose an attack (1-3):
      *        1: [ATTACK 1 NAME]\t\t2: [ATTACK 2 NAME]\t\t3: [ATTACK 3 NAME]
              [user input]
      *        [[CREATURE TYPE]] Choose an attack (1-3):
      *        1: [ATTACK 1 NAME]\t\t2: [ATTACK 2 NAME]\t\t3: [ATTACK 3 NAME]
    */

    void Cavern::setAttacks(std::stack<Creature *> creatures)
    {
      std::stack<Creature *> temp;
      if (creatures.top()->getCategory() == "ALIEN"){
        clearAlienStack();
      }
      else if (creatures.top()->getCategory() == "UNDEAD"){
        clearUndeadStack();
      }
      else if (creatures.top()->getCategory() == "MYSTICAL"){
        clearMysticalStack();
      }
      else if (creatures.top()->getCategory() == "UNKNOWN"){
        clearUnknownStack();
      }
      while (!creatures.empty()){
        Creature *creature = creatures.top();
        std::cout << "SELECT 2 ATTACKS FOR " << creature->getName() << "\n";
        for (int i = 0; i < 2; i++)
        {
          int attack_choice;
          creature->displayAttacks();
          std::cin >> attack_choice;
          while (attack_choice < 1 || attack_choice > 3)
          {
            std::cout << "INVALID INPUT. TRY AGAIN.\n";
            std::cin >> attack_choice;
          }
          creature->addAttack(attack_choice);
        }
        //Keeps the stack in order.
        temp.push(creature);
        creatures.pop();
      }

      while (!temp.empty())
      {
        Creature *creature = temp.top();
        if (creature->getCategory() == "ALIEN"){
          alien_stack_.push(creature);
        }
        else if (creature->getCategory() == "UNDEAD"){
          undead_stack_.push(creature);
        }
        else if (creature->getCategory() == "MYSTICAL"){
          mystical_stack_.push(creature);
        }
        else if (creature->getCategory() == "UNKNOWN"){
          unknown_stack_.push(creature);
        }
        temp.pop();
      }
    }
    //This method prob doesn't work
   /* void Cavern::setAttacks(std::stack<Creature*> creatures){
      if(creatures.empty()){return;}

      std::stack<Creature*> creature_stack;
      // Clears the cavern's stack of the given category
      if(creatures.top()->getCategory() == "ALIEN"){ 
        clearAlienStack();
        int attack = 0;
        std::cout << "SELECT 2 ATTACKS FOR " << creature_stack.top()->getName() << std::endl;
        for(int i = 0; i < 2; i++){
          while(attack < 1 || attack > 3){
            std::cout << ""
          }
        }

      }
      else if(creature_stack.top()->getCategory() == "UNDEAD"){
        clearUndeadStack();
      }
      else if(creature_stack.top()->getCategory() == "MYSTICAL"){
        clearMysticalStack();
      }
      else if(creature_stack.top()->getCategory() == "UNKNOWN"){
        clearUnknownStack();
      }
      
    } */