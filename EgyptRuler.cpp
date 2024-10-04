#include "EgyptRuler.h"

void StartGame()
{
    if (!CheckSafe())
    {
        ShowStartMessage();
        MakeNewAcrePrice();
        ShowAcrePrice();
        while(!ShowPlayerActions())
        {
            ShowErrorInputMessage();
        }
    }
    else
    {
        ShowSituation();
        MakeNewAcrePrice();
        ShowAcrePrice();
        while(!ShowPlayerActions())
        {
            ShowErrorInputMessage();
        }
    }

    while(year != 10)
    {
        MakeOneTurn();
        if (lost)
            break;
        MakeSafe();
        ShowSituation();
        MakeNewAcrePrice();
        ShowAcrePrice();
        while(!ShowPlayerActions())
        {
            ShowErrorInputMessage();
        }
        
    }
    
    EndGame();
}

bool CheckSafe()
{
    bool isOpened = false;
    isafe.open("save.txt");
    if(isafe.is_open())
    {
        cout << "Найдено сохранение, хотите загрузиться и продолжить игру с последнего сохраненного места?" << endl;
        cout << "[y | n]: ";
        char choice = ' ';
        cin >> choice;
        while ((choice != 'y') && (choice != 'n'))
        {
            cout << "Неправильно введенные данные, введите 'y' если yes и 'n' если no" << endl;
            cin >> choice;
        }

        if (choice == 'y')
        {
            isafe >> year >> lost >> population >> available_wheat >> city_area >> isPlague >> isDrought
            >> died_from_starving >> arrived_to_city >> gathered_wheat >> wheat_from_one_acre
            >> wheat_rats_ate >> player_buys_acres >> player_sells_acres >> wheat_for_eating
            >> wheat_for_seeding;
            for (int j = 0; j < AMNT_OF_YEARS; ++j)
                isafe >> starve_deaths[j];
            isOpened = true;
        }
    }
    isafe.close();

    return isOpened;
}

void MakeSafe()
{
    cout << "Хотите сохраниться и выйти из игры?" << endl << "[y | n]: ";
    char choice = ' ';
    cin >> choice;
    while ((choice != 'y') && (choice != 'n'))
    {
        cout << "Неправильно введенные данные, введите 'y' если yes и 'n' если no" << endl;
        cin >> choice;
    }
    cout << endl;

    if (choice == 'y')
    {
        osafe.open("save.txt");
        if (osafe.is_open())
        {
            osafe << year << " " << lost << " " << population << " " << available_wheat << " " << city_area << " " << isPlague 
            << " " << isDrought << " " << died_from_starving << " " << arrived_to_city << " " << gathered_wheat << " " 
            << wheat_from_one_acre << " " << wheat_rats_ate << " " << player_buys_acres << " " << player_sells_acres << " " 
            << wheat_for_eating << " " << wheat_for_seeding << " ";
            for (int j = 0; j < AMNT_OF_YEARS; ++j)
                osafe << starve_deaths[j] << " ";

            cout << "Игра сохранена";
        }
        else
        {
            cout << "Не удалось открыть файл сохранения";
        }
        osafe.close();
        abort();
    }
}

void MakeOneTurn()
{
    wheat_from_one_acre = random(MIN_WHEAT_FROM_ONE_ACRE, MAX_WHEAT_FROM_ONE_ACRE);
    int avail_area = wheat_for_seeding / WHEAT_PER_ACRE; // куда уходит wheat_for_seeding если ее больше чем территории?
    int extra_wheat = 0;
    if (avail_area > city_area)
    {
        extra_wheat = (avail_area - city_area) * WHEAT_PER_ACRE;
        avail_area = city_area;
    }


    int peasant_cant_harvest = (avail_area - (population * PEASANT_MAX_AREA)) * wheat_from_one_acre;

    if (peasant_cant_harvest > 0)
        gathered_wheat = (wheat_from_one_acre * avail_area) - peasant_cant_harvest;
    else
        gathered_wheat = wheat_from_one_acre * avail_area;

    available_wheat += gathered_wheat + extra_wheat;

    wheat_rats_ate = random(MIN_RATS_ATE_AMPL, MAX_RATS_ATE_AMPL) * available_wheat;
    available_wheat -= wheat_rats_ate;

    died_from_starving = 0;
    if (population * PEASANT_EAT_WHEAT > wheat_for_eating)
    {
        died_from_starving = population - (wheat_for_eating / PEASANT_EAT_WHEAT); 
        // остается ли остаток пшеницы, если последнему человеку осталось немного, но не достаточно?
    }
    else
    {
        available_wheat = available_wheat - population * PEASANT_EAT_WHEAT + wheat_for_eating;
    }

    starve_deaths[year-1] = (float)died_from_starving / (float)population; // логирование среднегодовой смертности
    if (starve_deaths[year-1] > MAX_DIED_FROM_STARVING)
    {
        lost = true;
    }
    population -= died_from_starving;

    if (population == 0)
    {
        lost = true;
    }

    arrived_to_city = died_from_starving / 2 + (5 - wheat_from_one_acre) * available_wheat / 600 + 1;

    if (arrived_to_city < 0)
        arrived_to_city = 0;
    else if(arrived_to_city > 50)
        arrived_to_city = 50;
    population += arrived_to_city;

    if (random((float)0, (float)1) <= PLAGUE_CHANCE)
    {
        isPlague = true;
        population /= 2;
    }
    else
        isPlague = false;

    if (random((float)0, (float)1) <= DROUGHT_CHANCE)
    {
        gathered_wheat /= 2;
        isDrought = true;
    }
    else
        isDrought = false;

    year++;
}

