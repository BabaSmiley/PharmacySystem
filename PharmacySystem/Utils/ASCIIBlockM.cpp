//
//  ASCIIBlockM.cpp
//  Some
//
//  Created by Cardasis, Jonathan (J.) on 4/13/17.
//  Copyright © 2017 Jonathan Cardasis. All rights reserved.
//

#ifndef ASCIIBlockM_cpp
#define ASCIIBlockM_cpp

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 Just some fancy ASCII art to spice up printing
 Usage:
 cout << BlockM() << endl; OR
 cout << BlockM(5) << endl; //where 5 is number of preceeding spaces
 */
struct ASCIIBlockM {
public:
    ASCIIBlockM(int preceedingSpaces = 0) {
        this->preceedingSpaces = preceedingSpaces;
        
        blockM = {
            R"( ____    ____  )",
            R"(|_   \  /   _| )",
            R"(  |   \/   |   )",
            R"(  | |\  /| |   )",
            R"( _| |_\/_| |_  )",
            R"(|____|  |____| )"
        };
    }
    
    int spaces() const { return preceedingSpaces; }
    int size() const { return (int)blockM.size(); }
    
    friend ostream& operator << (ostream& out, const ASCIIBlockM &m) {
        for(int i=0; i<m.size(); i++){
            for(int space=0; space<m.spaces(); space++)
            out << " ";
            
            out << m.blockM[i];
            if(i != m.size()-1)
            out << endl;
        }
        return out;
    }
private:
    vector<string> blockM;
    int preceedingSpaces;
};

#endif
