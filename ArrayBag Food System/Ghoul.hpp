/*
CSCI235 Spring 2024
Project 2 - Derived Classes
Georgina Woo
Dec 23 2023
Ghoul.hpp defines the constructors and private and public functions of the Ghoul class
*/

#ifndef GHOUL_HPP
#define GHOUL_HPP

#include "Creature.hpp"



class Ghoul : public Creature 
{
    public:

        enum Faction {NONE, FLESHGORGER, SHADOWSTALKER, PLAGUEWEAVER};
        
        /**
            Default constructor.
            Default-initializes all private members. 
            Default Category: UNDEAD
            Default decay: 0
            Default faction: NONE
            Booleans are default-initialized to False. 
        */
        Ghoul();

        /**
            Parameterized constructor.
            @param      : The name of the Ghoul (a reference string)
            @param      : The category of the Ghoul (a Category enum) with default value UNDEAD
            @param      : The Ghoul's hitpoints (an integer), with default value 1 if not provided, or if the value provided is 0 or negative
            @param      : The Ghoul's level (an integer), with default value 1 if not provided, or if the value provided is 0 or negative
            @param      : A flag indicating whether the Ghoul is tame, with default value False
            @param      : The level of decay (an integer), with default value 0 if not provided, or if the value provided is negative
            @param      : The faction (a Faction enum), with default value NONE if not provided
            @param      : A flag indicating whether the Ghoul can transform, with default value False
            @post       : The private members are set to the values of the corresponding parameters.
            Hint: Notice the default arguments in the parameterized constructor.
        */
        Ghoul(const std::string& name, Category category = UNDEAD, int hitpoints = 1, int level = 1, bool tame = false, int decay = 0, Faction faction = NONE, bool transformation = false);

        /**
            Getter for the level of decay.
            @return     : The level of decay (an integer)
        */
        int getDecay() const;

        /**
            Setter for the level of decay.
            @param      : A reference to the level of decay (an integer)
            @pre        : The level of decay must be >= 0 (do nothing otherwise)
            @post       : The level of decay is set to the value of the parameter.
            @return     : true if the level of decay was set, false otherwise
        */
        bool setDecay(const int& decay);

        /**
            Getter for the faction.
            @return     : The faction (a string representation of the Faction enum)
        */
        std::string getFaction() const;

        /**
            Setter for the faction.
            @param      : A reference to the faction (a Faction enum)
            @post       : The faction is set to the value of the parameter.
        */
        void setFaction(const Faction& faction);

        /**
            Getter for the transformation.
            @return     : The transformation (a boolean)
        */
        bool getTransformation() const;

        /**
            Setter for the transformation.
            @param      : A reference to the transformation (a boolean)
            @post       : The transformation is set to the value of the parameter.
        */
        void setTransformation(const bool& transformation);

        /**
        @post     : displays Ghoul data in the form:
        GHOUL - [NAME]\n
        CATEGORY: [CATEGORY]\n
        HP: [HITPOINTS]\n
        LVL: [LEVEL]\n
        TAME: [TRUE/FALSE]\n
        DECAY: [DECAY]\n
        FACTION: [FACTION]\n
        IT [CAN/CANNOT] TRANSFORM\n"
        
        Example:
        GHOUL - JHRAT
        CATEGORY: MYSTICAL
        HP: 4
        LVL: 8
        TAME: TRUE
        DECAY: 3
        FACTION: FLESHGORGER
        IT CAN TRANSFORM
        
    */
    virtual void display() const override;
        
        

    /**
        @post   : If the creature is UNDEAD, it becomes tame if not already, as it appreciates the gesture, even though as an UNDEAD it does not really eat. It gains 1 hitpoint from the mysterious powers it receives by wearing the mushroom as a hat. Nothing else happens.
                If the creature is of Faction FLESHGORGER, it becomes so offended by being offered a mushroom that it becomes untamed if it was tame. If it was already untamed, it leaves the Cavern. Nothing else happens.
                If the creature was of Faction SHADOWSTALKER, if it was untame, it hides from the mushroom and nothing happens. If it was tame, it eats the mushroom and loses 1 hitpoint, unless it only had 1 hitpoint left in which case it stays at 1 hitpoint but becomes untame. Nothing else happens.
        @return   : true if the creature leaves the Cavern, false otherwise
    */
    virtual bool eatMycoMorsel() override;

    private:
        int decay_;
        Faction faction_;
        bool transformation_;
};

#endif // GHOUL_HPP