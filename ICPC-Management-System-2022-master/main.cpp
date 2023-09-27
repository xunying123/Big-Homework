#include <iostream>
#include <unordered_map>
#include <set>
#include <cstring>
#include <string>
#include <vector>

struct pro {
    int tim;
    char num;
};

struct problem {
    int RE; // 每一种状态的最后一次提交时间
    int AC;
    int WA;
    int TLE;
    int times; // 最后一次提交时间
    int AC_times; // 第一次正确前错误的提交次数
    bool freeze; // 是否冻结
    int freeze_time; // 封榜后提交次数
    int F_AC; // 第一次AC的时间
    int all_times; // 总的提交次数
    int last; // 最后一次提交的状态
};

struct team {
    std::string name;
    problem p[27];
    int punish;
    int number; // 题目通过数
    std::string last; // 最后一次提交的状态
    char count; // 最后一次提交的题号
    pro RE; // 每一种状态的最后提交时间与题号
    pro AC;
    pro TLE;
    pro WA;
    std::vector<int> time_rank; // 为了排名

    bool operator<(const team &d1) const {
        if (number < d1.number) {
            return false;
        } else if (number > d1.number) {
            return true;
        } else if (punish < d1.punish) {
            return true;
        } else if (punish > d1.punish) {
            return false;
        } else {
            if (time_rank.size() != d1.time_rank.size()) {
                return time_rank.size() < d1.time_rank.size();
            }
            for (int i = 0; i < time_rank.size(); i++) {
                if (time_rank[i] < d1.time_rank[i])
                    return true;
                else if (time_rank[i] > d1.time_rank[i])
                    return false;
            }
            if (name < d1.name) return true;
            else return false;
        }
    }

};

int team_number = 0, problem_number = 0, runtime;
std::unordered_map<std::string, int> id;
int count, freeze = 0, in_time; // 表示状态，0为未封榜，1为封榜
team team_data[10001];
std::set<team> set_data;
std::unordered_map<std::string, int> rank;

