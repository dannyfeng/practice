#ifndef CHOICE_H
#define CHOICE_H
#include <vector>
#include <list>
//#include "gaussrand.h"

using namespace std;

#define USER_ID (-1)
#define CANDIDATE_NUM (100)

#define MAX_SCORE (100)
#define MAX_WILL (98)

#if 0   //do not use gauss rand
#define AVERAGE_SCORE (50)
//#define AVERAGE_WILL (33)

#define SCORE_STANDARD_DEVIATION (MAX_SCORE - AVERAGE_SCORE)
//#define WILL_STANDARD_DEVIATION (MAX_WILL - AVERAGE_WILL)

#define RAND_SCORE ((gaussrand() * SCORE_STANDARD_DEVIATION) + AVERAGE_SCORE + 1)
//#define RAND_WILL (gaussrand() * WILL_STANDARD_DEVIATION + AVERAGE_WILL)
#endif

enum GENDER {
	FEMALE = 0,
	MALE,
};

enum MODE {
    RANDOM_DATA_MODE = 1,
    PRESET_DATA_MODE,
};

typedef struct _Person {
	int looks;
	int wealth;
	int quality;
} Person;

typedef struct _Candidate {
//    int id;
	Person self;
	Person will;
    int matched_num;
	list<int> admirers;
} Candidate;

void print_all(void);
void print_one(int one, map<int, Candidate> &can_map);

#endif /* !CHOICE_H */

