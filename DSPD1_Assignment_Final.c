//  BY:->
//  Bhupesh Tiwary-BT21CSE002
//  Devansh Negi-BT21CSE012
#include <stdio.h>
char man_Of_All_Matches[25] = "";
char highest_Total_Individual_Run_Player[25] = "";

struct player
{
    char player_id[25];
    int previous_total_score;
    float previous_avg;
    int previous_total_wickets;
    float previous_wicket_avg;
    int total_runs_in_last_five_matches;
    int total_wickets_in_last_five_matches;
    int century;
    int present_match_score;
    int present_match_wicket;
    int player_role; // 1,2,3
    int pacer;       //1- Pacer 2- Non-Pacer
    int out;         //OUT =1 NOT_OUT=0
};

struct team
{
    struct player lineup[15];
    int team_no;
    int matches_played;
    int points;
} team_list[100];

struct match_played
{
    int match_id[25];
    int team_first; 
    int team_second; 
    int highest_run;
    char man_of_the_match[25]; //Stores player_id
    int wicket_taken_by_pacer;
    int result; //1,2,3
} total_matches_played[1000];

struct player_frequency_pair
{
    char player_id[25];
    int frequency_of_MOTM;
};

void team_Update(int first)
{
    printf("TEAM %d Update\n", first);
    for (int i = 0; i < 15; i++)
    {
        printf("PLAYER %d Update\n", i);
        int present_score = 0, present_wicket = 0;
        printf("\nEnter present_match_score\t");
        scanf(" %d", &present_score);
        printf("\nEnter present_match_wicket\t");
        scanf(" %d", &present_wicket);
        printf("\nEnter if out(1)or not_out(0)\t");
        scanf(" %d", &team_list[first].lineup[i].out);
        team_list[first].lineup[i].previous_total_score += team_list[first].lineup[i].present_match_score;
        team_list[first].lineup[i].previous_total_wickets += team_list[first].lineup[i].present_match_wicket;
        team_list[first].lineup[i].previous_avg = (team_list[first].lineup[i].previous_avg * (team_list[first].matches_played - 1) + team_list[first].lineup[i].present_match_score) / team_list[first].matches_played;
        team_list[first].lineup[i].previous_wicket_avg = (team_list[first].lineup[i].previous_wicket_avg * (team_list[first].matches_played - 1) + team_list[first].lineup[i].present_match_wicket) / team_list[first].matches_played;

        //Now update present match score after updating all values for previous matches
        team_list[first].lineup[i].present_match_score = present_score;
        team_list[first].lineup[i].present_match_wicket = present_wicket;

        if (team_list[first].lineup[i].present_match_score >= 100)
        {
            team_list[first].lineup[i].century += 1;
        }
    }
}
int match(int current_match_index, int first, int second)
{
    printf("Match %d between Team %d and Team %d\n", current_match_index + 1, first, second);

    printf("\nEnter match_id\t");
    scanf(" %s", &total_matches_played[current_match_index].match_id);
    printf("\nEnter highest run\t");
    scanf(" %d", &total_matches_played[current_match_index].highest_run);
    printf("\nEnter wicket taken by pacer\t");
    scanf(" %d", &total_matches_played[current_match_index].wicket_taken_by_pacer);

    printf("\nEnter result(1-> team %d won,2-> team %d won,3-> Tie)\t", first, second);
    scanf(" %d", &total_matches_played[current_match_index].result);

    if (total_matches_played[current_match_index].result == 1)
    {
        team_list[first].points += 2;
    }
    else if (total_matches_played[current_match_index].result == 2)
    {
        team_list[second].points += 2;
    }
    else
    {
        team_list[first].points++;
        team_list[second].points++;
    }
    total_matches_played[current_match_index].team_first = first;
    team_list[first].matches_played++;
    total_matches_played[current_match_index].team_first = second;
    team_list[second].matches_played++;

    current_match_index++;

    //Updating first team stats
    team_Update(first);
    //Updating second team stats
    team_Update(second);

    return current_match_index;
}