void print() { // 打印排名
    int j = 1;
    for (auto ite = set_data.begin(); ite != set_data.end(); ite++, j++) {
        std::cout << team_data[id[(*ite).name]].name << " " << j << " " << (*ite).number;
        std::cout << " " << (*ite).punish << " ";
        for (int i = 1; i < problem_number; i++) {
            if (team_data[id[(*ite).name]].p[i].freeze == 1) {
                if ((team_data[id[(*ite).name]].p[i].all_times - team_data[id[(*ite).name]].p[i].freeze_time) != 0)
                    std::cout << "-";
                std::cout << team_data[id[(*ite).name]].p[i].all_times - team_data[id[(*ite).name]].p[i].freeze_time;
                std::cout << "/" << team_data[id[(*ite).name]].p[i].freeze_time << " ";
            } else {
                if ((team_data[id[(*ite).name]].p[i].AC == 0) && (team_data[id[(*ite).name]].p[i].AC_times == 0))
                    std::cout << ". ";
                if ((team_data[id[(*ite).name]].p[i].AC == 0) && (team_data[id[(*ite).name]].p[i].AC_times != 0))
                    std::cout << "-" << team_data[id[(*ite).name]].p[i].AC_times << " ";
                if ((team_data[id[(*ite).name]].p[i].AC != 0) && (team_data[id[(*ite).name]].p[i].AC_times == 0))
                    std::cout << "+ ";
                if ((team_data[id[(*ite).name]].p[i].AC != 0) && (team_data[id[(*ite).name]].p[i].AC_times != 0))
                    std::cout << "+" << team_data[id[(*ite).name]].p[i].AC_times << " ";
            }
        }
        if (team_data[id[(*ite).name]].p[problem_number].freeze == 1) {
            if ((team_data[id[(*ite).name]].p[problem_number].all_times -
                 team_data[id[(*ite).name]].p[problem_number].freeze_time) != 0)
                std::cout << "-";
            std::cout << team_data[id[(*ite).name]].p[problem_number].all_times -
                         team_data[id[(*ite).name]].p[problem_number].freeze_time;
            std::cout << "/" << team_data[id[(*ite).name]].p[problem_number].freeze_time;
        } else {
            if ((team_data[id[(*ite).name]].p[problem_number].AC == 0) &&
                (team_data[id[(*ite).name]].p[problem_number].AC_times == 0))
                std::cout << ".";
            if ((team_data[id[(*ite).name]].p[problem_number].AC == 0) &&
                (team_data[id[(*ite).name]].p[problem_number].AC_times != 0))
                std::cout << "-" << team_data[id[(*ite).name]].p[problem_number].AC_times;
            if ((team_data[id[(*ite).name]].p[problem_number].AC != 0) &&
                (team_data[id[(*ite).name]].p[problem_number].AC_times == 0))
                std::cout << "+";
            if ((team_data[id[(*ite).name]].p[problem_number].AC != 0) &&
                (team_data[id[(*ite).name]].p[problem_number].AC_times != 0))
                std::cout << "+" << team_data[id[(*ite).name]].p[problem_number].AC_times;
        }
        std::cout << std::endl;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string in_name, order, order1;
    std::cin >> order;
    team in_team, temp;
    char ch1, ch2;
    memset(in_team.p, 0, sizeof(in_team.p));
    in_team.punish = in_team.number = 0; // 数据初始化
    in_team.AC.tim = in_team.WA.tim = in_team.TLE.tim = in_team.RE.tim = 0;
    for (int i = 1; i <= 26; i++) {
        in_team.p[i].WA = in_team.p[i].AC = in_team.p[i].TLE = in_team.p[i].RE = 0;
        in_team.p[i].times = in_team.p[i].AC_times = in_team.p[i].all_times = 0;
        in_team.p[i].freeze = in_team.p[i].freeze_time = in_team.p[i].F_AC = in_team.p[i].last = 0;
    }
    while (order != "START") {
        std::cin >> in_team.name;
        auto iter = id.find(in_team.name);
        if (iter == id.end()) {
            team_number++;
            team_data[team_number] = in_team;
            id[in_team.name] = team_number;
            set_data.insert(in_team);
            std::cout << "[Info]Add successfully." << std::endl;
            std::cin >> order;
        } else {
            std::cout << "[Error]Add failed: duplicated team name." << std::endl;
            std::cin >> order;
        }
    }
    std::cin >> order >> runtime >> order >> problem_number >> order;
    count = 1;
    for (auto iter = set_data.begin(); iter != set_data.end(); iter++) {
        rank[(*iter).name] = count;
        count++;
    }
    std::cout << "[Info]Competition starts." << std::endl;
    while (order != "END") {
        if (order == "ADDTEAM") {
            std::cin >> order;
            std::cout << "[Error]Add failed: competition has started." << std::endl;
        }
        if (order == "START") {
            std::cin >> order >> order >> order >> order;
            std::cout << "[Error]Start failed: competition has started." << std::endl;
        }
        if (order == "SUBMIT") {
            std::cin >> ch1 >> order >> in_name >> order >> order >> order1 >> in_time;
            in_team = team_data[id[in_name]];
            if (order == "Accepted") {
                set_data.erase(in_team);
                if (in_team.p[ch1 - 'A' + 1].AC == 0) {
                    in_team.p[ch1 - 'A' + 1].F_AC = in_time;
                    if ((freeze == 1) || (in_team.p[ch1 - 'A' + 1].freeze == 1)) {
                        in_team.p[ch1 - 'A' + 1].freeze = true;
                        in_team.p[ch1 - 'A' + 1].freeze_time++;
                    } else {
                        int i;
                        for (i = 0; i < in_team.time_rank.size(); i++) {
                            if (in_team.time_rank[i] < in_time) {
                                in_team.time_rank.insert(in_team.time_rank.begin() + i, 1, in_time);
                                break;
                            } else continue;
                        }
                        if (i == in_team.time_rank.size())
                            in_team.time_rank.push_back(in_time);
                        in_team.number++;
                        in_team.punish += in_time + 20 * in_team.p[ch1 - 'A' + 1].AC_times;
                    }
                } else {
                    if (in_team.p[ch1 - 'A' + 1].freeze == 1) {
                        in_team.p[ch1 - 'A' + 1].freeze = true;
                        in_team.p[ch1 - 'A' + 1].freeze_time++;
                    }
                }
                in_team.p[ch1 - 'A' + 1].last = 1;
                in_team.p[ch1 - 'A' + 1].AC = in_time;
                in_team.p[ch1 - 'A' + 1].times = in_time;
                in_team.last = "Accepted";
                in_team.count = ch1;
                in_team.AC.tim = in_time;
                in_team.AC.num = ch1;
                in_team.p[ch1 - 'A' + 1].all_times++;
                set_data.insert(in_team);
                team_data[id[in_name]] = in_team;
            }
            if (order == "Wrong_Answer") {
                in_team.WA.tim = in_time;
                in_team.WA.num = ch1;
                in_team.last = "Wrong_Answer";
                set_data.erase(in_team);
                in_team.p[ch1 - 'A' + 1].WA = in_team.p[ch1 - 'A' + 1].times = in_time;
                if (in_team.p[ch1 - 'A' + 1].AC == 0) in_team.p[ch1 - 'A' + 1].AC_times++;
                if (((freeze == 1) && (in_team.p[ch1 - 'A' + 1].AC == 0)) || (in_team.p[ch1 - 'A' + 1].freeze == 1)) {
                    in_team.p[ch1 - 'A' + 1].freeze = true;
                    in_team.p[ch1 - 'A' + 1].freeze_time++;
                    //in_team.p[ch1 - 'A' + 1].wrong_time++;
                }
                in_team.p[ch1 - 'A' + 1].last = 2;
                in_team.p[ch1 - 'A' + 1].all_times++;
                in_team.count = ch1;
                set_data.insert(in_team);
                team_data[id[in_name]] = in_team;
            }
            if (order == "Runtime_Error") {
                set_data.erase(in_team);
                in_team.RE.tim = in_time;
                in_team.RE.num = ch1;
                in_team.last = "Runtime_Error";
                in_team.p[ch1 - 'A' + 1].RE = in_team.p[ch1 - 'A' + 1].times = in_time;
                if (in_team.p[ch1 - 'A' + 1].AC == 0) in_team.p[ch1 - 'A' + 1].AC_times++;
                if (((freeze == 1) && (in_team.p[ch1 - 'A' + 1].AC == 0)) || (in_team.p[ch1 - 'A' + 1].freeze == 1)) {
                    in_team.p[ch1 - 'A' + 1].freeze = true;
                    in_team.p[ch1 - 'A' + 1].freeze_time++;
                }
                in_team.p[ch1 - 'A' + 1].last = 3;
                in_team.p[ch1 - 'A' + 1].all_times++;
                in_team.count = ch1;
                set_data.insert(in_team);
                team_data[id[in_name]] = in_team;
            }
            if (order == "Time_Limit_Exceed") {
                set_data.erase(in_team);
                in_team.TLE.tim = in_time;
                in_team.TLE.num = ch1;
                in_team.last = "Time_Limit_Exceed";
                in_team.p[ch1 - 'A' + 1].TLE = in_team.p[ch1 - 'A' + 1].times = in_time;
                if (in_team.p[ch1 - 'A' + 1].AC == 0) in_team.p[ch1 - 'A' + 1].AC_times++;
                if (((freeze == 1) && (in_team.p[ch1 - 'A' + 1].AC == 0)) || (in_team.p[ch1 - 'A' + 1].freeze == 1)) {
                    in_team.p[ch1 - 'A' + 1].freeze = true;
                    in_team.p[ch1 - 'A' + 1].freeze_time++;
                    //in_team.p[ch1 - 'A' + 1].wrong_time++;
                }
                in_team.p[ch1 - 'A' + 1].last = 4;
                in_team.p[ch1 - 'A' + 1].all_times++;
                in_team.count = ch1;
                team_data[id[in_name]] = in_team;
                set_data.insert(in_team);
            }
        }
        if (order == "FLUSH") {
            count = 1;
            for (auto iter = set_data.begin(); iter != set_data.end(); iter++) {
                rank[(*iter).name] = count;
                count++;
            }
            std::cout << "[Info]Flush scoreboard." << std::endl;
        }
        if (order == "FREEZE") {
            if (freeze == 1) std::cout << "[Error]Freeze failed: scoreboard has been frozen." << std::endl;
            else {
                std::cout << "[Info]Freeze scoreboard." << std::endl;
                freeze = 1;
            }
        }
        if (order == "SCROLL") {
            if (freeze == 1) {
                std::cout << "[Info]Scroll scoreboard." << std::endl;
                print();
                auto iter1 = set_data.end();
                auto iter2 = set_data.begin();
                iter1--;
                while (iter1 != iter2) {
                    for (int i = 1; i <= problem_number; i++) {
                        if (team_data[id[(*iter1).name]].p[i].freeze != 0) {
                            in_team = team_data[id[(*iter1).name]];
                            temp = *iter1;
                            iter1--;
                            set_data.erase(temp);
                            in_team.p[i].freeze = false;
                            in_team.p[i].freeze_time = 0;
                            if (in_team.p[i].F_AC != 0) {
                                in_team.number++;
                                in_team.punish += in_team.p[i].AC_times * 20 + in_team.p[i].F_AC;
                                int k;
                                for (k = 0; k < in_team.time_rank.size(); k++) {
                                    if (in_team.time_rank[k] < in_team.p[i].F_AC) {
                                        in_team.time_rank.insert(in_team.time_rank.begin() + k, 1, in_team.p[i].F_AC);
                                        break;
                                    } else continue;
                                }
                                if (k == in_team.time_rank.size())
                                    in_team.time_rank.push_back(in_team.p[i].F_AC);
                            }
                            set_data.insert(in_team);
                            set_data.insert(in_team);
                            auto iter3 = set_data.find(in_team);
                            iter1++;
                            team_data[id[in_team.name]] = in_team;
                            if (iter1 == iter3) {
                                continue;
                            } else {
                                std::cout << (*iter3).name << " ";
                                iter3++;
                                std::cout << (*iter3).name << " ";
                                iter3--;
                                std::cout << (*iter3).number << " " << (*iter3).punish;
                                std::cout << std::endl;
                                break;
                            }
                        } else {
                            continue;
                        }
                    }
                    iter1--;
                    iter2 = set_data.begin();
                }
                for (int i = 1; i <= problem_number; i++) { // 对第一名，因为不会再引起排名变化，所以单独排列
                    if (team_data[id[(*iter1).name]].p[i].freeze != 0) {
                        in_team = team_data[id[(*iter1).name]];
                        temp = *iter1;
                        iter1++;
                        set_data.erase(temp);
                        in_team.p[i].freeze = false;
                        in_team.p[i].freeze_time = 0;
                        if (in_team.p[i].F_AC != 0) {
                            in_team.number++;
                            in_team.punish += in_team.p[i].AC_times * 20 + in_team.p[i].F_AC;
                            int k;
                            for (k = 0; k < in_team.time_rank.size(); k++) {
                                if (in_team.time_rank[k] < in_time) {
                                    in_team.time_rank.insert(in_team.time_rank.begin() + k, 1, in_team.p[i].F_AC);
                                    break;
                                } else continue;
                            }
                            if (k == in_team.time_rank.size())
                                in_team.time_rank.push_back(in_team.p[i].F_AC);
                        }
                        set_data.insert(in_team);
                        iter1--;
                        team_data[id[in_team.name]] = in_team;
                    }
                }
                print();
                count = 1;
                for (auto iter = set_data.begin(); iter != set_data.end(); iter++) {
                    rank[(*iter).name] = count;
                    count++;
                }
            } else {
                std::cout << "[Error]Scroll failed: scoreboard has not been frozen." << std::endl;
            }
            freeze = 0;
        }
        if (order == "QUERY_RANKING") {
            std::cin >> in_name;
            auto iter = rank.find(in_name);
            if (iter != rank.end()) {
                std::cout << "[Info]Complete query ranking." << std::endl;
                if (freeze == 1)
                    std::cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled."
                              << std::endl;
                std::cout << in_name << " NOW AT RANKING " << rank[in_name] << std::endl;
            } else {
                std::cout << "[Error]Query ranking failed: cannot find the team." << std::endl;
            }
        }
        if (order == "QUERY_SUBMISSION") {
            std::cin >> in_name;
            auto iter = id.find(in_name);
            if (iter != id.end()) {
                std::cout << "[Info]Complete query submission." << std::endl;
                std::cin >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2;
                std::cin >> ch1 >> ch2;
                if (ch2 == 'L') {
                    std::cin >> ch2;
                    std::cin >> order >> order;
                    if (order == "STATUS=ALL") {
                        if (team_data[id[in_name]].last != "") { // 存疑
                            std::cout << in_name << " " << team_data[id[in_name]].count << " ";
                            if (team_data[id[in_name]].last == "Accepted")
                                std::cout << "Accepted " << team_data[id[in_name]].AC.tim;
                            if (team_data[id[in_name]].last == "Runtime_Error")
                                std::cout << "Runtime_Error " << team_data[id[in_name]].RE.tim;
                            if (team_data[id[in_name]].last == "Wrong_Answer")
                                std::cout << "Wrong_Answer " << team_data[id[in_name]].WA.tim;
                            if (team_data[id[in_name]].last == "Time_Limit_Exceed")
                                std::cout << "Time_Limit_Exceed " << team_data[id[in_name]].TLE.tim;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "STATUS=Accepted") {
                        if (team_data[id[in_name]].AC.tim != 0) {
                            std::cout << in_name << " " << team_data[id[in_name]].AC.num << " Accepted ";
                            std::cout << team_data[id[in_name]].AC.tim << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "STATUS=Runtime_Error") {
                        if (team_data[id[in_name]].RE.tim != 0) {
                            std::cout << in_name << " " << team_data[id[in_name]].RE.num << " Runtime_Error ";
                            std::cout << team_data[id[in_name]].RE.tim << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "STATUS=Wrong_Answer") {
                        if (team_data[id[in_name]].WA.tim != 0) {
                            std::cout << in_name << " " << team_data[id[in_name]].WA.num << " Wrong_Answer ";
                            std::cout << team_data[id[in_name]].WA.tim << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "STATUS=Time_Limit_Exceed") {
                        if (team_data[id[in_name]].TLE.tim != 0) {
                            std::cout << in_name << " " << team_data[id[in_name]].TLE.num << " Time_Limit_Exceed ";
                            std::cout << team_data[id[in_name]].TLE.tim << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                } else {
                    std::cin >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2 >> ch2;
                    std::cin >> order;
                    if (order == "ALL") {
                        if (team_data[id[in_name]].p[ch1 - 'A' + 1].times != 0) {
                            std::cout << in_name << " " << ch1;
                            if (team_data[id[in_name]].p[ch1 - 'A' + 1].last == 1)
                                std::cout << " Accepted ";
                            if (team_data[id[in_name]].p[ch1 - 'A' + 1].last == 2)
                                std::cout << " Wrong_Answer ";
                            if (team_data[id[in_name]].p[ch1 - 'A' + 1].last == 3)
                                std::cout << " Runtime_Error ";
                            if (team_data[id[in_name]].p[ch1 - 'A' + 1].last == 4)
                                std::cout << " Time_Limit_Exceed ";
                            std::cout << team_data[id[in_name]].p[ch1 - 'A' + 1].times;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "Accepted") {
                        if (team_data[id[in_name]].p[ch1 - 'A' + 1].AC != 0) {
                            std::cout << in_name << " " << ch1 << " Accepted "
                                      << team_data[id[in_name]].p[ch1 - 'A' + 1].AC;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "Runtime_Error") {
                        if (team_data[id[in_name]].p[ch1 - 'A' + 1].RE != 0) {
                            std::cout << in_name << " " << ch1 << " Runtime_Error "
                                      << team_data[id[in_name]].p[ch1 - 'A' + 1].RE;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "Wrong_Answer") {
                        if (team_data[id[in_name]].p[ch1 - 'A' + 1].WA != 0) {
                            std::cout << in_name << " " << ch1 << " Wrong_Answer "
                                      << team_data[id[in_name]].p[ch1 - 'A' + 1].WA;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                    if (order == "Time_Limit_Exceed") {
                        if (team_data[id[in_name]].p[ch1 - 'A' + 1].TLE != 0) {
                            std::cout << in_name << " " << ch1 << " Time_Limit_Exceed "
                                      << team_data[id[in_name]].p[ch1 - 'A' + 1].TLE;
                            std::cout << std::endl;
                        } else {
                            std::cout << "Cannot find any submission." << std::endl;
                        }
                    }
                }
            } else {
                std::cout << "[Error]Query submission failed: cannot find the team." << std::endl;
                std::cin >> ch1;
                getline(std::cin, in_name);
            }
        }
        std::cin >> order;
    }
    std::cout << "[Info]Competition ends." << std::endl;
    return 0;
}