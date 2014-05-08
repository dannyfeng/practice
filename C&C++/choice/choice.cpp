#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <map>

#include "choice.h"

using namespace std;

map<int, Candidate> men;
map<int, Candidate> women;
map<int, Candidate> women_match_done;

void rand_data_of_one_person(Candidate &can)
{
    can.self.looks = rand() % MAX_SCORE + 1;
    can.self.wealth = rand() % MAX_SCORE + 1;
    can.self.quality = rand() % MAX_SCORE + 1;

    can.will.looks = rand() % MAX_WILL + 1;
    can.will.wealth = rand() % (MAX_WILL + 1 - can.will.looks) + 1;
    can.will.quality = 100 - can.will.looks - can.will.wealth;
}

/* generate random data */
void gen_data(GENDER gender, Candidate &oneself)
{
	int i = 0;
	if (MALE == gender)
	{
		men[USER_ID] = oneself;
        print_one(USER_ID, men);

        rand_data_of_one_person(women[i]);
        print_one(i, women);
	}
	else if (FEMALE == gender)
	{
		women[USER_ID] = oneself;
        print_one(USER_ID, women);

        rand_data_of_one_person(men[i]);
        print_one(i, men);
	}
	++i;
	for(; i < CANDIDATE_NUM; ++i)
	{
        rand_data_of_one_person(men[i]);
        print_one(i, men);
        rand_data_of_one_person(women[i]);
        print_one(i, women);
	}

    cout << "generate data completed" << endl;
}

void read_data(GENDER gender, Candidate &oneself)
{
    //the file operation makes me crazy
}

map<int, Candidate>::iterator man_pick_women(Candidate &can, 
    map<int, Candidate> &can_map)
{
    int tmp_score, match_score = 0, tmp_sum, sum = 0;
    map<int, Candidate>::iterator matched_one, can_it;

    matched_one = can_map.end(); 
    for (can_it = can_map.begin(); can_it != can_map.end(); ++can_it)
    {
        // use temporary variable to avoid extreamly long expression 
        tmp_score = can.will.looks * can_it->second.self.looks
            + can.will.wealth * can_it->second.self.wealth
            + can.will.quality * can_it->second.self.quality;
        tmp_sum = can_it->second.self.looks + can_it->second.self.wealth 
            + can_it->second.self.quality;
        if ((tmp_score > match_score)
            || (tmp_score == match_score && tmp_sum > sum)
            )
        {
            match_score = tmp_score;
            sum         = tmp_sum;
            matched_one = can_it;
        }
    }

    return matched_one;
}

list<int>::iterator woman_pick_men(Candidate &can, 
    map<int, Candidate> &can_map, list<int> &lst)
{
    int tmp_score, match_score = 0, tmp_sum, sum = 0;
    list<int>::iterator matched_one, l_it;

    matched_one = lst.end();
    for (l_it = lst.begin(); l_it != lst.end(); ++l_it)
    {
        // use temporary variable to avoid recalculation
        tmp_score = can.will.looks * can_map[*l_it].self.looks
            + can.will.wealth * can_map[*l_it].self.wealth
            + can.will.quality * can_map[*l_it].self.quality;
        tmp_sum = can_map[*l_it].self.looks + can_map[*l_it].self.wealth 
            + can_map[*l_it].self.quality;
        if ((tmp_score > match_score)
            || (tmp_score == match_score && tmp_sum > sum)
            )
        {
            match_score = tmp_score;
            sum         = tmp_sum;
            matched_one = l_it;
        }
    }

    return matched_one;
}

void do_matching(void)
{
    int key;
    int max_admirers;
    list<int>::iterator matched_one, l_it;
    map<int, Candidate>::iterator the_one, can_it;

    cout << "do_matching enter" << endl;

    //do vote, append the male admirers to the women's admirers list.
    for (can_it = men.begin(); can_it != men.end(); ++can_it)
    {
        //women[man_pick_women(can_it->second, women)->first].admirers.push_back(can_it->first);
        man_pick_women(can_it->second, women)->second.admirers.push_back(can_it->first);
    }

    cout << "man_pick_women cmp" << endl;

    //women choose the best man.
    while (women.size() > 0)
    {
        max_admirers = 0;
        the_one = women.end();
        for (can_it = women.begin(); can_it != women.end(); ++can_it)
        {
            if (can_it->second.admirers.size() > max_admirers)
            {
                max_admirers = can_it->second.admirers.size();
                the_one = can_it;
            }
        }

        cout << "found woman with most admirers" << endl;

        key = the_one->first;
        matched_one = woman_pick_men(women[key], men, women[key].admirers);

        cout << "woman_pick_men cmp" << endl;

        women[key].matched_num = *matched_one;  // a couple
        men[*matched_one].matched_num = key;    // matched

        cout << "currend matched couple" << endl;
        print_one(key, women);
        print_one(*matched_one, men);

        women[key].admirers.erase(matched_one); // erase choosen man from admirer list
        women_match_done[key] = women[key];
        women.erase(the_one);    // erase successfully matched women from map<int, Candidate> women

        //append unchooen men to other women
        for (l_it = women_match_done[key].admirers.begin(); 
            l_it != women_match_done[key].admirers.end(); ++l_it)
        {
            //women[man_pick_women(men[*l_it], women)->first].admirers.push_back(*l_it);
            man_pick_women(men[*l_it], women)->second.admirers.push_back(*l_it);
        }
        cout << "append men cmp" << endl;

        women_match_done[key].admirers.clear(); //clear the already matched woman's admirer list
    }
}