int strcmp(char s1[], char s2[])
{
    int i = 0;
    while (s1[i] == s2[i] && s1[i] != '\0')
        i++;
    return s1[i] - s2[i];
}
void copy_string(char s1[], char s2[])
{
    int i = 0;
    while (s1[i] != '\0')
    {
        s2[i] = s1[i];
        i++;
    }
}
int get_Century_Count(char s[], int total_teams)
{
    int century_count = 0;
    for (int i = 0; i < total_teams; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (strcmp(s, team_list[i].lineup[j].player_id) == 0)
                century_count = team_list[i].lineup[j].century;
        }
    }
    return century_count;
}
void merge_MOTM(struct player_frequency_pair a[], int low, int mid, int high)
{
    struct player_frequency_pair ans[100];
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high)
    {
        //Frequency in decreasing order
        if (a[i].frequency_of_MOTM > a[j].frequency_of_MOTM)
            ans[k++] = a[i++];
        else if (a[i].frequency_of_MOTM < a[j].frequency_of_MOTM)
            ans[k++] = a[j++];
        else // On basis of name increasing order
        {
            if (strcmp(a[i].player_id, a[j].player_id) < 0)
                ans[k++] = a[i++];
            else
                ans[k++] = a[j++];
        }
    }

    while (i <= mid)
        ans[k++] = a[i++];
    while (j <= high)
        ans[k++] = a[j++];

    for (int i = low; i <= high; i++)
        a[i] = ans[i];
}
void sort_MOTM(struct player_frequency_pair MOTM_Players[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        sort_MOTM(MOTM_Players, low, mid);
        sort_MOTM(MOTM_Players, mid + 1, high);
        merge_MOTM(MOTM_Players, low, mid, high);
    }
}
void merge_player(struct player a[], int low, int mid, int high)
{
    struct player ans[100];
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high)
    {
        //on basis of current runs in decreasing order
        if (a[i].present_match_score > a[j].present_match_score)
            ans[k++] = a[i++];
        else if (a[i].present_match_score < a[j].present_match_score)
            ans[k++] = a[j++];
        else //Names in increasing order
        {
            if (strcmp(a[i].player_id, a[j].player_id) < 0)
                ans[k++] = a[i++];
            else
                ans[k++] = a[j++];
        }
    }

    while (i <= mid)
        ans[k++] = a[i++];
    while (j <= high)
        ans[k++] = a[j++];

    for (int i = low; i <= high; i++)
        a[i] = ans[i];
}
void sort_player(struct player list[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        sort_player(list, low, mid);
        sort_player(list, mid + 1, high);
        merge_player(list, low, mid, high);
    }
}
void merge_team(int low, int mid, int high)
{
    struct team ans[100];
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high)
    {
        //on basis of Team Points in decreasing order
        if (team_list[i].points > team_list[j].points)
            ans[k++] = team_list[i++];
        else
            ans[k++] = team_list[j++];
    }

    while (i <= mid)
        ans[k++] = team_list[i++];
    while (j <= high)
        ans[k++] = team_list[j++];

    for (int i = low; i <= high; i++)
        team_list[i] = ans[i];
}
void sort_team(int low, int high) // Sort on basis of Team Points
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        sort_team(low, mid);
        sort_team(mid + 1, high);
        merge_team(low, mid, high);
    }
}
void merge_Combinations(struct player a[], int low, int mid, int high)
{
    struct player ans[100];
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high)
    {
        if (a[i].player_role == 2 && a[j].player_role == 2) // Sorting Bowlers
        {
            // sort on basis of previous total wicket
            if (a[i].previous_total_wickets > a[j].previous_total_wickets)
                ans[k++] = a[i++];
            else if (a[i].previous_total_wickets < a[j].previous_total_wickets)
                ans[k++] = a[j++];
            else // sort on basis of previous wicket avg
            {
                if (a[i].previous_wicket_avg > a[j].previous_wicket_avg)
                    ans[k++] = a[i++];
                else if (a[i].previous_wicket_avg < a[j].previous_wicket_avg)
                    ans[k++] = a[j++];
            }
        }
        else // Sorting batsman
        {
            // sort on basis of previous total runs
            if (a[i].previous_total_score > a[j].previous_total_score)
                ans[k++] = a[i++];
            else if (a[i].previous_total_score < a[j].previous_total_score)
                ans[k++] = a[j++];
            else // sort on basis of previous wicket avg
            {
                if (a[i].previous_avg > a[j].previous_avg)
                    ans[k++] = a[i++];
                else if (a[i].previous_avg < a[j].previous_avg)
                    ans[k++] = a[j++];
            }
        }
    }

    while (i <= mid)
        ans[k++] = a[i++];
    while (j <= high)
        ans[k++] = a[j++];

    for (int i = low; i <= high; i++)
        a[i] = ans[i];
}
void sort_Combinations(struct player list[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        sort_Combinations(list, low, mid);
        sort_Combinations(list, mid + 1, high);
        merge_Combinations(list, low, mid, high);
    }
}
int present(struct player_frequency_pair player_list[], int size, char s[])
{
    int ans = -1, found = 0;
    for (int i = 0; (i < size) && (found == 0); i++)
    {
        if (strcmp(s, player_list[i].player_id) == 0)
        {
            found = 1;
            ans = i;
        }
    }
    return ans;
}

