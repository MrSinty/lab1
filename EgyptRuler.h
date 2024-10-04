#pragma once

#include <iostream>
#include <random>
#include <fstream>
#include <conio.h>
#include <string>

using namespace std;

const short kAmntOfYears = 10;

const short kMinAcrePrice = 17;
const short kMaxAcrePrice = 26;

const short kMinWheatFromOneAcre = 1;
const short kMaxWheatFromOneAcre = 6;

const float kMinRatsAteAmpl = 0;
const float kMaxRatsAteAmpl = 0.07;

const short kPeasantEatWheat = 20;
const float kMaxDiedFromStarving = 0.45;
const float kPlagueChance = 0.15;
const float kDroughtChance = 0.20;

const short kPeasantMaxArea = 10;
const float kWheatPerAcre = 0.5;

unsigned short year = 1;
bool lost = false;

ifstream isafe;
ofstream osafe;

int population = 100;
int available_wheat = 2800;
int city_area = 1000;

int died_from_starving = 0;
int arrived_to_city = 0;
bool isPlague = false;
bool isDrought = false;
int gathered_wheat = 0;
int wheat_from_one_acre = 0;
int wheat_rats_ate = 0;
int acre_price = 0;

int player_buys_acres = 0;
int player_sells_acres = 0;
int wheat_for_eating = 0;
int wheat_for_seeding = 0;

float starve_deaths[kAmntOfYears];
float mean_year_deaths = 0;
float acres_for_one_peasant = 0;

float random(float min, float max);
int random(const short min, const short max);

void StartGame();
bool CheckSafe();
void MakeSafe();
bool CheckInput(string []);
void ShowErrorInputMessage();
void MakeOneTurn();
void MakeNewAcrePrice();
void ShowAcrePrice();
void ShowStartMessage();
void ShowSituation();
bool ShowPlayerActions();
void EndGame();