void ShowStartMessage()
{
    cout << "Владыка Песков\n\n";
    cout <<"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀  ⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        <<"⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀    ⠀⠀⠀⣠⣞⢻⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        << "⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀  ⠀⠀ ⠀⢀⠔⠑⠒⠂⢏⠵⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        << "⠀⠀⠀ ⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⢀⡔⠁⠀⠤⠀⠀⠈⡏⠡⠵⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        << "⠀⠀⠀⠀   ⠀⠀⠀⠀⠀⠀⠀⡴⠋⠀⠒⠒⠋⠉⠉⠉⠸⡐⠛⠋⠳⡀⠀⠀⠀⠀⢀⣔⣻⣦⡀⠀⠀⠀⠀⠀⠀\n"
        << "⠀⠀⠀⠀⠀⠀ ⠀⠀ ⠀ ⣠⠊⠈⠀⠀⠀⠁⠀⢀⣀⣀⣀⣣⠡⠶⠤⠬⠦⡀⢀⡴⠣⣤⣉⣫⢻⣆⠀⠀⠀⠀⠀\n"
        << "   ⣠⣾⣦⡀⠀⠀⠀⣠⠊⠀⠀⠀⠀⠀⠀⠀⠐⢀⣀⠀⠀⠈⡆⠒⠒⠒⠒⠘⢿⡒⠒⣤⣄⣈⡖⣤⣑⣄⠀⠀⠀\n"
        << "⠀⢀⣾⣿⡽⠿⣿⣦⣀⠞⠁⡀⠀⠀⢀⠈⠉⠉⠉⠉⠉⠉⠉⠁⠀⠸⡉⠩⠭⢥⠀⠀⠙⢶⠮⢤⣤⠸⡬⢭⡞⠳⡄⠀\n"
        << "⠐⠛⠚⠓⠒⠓⠓⠓⠓⠒⠒⠒⠒⠒⠒⠒⠛⠛⠛⠛⠛⠛⠛⠛⠒⠒⠓⠛⠛⠛⠛⠛⠛⠒⠓⠒⠒⠒⠓⠒⠛⠛⠛⠂\n\n\n";
    cout << "О великий правитель Египта! Я, твой верный помощник, рад приветствовать тебя на этом посту.\n"
    << "Тебе вверена судьба наших земель, и перед тобой великие задачи. \n"
    << "Народ твой ждет мудрых решений, а земли – заботливой руки.\n\n"
    << "На начало твоего правления у тебя есть 2 800 бушелей пшеницы, 100 подданных и 1 000 акров плодородной земли, \n"
    << "которая ждет твоего распоряжения. Эти земли можно продать или купить за пшеницу, чтобы увеличить свои владения. \n"
    << "Решай, сколько пшеницы ты пожелаешь выделить на еду для твоих людей, а сколько – для посадки новых культур.\n"
    << "Твои правления будут длиться 10 лет, и от твоей мудрости зависит, станешь ли ты легендой для будущих поколений\n"
    << "или имя твоё будет забыто в песках времени.\n"
    << "Да благословят тебя боги, Фараон. Впереди великие годы твоего правления!\n\n";
}

void ShowSituation()
{
    cout << "Мой великий правитель, соизволь поведать тебе:\n";
    cout << "В году " << year << " твоего высочайшего правления,\n";

    if (died_from_starving)
        cout << died_from_starving << " человек умерли с голоду, и\n";

    if (arrived_to_city)
        cout << arrived_to_city << " человек прибыли в наш великий город;\n";

    if (isPlague)
        cout << "Чума, увы, забрала половину твоего народа;\n";

    cout << "Население города сейчас составляет " << population << " человек;\n";
    if (isDrought)
    {
        cout << "К несчастью, засуха поразила наши поля, и половина посевов погибла;\n";
        cout << "Мы собрали " << gathered_wheat << " бушелей пшеницы, по " << wheat_from_one_acre/2 << " бушелей с акра;\n";
    }
    else   
        cout << "Мы собрали " << gathered_wheat << " бушелей пшеницы, по " << wheat_from_one_acre << " бушелей с акра;\n";
    cout << "Крысы поели " << wheat_rats_ate << " бушелей пшеницы, оставив в амбарах " << available_wheat << " бушелей;\n";
    cout << "Город сейчас занимает " << city_area << " акров;\n";
}

void MakeNewAcrePrice() { acre_price = random(MIN_ACRE_PRICE, MAX_ACRE_PRICE); }

void ShowAcrePrice() { cout << "Один акр земли стоит ныне " << acre_price << " бушелей.\n\n"; }