int main()
{
    //Array of match played=>each match played has 2 teams=>each team has 15 players
    int total_teams;
    printf("Enter number of teams\n");
    scanf("%d", &total_teams);
    int total_matches_played_by_group = total_teams * (total_teams - 2) / 4;
    int current_match_index = 0;
    // Program only works for 6 or more even numbered teams
    if (total_teams <= 6 || total_teams % 2 != 0)
        return 0;

    //Intitialization of Teams and players
    for (int i = 0; i < total_teams; i++)
    {
        printf("TEAM %d initialization\n", i);

        team_list[i].matches_played = 0;
        team_list[i].points = 0;
        team_list[i].team_no = i;
        for (int j = 0; j < 15; j++)
        {
            printf("PLAYER %d initialization\n", j);

            printf("\nEnter player_id\t");
            scanf(" %s", &team_list[i].lineup[j].player_id);
            printf("\nEnter player_role(1=Batsman,2=Bowler,3=All-Rounder)\t");
            scanf(" %d", &team_list[i].lineup[j].player_role);
            if (team_list[i].lineup[j].player_role != 1)
            {
                printf("\nEnter 1 if player is pacer\t");
                scanf(" %d", &team_list[i].lineup[j].pacer);
            }
            else
            {
                team_list[i].lineup[j].pacer = 0;
            }
            team_list[i].lineup[j].previous_total_score = 0;
            team_list[i].lineup[j].previous_avg = 0;
            team_list[i].lineup[j].previous_total_wickets = 0;
            team_list[i].lineup[j].previous_wicket_avg = 0;
            team_list[i].lineup[j].total_runs_in_last_five_matches = 0;
            team_list[i].lineup[j].total_wickets_in_last_five_matches = 0;
            team_list[i].lineup[j].century = 0;
            team_list[i].lineup[j].present_match_score = 0;
            team_list[i].lineup[j].present_match_wicket = 0;
            team_list[i].lineup[j].out = 0;
        }
    }

    //Round Robin For Group 1
    printf("Group 1 Round Robin\n");
    for (int i = 0; i < total_teams / 2 - 1; i++)
    {
        for (int j = i + 1; j < total_teams / 2; j++)
        {
            current_match_index = match(current_match_index, i, j);
        }
    }
    //Round Robin For Group 2
    printf("Group 2 Round Robin\n");
    for (int i = total_teams / 2; i < total_teams - 1; i++)
    {
        for (int j = i + 1; j < total_teams; j++)
        {
            current_match_index = match(current_match_index, i, j);
        }
    }

    // Sort on the basis of points
    sort_team(0, total_teams / 2 - 1);
    sort_team(total_teams / 2, total_teams - 1);
    //Have a match between first 2 teams of each group

    printf("--*--*--*--*--*--*--*--*--*GROUP 1 Semi-Finals--*--*--*--*--*--*--*--*--*\n");

    printf("Match %d between Team %d and Team %d \n", current_match_index + 1, team_list[0].team_no, team_list[1].team_no);
    printf("\nEnter match_id\t");
    scanf(" %s", &total_matches_played[current_match_index].match_id);
    printf("\nEnter highest run\t");
    scanf(" %d", &total_matches_played[current_match_index].highest_run);
    printf("\nEnter wicket taken by pacer\t");
    scanf(" %d", &total_matches_played[current_match_index].wicket_taken_by_pacer);

    printf("\nEnter result(1-> team %d won,2-> team %d won,3-> Tie)\t", team_list[0].team_no, team_list[1].team_no);
    scanf(" %d", &total_matches_played[current_match_index].result);
    if (total_matches_played[current_match_index].result == 1)
    {
        team_list[0].points += 2;
    }
    else if (total_matches_played[current_match_index].result == 2)
    {
        team_list[1].points += 2;

        // SHIFT WINNER TO 0th Position
        struct team t = team_list[1];
        team_list[1] = team_list[0];
        team_list[0] = t;
    }
    else
    {
        team_list[0].points++;
        team_list[1].points++;
    }
    total_matches_played[current_match_index].team_first = team_list[0].team_no;
    team_list[0].matches_played++;
    total_matches_played[current_match_index].team_first = team_list[1].team_no;
    team_list[1].matches_played++;
    current_match_index++;

    //Updating first team stats
    team_Update(team_list[0].team_no);
    //Updating second team stats
    team_Update(team_list[1].team_no);

    printf("--*--*--*--*--*--*--*--*--*GROUP 2 Semi-Finals--*--*--*--*--*--*--*--*--*\n");

    int x = total_teams / 2;
    printf("Match %d between Team %d and Team %d \n", current_match_index + 1, team_list[x].team_no, team_list[x + 1].team_no);
    printf("\nEnter match_id\t");
    scanf(" %s", &total_matches_played[current_match_index].match_id);
    printf("\nEnter highest run\t");
    scanf(" %d", &total_matches_played[current_match_index].highest_run);
    printf("\nEnter wicket taken by pacer\t");
    scanf(" %d", &total_matches_played[current_match_index].wicket_taken_by_pacer);

    printf("\nEnter result(1-> team %d won,2-> team %d won,3-> Tie)\t", team_list[x].team_no, team_list[x + 1].team_no);
    scanf(" %d", &total_matches_played[current_match_index].result);
    if (total_matches_played[current_match_index].result == 1)
    {
        team_list[x].points += 2;
    }
    else if (total_matches_played[current_match_index].result == 2)
    {
        team_list[x + 1].points += 2;

        // SHIFT WINNER TO (total_teams / 2 )th Position
        struct team t = team_list[x + 1];
        team_list[x + 1] = team_list[x];
        team_list[x] = t;
    }
    else
    {
        team_list[x].points++;
        team_list[x + 1].points++;
    }
    total_matches_played[current_match_index].team_first = team_list[x].team_no;
    team_list[x].matches_played++;
    total_matches_played[current_match_index].team_first = team_list[x + 1].team_no;
    team_list[x + 1].matches_played++;
    current_match_index++;
    //Updating first team stats
    team_Update(team_list[x].team_no);
    //Updating second team stats
    team_Update(team_list[x + 1].team_no);

    //Finals between Team at index 0 and at index total_teams/2

    printf("--*--*--*--*--*--*--*--*--*FINALS--*--*--*--*--*--*--*--*--*\n");

    x = 0;
    int y = total_teams / 2;
    int winner_index;
    printf("Match %d between Team %d and Team %d \n", current_match_index + 1, team_list[x].team_no, team_list[y].team_no);
    printf("\nEnter match_id\t");
    scanf(" %s", &total_matches_played[current_match_index].match_id);
    printf("\nEnter highest run\t");
    scanf(" %d", &total_matches_played[current_match_index].highest_run);
    printf("\nEnter wicket taken by pacer\t");
    scanf(" %d", &total_matches_played[current_match_index].wicket_taken_by_pacer);

    printf("\nEnter result(1-> team %d won,2-> team %d won,3-> Tie)\t", team_list[x].team_no, team_list[y].team_no);
    scanf(" %d", &total_matches_played[current_match_index].result);
    if (total_matches_played[current_match_index].result == 1)
    {
        team_list[x].points += 2;
        winner_index = x;
    }
    else if (total_matches_played[current_match_index].result == 2)
    {
        team_list[y].points += 2;
        winner_index = y;
    }
    else
    {
        team_list[x].points++;
        team_list[y].points++;
    }
    total_matches_played[current_match_index].team_first = team_list[x].team_no;
    team_list[x].matches_played++;
    total_matches_played[current_match_index].team_first = team_list[y].team_no;
    team_list[y].matches_played++;
    current_match_index++;
    //Updating first team stats
    team_Update(team_list[x].team_no);
    //Updating second team stats
    team_Update(team_list[y].team_no);
    printf("WINNER OF the Tournament is TEAM %d", team_list[winner_index].team_no);

    // EXECUTING THE FUNCTIONS

    //PART 1
    valid_Combinations(total_teams);
    //PART 2
    highest_total_individual_run(total_matches_played, total_teams, 1);
    //PART 3
    man_Of_The_Match(total_teams, total_matches_played_by_group * 2); // 2 Groups are there
    //PART 4
    check_For_Same_Player();
    // PART 5
    // 2 Groups are there along with 2 semifinals and 1 final
    highest_average_by_individual(team_list, total_matches_played, total_teams, total_matches_played_by_group * 2 + 3);
    // PART 6
    difference_In_Total_Wickets_Between_Pacers_And_Spinners_Of_All_Teams(total_teams, total_matches_played_by_group * 2 + 3);
    // PART 7
    int k;
    printf("Enter k for K-times man of the match");
    scanf("%d", &k);
    k_times_Man_Of_The_Match(k, total_teams, total_matches_played_by_group * 2 + 3);
    return 0;
}

