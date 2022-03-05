#include<iostream>
#include<vector>
#include <stdlib.h>
#include <iomanip> 
#include<unordered_set>
#include<unordered_map>
#include<utility>
#include <string>

using namespace std;

struct SetHasher {
    int operator()(const vector< vector<int> > &V) const {
        int hash = V.size();
        for(int i=0;i<V.size();i++){
            for(int j=0;j<V[i].size();j++){
                hash ^= V[i][j] + 0x9e3779b9 + (hash << 6) + (hash >> 2); 
            }
        }
        return hash;
    }
};

void genState(int n, int m, vector<vector<int> >& state){
    /*
    Randomly generate water into the front n tubes, and the result will store in state whihch has total m tubes.
    :param: n, m, state
    :type: int, int, vector<vector<int> >
    :returns: void
    */
    unordered_set<int> hasAppear;
    for(int i=0; i<n; i++){
        for (int j=0;j<4;j++){
            int sample = rand()%(n*4);
            while (hasAppear.count(sample)!=0) sample = rand()%(n*4);
            state[sample/4][sample%4]=i;
            hasAppear.insert(sample);
        }
    }
}

void findNextState(vector<vector<int> >& state, vector<vector<vector<int> > >& nextStates){
    /*
    Find all possible next states which can be reached from state, and save them in nextStates
    :param: state, nextStates
    :type: vector<vector<int> >, vector<vector<vector<int> > 
    :returns: void
    */

    //Find all empty tubes in state
    vector<int> emptyTubes;
    for(int i=0;i<state.size();i++){
        bool isEmpty = true;
        for (int j=0;j<4;j++){
            if (state[i][j]!=-1) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) emptyTubes.push_back(i);
    }
    //Build a mapping where key is color and the value is its corresponding positions
    unordered_map<int, vector<int> > mapping;
    for (int i=0;i<state.size();i++){
        for(int j=0; j<4;j++){
            if (state[i][j]!=-1){
                if (mapping.count(state[i][j])==0){
                    vector<int> tmp;
                    mapping[state[i][j]] = tmp;
                }
                //Check the entry above it. If it is the same, do not add to mapping.
                if (j==0 || state[i][j]!=state[i][j-1]) mapping[state[i][j]].push_back(i*4+j);
            }
        }
    }
    //Iterate through each colors, check all possible next states by moving entries with corresponding color
    for (unordered_map<int, vector<int> >::iterator iter=mapping.begin(); iter!=mapping.end(); iter++){
        //Build information about each entries
        unordered_map<int, pair<int, int> > isTop;   //key: Position, value: depth, empty space
        for(int i=0;i<(*iter).second.size();i++){
            int tube = (*iter).second[i]/4;
            int pos = (*iter).second[i]%4;
            pair<int, int> tmp;
            int index = pos+1;
            while (index<4){
                if (state[tube][index]!=(*iter).first) break;
                index++;
            }
            tmp.first = index-pos;
            if (pos==0) {
                tmp.second = 0;
                isTop[(*iter).second[i]] = tmp;
            }else if (state[tube][pos-1]==-1){
                tmp.second = pos;
                isTop[(*iter).second[i]] = tmp;
            }
        }
        for(unordered_map<int, pair<int, int> >::iterator ptr=isTop.begin(); ptr!=isTop.end();ptr++){
            int tube = ptr->first/4;
            int pos = ptr->first%4;
            //Perform operations to move water from ptr tube to empty tubes
            if (ptr->second.first+ptr->second.second!=4){
                for(int i=0;i<emptyTubes.size();i++){
                    vector<vector<int> > newState = state;
                    int targetTube = emptyTubes[i];
                    for(int j=0;j<ptr->second.first;j++){
                        newState[targetTube][3-j] = iter->first;
                        newState[tube][pos+j] = -1;
                    }
                    nextStates.push_back(newState);
                }
            }
            
            //Perform operations to move water with the same color
            unordered_map<int, pair<int, int> >::iterator target=ptr;
            target++;
            //Pour from ptr tube to target tube
            for(;target!=isTop.end();target++){
                if (target->second.second==0) continue;
                vector<vector<int> > newState = state;
                int targetTube = target->first/4;
                int targetPos = target->first%4-1;
                int transferAmount = target->second.second>ptr->second.first ? ptr->second.first : target->second.second; 
                for(int i=0; i<transferAmount; i++){
                    newState[targetTube][targetPos-i] = iter->first;
                    newState[tube][pos+i] = -1;
                }
                nextStates.push_back(newState);
            }
            //Pour from target tube to ptr tube
            if (ptr->second.second>0){
                unordered_map<int, pair<int, int> >::iterator target=ptr;
                target++;
                for(;target!=isTop.end();target++){
                    vector<vector<int> > newState = state;
                    int targetTube = target->first/4;
                    int targetPos = target->first%4;
                    int transferAmount = target->second.first>ptr->second.second ? ptr->second.second : target->second.first;
                    for(int i=0; i<transferAmount; i++){
                        newState[tube][pos-1-i] = iter->first;
                        newState[targetTube][targetPos+i] = -1;
                    } 
                    nextStates.push_back(newState);
                }
            }
        }
    }
}