bool ShowPlayerActions()
{
    string splayer_buys_acres;
    string splayer_sells_acres;
    string swheat_for_eating;
    string swheat_for_seeding;

    cout << "Что пожелаешь, о мудрейший из правителей?\n";
    cout << "Сколько акров земли повелеваешь купить? ";
    cin >> splayer_buys_acres;
    cout << "Сколько акров земли повелеваешь продать? ";
    cin >> splayer_sells_acres;
    cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
    cin >> swheat_for_eating;
    cout << "Сколько бушелей пшеницы повелеваешь потратить на сеяние? ";
    cin >> swheat_for_seeding;
    cout << endl;

    string inputs[4] = {splayer_buys_acres, splayer_sells_acres, swheat_for_eating, swheat_for_seeding};
    if (!CheckInput(inputs))
    {
        return false;
    }

    city_area += player_buys_acres;
    available_wheat -= player_buys_acres * acre_price;
    city_area -= player_sells_acres;
    available_wheat += player_sells_acres * acre_price;
    available_wheat -= (wheat_for_eating + wheat_for_seeding);

    cout << "Твои решения определят будущее нашего великого народа!\n\n";

    return true;
}

void ShowErrorInputMessage()
{
    cout << "Смилуйтесь, мой повелитель! У нас нет столько ресурсов! Давайте я повторю свой отчет на этот год\n";
    ShowSituation();
    ShowAcrePrice();
}

bool CheckInput(string test_strings[])
{
    bool checkFailed = false;
    for(int j = 0; j < 4; ++j)
    {
        int i = 0;
        while (test_strings[j][i])
        {
            if(test_strings[j][i]<'0' || test_strings[j][i]>'9') 
            {
                checkFailed = true;
                break;
            }

            i++;
        }
    }

    if (!checkFailed)
    {
        player_buys_acres = stoi(test_strings[0]);
        player_sells_acres = stoi(test_strings[1]);
        wheat_for_eating = stoi(test_strings[2]);
        wheat_for_seeding = stoi(test_strings[3]);
    }

    if ((int)(available_wheat - wheat_for_eating - wheat_for_seeding + player_sells_acres * acre_price - player_buys_acres * acre_price) < 0 || 
    player_sells_acres > city_area)
    {
        checkFailed = true;
    }

    return !checkFailed;
}

void EndGame()
{
    if(lost)
    {
        cout << "О, великий правитель! Горе и страдание постигли наш народ. Слишком много душ покинули этот мир \n"
        << "из-за голода и болезней, и теперь в городе слышны крики недовольства. Народ, измученный лишениями, \n"
        << "готов восстать, и ты рискуешь потерять свой трон. Неужели твоё правление завершится столь плачевно? \n"
        << "Боги отступили, и судьба Египта висит на волоске...\n";
        cout.precision(4);
        cout << starve_deaths[year-2] * 100 << " процентов населения умерло от голода\n\n";
    }
    else
    {
        for(int i = 0; i < 10; ++i)
        {
            mean_year_deaths += starve_deaths[i];
        }
        mean_year_deaths /= 10;
        acres_for_one_peasant = city_area / population;

        cout << "Результаты игры:\n";
        if (mean_year_deaths > 0.33 && acres_for_one_peasant < 7)
            cout << "Ваша неспособность управлять землей и народом привела к хаосу.\n"
            << "Восставшие египтяне свергли вас с трона и изгнали из дворца.\n"
            << "Теперь вы скитаетесь по пустыне, мечтая об утраченном величии\n\n";
        else if (mean_year_deaths > 0.1 && acres_for_one_peasant < 9)
            cout << "Вы правили, как жестокий фараон, и народ терпел вас с трудом.\n"
            << "Когда ваша власть ослабла, они с радостью освободились от вашего правления.\n\n"
            << "Никто больше не желает видеть вас на троне\n";
        else if (mean_year_deaths > 0.03 && acres_for_one_peasant < 10)
            cout << "Ваше правление было достойным, хотя не все были довольны вашими решениями.\n"
            << "Однако многие сограждане с уважением вспоминают о ваших достижениях и готовы вновь увидеть вас у власти\n\n";
        else
            cout << "Ваше правление стало легендой. Даже такие великие фараоны, \n"
            << "как Рамсес II и Тутанхамон, гордились бы вашими деяниями.\n"
            << "Народ славит вас как самого мудрого и справедливого правителя Египта!\n\n";

        cout << "Статистика:\n";
        cout << "Среднегодовой процент смерти от голода: " << mean_year_deaths * 100 << endl;
        cout << "Количество акров на одного жителя: " << acres_for_one_peasant << endl << endl;
    }

    cout << "****КОНЕЦ****\n";
    cout << "Нажмите любую клавишу, чтобы выйти\n";
    _getch();
}

float random(float min, float max)
{
    return (float)(rand())/RAND_MAX*(max - min) + min;
}

int random(const short min, const short max)
{
    return (rand() % (max - min + 1) + min);
}

int main (int argc, char* argv[])
{
    srand(time(0));
    for (int i = 0; i < AMNT_OF_YEARS; ++i)
        starve_deaths[i] = 0;
    StartGame();
}