//PART 1
void valid_Combinations(int total_teams)
{
    struct player batsman[100];
    struct player bowler[100];
    struct player playing_11[11];
    int curr_batsman_index = 0;
    int curr_bowler_index = 0;
    // Getting all batsman and bowlers
    for (int i = 0; i < total_teams; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (team_list[i].lineup[j].player_role != 1)
            {
                bowler[curr_bowler_index] = team_list[i].lineup[j];
                curr_bowler_index++;
            }
            else
            {
                batsman[curr_batsman_index] = team_list[i].lineup[j];
                curr_batsman_index++;
            }
        }
    }
    if (curr_batsman_index <= 3 || curr_bowler_index <= 2)
    {
        printf("Team Not Possible");
    }
    else
    {
        // Sorting to get best 2 players
        sort_Combinations(batsman, 0, curr_batsman_index);
        sort_Combinations(bowler, 0, curr_bowler_index);

        int batsman_number = 5;
        while (batsman_number <= 7)
        {
            printf("POSSIBLE Team Combo:->");
            int bowler_number = 11 - batsman_number;
            for (int i = 0; i < batsman_number; i++)
                printf("Batsman selected with player id => %s ", batsman[i].player_id);
            for (int i = 0; i < bowler_number; i++)
                printf("Bowler selected with player id => %s ", bowler[i].player_id);
            batsman_number++;
        }
    }
}
//PART 2
void highest_total_individual_run(struct match_played matches[], int total_teams, int matches_played)
{
    int max_individual_run = team_list[0].lineup[0].previous_total_score;
    int team_index = 0, player_index = 0;
    int max_individual_run_count = 1;
    for (int i = 0; i < total_teams; i++) //Traverse through all teams
    {
        for (int j = 0; j < 15; j++) //Traverse through all players
        {
            int curr_individual_run = team_list[i].lineup[j].previous_total_score;
            if (max_individual_run < curr_individual_run)
            {
                max_individual_run = curr_individual_run;
                team_index = i;
                player_index = j;
                max_individual_run_count = 1;
            }
            else if (max_individual_run == curr_individual_run)
            {
                max_individual_run_count++;
            }
        }
    }

    if (max_individual_run_count == 1)
    {
        copy_string(team_list[team_index].lineup[player_index].player_id, highest_Total_Individual_Run_Player);
        printf("Highest Total Individual Run Is By Player %s ", team_list[team_index].lineup[player_index].previous_total_score);
    }
    else //Multiple Player Exist with same previous_total_match_score
    {
        struct player possible_player_list[100];
        int possible_player_list_index = 0;
        for (int i = 0; i < total_teams; i++) //Traverse through all teams
        {
            for (int j = 0; j < 15; j++) //Traverse through all players
            {
                int curr_individual_run = team_list[i].lineup[j].previous_total_score;
                if (max_individual_run == curr_individual_run)
                {
                    possible_player_list[possible_player_list_index] = team_list[team_index].lineup[j];
                }
            }
        }

        // Sorts on Both variables runs and name later
        sort_player(possible_player_list, 0, possible_player_list_index);
        printf("Highest Total Individual Run Is By Player %s ", possible_player_list[0].player_id);
        copy_string(possible_player_list[0].player_id, highest_Total_Individual_Run_Player);
    }
}

