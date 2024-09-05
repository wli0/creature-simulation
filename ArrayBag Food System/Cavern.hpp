/*
CSCI235 Spring 2024
Project 3 - Cavern Class
Georgina Woo
Dec 24 2023
Cavern.hpp declares the Cavern class along with its private and public members
*/
#ifndef CAVERN_HPP
#define CAVERN_HPP

#include "ArrayBag.hpp"
#include "Creature.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

class Cavern : public ArrayBag<Creature*>{
  public:
    /**
      Default constructor.
      Default-initializes all private members. 
    */
    Cavern();

    Cavern(const std::string& fileName);

    /** 
     * @param   :   A Creature entering the Cavern
     * @post    :   If the given Creature is not already in the Cavern, add Creature to the Cavern and updates the level sum and the tame Creature count if the creature is tame.
     * @return  :   returns true if a Creature was successfully added to the Cavern (i.e. items_), false otherwise
                :   Hint: Use the above definition of equality will help determine if a Creature is already in the Cavern
    **/
    bool enterCavern(Creature* new_creature);


    /** 
     * @param   :   A Creature leaving the Cavern
     * @return  :  returns true if a creature was successfully removed from the Cavern (i.e. items_), false otherwise
     * @post    :    removes the creature from the Cavern and updates the level sum and the tame count if the creature is tame.
    **/
    bool exitCavern(Creature* creature_to_remove);

    /** 
    * @return   :  The integer level count of all the creatures currently in the Cavern
    **/
    int getLevelSum() const;

    /**
    * @return   :  The average level of all the creatures in the Cavern
    * @post     :    Computes the average level of the Cavern rounded to the NEAREST integer.
    **/
    int calculateAvgLevel() const;

    /**
    * @return   :  The integer count of tame Creatures in the Cavern
    **/
    int getTameCount() const;

    /**
    * @return   :  The percentage (double) of all the tame creatures in the Cavern
    * @post     :    Computes the percentage of tame creatures in the Cavern rounded up to 2 decimal places.
    **/
    double calculateTamePercentage() const;

    /**
      * @param  :   A string representing a creature Category with value in 
                  ["UNKNOWN", "UNDEAD", "MYSTICAL", "ALIEN"]
      * @return  :  An integer tally of the number of creatures in the Cavern of the given category. 
                  If the argument string does not match one of the expected category values, 
                  the tally is zero. 
                  NOTE: no pre-processing of the input string necessary, only uppercase input will match.
    **/
    int tallyCategory(const std::string& category) const;

    /**
        @param  :   An integer representing the level treshold of the creatures to be removed from the Cavern, with default value 0
        @post   :   Removes all creatures from the Cavern whose level is less than the given level. If no level is given, removes all creatures from the Cavern. Ignore negative input.
        @return :   The number of creatures removed from the Cavern
    */
    int releaseCreaturesBelowLevel(int level = 0);

    /**
        @param  : A string representing a creature Category with a value in ["UNKNOWN", "UNDEAD", "MYSTICAL", "ALIEN"], or default value "ALL" if no category is given
        @post   : Removes all creatures from the Cavern whose category matches the given category. If no category is given, removes all creatures from the Cavern.
        @return : The number of creatures removed from the Cavern
                  NOTE: no pre-processing of the input string necessary, only uppercase input will match. If the input string does not match one of the expected category values, do not remove any creatures.
    */
    int releaseCreaturesOfCategory(const std::string& category = "ALL");

    /**
     * @post    : Outputs a report of the creatures currently in the Cavern in the form:
                  "UNKNOWN: [x]\nUNDEAD: [x]\nMYSTICAL: [x]\nALIEN: [x]\n\nThe average level is: [x] \n[x]% are tame.\n"
                  Note that the average level should be rounded to the NEAREST integer, and the percentage of tame creatures in the Cavern should be rounded to 2 decimal places.

                  Example output: 
                  UNKNOWN: 3
                  UNDEAD: 5
                  MYSTICAL: 8
                  ALIEN: 6

                  AVERAGE LEVEL: 7
                  TAME: 46.67%
    */
    void cavernReport() const;

    void displayCreatures() const;
    
    void displayCategory(const std::string& category) const;

    void mycoMorselFeast();

  private:
    int level_sum_;     // sum of all the levels of the creatures in the cavern
    int tame_count_;    // number of tame creatures in the cavern

};
#endif
