
#ifndef TICKET_SYSTEM_USER_H
#define TICKET_SYSTEM_USER_H

#include<iostream>
#include "bpt.h"
#include "hash_map.h"

struct user {
    char password[31];
    char name[21];
    char mail[31];
    int privilege;

    friend bool operator==(const user &a, const user &b) {
        return a.privilege == b.privilege && strcmp(a.password, b.password) == 0 && strcmp(a.name, b.name) == 0 &&
               strcmp(a.mail, b.mail) == 0;
    }

    friend bool operator<(const user &a, const user &b) {
        return a.privilege < b.privilege && strcmp(a.password, b.password) < 0 && strcmp(a.name, b.name) < 0 &&
               strcmp(a.mail, b.mail) < 0;
    }

};

BPT<m_string<21>, user> user_data("user");
_hash_map log_in;

void add_user(const char *order) {
    int i = 0, count = 0;
    std::string current, new_user;
    user temp;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'c': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            current += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'u': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            new_user += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'p': {
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.password[j] = order[i];
                        }
                        temp.password[j] = '\0';
                        i--;
                        break;
                    }
                    case 'n': {
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.name[j] = order[i];
                        }
                        temp.name[j] = '\0';
                        i--;
                        break;
                    }
                    case 'm': {
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.mail[j] = order[i];
                        }
                        temp.mail[j] = '\0';
                        i--;
                        break;
                    }
                    case 'g': {
                        i += 4;
                        temp.privilege = 0;
                        while (order[i] != ' ' && order[i] != '\0') {
                            temp.privilege *= 10;
                            temp.privilege += order[i++] - '0';
                        }
                        i--;
                        break;
                    }

                }
            }
        }
    }
    auto found = user_data.readonly(new_user);
    if (new_user == "I_am_the_admin") {
        if (user_data.empty()) {
            temp.privilege = 10;
            user_data.insert(new_user, temp);
            std::cout << 0 << '\n';
            return;
        } else {
            std::cout << "-1" << '\n';
            return;
        }
    }
    if (found == nullptr) {
        char s[21];
        strcpy(s, current.c_str());
        int pp = log_in.find(s);
        if (pp != -1) {
            if (pp > temp.privilege) {
                user_data.insert(new_user, temp);
                std::cout << 0 << '\n';
                return;
            }
        }
        std::cout << "-1" << '\n';
        return;
    } else {
        std::cout << "-1" << '\n';
        return;
    }
}

void login(char *order) {
    int i = 0, count = 0;
    std::string username, password;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'p': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            password += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'u': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            username += order[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding1 = user_data.readonly(username);
    char s[21];
    strcpy(s, username.c_str());
    int pp = log_in.find(s);
    if (finding1 != nullptr) {
        m_string<31> _password(password);
        if (pp == -1 && strcmp(_password._str, finding1->password) == 0) {
            log_in.insert(s, finding1->privilege);
            std::cout << 0 << '\n';
            return;
        }
    }
    std::cout << "-1" << '\n';
}

void logout(char *order) {
    int i = 0, count = 0;
    std::string username;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'u': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            username += order[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    char s[21];
    strcpy(s, username.c_str());
    int pp = log_in.find(s);
    if (pp == -1) {
        std::cout << "-1" << '\n';
        return;
    }
    log_in.erase(s);
    std::cout << 0 << '\n';
}

void query_profile(char *order) {
    int i = 0, count = 0;
    std::string current, new_user;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'c': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            current += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'u': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            new_user += order[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding1 = user_data.readonly(new_user);
    char s[21];
    strcpy(s, current.c_str());
    int pp = log_in.find(s);
    if ((pp != -1) && (finding1 != nullptr)) {
        if (current == new_user || pp > finding1->privilege) {
            std::cout << new_user << " " << finding1->name << " " << finding1->mail << " " << finding1->privilege
                      << '\n';
            return;
        }
    }
    std::cout << "-1" << '\n';
}

void modify_profile(char *order) {
    int i = 0, count = 0;
    std::string current, new_user;
    user temp;
    temp.privilege = 0;
    int p = 0, u = 0, n = 0, m = 0, g = 0;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'c': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            current += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'u': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            new_user += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'p': {
                        p = 1;
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.password[j] = order[i];
                        }
                        temp.password[j] = '\0';
                        i--;
                        break;
                    }
                    case 'n': {
                        n = 1;
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.name[j] = order[i];
                        }
                        temp.name[j] = '\0';
                        i--;
                        break;
                    }
                    case 'm': {
                        m = 1;
                        i += 4;
                        int j = 0;
                        for (; order[i] != '\0' && order[i] != ' '; i++, j++) {
                            temp.mail[j] = order[i];
                        }
                        temp.mail[j] = '\0';
                        i--;
                        break;
                    }
                    case 'g': {
                        g = 1;
                        i += 4;
                        temp.privilege = 0;
                        while (order[i] != ' ' && order[i] != '\0') {
                            temp.privilege *= 10;
                            temp.privilege += order[i++] - '0';
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding1 = user_data.readwrite(new_user);
    char s[21];
    strcpy(s, current.c_str());
    int pp = log_in.find(s);
    if ((pp != -1) && (finding1 != nullptr)) {
        if ((pp > finding1->privilege || current == new_user) && (pp > temp.privilege)) {
            if (p) {
                strcpy(finding1->password, temp.password);
            }
            if (n) {
                strcpy(finding1->name, temp.name);
            }
            if (m) {
                strcpy(finding1->mail, temp.mail);
            }
            if (g) {
                finding1->privilege = temp.privilege;
            }
            strcpy(s, new_user.c_str());
            int ppp = log_in.find(s);
            if (ppp != -1) {
                log_in.erase(s);
                log_in.insert(s, finding1->privilege);
            }
            std::cout << new_user << " " << finding1->name << " " << finding1->mail << " " <<finding1->privilege
                      << '\n';
            return;
        }
    }
    std::cout << "-1" << '\n';
}

#endif //TICKET_SYSTEM_USER_H