//PART 3
void man_Of_The_Match(int total_teams, int number_of_matches_played)
{
    struct player_frequency_pair MOTM_Players[25];
    int MOTM_Player_count = 0;
    for (int i = 0; i < 25; i++)
    {
        MOTM_Players[i].frequency_of_MOTM = 0;
        // MOTM_Players[i].player_id[0] = "";
    }

    // Getting frequency of all MOTM PLAYERS
    for (int i = 0; i < number_of_matches_played; i++)
    { // look for player_id in struct key_value_pair
        int index = present(MOTM_Players, MOTM_Player_count, total_matches_played[i].man_of_the_match);
        if (index == -1)
        {
            ++MOTM_Player_count;
            //Copy str1 to str2
            copy_string(total_matches_played[i].man_of_the_match, MOTM_Players[MOTM_Player_count].player_id);
            MOTM_Players[MOTM_Player_count].frequency_of_MOTM += 1;
        }
        else
        {
            MOTM_Players[index].frequency_of_MOTM += 1;
        }
    }

    sort_MOTM(MOTM_Players, 0, MOTM_Player_count); //In decreasing order  ,1 for integer based sorting
    printf("Man Of ALL Matches Is Player=> %s \n", MOTM_Players[0].player_id);
    copy_string(MOTM_Players[0].player_id, man_Of_All_Matches);
}

