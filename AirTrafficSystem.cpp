#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
using namespace std;
const int HEIGHT = 25000;
const int WIDTH = 100000;
const int LENGTH = 100000;
const int TIMER = 610000000;
const int HEIGHTDIST = 1000;
const int HORIZONTALDIST = 3000;
const int TEST_INPUT = 160;
const int ARRAY_SIZE = TEST_INPUT / 8;
static int uaID = ARRAY_SIZE; //sets the id number of unknown planes
static int UNIVERSAL_COUNTER = 0;
static int second_counter = 0;
static int second1 = 0;
int delcount = 0;
int addcount = 0;
static int n = 0;
// Track File
ofstream Track("C:\\Users\\c_kokoro\\Downloads\\Track.txt");
vector <string> AirspaceLog;
vector <string> AirspaceStatus;
//static int mutex = 0;'
//The Velocity class takes the speed of the aircraft given by the input file and returns the x,y and z components of it
class Velocity {
public:
    Velocity() {
        xSpeed = ySpeed = zSpeed = -1000000;
    }
    Velocity(int spex, int spey, int spez) {//create velocity vector and its 3d components
        xSpeed = spex;
        ySpeed = spey;
        zSpeed = spez;
    }
    ~Velocity() {} //default destrcutor
    int getXSpeed() { //three getters
        return xSpeed;
    }
    int getYSpeed() {
        return ySpeed;
    }
    int getZSpeed() {
        return zSpeed;
    }
    void setVelocity(int spex, int spey, int spez) {//setter used to define new velocity
        xSpeed = spex;
        ySpeed = spey;
        zSpeed = spez;
    }
protected:
    int xSpeed;
    int ySpeed;
    int zSpeed;
};
//The Position class takes the position of the aircraft given by the input file and returns the x,y and z components of it
class Position {
public:
    Position() {
        xPos = yPos = zPos = -1;
    }
    Position(int x, int y, int z) {
        xPos = x;
        yPos = y;
        zPos = z;
    }
    virtual ~Position() {}
    int getXPos() {
        return xPos;
    }
    int getYPos() {
        return yPos;
    }
    int getZPos() {
        return zPos;
    }
    void setPositions(int x, int y, int z) {
        xPos = x;
        yPos = y;
        zPos = z;
    }
protected:
    int xPos;
    int yPos;
    int zPos;
};
//The Aircraft class takes everything from the input file and creates an aircraft constructor for each aircraft in the file
//This class gets called from main().
class Aircraft {
public:
    Aircraft() {
        aircraft_id = -2;
        current_velocity = Velocity();
        current_position = Position();
        rel_time = -2;
        magVel = 0;
        wasInSpace = false;
        permission = false;
    }
    Aircraft(int id, int spex, int spey, int spez, int x, int y, int z, int rt) {
        permission = false;
        setAircraft_ID(id);
        current_velocity = Velocity(spex, spey, spez);
        current_position = Position(x, y, z);
        rel_time = rt;
        string input = "plane released at time : " + to_string(rt);
        magVel = CalcMag(spex, spey, spez);
        wasInSpace = false;
        data.push_back(input);
    }
    ~Aircraft() {
    }
// Called in case 11
    void project(int time) {
        cout << aircraft_id << " |";
        cout << unknownID << " |";
        cout << current_velocity.getXSpeed() << " " << current_velocity.getYSpeed() << " " << current_velocity.getZSpeed() << " |";
        cout << (current_position.getXPos() + current_velocity.getXSpeed() * time) << " " << (current_position.getYPos() + current_velocity.getYSpeed() * time) << " " << (current_position.getZPos() + current_velocity.getZSpeed() * time) << " |";
        cout << rel_time << " |" << endl;
    }
    void printSimplifiedInfo() {
        cout << "Aircraft ID: ";
        cout << aircraft_id << " | ";
// cout << unknownID << " |";
        cout << "Current velocity: ";
        cout << current_velocity.getXSpeed() << " " << current_velocity.getYSpeed() << " " << current_velocity.getZSpeed() << " | ";
        cout << "Current position: ";
        cout << current_position.getXPos() << " " << current_position.getYPos() << " " << current_position.getZPos() << " |";
        cout << " Release Time: ";
        cout << rel_time << " | " << endl;
    }
    string getSimplifiedInfo() {
        string info;
        info = to_string(aircraft_id) + " |";
        info += to_string(unknownID) + " |";
        info += to_string(current_velocity.getXSpeed()) + " " + to_string(current_velocity.getYSpeed()) + " " + to_string(current_velocity.getZSpeed()) + " |";
        info += to_string(current_position.getXPos()) + " " + to_string(current_position.getYPos()) + " " + to_string(current_position.getZPos()) + " |";
        info += to_string(rel_time) + " |";
        return info;
    }
    void printInfo() {
        cout << "#############\n";
        cout << " id : " << aircraft_id << endl;
        cout << " Unknown Craft : " << unknownID << endl;
        cout << " current velocity : x-> " << current_velocity.getXSpeed() << " | y-> " << current_velocity.getYSpeed() << " | z-> " << current_velocity.getZSpeed() << endl;
        cout << " current position : x-> " << current_position.getXPos() << " | y-> " << current_position.getYPos() << " | z-> " << current_position.getZPos() << endl;
        cout << " entry time : " << rel_time << endl;
        cout << "#############\n\n\n\n";
    }
    double CalcMag(double spex, double spey, double spez) { //calculate the magnitude of the velocity vector
        double x = spex * spex;
        double y = spey * spey;
        double z = spez * spez;
        return (sqrt(x + y + z));
    }
    void setCurrent_Velocity(int spex, int spey, int spez) { //set the value of the curr vel
        current_velocity = Velocity(spex, spey, spez);
    }
    Velocity getCurrent_Velocity() {
        return current_velocity;
    }
    void setCurrent_Position(int lx, int ly, int lz) { // set value of curr position
        current_position = Position(lx, ly, lz);
    }
    Position getCurrent_Position() {// return the current position value
        return current_position;
    }
    Position getAnticipated_Position() { // return the anticipated position value
        return anticipated_position;
    }
    void projectPosition(int time) {
        projected_position = Position(current_position.getXPos() + time * current_velocity.getXSpeed(), current_position.getYPos() + time * current_velocity.getYSpeed(), current_position.getZPos() + time * current_velocity.getZSpeed());
    }
    Position getProjected_Position() {
        return projected_position;
    }
    void setAircraft_ID(int val) {
        switch (val)
        {
        case -1: //if val has no positive value
            unknownID = true;// declare plane unknown and inc unknown airplane ID
            aircraft_id = uaID++;
            break;
        default: // regular assignement
            unknownID = false;
            aircraft_id = val;
            break;
        }
    }
    int getAircraft_ID() {
        return aircraft_id;
    }
    void nextPos(int time) {
        int xTravel = (time * current_velocity.getXSpeed());
        int xfinal = current_position.getXPos() + xTravel;
        int yTravel = (time * current_velocity.getYSpeed());
        int yfinal = current_position.getYPos() + yTravel;
        int zTravel = (time * current_velocity.getZSpeed());
        int zfinal = current_position.getZPos() + zTravel;
        anticipated_position = Position(xfinal, yfinal, zfinal);
        changeVal();
    }
    void changeVal() {
        current_position.setPositions(anticipated_position.getXPos(), anticipated_position.getYPos(), anticipated_position.getZPos());
    }
    void setRel_Time(int time) {
        rel_time = time;
    }
    int getEntry_Time() {
        return rel_time;
    }
    void setEntry(int rel) {
        rel_time = rel;
    }
    void setUnknownID(bool unifent) {
        unknownID = unifent;
    }
    bool getUnknownID() {
        return unknownID;
    }
    bool getWasInSpace() {
        return wasInSpace;
    }
    void setWasInSPace(bool set) {
        wasInSpace = set;
    }
    void changeSpeed(double speed) {
        int x = speed * (current_velocity.getXSpeed() / magVel);
        int y = speed * (current_velocity.getYSpeed() / magVel);
        int z = speed * (current_velocity.getZSpeed() / magVel);
        current_velocity = Velocity(x, y, z);
    }
    bool getPermission() {
        return permission;
    }
    void setPermission(bool perm) {
        permission = perm;
    }
    void changeDirection(int newXPos, int newYPos, int newZpos) {
        double newVecMag = CalcMag(newXPos - current_position.getXPos(), newYPos - current_position.getYPos(), newZpos - current_position.getZPos());
        int x = magVel * (newXPos - current_position.getXPos()) / newVecMag;
        int y = magVel * (newYPos - current_position.getYPos()) / newVecMag;
        int z = magVel * (newZpos - current_position.getZPos()) / newVecMag;
        current_velocity = Velocity(x, y, z);
    }
    bool isInBlock() { //check to see if the plane is inside the block
        if ((current_position.getXPos() >= 0 && current_position.getXPos() <= WIDTH) && (current_position.getYPos() >= 0 && current_position.getYPos() <= LENGTH) && (current_position.getZPos() >= 0 && current_position.getZPos() <= (HEIGHT))) {
            wasInSpace = true;
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }
    void pushData(string input) {
        data.push_back(input);
    }
    void outputData() {
        for (int i = 0; i < data.size(); i++) {
            cout << data.at(i) << endl;
        }
    }
private:
    int aircraft_id;
    Velocity current_velocity;
    Position current_position;
    Position anticipated_position; //locaion where the plane should go to
    Position projected_position;
    std::vector<string> data;
    int rel_time; //release time
    double magVel;//magnitude of the velocity vector
    bool unknownID;
    bool permission;
    bool wasInSpace;
};
//didnt check yet
class Hit
{
public:
    Hit()
    {
        aircraft_id = 0;
        x = 0;
        y = 0;
        z = 0;
        isSeen = false;
    }
    bool isSeen;
    Hit(int id, int xp, int yp, int zp, bool ufo, int rel)
    {
        aircraft_id = id;
        unidentified = ufo;
        x = xp;
        y = yp;
        z = zp;
        rel_time = rel;
        isSeen = false;
    }
    ~Hit() {};
    int getAircraft_ID()
    {
        return aircraft_id;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    int getZ()
    {
        return z;
    }
    int getRel() {
        return rel_time;
    }
    void hitInfo() {
        if (unidentified) {
            cout << "id : " << "unknown" << endl;
        }
        else {
            cout << "id : " << getAircraft_ID() << endl;
        }
        cout << "x : " << getX() << endl;
        cout << "y : " << getY() << endl;
        cout << "z : " << getX() << endl;
        cout << "******\n\n\n";
    }
// Push to Track File
    void insertHitInfo() {
        if (unidentified) {
            Track << "id : " << "unknown" << endl;
        }
        else {
            Track << "id : " << getAircraft_ID() << endl;
        }
        Track << "x : " << getX() << endl;
        Track << "y : " << getY() << endl;
        Track << "z : " << getX() << endl;
        Track << "******\n";
    }
// Print to terminal
    void simpleHitInfo() {
        if (unidentified) {
            cout << "Aircraft ID: Unknown" << " | Aircraft Position: " << x << " " << y << " " << z << " |\n";
        }
        else
        {
            cout << "Aircraft ID: " << aircraft_id << " | Aircraft Position: " << x << " " << y << " " << z << " |\n";
        }
    }
private:
    int aircraft_id;
    bool unidentified;
    int x;
    int y;
    int z;
    int rel_time;
};
//Function is used to print out a string letting us know if an aircraft has entered our airspace using its ID and position.
//Pushes to Airspace Log file
void RadarLog(int plane_id, Position position) {
    string log = "Time :" + to_string(UNIVERSAL_COUNTER) + " || aircraft :" + to_string(plane_id) + "| current pos: " + to_string(position.getXPos()) + " " + to_string(position.getYPos()) + " " + to_string(position.getZPos()) + " | " + " entered space";
    AirspaceLog.push_back(log);
}
//Function RadarStatus is used to print out a string letting us know if an aircraft is still in our airspace using its ID and position.
//Pushes to Airspace status file
void RadarStatus(int plane_id, Position position) {
    string log = "Time :" + to_string(UNIVERSAL_COUNTER) + " || aircraft :" + to_string(plane_id) + "| current pos: " + to_string(position.getXPos()) + " " + to_string(position.getYPos()) + " " + to_string(position.getZPos()) + " | " + " still in space";
    AirspaceStatus.push_back(log);
}
//Function g is used to print out a string letting us know if an aircraft has left the airspace using its ID.
//Pushes to Airspace log file
void CommLog(int plane_id) {
    string log = "Time :" + to_string(UNIVERSAL_COUNTER) + " || aircraft :" + to_string(plane_id) + " left space";
    AirspaceLog.push_back(log);
}
//Function Commstatus is used to print out a string letting us know if an aircraft is no longer in the space using its ID.
//Pushes to Airspace Status file
void Commstatus(int plane_id) {
    string log = "Time :" + to_string(UNIVERSAL_COUNTER) + " || aircraft :" + to_string(plane_id) + " no longer in space";
    AirspaceStatus.push_back(log);
}
//compare 2 aircrafts, if the distance between them is too small, ,return TRUE, if not return FALSE. This is used to detect collisions.
bool aircraftCollision(Aircraft original, Aircraft invader, int n) {
    original.nextPos(n);
    invader.nextPos(n);
    if (abs(original.getAnticipated_Position().getZPos() - invader.getAnticipated_Position().getZPos()) <= HEIGHTDIST) {
        if (abs(original.getAnticipated_Position().getYPos() - invader.getAnticipated_Position().getYPos()) <= HORIZONTALDIST) {
            if (abs(original.getAnticipated_Position().getXPos() - invader.getAnticipated_Position().getXPos()) <= HORIZONTALDIST) {
                return true;
            }
        }
    }
    return false;
}
//future collisions
bool aircraftfutureCollision(Aircraft original, Aircraft invader, int n) {
    original.nextPos(n);
    invader.nextPos(n);
    if (abs(original.getProjected_Position().getZPos() - invader.getProjected_Position().getZPos()) <= HEIGHTDIST) {
        if (abs(original.getProjected_Position().getYPos() - invader.getProjected_Position().getYPos()) <= HORIZONTALDIST) {
            if (abs(original.getProjected_Position().getXPos() - invader.getProjected_Position().getXPos()) <= HORIZONTALDIST) {
                return true;
            }
        }
    }
    return false;
}
//If a plane is in the block(airspace), then it gets added to the vector Hit.
void hitList(Aircraft* planes, std::vector<Hit>& allHits) {
//std::vector<Hit> allHits;
    allHits.clear();
    for (int i = 0; i < ARRAY_SIZE * 2; i++)
    {
        if (planes[i].isInBlock())
        {
            Hit hit = Hit(planes[i].getAircraft_ID(), planes[i].getCurrent_Position().getXPos(), planes[i].getCurrent_Position().getYPos(), planes[i].getCurrent_Position().getZPos(), planes[i].getUnknownID(), planes[i].getEntry_Time());
            allHits.push_back(hit);
        }
    }
//return allHits;
}
//prints the hit list to terminal
void printHits(std::vector<Hit> Hits) {
    Track << "Time : " << UNIVERSAL_COUNTER << endl;
    for (int i = 0; i < Hits.size(); i++) {
        if (Hits[i].getRel() <= UNIVERSAL_COUNTER) {
            cout << "Hit | ";
            Hits[i].simpleHitInfo();
            Hits[i].insertHitInfo();
        }
        /*if (true) {
        for (int l = 0; l < 1; l++) {
        for (int y = 0; y < 211132767; y++) {
        }
        cout << " ";
        }
        cout << endl;
        }*/
    }
    Hits.clear();
}
/*
void loopHitList(Aircraft* planes, std::vector<Hit>& allHits) {
while (true)
{
if(mut.mutex == 1){
if (true) {
for (int l = 0; l < 5; l++) {
for (int y = 0; y < 111132767; y++) {
}
cout << " " << l + 1 << " ";
}
cout << endl;
}
hitList(planes, allHits);
printHits(allHits);
if ((second_counter != 0) && second_counter % 5 == 0) {
for (int l = 0; l < 5; l++) {
for (int y = 0; y < 211132767; y++) {
}
cout << " " << l + 1 << " ";
}
cout << endl;
}
//Mutex.unlock();
mut.mutex = 0;
}
}
}
*/
//warns about future collisions
void collisionFuture(Aircraft* planes, int n)
{
    std::vector <Aircraft> collisions;
    for (int i = 0; i < ARRAY_SIZE * 2; i++) {
        for (int j = i + 1; j < ARRAY_SIZE * 2; j++) {
            if (i > ARRAY_SIZE * 2) {
                break;
            }
            if (planes[i].getEntry_Time() != planes[j].getEntry_Time()) {
                if (planes[i].getEntry_Time() <= UNIVERSAL_COUNTER && planes[j].getEntry_Time() <= UNIVERSAL_COUNTER) {
                    planes[i].projectPosition(n);
                    planes[j].projectPosition(n);
                    if (aircraftfutureCollision(planes[i], planes[j], n)) {
                        collisions.push_back(planes[i]);
                        collisions.push_back(planes[j]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < collisions.size(); i = i + 2) {
        if ((collisions[i].getEntry_Time() <= UNIVERSAL_COUNTER) && (collisions[i + 1].getEntry_Time() <= UNIVERSAL_COUNTER)) {
            if (collisions[i].getAircraft_ID() != -2) {
                if (collisions[i + 1].getAircraft_ID() != -2) {
                    if (collisions[i].getUnknownID()) {
                        cout << "WARNING : Imminent collision between Unknown Aircraft : " << collisions[i].getAircraft_ID() << " ";
                    }
                    else {
                        cout << "WARNING : Imminent collision between Aircraft : " << collisions[i].getAircraft_ID() << " ";
                    }
                    if (collisions[i + 1].getUnknownID()) {
                        cout << "and Unknown Aircraft : " << collisions[i + 1].getAircraft_ID() << " " << endl;
                    }
                    else {
                        cout << "and between Aircraft : " << collisions[i + 1].getAircraft_ID() << " " << endl;
                    }
                }
            }
        }
    }
//commands
}
//Not used nabu
void collisionWarn(Aircraft* planes)
{
    std::vector <Aircraft> collisions;
    for (int i = 0; i < ARRAY_SIZE * 2; i++) {
        for (int j = i + 1; j < ARRAY_SIZE * 2; j++) {
            if (i > ARRAY_SIZE * 2) {
                break;
            }
            if (planes[i].getEntry_Time() != planes[j].getEntry_Time()) {
                if (aircraftCollision(planes[i], planes[j], 0)) {
                    collisions.push_back(planes[i]);
                    collisions.push_back(planes[j]);
                }
            }
        }
    }
    for (int i = 0; i < collisions.size(); i = i + 2) {
        if ((collisions[i].getEntry_Time() <= UNIVERSAL_COUNTER) && (collisions[i + 1].getEntry_Time() <= UNIVERSAL_COUNTER)) {
            if (collisions[i].getUnknownID()) {
                cout << "WARNING : Imminent collision between Unknown Aircraft : " << collisions[i].getAircraft_ID() << " ";
            }
            else {
                cout << "WARNING : Imminent collision between Aircraft : " << collisions[i].getAircraft_ID() << " ";
            }
            if (collisions[i + 1].getUnknownID()) {
                cout << "and Unknown Aircraft : " << collisions[i + 1].getAircraft_ID() << " " << endl;
            }
            else {
                cout << "and between Aircraft : " << collisions[i + 1].getAircraft_ID() << " " << endl;
            }
        }
    }
//commands
}
//print plane information, when it has entered the airspace, or left the airspace
void printPlanes(Aircraft* planes) {
    for (int i = 0; i < ARRAY_SIZE * 2; i++) {
        if (planes[i].isInBlock()) {
            if (planes[i].getEntry_Time() <= UNIVERSAL_COUNTER && (planes[i].getEntry_Time() > 0)) {
                if (planes[i].getPermission() == false) {
                    string in = "Aircraft " + to_string(planes[i].getAircraft_ID()) + " has entered the airspace.";
                    RadarLog(planes[i].getAircraft_ID(), planes[i].getCurrent_Position());
                    cout << in << "\n";
                    planes[i].setPermission(true);
                }
                planes[i].printSimplifiedInfo();
                RadarStatus(planes[i].getAircraft_ID(), planes[i].getCurrent_Position());
            }
        }
        else if ((!(planes[i].isInBlock()) && planes[i].getWasInSpace())) {
            int val = planes[i].getAircraft_ID();
            string out = "Note : Plane " + to_string(val) + " has left the airspace";
            cout << out << endl;
            CommLog(planes[i].getAircraft_ID());
            planes[i] = Aircraft();
            planes[i].setAircraft_ID(val);
            planes[i].setEntry(-2);
//planes[i].printSimplifiedInfo();
            planes[i].setWasInSPace(false);
        }
        else if ((!(planes[i].isInBlock()) && !(planes[i].getWasInSpace()))) {
            if (planes[i].getAircraft_ID() != -2 && planes[i].getEntry_Time() <= UNIVERSAL_COUNTER)
                Commstatus(planes[i].getAircraft_ID());
        }
    }
}
//Timer???
void planeIncrements(Aircraft* planes) {
//if(mut.mutex==0){
    for (int l = 0; l < 5; l++) {
        for (int y = 0; y < TIMER; y++) {
            ;
        }
        cout << " . ";
    }
    cout << endl;
    for (int i = 0; i < ARRAY_SIZE * 2; i++) {
        if (i == 1) {
            i = i;
        }
        if (planes[i].isInBlock()) {
//planes[i].printSimplifiedInfo(); WILL BE USED LATER
            if (true) {
                for (int l = 0; l < 1; l++) {
                    for (int y = 0; y < TIMER; y++) {
                        ;
                    }
//cout << " ";
                }
//cout << endl;
            }
//if (!(planes[i].isInBlock())) {
//int val = planes[i].getAircraft_ID();
//cout << "\n\n\n######";
//cout << "\n Note : Plane " << val << " will no longer be in the airspace in next run" << endl;
//cout << "######\n\n\n";
//}
        }
        else if ((!(planes[i].isInBlock()) && planes[i].getWasInSpace())) {
            int val = planes[i].getAircraft_ID();
            cout << "\n Note : Plane " << val << " has left the airspace" << endl;
//planes[i].printSimplifiedInfo();
            planes[i].setWasInSPace(false);
        }
    }
    for (int l = 0; l < 5; l++) {
        for (int y = 0; y < TIMER; y++) {
            ;
        }
//cout << " . ";
    }
//cout << endl;
//Mutex.unlock();
// mut.mutex = 1;
//}
    second_counter = second_counter + 1;
}
void commands(int, int, Aircraft*);
//find unknown aircract and assign it an ID through the command function.
void BackgroundSearch(Aircraft* planes) {
    for (int i = 0; i < ARRAY_SIZE * 2; i++) {
        if (planes[i].getAircraft_ID() != -2 && planes[i].getUnknownID() && planes[i].getEntry_Time() < UNIVERSAL_COUNTER) {
            string unk = "Unknown plane inside airspace detected";
            AirspaceLog.push_back(unk);
            commands(7, planes[i].getAircraft_ID(), planes);
        }
    }
}
void createFile();
void takeAction() {
    cout << "There are no more aircrafts. Take action \n\n";
}
void loopDisplay(Aircraft* planes, std::vector<Hit>& allHits) {
//for (int i = 0; i < ARRAY_SIZE*2; i++) {
    int empty_checker = 0;
    while (true)
    {
//planeIncrements(planes);
        if ((second_counter % 5) == 0) {
            cout << "\nTime :" << UNIVERSAL_COUNTER << endl;
            printPlanes(planes);
            collisionFuture(planes, 10);
//cout << "\n\n";
        }
        if ((second1 % 15) == 0) {
            hitList(planes, allHits);
            printHits(allHits);
        }
        for (int i = 0; i < ARRAY_SIZE * 2; i++) {
            if (planes[i].getEntry_Time() < UNIVERSAL_COUNTER && (planes[i].getEntry_Time() > -1)) {
                planes[i].nextPos(1);
            }
        }
        for (int j = 0; j < ARRAY_SIZE * 2; j++) {
            if (planes[j].getEntry_Time() == -2) {
                empty_checker++;
            }
        }
        if (empty_checker >= ((ARRAY_SIZE * 2)) - 3) {
            cout << "All planes have left the space" << endl;
            break;
        }
        else
        {
            empty_checker = 0;
        }
        UNIVERSAL_COUNTER = UNIVERSAL_COUNTER + 1;
        second_counter = (second_counter + 1) % 5;
        second1 = (second1 + 1) % 15;
        for (int l = 0; l < 1; l++) {
            for (int y = 0; y < TIMER; y++) {
                ;
            }
//cout << UNIVERSAL_COUNTER << " ";
        }
        if ((UNIVERSAL_COUNTER % 5 == 0) && UNIVERSAL_COUNTER > 0) {
            createFile();
        }
        if (UNIVERSAL_COUNTER % 15 == 0) {
            BackgroundSearch(planes);
        }
    }
}
void DisplayMenu()
{
    cout << endl;
    cout << endl;
    cout << "*********************ATC PROJECT 2019********************" << endl;
    cout << "************************ Menu ************************" << endl;
    cout << "a) Change altitude of an aircraft?" << endl;
    cout << "b) Change the velocity of an aircraft?" << endl;
    cout << "c) Change direction in horizontal plane?" << endl;
    cout << "d) Enter or leave a holding pattern (DOESNT WORK)" << endl;
    cout << "e) Report current position and velocity?" << endl;
    cout << "f) Add or delete an aircraft? (DELETE DOESNT WORK)" << endl;
    cout << "g) Project aircraft positons" << endl;
    cout << "h) Change the elevation of an aircraft" << endl;
    cout << endl << "Please choose an option: ";
}
void commandManager(Aircraft* planes)
{
    while (1)
    {
        DisplayMenu();
        char koko;
        int planeid;
        cin >> koko;
        cout << endl;
        switch (koko)
        {
        case 'a': //increase/decrease height
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(1, planeid, planes);
            break;
        case 'b': //change velocity
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(2, planeid, planes);
            break;
        case 'c': //change direction
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(3, planeid, planes);
            break;
        case 'd':
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(6, planeid, planes);
            break;
        case 'e': //change position in plane
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(7, planeid, planes);
            break;
        case 'f': //change position in plane
            planeid = 0;
            cout << endl;
            commands(8, planeid, planes);
            break;
        case 'g': //change position in plane
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(11, planeid, planes);
            break;
        case 'h': //change position in plane
            cout << "Enter plane ID: ";
            cin >> planeid;
            cout << endl;
            commands(9, planeid, planes);
            break;
        default:
            break;
        }
    }
}
void commands(int com, int plane_id, Aircraft* aircrafts) {
    string dataval;
    switch (com)
    {
    case 1: //increase height
        int n;
        cout << "Enter an altitude value n X 1000 : ";
        cin >> n;
        cout << endl;
        for (int l = 0; l < (ARRAY_SIZE + addcount); l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " old altitude was " << aircrafts[l].getCurrent_Position().getZPos() << endl;
                aircrafts[l].setCurrent_Position(aircrafts[l].getCurrent_Position().getXPos(), aircrafts[l].getCurrent_Position().getYPos(), aircrafts[l].getCurrent_Position().getZPos() + n * 1000);
                dataval = "altitude change by " + to_string((n * 1000));
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " new altitude is " << aircrafts[l].getCurrent_Position().getZPos() << endl;
                cout << dataval << endl;
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 2:// magnify speed
        cout << "Enter the number you want the speed magnified by " << endl;
        double s;
        cin >> s;
        for (int l = 0; l < ARRAY_SIZE + addcount; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " old velocity was " << aircrafts[l].getCurrent_Velocity().getXSpeed() << " " << aircrafts[l].getCurrent_Velocity().getYSpeed() << " " << aircrafts[l].getCurrent_Velocity().getZSpeed() << endl;
                aircrafts[l].setCurrent_Velocity(s * aircrafts[l].getCurrent_Velocity().getXSpeed(), s * aircrafts[l].getCurrent_Velocity().getYSpeed(), s*aircrafts[l].getCurrent_Velocity().getZSpeed());
                dataval = "Speed magnitude changed to " + to_string((s));
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " new velocity is " << aircrafts[l].getCurrent_Velocity().getXSpeed() << " " << aircrafts[l].getCurrent_Velocity().getYSpeed() << " " << aircrafts[l].getCurrent_Velocity().getZSpeed() << endl;
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 3: //change direction youre flying
        int x;
        int y;
        cout << "Please choose the direction you want to change\n";
        cout << "1. x,y\n";
        cout << "2. x,-y\n";
        cout << "3. -x,y\n";
        cout << "4. -x,-y\n";
        cout << ">";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            x = 1;
            y = 1;
            break;
        case 2:
            x = 1;
            y = -1;
            break;
        case 3:
            x = -1;
            y = 1;
            break;
        case 4:
            x = -1;
            y = -1;
            break;
        default:
            break;
        }
        for (int l = 0; l < ARRAY_SIZE + addcount; l++)
        {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " old x and y velocity was " << aircrafts[l].getCurrent_Velocity().getXSpeed() << " " <<aircrafts[l].getCurrent_Velocity().getYSpeed() << endl;
                aircrafts[l].setCurrent_Velocity(x * aircrafts[l].getCurrent_Velocity().getXSpeed(), y * aircrafts[l].getCurrent_Velocity().getYSpeed(), aircrafts[l].getCurrent_Velocity().getZSpeed());
                dataval = "horizontal direction changed by " + to_string(choice);
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " new x and y velocity is " << (aircrafts[l].getCurrent_Velocity().getXSpeed()) << " " << (aircrafts[l].getCurrent_Velocity().getYSpeed()) << endl;
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 5:
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " current speed is " << aircrafts[l].getCurrent_Velocity().getXSpeed() << " " << aircrafts[l].getCurrent_Velocity().getYSpeed() << " " << aircrafts[l].getCurrent_Velocity().getZSpeed() << endl;
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " current position is " << aircrafts[l].getCurrent_Position().getXPos() << " " << aircrafts[l].getCurrent_Position().getYPos() << " " << aircrafts[l].getCurrent_Position().getZPos() << endl;
                dataval = "Speed and Position requested by ATC at " + to_string(UNIVERSAL_COUNTER) + "for plade ID " + to_string(aircrafts[l].getAircraft_ID());
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 6:
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                aircrafts[l].setCurrent_Velocity(-1 * aircrafts[l].getCurrent_Velocity().getXSpeed(), aircrafts[l].getCurrent_Velocity().getYSpeed(), -1 * aircrafts[l].getCurrent_Velocity().getZSpeed());
                dataval = "holding modified at " + to_string(UNIVERSAL_COUNTER);
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 7:
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                aircrafts[l].printSimplifiedInfo();
                dataval = "Time :" + to_string(UNIVERSAL_COUNTER) + "ATC requested Plane " + to_string(aircrafts[l].getAircraft_ID()) + " to identify";
                AirspaceLog.push_back(dataval);
                AirspaceLog.push_back(aircrafts[l].getSimplifiedInfo());
                aircrafts[l].pushData(dataval);
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 8:
        int choix;
        int a1, a2, a3, a4, a5, a6, a7;
        cout << "Choose your option \n1.add\n2.delete.\n";
        cin >> choix;
        if (choix == 1) {
            cout << " Enter the 7 values of the plane (id,sx,sy,sz,x,y,z) " << endl;
            cin >> a1;
            cin >> a2;
            cin >> a3;
            cin >> a4;
            cin >> a5;
            cin >> a6;
            cin >> a7;
            if (addcount < (ARRAY_SIZE * 2)) {
                aircrafts[8 + addcount] = Aircraft(a1, a2, a3, a4, a5, a6, a7, UNIVERSAL_COUNTER);
                addcount++;
            }
        }
        else
        {
            cout << "Enter plane ID: ";
            cin >> plane_id;
            for (int l = 0; l < ARRAY_SIZE * 2; l++) {
                if (aircrafts[l].getAircraft_ID() == plane_id) {
                    aircrafts[l] = Aircraft();
                    dataval = "Aircraft deleted";
                    aircrafts[l].pushData(dataval);
                    AirspaceLog.push_back(dataval);
                    delcount++;
                    break;
                }
                else {
                    ;//cout << "Error : No such aircraft found";
                }
            }
        }
        break;
    case 9:
        int z;
        cout << "Enter the new elevation value for x " << endl;
        cin >> x;
        cout << endl;
        cout << "Enter the new elevation value for y " << endl;
        cin >> y;
        cout << endl;
        cout << "Enter the new elevation value for z " << endl;
        cin >> z;
        cout << endl;
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " old position was " << aircrafts[l].getCurrent_Position().getXPos() << " " << aircrafts[l].getCurrent_Position().getYPos() << " " << aircrafts[l].getCurrent_Position().getZPos() << endl;
                aircrafts[l].setCurrent_Position(x, y, z);
                dataval = "Elevation changed to " + to_string(x) + " " + to_string(y) + " " + to_string(z);
                aircrafts[l].pushData(dataval);
                cout << "Aircraft ID " << aircrafts[l].getAircraft_ID() << " new position is " << aircrafts[l].getCurrent_Position().getXPos() << " " << aircrafts[l].getCurrent_Position().getYPos() << " " << aircrafts[l].getCurrent_Position().getZPos() << endl;
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 10:// costa didnt use
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                aircrafts[l].outputData();
                dataval = "Elecation changed to " + to_string(x) + " " + to_string(y) + " " + to_string(z);
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
        break;
    case 11:
        cout << "Enter the number of seconds into the future";
        cin >> n;
        for (int l = 0; l < ARRAY_SIZE * 2; l++) {
            if (aircrafts[l].getAircraft_ID() == plane_id) {
                aircrafts[l].project(n);
                dataval = "ATC requested projected location by " + to_string(n) + " seconds";
                AirspaceLog.push_back(dataval);
                aircrafts[l].pushData(dataval);
                break;
            }
            else {
                ;//cout << "Error : No such aircraft found";
            }
        }
    }
}
void createFile() {
    ofstream out("C:\\Users\\c_kokoro\\Downloads\\Airspacelog.txt");
    ofstream nout("C:\\Users\\c_kokoro\\Downloads\\Status.txt");
    for (int i = 0; i < AirspaceLog.size(); i++) {
        out << AirspaceLog.at(i) << endl;
    }
    for (int j = 0; j < AirspaceStatus.size(); j++) {
        nout << AirspaceStatus.at(j) << endl;
    }
}
void main() {
    Aircraft* planes = new Aircraft[ARRAY_SIZE * 2];
    std::vector <Hit> Hits;
    /*
    const int X = 100000;
    const int Y = 100000;
    const int Z = 25000;
    */ //ID, SP X,SP Y, SP Z, X, Y, Z, RelTime
    int airplane_schedule[TEST_INPUT] =
    {
        0, -641, 283, 500, 95000, 101589, 10000, 13,
        1, -223, -630, -526, 71000, 100000, 13000, 16,
        -1, -180, -446, -186, 41000, 100000, 6000, 31,
        3, 474, -239, 428, 38000, 0, 14000, 44,
        -1, -535, 520, 360, 95000, 100000, 1000, 49,
        -1, -164, -593, -501, 83000, 100000, 12000, 67,
        6, 512, 614, 520, 86000, -1571, 9000, 87,
        7, -275, 153, -399, 47000, 100000, 12000, 103,
        8, -129, 525, -300, 92000, 100000, 1000, 123,
        9, 437, 574, 339, 32000, 0, 8000, 129,
        10, 568, 538, 192, 50000, 0, 4000, 133,
        11, 347, 529, -599, 83000, -1799, 10000, 140,
        12, -512, -482, 578, 35000, 100000, 4000, 142,
        13, 619, -280, 194, 74000, 0, 10000, 157,
        14, -141, 427, -321, 41000, 102251, 11000, 165,
        15, -199, 242, -205, 56000, 100000, 4000, 172,
        16, 315, -197, -365, 77000, 0, 1000, 187,
        17, -138, 455, 602, 23000, 102290, 14000, 199,
        18, -150, 557, -356, 38000, 100000, 1000, 204,
        19, 194, 184, 598, 35000, 0, 2000, 221,
    };
    for (int i = 0; i < TEST_INPUT; i = i + 8) {
        int j = (i) / 8;
        planes[j] = Aircraft(airplane_schedule[i], airplane_schedule[i + 1], airplane_schedule[i + 2], airplane_schedule[i + 3], airplane_schedule[i + 4], airplane_schedule[i + 5], airplane_schedule[i + 6], airplane_schedule[i + 7]);
    }
    for (int i = 0; i < ARRAY_SIZE; i++) {
        planes[ARRAY_SIZE + i] = Aircraft();
    }
//std::thread Background(BackgroundSearch, std::ref(planes));
    std::thread DisplayManager(loopDisplay, std::ref(planes), std::ref(Hits));
//std::thread HitListing(loopHitList, std::ref(planes), std::ref(Hits));
    std::thread Commands(commandManager, std::ref(planes));
    Commands.detach();
    DisplayManager.join();
//HitListing.join();
//hitList(planes,Hits);
//printHits(Hits);
//collisionWarn(planes);
    system("pause");
    delete[] planes;
}