bool checkIsDesired(vector<vector<int> >& state){
    /*
    Check whether the state is the desired final state(All tubes are either empty or filled with the same color of water).
    :param: state
    :type: vector<vector<int> >
    :returns: bool
    */

    for(int i=0; i<state.size();i++){
        for(int j=1; j<4;j++){
            if (state[i][j]!=state[i][j-1]) return false;
        }
    }
    return true;
}

void printState(vector<vector<int> >& state){
    /*
    Print state in a tidy way
    :param: state
    :type: vector<vector<int> >
    :returns: void
    */

    for(int i=0; i<4; i++){
        for (int j=0; j<state.size();j++){
            if (state[j][i]!=-1) cout<<"|"<<setw (1) << state[j][i] <<"| ";
            else cout<<"| | ";
        }
        cout<<endl;
    }
    for(int i=0; i<state.size();i++){
        cout<<" ‾  ";
    }
    cout<<endl;
}

bool recursiveCheck(vector<vector<int> >& state, unordered_set<vector<vector<int> >, SetHasher>& hasVisited, vector<vector<vector<int> > > &successPath){
    /*
    Check whether the state can be solved, and if it can be solved, we would update the successPath with the steps to solve the state.
    :param: state, hasVisited, successPath
    :type: vector<vector<int> >, unordered_set<vector<vector<int> >, SetHasher>, vector<vector<vector<int> > >
    :returns: bool
    */

    if (checkIsDesired(state)){
        successPath.push_back(state);
        return true;
    }
    vector<vector<vector<int> > > nextStates;
    findNextState(state, nextStates);
    for(int i=0; i<nextStates.size(); i++){
        if (hasVisited.count(nextStates[i])==0){
            hasVisited.insert(nextStates[i]);
            if (recursiveCheck(nextStates[i], hasVisited, successPath)){
                successPath.push_back(state);
                return true;
            }
        }
    }
    return false;
}



int main(int argc, char *argv[]){
    if (argc!=4) {
        cout<< "Invalid argument: Please follow ./waterSort [# Tubes] [# filled tubes] [whether print intermediate state true/false]\n";
        return -1;
    }
    srand (time(NULL));
    int m=atoi(argv[1]);
    int n=atoi(argv[2]);
    bool toPrint = strcmp(argv[3], "true")==0;
    vector<vector<int> > state(m, vector<int>(4, -1));
    genState(n, m, state);
    printState(state);
    unordered_set<vector<vector<int> >, SetHasher > hasVisited;
    vector<vector<vector<int> > > successPath;
    
    if (recursiveCheck(state, hasVisited, successPath)){
        cout<<"Can be solved" << endl;
        if (toPrint){
            cout<<"One possible solution in " << successPath.size()-1 << "steps." << endl;
            for (int i=successPath.size()-1; i>=0;i--){
                printState(successPath[i]);
                cout<<endl;
            }
        }
    }else{
        cout<<"Cannot be solved"<<endl;
    }
    
}