//PART 4
void check_For_Same_Player()
{
    if (strcmp(man_Of_All_Matches, highest_Total_Individual_Run_Player) == 0)
        printf("Player is Same\n");
    else
        printf("Player is NOT Same\n");
}

// PART 5
void highest_average_by_individual(struct team team_list[], struct match_played matches[], int total_teams, int matches_played)
{
    float max_average = (team_list[0].lineup[0].previous_avg * matches_played - 1 + team_list[0].lineup[0].present_match_score) / matches_played;
    for (int i = 0; i < total_teams; i++) //Traverse through all teams
    {
        for (int j = 0; j < 15; j++) //Traverse through all players
        {
            int average = (team_list[i].lineup[j].previous_avg * matches_played - 1 + team_list[i].lineup[j].present_match_score) / matches_played;
            if (max_average < average)
                max_average = average;
        }
    }
    printf("Max Average By An Individual is => %f ", max_average);
}

// PART 6
void difference_In_Total_Wickets_Between_Pacers_And_Spinners_Of_All_Teams(int total_teams, int matches_played)
{
    int total_pacer_wickets = 0;
    int total_spinner_wickets = 0;
    for (int i = 0; i < total_teams; i++) //Traverse through all teams
    {
        for (int j = 0; j < 15; j++) //Traverse through all players
        {
            if (team_list[i].lineup[j].pacer == 1)
                total_pacer_wickets += team_list[i].lineup[j].previous_total_wickets + team_list[i].lineup[j].present_match_wicket;
            else
                total_spinner_wickets += team_list[i].lineup[j].previous_total_wickets + team_list[i].lineup[j].present_match_wicket;
        }
    }
    int diff = total_pacer_wickets - total_spinner_wickets;
    if (diff > 0)
        printf("Pacers Have %d More Wickets Than Spinners Of All Teams Is %d \n", diff);
    else
        printf("Spinners Have %d More Wickets Than Pacers Of All Teams Is %d \n", -1 * diff);
}

// PART 7
void k_times_Man_Of_The_Match(int k, int total_teams, int number_of_matches_played)
{
    struct player_frequency_pair MOTM_Players[25];
    int MOTM_Player_count = 0;
    for (int i = 0; i < 25; i++)
    {
        MOTM_Players[i].frequency_of_MOTM = 0;
        // MOTM_Players[i].player_id[0] = "\0";
    }

    // Getting frequency of all MOTM PLAYERS
    for (int i = 0; i < number_of_matches_played; i++)
    { // look for player_id in struct key_value_pair
        int index = present(MOTM_Players, MOTM_Player_count, total_matches_played[i].man_of_the_match);
        if (index == -1)
        {
            ++MOTM_Player_count;
            //Copy str1 to str2
            copy_string(total_matches_played[i].man_of_the_match, MOTM_Players[MOTM_Player_count].player_id);
            MOTM_Players[MOTM_Player_count].frequency_of_MOTM += 1;
        }
        else
        {
            MOTM_Players[index].frequency_of_MOTM += 1;
        }
    }

    sort_MOTM(MOTM_Players, 0, MOTM_Player_count); //In decreasing order,on basis of runs

    int at_least_k_times_count = 0;
    int flag = 1;
    for (int i = 0; i < MOTM_Player_count && flag == 1; i++)
    {
        if (MOTM_Players[i].frequency_of_MOTM >= k)
        {
            at_least_k_times_count++;
        }
        else
        {
            flag = 0;
        }
    }

    if (at_least_k_times_count == 0)
    {
        printf("No Such Player\n");
    }
    else
    {
        int century_count[25];
        int max_century_index = -1;
        int max_centuries = 0;
        int flag = 1;
        // Get Century Count Of All Players
        for (int i = 0; i <= at_least_k_times_count && flag == 1; i++)
        {
            century_count[i] = get_Century_Count(MOTM_Players[i].player_id, total_teams);
            if (max_centuries < century_count[i])
            {
                max_centuries = century_count[i];
                max_century_index = i;
            }
        }
        printf("Max Centuries of Man Of The Match Is By Player:%s", MOTM_Players[max_century_index].player_id);
    }
}