void print_one(int one, map<int, Candidate> &can_map)
{
    cout<< one << "\t"
        << can_map[one].self.looks << "\t"
        << can_map[one].self.wealth << "\t"
        << can_map[one].self.quality << "\t"
        << "\t"
        << can_map[one].will.looks << "\t"
        << can_map[one].will.wealth << "\t"
        << can_map[one].will.quality << "\t"
        << endl;
}

void print_all(void)
{
    map<int, Candidate>::iterator the_one, can_it;

    cout<< "women:id,\tl,\tw,\tq\tw_l,\tw_w,\tw_q"
        << "\t<-->\t"
        << "men:id,\tl,\tw,\tq\tw_l,\tw_w,\tw_q"
        << endl;
    for (can_it = women_match_done.begin(); can_it != women_match_done.end(); ++can_it)
    {
        cout// print women
            << can_it->first << "\t"
            << can_it->second.self.looks << "\t"
            << can_it->second.self.wealth << "\t"
            << can_it->second.self.quality << "\t"
            << can_it->second.will.looks << "\t"
            << can_it->second.will.wealth << "\t"
            << can_it->second.will.quality << "\t"
            << "\t"
            // print men
            << can_it->second.matched_num << "\t"
            << men[can_it->second.matched_num].self.looks << "\t"
            << men[can_it->second.matched_num].self.wealth << "\t"
            << men[can_it->second.matched_num].self.quality << "\t"
            << men[can_it->second.matched_num].will.looks << "\t"
            << men[can_it->second.matched_num].will.wealth << "\t"
            << men[can_it->second.matched_num].will.quality << "\t"
            << endl;
    }

    cout << endl;
    
    cout<< "men:id,\tl,\tw,\tq\tw_l,\tw_w,\tw_q"
        << "\t<-->\t"
        << "women:id,\tl,\tw,\tq\tw_l,\tw_w,\tw_q"
        << endl;
    for (can_it = men.begin(); can_it != men.end(); ++can_it)
    {
        cout// print men
            << can_it->first << "\t"
            << can_it->second.self.looks << "\t"
            << can_it->second.self.wealth << "\t"
            << can_it->second.self.quality << "\t"
            << can_it->second.will.looks << "\t"
            << can_it->second.will.wealth << "\t"
            << can_it->second.will.quality << "\t"
            << "\t"
            // print women
            << can_it->second.matched_num << "\t"
            << women_match_done[can_it->second.matched_num].self.looks << "\t"
            << women_match_done[can_it->second.matched_num].self.wealth << "\t"
            << women_match_done[can_it->second.matched_num].self.quality << "\t"
            << women_match_done[can_it->second.matched_num].will.looks << "\t"
            << women_match_done[can_it->second.matched_num].will.wealth << "\t"
            << women_match_done[can_it->second.matched_num].will.quality << "\t"
            << endl;
    }
}

int main(void)
{
	int mode = -1;
	int gender = -1;
	Candidate oneself = {0};

	srand(unsigned(time(0)));   //for rand()

    // do the fucking input
mode_input:
    cout << "Please choose the mode, 1 for random data, 2 for pre_set data:" << endl;
   	cin >> mode;
    while (mode < RANDOM_DATA_MODE || mode > PRESET_DATA_MODE)
    {
        cout << "inputed invalid mode number" << endl;
        goto mode_input;
    }

info_input:
   	cout << "please input your information:" << endl;
   	cin >> gender >> oneself.self.looks >> oneself.self.wealth >> oneself.self.quality
   		>> oneself.will.looks >> oneself.will.wealth >> oneself.will.quality;
    while (gender < FEMALE || gender > MALE
        || oneself.self.looks < 0 || oneself.self.looks > MAX_SCORE
        || oneself.self.wealth < 0 || oneself.self.wealth > MAX_SCORE
        || oneself.self.quality < 0 || oneself.self.quality > MAX_SCORE
        || oneself.will.looks < 1 || oneself.will.looks > MAX_WILL
        || oneself.will.wealth < 1 || oneself.will.wealth > MAX_WILL
        || oneself.will.quality < 1 || oneself.will.quality > MAX_WILL
        || 100 != oneself.will.looks + oneself.will.wealth + oneself.will.quality)
    {
        cout << "inputed wrong information" << endl;
        goto info_input;
    }

    switch (mode)
    {
    case RANDOM_DATA_MODE:
        gen_data((GENDER)gender, oneself);
        break;
    case PRESET_DATA_MODE:
        read_data((GENDER)gender, oneself);
        break;
    default:
        break;
    }

    do_matching();

    cout << endl;
    print_all();
    cout << endl;

    cout << "here is your matching result:" << endl;
    switch (gender)
    {
    case MALE:
        cout << "your data:" << endl;
        print_one(USER_ID, men);
        cout << "matched lady:" << endl;
        print_one(men[USER_ID].matched_num, women_match_done);
        break;
    case FEMALE:
        cout << "your data:" << endl;
        print_one(USER_ID, women_match_done);
        cout << "matched gentleman:" << endl;
        print_one(women_match_done[USER_ID].matched_num, men);
    default:
        break;
    }

    return 0;
}
